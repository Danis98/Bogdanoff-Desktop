#include "coingecko_price.h"

#include <set>

CoinGeckoPrice::CoinGeckoPrice(const json& coingecko_json)
    : Price("coingecko")
    , _session(COINGECKO_BASE_URL)
{}


void CoinGeckoPrice::_load_asset_ids()
{
    std::cout << "Loading CoinGecko symbol list" << std::endl;

    // Only holding L1 and ERC-20 tokens, filter out the junk to avoid name collisions
    const std::set<std::string> ALLOWED_CHAINS = {"ethereum"};

    GetParams params(json({
        {"include_platform", "true"}
    }));

    bool success = _session.get("/coins/list", params.to_string());
    if(!success)
        return;
    const json coins = json::parse(_session.read_buffer());

    if(coins.contains("status") && coins["status"].contains("error_code"))
    {
        std::cerr << "ERROR while loading CoinGecko coin ids: " << coins["status"]["error_message"] << std::endl;
        return;
    }

    if(!_asset_ids)
        _asset_ids = std::make_optional<std::map<std::string, std::string>>();

    for(const auto& it : coins)
    {
        std::string id = it["id"];
        std::string symbol = it["symbol"];
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), toupper);

        const json& chains = it["platforms"];
        bool valid_chain = true;
        for(const auto& it : chains.items())
        {
            valid_chain = false;
            if(ALLOWED_CHAINS.find(it.key()) != ALLOWED_CHAINS.end())
            {
                valid_chain = true;
                break;
            }
        }

        if(valid_chain)
            (*_asset_ids)[symbol] = id;
    }
}

void CoinGeckoPrice::_load_prices(const std::vector<std::string>& assets)
{
    std::cout << "Loading CoinGecko price for ";
    for(const std::string& asset : assets)
        std::cout << asset << " ";
    std::cout << std::endl;

    if(!_usd_prices)
        _usd_prices = std::make_optional<std::map<std::string, double>>();
    
    if(!_asset_ids)
        _load_asset_ids();
    
    std::string asset_list = "";
    for(const std::string& asset : assets)
        asset_list += (asset_list.size() == 0 ? "" : ",") + (*_asset_ids)[asset];

    GetParams params({
        {"ids", asset_list},
        {"vs_currencies", "usd"}
    });

    bool success = _session.get("/simple/price", params.to_string());
    if(!success)
        return;
    const json prices = json::parse(_session.read_buffer());

    for(const auto& it : prices.items())
    {
        std::string id = it.key();
        for(const std::string& asset : assets)
            if((*_asset_ids)[asset] == id)
                (*_usd_prices)[asset] = it.value()["usd"];
    }
}