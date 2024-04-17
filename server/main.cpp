
#include "server.h"


int main(int argc, char* argv[]) {
	int port{6000};
	if (argc >= 2) {
		port = std::stoi(argv[1]);
		
	}
	Server server{ port };
	server.start();
	while (true)
	{
		server.update();
	}
}
