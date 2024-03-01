#pragma once

#include <memory>
#include <future>
#include <string>
#include <thread>
#include <atomic>
#include "client.h"
#include "common.h"

void handleRequest(Client&, std::atomic_bool&, messagingData&);

class RequestHandler {
public:
	RequestHandler(Client&, std::atomic_bool&, messagingData&);
	~RequestHandler();
	void handleRequests();
private:
	Client& m_client;
	std::atomic_bool& m_quit;
	messagingData& m_data;
	std::thread m_reqThread;
};