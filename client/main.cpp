#include <iostream>
#include <string>
#include <thread>
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

	bool quit = false;
	messagingData mData;
	std::thread reqThread([&]() {
		while (!quit)
		{
			handleRequest(c,quit,mData);
		}
		});
	while (!quit)
	{


		auto input = std::async(std::launch::async, [&]() {
			std::string input;
			std::getline(std::cin, input);
			return input;
			});
		const std::string inputString = input.get();
		// handle user input
		handleInput(c, quit, mData, inputString);
	} 
	try
	{

		if (reqThread.joinable()) {
			reqThread.join();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
}
