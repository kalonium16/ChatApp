#pragma once
#include <memory>
#include <string>
#include <atomic>
#include "client.h"
#include "common.h"
#include <charconv>

class InputHandler
{
public:
	InputHandler(Client& c, std::atomic_bool& bQuit, messagingData& mData);
	~InputHandler() = default;

	void handleInput(std::string);
private:
	Client& m_client;
	std::atomic_bool& m_quit;
	messagingData& m_messageData;
};


void handleInput(Client& c, std::atomic_bool& bQuit, messagingData& mData, const std::string& inputString);