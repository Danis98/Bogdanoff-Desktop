#pragma once

#include "http/http_session.h"
#include "json/json.hpp"

#include "client.h"

constexpr char ETHERSCAN_BASE_URL[] = "https://api.etherscan.io/api";

class EtherscanClient : public Client
{
public:
    EtherscanClient(const json etherscan_json);
    
    bool load_balances() noexcept override { 
        if(!_balances)
            _balances = std::make_optional<std::map<std::string, double>>();
        return _calculate_balances_from_transactions();
    }

    double current_balance_usd() noexcept override {return 0;} // No price info yet

private:
    bool _load_transaction_page(const int page) override;
    bool _load_etherscan_transaction_page(const int page, bool token);

    HttpSession              _session;

    std::string                        _api_token;
    std::map<std::string, std::string> _contract_to_token;
    std::vector<std::string>           _addresses;
};