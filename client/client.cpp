#include "client.h"

void Client::isServerUp() {
	Network::Request<RequestTypes> req;
	req.getHeader().id = RequestTypes::IsServerUp;
	send(req);
}

void Client::getOterClients() {
	Network::Request<RequestTypes> req;
	req.getHeader().id = RequestTypes::GetOtherClients;
	send(req);
}

void Client::messageToUser(uint32_t user, std::string message) {
	Network::Request<RequestTypes> req;
	req.getHeader().id = RequestTypes::MessageToClient;
	req.getHeader().reqData.reciverId = user;

	req << message;

	send(req);
}

void Client::isUserOnline(uint32_t user) {
	Network::Request<RequestTypes> req;
	req.getHeader().id = RequestTypes::IsClientOnline;
	req.getHeader().reqData.reciverId = user;
	send(req);
}