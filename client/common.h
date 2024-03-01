#pragma once
#include <atomic>
#include <memory>
struct messagingData
{
	std::atomic_bool waitingForMessage = false;
	std::atomic_bool isOnline{ false };
	std::atomic<uint32_t> userId{};
};
