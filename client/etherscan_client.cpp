#include "etherscan_client.h"

#include <algorithm>
#include <cstdint>
#include <string>

EtherscanClient::EtherscanClient(const json etherscan_json)
    : Client("Etherscan")
    , _session(std::string(ETHERSCAN_BASE_URL))
    , _api_token(etherscan_json["token"])
{
    for(const auto& it : etherscan_json["public_addresses"])
    {
        _addresses.push_back(it.get<std::string>());
    }
}

double parse_value(const std::string& value, size_t multiplier)
{
    std::string s = value;
    if(s.size() < multiplier)
        s.insert(s.begin(), multiplier - s.size(), '0');
    s.insert(s.begin() + s.size() - multiplier, '.');
    return std::stod(s);
}

bool EtherscanClient::_load_etherscan_transaction_page(const int page, const bool token)
{
    std::cout << "Loading Etherscan transaction page " << page << ", token = " << token << std::endl;
    // Etherscan does not tell us if there is a next page, we try the next one until we get an empty one
    GetParams params({
        {"module", "account"},
        {"action", token ? "tokentx" : "txlist"},
        {"address", _addresses[0]}, // Deal with multiple addresses later
        {"startblock", "0"},
        {"endblock", "99999999"},
        {"page", std::to_string(page + 1)},
        {"offset", "100"},
        {"sort", "asc"},
        {"apikey", _api_token}
    });

    bool success = _session.get("", params.to_string());
    if(!success)
        return false;

    const json transactions = json::parse(_session.read_buffer());
    if(transactions["message"].get<std::string>() != "OK")
        return false;

    for(const auto& it : transactions["result"])
    {
        Transaction transaction;

        transaction.direction = it["from"] == _addresses[0] ? Transaction::Direction::OUTGOING : Transaction::Direction::INCOMING;
        transaction.asset = token ? it["tokenSymbol"].get<std::string>() : "ETH";
        transaction.qty = parse_value(it["value"].get<std::string>(), token ? std::stoi(it["tokenDecimal"].get<std::string>()) : 18);
        transaction.account_from = it["from"];
        transaction.account_to = it["to"];

        if(!_transactions)
            *_transactions = {};
        
        _transactions->push_back(transaction);
    }
    return true;
}

bool EtherscanClient::_load_transaction_page(const int page)
{
    // Hack: we query both normal and token transactions, and return has_next=false if BOTH fail
    bool eth_success   = _load_etherscan_transaction_page(page, false);
    bool token_success = _load_etherscan_transaction_page(page, true);

    return eth_success || token_success;
}