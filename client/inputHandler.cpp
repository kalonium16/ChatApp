
#include "inputHandler.h"


void handleInput(Client& c, bool& bQuit, messagingData& mData, const std::string& inputString)
{
	if (mData.waitingForMessage) {
		if (mData.userId == 0) {
			try
			{
				mData.userId = std::stoi(inputString);
					c.isUserOnline(mData.userId);
				return;
			}
			catch(const std::exception& e)
			{
				mData.userId = 0;
				mData.waitingForMessage = false;
				mData.isOnline = false;
				std::cerr << e.what() << '\n';
				std::cout << R"(Input "M" or "Message" to send messaage: )" << "\n";
			}				
		}
		else if (mData.isOnline) {
			c.messageToUser(mData.userId, inputString);
			mData.userId = 0;
			mData.waitingForMessage = false;
			mData.isOnline = false;
			std::cout << R"(Input "M" or "Message" to send messaage: )" << "\n";
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

		std::cout << "Unknown command try again" << std::endl;
		std::cout << R"(Input "M" or "Message" to send messaage: )" << "\n";
	}
}