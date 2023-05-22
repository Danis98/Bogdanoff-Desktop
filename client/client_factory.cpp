#include "blockchain_info_client.h"
#include "coinex_client.h"
#include "etherscan_client.h"

void create_clients(const json& config_json, std::vector<std::unique_ptr<Client>>& clients)
{
    for(const auto& client_config : config_json)
    {
        if(client_config["enabled"].get<std::string>() == "false")
            continue;
        if(client_config["venue"] == "coinex")
            clients.push_back(std::make_unique<CoinexClient>(client_config));
        else if(client_config["venue"] == "etherscan")
            clients.push_back(std::make_unique<EtherscanClient>(client_config));
        else if(client_config["venue"] == "blockchain_info")
            clients.push_back(std::make_unique<BlockchainInfoClient>(client_config));
        else continue;
    }
}