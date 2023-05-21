#include "client.h"

#include <iostream>

#include <thread>

Client::Client(const std::string& name)
    : _name(name)
{}

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