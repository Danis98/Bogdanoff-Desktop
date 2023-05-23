#pragma once

#include "http/http_session.h"
#include "json/json.hpp"

#include "price.h"

constexpr char COINGECKO_BASE_URL[] = "https://api.coingecko.com/api/v3";

class CoinGeckoPrice : public Price
{
public:
    CoinGeckoPrice(const json& coingecko_json);

protected:
    void _load_asset_ids();
    void _load_prices(const std::vector<std::string>& assets) override;

    HttpSession _session;

    std::optional<std::map<std::string, std::string>> _asset_ids;
};