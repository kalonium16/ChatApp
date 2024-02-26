#pragma once
#include <memory>
#include <boost/asio.hpp>

#include "tsdeque.h"
#include "request.h"

namespace Network {
	template<typename T>
	class ServerInterface;


	template <typename T>
	class Connection : public std::enable_shared_from_this<Connection<T>>
	{
	public:
		enum class owner {
			server,
			client
		};

		Connection(owner parent, boost::asio::io_context& context, boost::asio::ip::tcp::socket socket, TsDeque<OwnedRequest<T>>& dqIn) : m_context(context), m_socket(std::move(socket)), m_dqRequestIn(dqIn) {
			m_ownerType = parent;
		};
		virtual ~Connection() {
		};

		void connectToClient(Network::ServerInterface<T>* server, uint32_t uid = 0) {
			if (m_ownerType == owner::server) {
				if (m_socket.is_open()) {
					id = uid;

					ReadHeader();
				}
			}
		}

		void connectToServer(const boost::asio::ip::tcp::resolver::results_type& endPoint) {
			if (m_ownerType == owner::client) {
				boost::asio::async_connect(m_socket, endPoint, [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {
					if (!ec) {
						ReadHeader();
					}
					else {
						std::cout << "Client: Cannot connect to server!";
					}
					});
			}
		}
		// Send request
		void send(const Request<T>& req) {
			boost::asio::post(m_context,
				[this, req]() {
					bool writingReqOut = !m_dqRequestOut.empty();
					m_dqRequestOut.push_back(req);
					if (!writingReqOut) {
						WriteHeader();
					}
				});
		};

		bool isConnected() const {
			return (!m_context.stopped() && m_socket.is_open());
		}

		void disconnect() {
			if (isConnected()) {
				boost::asio::post(m_context, [this]() {
					m_socket.close();
					});
			}
		}

		uint32_t getId() {
			return id;
		}
	private:
		// read request header if body size > 0 read body else add request to request handler 
		void ReadHeader() {
			boost::asio::async_read(m_socket, boost::asio::buffer(&m_reqTempIn.getHeader(), sizeof(RequestHeader<T>)), [this](std::error_code ec, std::size_t length) {
				if (!ec) {
					if (m_reqTempIn.getHeader().size > 0) {
						m_reqTempIn.getBody().resize(m_reqTempIn.getHeader().size);
						ReadBody();
					}
					else {
						AddToIncomingReqDeque();
					}
				}
				else {
					std::cout << "[" << id << "] Read Header Fail.\n";
					m_socket.close();
				}

				});
		}
		// read body then add request to request handler 
		void ReadBody() {
			boost::asio::async_read(m_socket, boost::asio::buffer(m_reqTempIn.getBody().data(), m_reqTempIn.getBody().size()), [this](std::error_code ec, std::size_t length) {
				if (!ec) {
						AddToIncomingReqDeque();
				}
				else {
					std::cout << "[" << id << "] Read Body Fail.\n";
					m_socket.close();
				}
				});
		}
		// write request header if body size > 0 write request body
		void WriteHeader() {
			boost::asio::async_write(m_socket, boost::asio::buffer(&m_dqRequestOut.front().getHeader(), sizeof(RequestHeader<T>)), [this](std::error_code ec, std::size_t length) {
				if (!ec) {
					if (m_dqRequestOut.front().getBody().size() > 0) {
						WriteBody();
					}
					else {
						m_dqRequestOut.pop_front();
						if (!m_dqRequestOut.empty()) {
							WriteHeader();
						}
					}
				}
				else {
					std::cout << "[" << id << "] Write Header Fail.\n";
					m_socket.close();
				}
				});
		}
		// write request body
		void WriteBody() {
			boost::asio::async_write(m_socket, boost::asio::buffer(m_dqRequestOut.front().getBody().data(), m_dqRequestOut.front().getBody().size()), [this](std::error_code ec, std::size_t length) {
				if (!ec) {
					m_dqRequestOut.pop_front();
					if (!m_dqRequestOut.empty()) {
						WriteHeader();
					}
				}
				else {
					std::cout << "[" << id << "] Write Body Fail.\n";
					m_socket.close();
				}
				});
		}
		//add request to request handler
		void AddToIncomingReqDeque() {
			if (m_ownerType == owner::server) {
				m_dqRequestIn.push_back({ this->shared_from_this(),m_reqTempIn });
				m_reqTempIn.clear();
			}
			else {
				m_dqRequestIn.push_back({ nullptr,m_reqTempIn});
				m_reqTempIn.clear();
			}
			ReadHeader();
		}
	private:
		boost::asio::ip::tcp::socket m_socket;
		boost::asio::io_context& m_context;

		TsDeque<Request<T>> m_dqRequestOut{};
		TsDeque<OwnedRequest<T>>& m_dqRequestIn{};
		Request<T> m_reqTempIn{};
		owner m_ownerType = owner::server;
		uint32_t id{};

	};
}