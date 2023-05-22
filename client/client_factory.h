#pragma once

#include <vector>

#include "json/json.hpp"

#include "client.h"

void create_clients(const json& config_json, std::vector<std::unique_ptr<Client>>& clients);