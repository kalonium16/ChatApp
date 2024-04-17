#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "client.h"
#include "inputHandler.h"
#include "requestHandler.h"
#include "common.h"

void mainLoop(Client&);

int main(int argc, char* argv[]) {
	std::string ipAddress = "127.0.0.1";
	std::string port = "6000";
	if (argc >= 3) {
		ipAddress = argv[1];
		port = argv[2];
	}
	Client c;
	c.connect(ipAddress, port);
	mainLoop(c);
}


void mainLoop(Client& c) {

	std::atomic_bool quit = false;
	messagingData mData;
	RequestHandler reqHandler{ c, quit, mData };
	while (!quit)
	{


		
		std::string input;
		std::getline(std::cin, input);
		// handle user input
		handleInput(c, quit, mData, input);
	} 

}
