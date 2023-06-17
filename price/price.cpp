#include "price.h"

#include <algorithm>
#include <limits>

PriceSource::PriceSource(const std::string& name)
    : _name(name)
{}

std::map<std::string, double> PriceSource::get_usd_price(const std::vector<std::string>& assets)
{
    // Initial load
    if(!_usd_prices)
        _load_prices(assets);
    
    // Reload if asset is not loaded yet, we might want to change this logic
    for(const std::string& asset : assets)
    {
        if(_usd_prices->find(asset) == _usd_prices->end())
        {
            _load_prices(assets);
            break;
        }
    }

    std::map<std::string, double> prices;
    for(const std::string& asset : assets)
    {
        prices[asset] =
            _usd_prices->find(asset) == _usd_prices->end()
                ? std::numeric_limits<double>::quiet_NaN()
                : (*_usd_prices)[asset];
    }

    return prices;
}

PriceSeries PriceSource::get_usd_price_history(const std::string& asset, uint64_t start, uint64_t end)
{
    //TODO: Add caching, CoinGecko rate limits are quite low

    // Initial load
    if(!_price_history)
        _load_history(asset, start, end);

    // Reload if asset is not loaded yet or if the time range loaded is too narrow
    if(_price_history->find(asset) == _price_history->end())
    {
        const auto& history = (*_price_history).at(asset);
        if(history.start_time() > start || history.end_time() < end)
        {
            _load_history(asset, start, end);
        }
    }

    return (*_price_history).at(asset);
}

PriceSeries::PriceSeries(const std::string& symbol)
    : _symbol(symbol)
{}

void PriceSeries::append(uint64_t ts, double price)
{
    _start_time = (_data.size() == 0 || _start_time > ts) ? ts : _start_time;
    _end_time   = _end_time < ts ? ts : _end_time;

    _data.push_back({ts, price});


    // You're supposed to append in chronological order, but you can never be sure eh?
    if(_data.size() > 1 && ts < _data[_data.size() - 2].first)
    {
        std::sort(_data.begin(), _data.end());
    }
}