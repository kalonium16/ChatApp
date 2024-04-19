#include <iostream>
#include <charconv>
#include "server.h"


int main(int argc, char* argv[]) {
	int port{6000};
	if (argc >= 2) {
		std::string portString{ argv[1] };
		auto err = std::from_chars(portString.data(), portString.data() + portString.size(), port).ec;
		if (err == std::errc::invalid_argument || err == std::errc::result_out_of_range) {
			std::cout << "Wrong port try again";
			return 0;
		}
		else {
			if (port < 0 && port > 65535) {
				std::cout << "Wrong port try again";
				return 0;
			}
		}
	}
	Server server{ port };
	server.start();
	while (true)
	{
		server.update();
	}
}
