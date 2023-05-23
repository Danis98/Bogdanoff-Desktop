#include "price_factory.h"
#include "coingecko_price.h"

void create_prices(const json& config_json, std::vector<std::unique_ptr<Price>>& prices)
{
    for(const auto& price_config : config_json)
    {
        if(price_config["enabled"].get<std::string>() == "false")
            continue;
        if(price_config["source"] == "coingecko")
            prices.push_back(std::make_unique<CoinGeckoPrice>(price_config));
        else continue;
    }
}