#include "requestHandler.h"


void handleRequest(Client& c, bool& quit, messagingData& mData, std::future<std::string>& inputFuture) {
	while (!quit && (inputFuture.wait_for(std::chrono::seconds(0)) != std::future_status::ready))
	{
		if (c.isConnected()) {
			if (!c.incoming().empty()) {
				auto req = c.incoming().pop_front().req;
				switch (req.getHeader().id)
				{
				case RequestTypes::ServerAccept:
				{
					std::cout << "Online\n";
					std::cout << R"(Input "M" or "Message" to send messaage: )" << "\n";
					std::cout << R"(Input "Q" or "Quit" to quit: )" << "\n";
				}
				break;
				case RequestTypes::IsServerUp: {
					std::cout << "Server is Up!" << '\n';
				};
											 break;
				case RequestTypes::GetOtherClients: {
					auto size = req.getHeader().size / sizeof(uint32_t);
					uint32_t userId{};
					if (size == 0) {
						std::cout << "There are no other user to message" << '\n';
						mData.waitingForMessage = false;
						std::cout << R"(Input "M" or "Message" to send messaage: )" << "\n";
					}
					else {
						std::cout << "You can write message to this users: \n";
						mData.waitingForMessage = true;
						for (auto i = 0; i < size; i++) {
							req >> userId;
							std::cout << "User: " << userId << "\n";
						}
						std::cout << "Enter users Id to message: ";
					}
				};
												  break;
				case RequestTypes::MessageToClient: {
					std::string message{};
					//message.resize(req.getHeader().size);
					req >> message;
					std::cout << "[" << req.getHeader().reqData.reciverId << "]: " << message << std::endl;
				};
												  break;
				case RequestTypes::IsClientOnline: {
					req >> mData.isOnline;
					if (mData.isOnline) {
						std::cout << "Enter your message: ";
					}
					else {
						std::cout << "User is not online!" << std::endl;
						mData.userId = 0;
						mData.waitingForMessage = false;
						std::cout << R"(Input "M" or "Message" to send messaage: )" << "\n";
					}
				};
												 break;
				default:
					break;
				}
			}
		}
		else {
			std::cout << "Sever Down\n";
			quit = true;
		}
	}

}