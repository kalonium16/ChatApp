#include <iostream>
#include <string>
#include "client.h"
#include "inputHandler.h"
#include "requestHandler.h"
#include "common.h"

void mainLoop(Client&);

int main(int argc, char* argv[]) {
	if (argc >= 3) {
		Client c;
		c.connect(argv[1], argv[2]);
		mainLoop(c);
	}
	else {
		//if no ip and port specified use local 6000 port
		Client c;
		c.connect("127.0.0.1", "6000");
		mainLoop(c);
	}
}


void mainLoop(Client& c) {

	bool bQuit = false;
	messagingData mData;
	while (!bQuit)
	{


		auto input = std::async(std::launch::async, []() {
			std::string input;
			std::getline(std::cin, input);
			return input;
			});
		//handle requests from server
		handleRequest(c, bQuit, mData, input);

		const std::string inputString = input.get();
		// handle user input
		handleInput(c, bQuit, mData, inputString);
		
	}

}
