#pragma once

#include <vector>

#include "json/json.hpp"

#include "price.h"

void create_price_sources(const json& config_json, std::vector<std::unique_ptr<PriceSource>>& price_sources);