#include "coinex_client.h"

#include <algorithm>
#include <string>

#include "chrono/timing.h"
#include "crypto/md5.h"

CoinexClient::CoinexClient(const json coinex_json)
    : Client("CoinEx")
    , _session(std::string(COINEX_BASE_URL))
    , _access_id(coinex_json["access_id"])
    , _secret(coinex_json["secret"])
{}

bool CoinexClient::load_balances() noexcept
{
    if(!_balances)
        _balances = std::make_optional<std::map<std::string, double>>();

    std::cout << "Loading CoinEx balances" << std::endl;

    GetParams params({
        {"access_id", _access_id},
        {"tonce", std::to_string(current_time_millis())}
    });
    std::string signature = _sign(params);
    
    bool success = _session.get("/balance/info", params.to_string(), {std::string("Authorization: ") + signature});
    if(!success)
        return false;
    const json balances = json::parse(_session.read_buffer());

    if(balances["code"].get<int>() != 0)
        return false;

    _balances->clear();
    for(const auto& it : balances["data"].items())
    {
        double available = std::stod(it.value()["available"].get<std::string>());
        double frozen    = std::stod(it.value()["frozen"].get<std::string>());
        (*_balances)[it.key()] = available + frozen;
    }

    return true;
}

bool CoinexClient::_load_transaction_page(const int page)
{
    GetParams params({
        {"access_id", _access_id},
        {"page", std::to_string(page + 1)},
        {"limit", "100"},
        {"tonce", std::to_string(current_time_millis())}
    });
    std::string signature = _sign(params);
    
    bool success = _session.get("/account/balance/history", params.to_string(), {std::string("Authorization: ") + signature});
        if(!success)
        return false;

    const json transactions = json::parse(_session.read_buffer());
    if(transactions["status"].get<std::string>() != "1")
        return false;

    for(const auto& it : transactions["result"])
    {
        Transaction transaction;

        transaction.direction = it["change"].get<std::string>()[0] == '-' ? Transaction::Direction::OUTGOING : Transaction::Direction::INCOMING;
        transaction.asset = it["asset"];
        transaction.qty = std::abs(std::stof(it["change"].get<std::string>())); // If we have more than 18 ETH this will overflow
        transaction.account_from = "";
        transaction.account_to = "";

        if(!_transactions)
            *_transactions = {};
        
        _transactions->push_back(transaction);
    }
    return true;
}

std::string CoinexClient::_sign(const GetParams& params)
{
    std::string to_sign = params.to_string();
    to_sign += "&secret_key=" + _secret;
    std::string signature = md5(to_sign);
    std::transform(signature.begin(), signature.end(), signature.begin(), ::toupper);
    return signature;
}