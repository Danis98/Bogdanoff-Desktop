#include "price.h"

#include <limits>

Price::Price(const std::string& name)
    : _name(name)
{}

std::map<std::string, double> Price::get_usd_price(const std::vector<std::string>& assets)
{
    if(!_usd_prices)
        _load_prices(assets);
    
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