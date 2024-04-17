
#include "inputHandler.h"


void handleInput(Client& c, std::atomic_bool& bQuit, messagingData& mData, const std::string& inputString)
{
	if (mData.waitingForMessage) {
		if (mData.userId == 0) {
			int temp{};
			if (std::from_chars(inputString.data(), inputString.data() + inputString.size(), temp).ec == std::errc{}) {
				mData.userId = temp;
				c.isUserOnline(mData.userId);
				return;
			}
			else {
				mData.userId = 0;
				mData.waitingForMessage = false;
				mData.isOnline = false;
				std::cout << R"(Wrong Id format!)" << std::endl;
				std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
			}

		}
		else if (mData.isOnline) {
			c.messageToUser(mData.userId, inputString);
			mData.userId = 0;
			mData.waitingForMessage = false;
			mData.isOnline = false;
			std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
			return;
		}
	}
	else {
		if (inputString == "Up" || inputString == "up") {
			c.isServerUp();
			return;
		}

		if (inputString == "Quit" || inputString == "quit" || inputString == "q" || inputString == "Q") {
			bQuit = true;
			return;
		}


		if (inputString == "M" || (inputString) == "message" || inputString == "Message") {
			c.getOterClients();
			return;
		}

		std::cout << "Unknown command try again" << "\n";
		std::cout << R"(Input "M" or "Message" to send messaage: )" << std::endl;
	}
}