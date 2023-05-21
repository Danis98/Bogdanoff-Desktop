#pragma once

#include "http/http_session.h"
#include "json/json.hpp"

constexpr char BITCOIN_BASE_URL[] = "https://api.coinex.com/v1";

class CoinexClient
{
public:
    CoinexClient(const json coinex_json);

    const json list_markets();
    const json account_balance();
    const json account_history(const int page, const int limit);
private:
    std::string _sign(const GetParams& params);

    HttpSession _session;

    std::string _access_id;
    std::string _secret;
};