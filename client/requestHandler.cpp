#include "requestHandler.h"



RequestHandler::RequestHandler(Client& c, std::atomic_bool& quit, messagingData& mData):m_client(c), m_quit(quit), m_data(mData)
{
	m_reqThread = std::thread{ &RequestHandler::handleRequests,this };
}

RequestHandler::~RequestHandler()
{
	m_quit = true;
	m_client.incoming().stopWaiting();
	if (m_reqThread.joinable())
	{
		m_reqThread.join();
	}
}

void RequestHandler::handleRequests()
{
	while (!m_quit) {
		if (m_client.isConnected()) {
			if (!m_client.incoming().empty()) {
				auto req = m_client.incoming().pop_front().req;
				switch (req.getHeader().id)
				{
				case RequestTypes::ServerAccept:
				{
					std::cout << "Online\n";
					std::cout << R"(Input "M" or "Message" to send messaage: )" << "\n";
					std::cout << R"(Input "Q" or "Quit" to quit: )" << std::endl;
				}
				break;
				case RequestTypes::IsServerUp: {
					std::cout << "Server is Up!" << '\n';
				};
											 break;
				case RequestTypes::GetOtherClients: {
					printOtherUsers(req);
				};
												  break;
				case RequestTypes::MessageToClient: {
					printUserMessage(req);
				};
												  break;
				case RequestTypes::IsClientOnline: {
					printUserStatus(req);
				};
												 break;
				default:
					break;
				}
			}
			else {
				m_client.incoming().waitForMore();

			}
		}
		else {
			std::cout << "Sever Down\n";
			m_quit = true;
		}
	}
}

void RequestHandler::printUserStatus(Network::Request<RequestTypes>& req)
{
	req >> m_data.isOnline;
	if (m_data.isOnline) {
		std::cout << "Enter your message: " << std::flush;
	}
	else {
		std::cout << "User is not online!" << std::endl;
		m_data.userId = 0;
		m_data.waitingForMessage = false;
		std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
	}
}

void RequestHandler::printUserMessage(Network::Request<RequestTypes>& req)
{
	std::string message{};
	//message.resize(req.getHeader().size);
	req >> message;
	std::cout << "[" << req.getHeader().reqData.senderId << "]: " << message << std::endl;
}

void RequestHandler::printOtherUsers(Network::Request<RequestTypes>& req)
{
	auto size = req.getHeader().size / sizeof(uint32_t);
	uint32_t userId{};
	if (size == 0) {
		std::cout << "There are no other user to message" << '\n';
		m_data.waitingForMessage = false;
		std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
	}
	else {
		std::cout << "You can write message to this users: \n";
		m_data.waitingForMessage = true;
		for (auto i = 0; i < size; i++) {
			req >> userId;
			std::cout << "User: " << userId << "\n";
		}
		std::cout << "Enter users Id to message: " << std::flush;
	}
}
