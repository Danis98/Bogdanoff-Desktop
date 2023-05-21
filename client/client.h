#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <iostream>

struct Transaction
{
    enum class Direction : uint8_t
    {
        INCOMING,
        OUTGOING
    };

    Direction   direction;
    std::string asset;
    double      qty;
    std::string account_from;
    std::string account_to;
};

class Client
{
public:
    Client(const std::string& name);

    // Current balance by asset
    const std::map<std::string, double>& current_balance() noexcept
    {
        if(!_balances)
            load_balances();
        return *_balances;
    }
    virtual bool load_balances() noexcept = 0;

    // Current balance in USD
    virtual double current_balance_usd() noexcept = 0;

    // Load transactions from venue, handles pagination
    // Returns true on loading success
    bool load_transactions() noexcept;

    const std::string& get_name() const noexcept { return _name; }

protected:
    // Return true if there is a next page to read
    // Throws on fail
    virtual bool _load_transaction_page(const int page) =0;

    // Return true on success
    bool _calculate_balances_from_transactions();

    std::string                                  _name;
    std::optional<std::map<std::string, double>> _balances;
    std::optional<std::vector<Transaction>>      _transactions;
};