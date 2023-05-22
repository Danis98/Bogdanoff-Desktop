#include "blockchain_info_client.h"

BlockchainInfoClient::BlockchainInfoClient(const json& blockchain_info_json)
    : Client("Blockchain.info")
    , _session(BLOCKCHAIN_INFO_BASE_URL)
{
    for(const auto& it : blockchain_info_json["addresses"])
    {
        _addresses.push_back(it.get<std::string>());
    }
}

 bool BlockchainInfoClient::load_balances() noexcept
 { 
    if(!_balances)
        _balances = std::make_optional<std::map<std::string, double>>();
    
    std::cout << "Loading Blockchain.info balances" << std::endl;

    bool success = _session.get(std::string("/addressbalance/") + _addresses[0]);
    if(!success)
        return false;

    uint64_t balance_satoshis = std::stoull(std::string(_session.read_buffer()));

    (*_balances)["BTC"] = (double) balance_satoshis / 100'000'000ULL;

    return true;
}

// Blockchain.info does not expose transaction lists
bool BlockchainInfoClient::_load_transaction_page(const int page)
{
    return true;
}