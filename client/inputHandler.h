#pragma once
#include <memory>
#include <string>
#include <atomic>
#include "client.h"
#include "common.h"


void handleInput(Client& c, std::atomic_bool& bQuit, messagingData& mData, const std::string& inputString);