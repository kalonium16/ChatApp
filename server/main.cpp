
#include "server.h"


int main(int argc, char* argv[]) {
	if (argc >= 2) {
		auto  port = std::stoi(argv[1]);
		Server server{port};
		server.start();
		while (true)
		{
			server.update();
		}
	}
	else {
		//if no port specified use local 6000 port
		Server server{ 6000 };
		server.start();
		while (true)
		{
			server.update();
		}
	}
}
