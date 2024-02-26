#include "server.h"


Server::Server(int port) :ServerInterface<RequestTypes>{ port }
{
}
//Send client confirmation of successful connecting
bool Server::onClientConnect(std::shared_ptr<Network::Connection<RequestTypes>> client)
{
	Network::Request<RequestTypes> req;
	req.getHeader().id = RequestTypes::ServerAccept;
	client->send(req);
	return true;
}
//respond to request depending on enum type
void Server::onRequest(std::shared_ptr<Network::Connection<RequestTypes>> client, Network::Request<RequestTypes>& req) {
	switch (req.getHeader().id)
	{
	case RequestTypes::IsServerUp:
	{
		std::cout << "[" << client->getId() << "]: Server Up\n";

		client->send(req);
	}
	break;
	//send id of other clients
	case RequestTypes::GetOtherClients:
	{
		for (auto el : this->getConnections()) {
			if (el.first != client->getId())
				req << el.first;
		}
		client->send(req);
	}
	break;
	//send message from client to client with id
	case RequestTypes::MessageToClient:
	{
		auto clientId = req.getHeader().reqData.reciverId;
		if (this->getConnections().contains(clientId)) {
			this->getConnections()[clientId]->send(req);
		}
	}
	break;
	//Check if user with this id conected to server
	case RequestTypes::IsClientOnline:
	{
		auto clientId = req.getHeader().reqData.reciverId;
		auto isOnline = this->getConnections().contains(clientId);
		req << isOnline;
		client->send(req);
	}
	break;
	default:
		break;
	}
}
