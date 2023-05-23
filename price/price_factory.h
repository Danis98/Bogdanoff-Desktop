#pragma once

#include <vector>

#include "json/json.hpp"

#include "price.h"

void create_prices(const json& config_json, std::vector<std::unique_ptr<Price>>& prices);