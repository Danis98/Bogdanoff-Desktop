#pragma once

#include "http/http_session.h"
#include "json/json.hpp"

#include "client.h"

constexpr char BLOCKCHAIN_INFO_BASE_URL[] = "https://blockchain.info/q";

class BlockchainInfoClient : public Client
{
public:
    BlockchainInfoClient(const json& blockchain_info_json);
    
    bool load_balances() noexcept override;

    double current_balance_usd() noexcept override {return 0;} // No price info yet

private:
    bool _load_transaction_page(const int page) override;
    bool _load_etherscan_transaction_page(const int page, bool token);

    HttpSession              _session;

    std::vector<std::string> _addresses;
};