#pragma once
#include <iostream>
#include "server_interface.h"
#include "request.h"
#include "connection.h"
#include "req_types.h"


class Server : public Network::ServerInterface<RequestTypes>
{
public:
	Server(int port);
	virtual ~Server() = default;

protected:

	virtual bool onClientConnect(std::shared_ptr<Network::Connection<RequestTypes>> client);
	virtual void onRequest(std::shared_ptr<Network::Connection<RequestTypes>> client, Network::Request<RequestTypes>& req);
	
};