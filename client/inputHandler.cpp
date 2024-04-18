
#include "inputHandler.h"


InputHandler::InputHandler(Client& c, std::atomic_bool& bQuit, messagingData& mData):
	m_client(c),m_quit(bQuit),m_messageData(mData)
{
}

void InputHandler::handleInput(std::string inputString)
{
	if (m_messageData.waitingForMessage) {
		if (m_messageData.userId == 0) {
			int temp{};
			auto err = std::from_chars(inputString.data(), inputString.data() + inputString.size(), temp).ec;
			if (err == std::errc::invalid_argument || err == std::errc::result_out_of_range) {
				m_messageData.userId = 0;
				m_messageData.waitingForMessage = false;
				m_messageData.isOnline = false;
				std::cout << R"(Wrong Id format!)" << std::endl;
				std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
				return;
			}
			else {
				m_messageData.userId = temp;
				m_client.isUserOnline(m_messageData.userId);
				return;
			}

		}
		else if (m_messageData.isOnline) {
			m_client.messageToUser(m_messageData.userId, inputString);
			m_messageData.userId = 0;
			m_messageData.waitingForMessage = false;
			m_messageData.isOnline = false;
			std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
			return;
		}
	}
	else {
		if (inputString == "Up" || inputString == "up") {
			m_client.isServerUp();
			return;
		}

		if (inputString == "Quit" || inputString == "quit" || inputString == "q" || inputString == "Q") {
			m_quit = true;
			return;
		}


		if (inputString == "M" || (inputString) == "message" || inputString == "Message") {
			m_client.getOterClients();
			return;
		}

		std::cout << "Unknown command try again" << "\n";
		std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
	}
}
