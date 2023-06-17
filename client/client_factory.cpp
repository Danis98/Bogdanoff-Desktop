#include "client_factory.h"
#include "blockchain_info_client.h"
#include "coinex_client.h"
#include "etherscan_client.h"

void create_clients(const json& config_json, std::vector<std::unique_ptr<Client>>& clients, const std::vector<std::unique_ptr<PriceSource>>& price_sources)
{
    for(const auto& client_config : config_json)
    {
        if(client_config["enabled"].get<std::string>() == "false")
            continue;

        size_t price_idx;
        for(price_idx = 0; price_idx < price_sources.size(); price_idx++)
        {
            if(price_sources[price_idx]->get_name() == client_config["price"])
                break;
        }
        if(price_idx == price_sources.size())
        {
            std::cerr << "Could not find price for " << client_config["price"] << std::endl;
            continue;
        }

        if(client_config["venue"] == "coinex")
            clients.push_back(std::make_unique<CoinexClient>(client_config, *price_sources[price_idx]));
        else if(client_config["venue"] == "etherscan")
            clients.push_back(std::make_unique<EtherscanClient>(client_config, *price_sources[price_idx].get()));
        else if(client_config["venue"] == "blockchain_info")
            clients.push_back(std::make_unique<BlockchainInfoClient>(client_config, *price_sources[price_idx].get()));
        else continue;
    }
}