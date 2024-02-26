#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <map>
#include "network_common.h"
namespace Network {
	template <typename T>
	class ServerInterface
	{
	public:
		ServerInterface(int port) : m_acceptor(m_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
		};
		virtual ~ServerInterface() {
			stop();
		};
		//Start server and waait for clients connections
		bool start() {
			try
			{
				WaitForConnection();

				m_contextThread = std::thread([this]() {
					m_context.run();
					});
			}
			catch (const std::exception& e)
			{
				std::cerr << "SERVER Exception: " << e.what() << std::endl;
				return false;
			}
			std::cout << "SERVER Started!" << std::endl;
			return true;
		}

		void stop() {
			m_context.stop();
			if (m_contextThread.joinable()) {
				m_contextThread.join();
				std::cout << "SERVER Stoped!" << std::endl;
			}
		}
		//Saves incoming connections and calls themselves to wait for another connection
		void WaitForConnection() {
			m_acceptor.async_accept([this](std::error_code err, boost::asio::ip::tcp::socket socket) {
				if (!err) {
					std::cout << "SERVER New connection" << socket.remote_endpoint() << '\n';
					auto newConn = std::make_shared<Connection<T>>(Connection<T>::owner::server, m_context, std::move(socket), m_dqRequestsIn);
					auto id = m_IDCounter++;
					if (onClientConnect(newConn)) {
						m_Connections[id] = std::move(newConn);
						m_Connections[id]->connectToClient(this, id);
						std::cout << "[" << id << "] Approved\n";
					}
					else {
						std::cout << "- Connection Denied\n";
					}
				}
				else {
					std::cout << "SERVER New Error" << err.message() << '\n';
				}
				WaitForConnection();
			});
		}
		//Send req to client
		void sendReqToClient(std::shared_ptr<Connection<T>> client, const Request<T>& req) {
			if (client && client->isConnected()) {
				client->send(req);
			}
			else {
				onClientDisconnect(client);
				client.reset();
				auto el = m_Connections.find(client->getId());
				m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), el), m_Connections.end());
			}
		}
		//handle request if there any
		void update() {

			while (!m_dqRequestsIn.empty()) {
			std::cout << "update \n";
				auto req = m_dqRequestsIn.pop_front();
				onRequest(req.clientReq, req.req);
			}
		}

		auto& getConnections() {
			return m_Connections;
		}
	protected:
		virtual bool onClientConnect(std::shared_ptr<Connection<T>> client) {
			return false;
		}

		virtual void onClientDisconnect(std::shared_ptr<Connection<T>> client) {

		}

		virtual void onRequest(std::shared_ptr<Connection<T>> client, Request<T>& req)
		{

		}
	private:
		boost::asio::io_context m_context{};
		std::thread m_threadContext;
		boost::asio::ip::tcp::acceptor m_acceptor{};

		TsDeque<OwnedRequest<T>> m_dqRequestsIn;
		std::map<uint32_t,std::shared_ptr<Connection<T>>> m_Connections;


		std::thread m_contextThread;
		uint32_t m_IDCounter = 100000;
	};

}