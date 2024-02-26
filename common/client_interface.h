#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include "connection.h"
#include "tsdeque.h"
#include "request.h"
namespace Network { 
	template <typename T>
	class ClientInterface
	{
	public:
		ClientInterface() {

		};
		virtual ~ClientInterface() {
			disconnect();
		};
		//Connect to server and save connection for further uses
		bool connect(std::string_view host, std::string_view port) {
			try {
				boost::asio::ip::tcp::resolver resolver(m_context);
				boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);


				m_connection = std::make_unique<Connection<T>>(Connection<T>::owner::client, m_context,boost::asio::ip::tcp::socket(m_context), m_dqRequestIn);
				m_connection->connectToServer(endpoints);

				thrContext = std::thread([this]() {
					m_context.run();
					});
			}
			catch (std::exception& e) {
				std::cerr << "Client Exception" << e.what() << '\n';
				return false;
			}
			return true;
		}

		bool isConnected()
		{
			if (m_connection)
				return m_connection->isConnected();
			else
				return false;
		}

		void disconnect()
		{
			if (isConnected())
			{
				m_connection->disconnect();
			}
			
			m_context.stop();
			if (thrContext.joinable())
				thrContext.join();

			m_connection.release();
		}

		void send(const Request<T>& req) {
			if (isConnected()) {
				m_connection->send(req);
			}
		}

		TsDeque<OwnedRequest<T>>& incoming()
		{
			return m_dqRequestIn;
		}

	private:
		boost::asio::io_context m_context{};
		std::thread thrContext;

		std::unique_ptr<Connection<T>> m_connection;

		TsDeque<OwnedRequest<T>> m_dqRequestIn;

	};

}