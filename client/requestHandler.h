#pragma once

#include <memory>
#include <future>
#include <string>
#include "client.h"
#include "common.h"

void handleRequest(Client&, bool&, messagingData&, std::future<std::string>&);