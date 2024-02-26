#pragma once
#include <memory>
#include <string>
#include "client.h"
#include "common.h"


void handleInput(Client& c, bool& bQuit, messagingData& mData, const std::string& inputString);