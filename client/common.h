#pragma once

struct messagingData
{
	bool waitingForMessage = false;
	uint32_t userId{};
	bool isOnline{ false };
};
