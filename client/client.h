#pragma once
#include <memory>
#include "client_interface.h"
#include "request.h"
#include "req_types.h"



class Client : public Network::ClientInterface<RequestTypes> {
public:
	void isServerUp();

	void getOterClients();

	void messageToUser(uint32_t user, std::string message);

	void isUserOnline(uint32_t user);
};