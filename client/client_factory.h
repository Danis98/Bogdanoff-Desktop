#pragma once

#include <vector>

#include "json/json.hpp"
#include "price/price_factory.h"

#include "client.h"

void create_clients(const json& config_json, std::vector<std::unique_ptr<Client>>& clients, const std::vector<std::unique_ptr<Price>>& prices);