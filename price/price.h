#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

class PriceSource
{
public:
    PriceSource(const std::string& name);

    std::map<std::string, double> get_usd_price(const std::vector<std::string>& asset);

    const std::string& get_name() const { return _name; }
protected:
    virtual void _load_prices(const std::vector<std::string>& assets) = 0;

    std::string _name;

    std::optional<std::map<std::string, double>> _usd_prices;
};