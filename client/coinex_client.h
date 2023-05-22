#pragma once

#include "http/http_session.h"
#include "json/json.hpp"

#include "client.h"

constexpr char COINEX_BASE_URL[] = "https://api.coinex.com/v1";

class CoinexClient : public Client
{
public:
    CoinexClient(const json& coinex_json);

    bool load_balances() noexcept override;

    double current_balance_usd() noexcept override {return 0;} // No price info yet

private:
    bool _load_transaction_page(const int page) override;
    std::string _sign(const GetParams& params);

    HttpSession _session;

    std::string _access_id;
    std::string _secret;
};