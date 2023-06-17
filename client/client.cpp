#include "client.h"

#include <iostream>

#include <thread>

Client::Client(const std::string& name, PriceSource& price_source)
    : _name(name)
    , _price_source(price_source)
{}


std::map<std::string, double> Client::current_balance_usd() noexcept
{
    if(!_balances)
        load_balances();
    
    std::map<std::string, double> usd_balances;
    std::vector<std::string> assets;
    for(auto const& it: *_balances)
        assets.push_back(it.first);
    
    std::map<std::string, double> prices = _price_source.get_usd_price(assets);

    for(auto const& asset: assets)
    {
        usd_balances[asset] = (*_balances)[asset] * prices[asset];
    }

    return usd_balances;
}

bool Client::load_transactions() noexcept
{
    if(!_transactions)
        _transactions = std::make_optional<std::vector<Transaction>>();

    size_t page_to_load = 0; // pages are 0-indexed internally, the client has to increment to get 1-indexed
    bool has_next = true;

    while(has_next)
    {
        try
        {
            has_next = _load_transaction_page(page_to_load);
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << "Could not load transactions for venue " << _name << std::endl;
            std::cerr << e.what() << std::endl;
            return false;
        }

        page_to_load++;
    }

    return true;
}

bool Client::_calculate_balances_from_transactions()
{
    if(!_transactions)
        if(!load_transactions())
            return false;
    for(const Transaction& transaction : *_transactions)
    {
        switch(transaction.direction)
        {
        case Transaction::Direction::INCOMING:
            (*_balances)[transaction.asset] += transaction.qty;
            break;
        case Transaction::Direction::OUTGOING:
            (*_balances)[transaction.asset] -= transaction.qty;
            break;
        }
    }
    return true;
}
