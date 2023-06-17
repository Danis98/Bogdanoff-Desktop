#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

class PriceSeries;

class PriceSource
{
public:
    PriceSource(const std::string& name);

    std::map<std::string, double> get_usd_price(const std::vector<std::string>& assets);
    PriceSeries get_usd_price_history(const std::string& asset, uint64_t start, uint64_t end);

    const std::string& get_name() const { return _name; }
protected:
    virtual void _load_prices(const std::vector<std::string>& assets) = 0;
    virtual void _load_history(const std::string& asset, uint64_t start, uint64_t end) = 0;

    std::string _name;

    std::optional<std::map<std::string, double>>      _usd_prices;
    std::optional<std::map<std::string, PriceSeries>> _price_history;
};

class PriceSeries
{
public:
    PriceSeries(const std::string& symbol);

    void append(uint64_t ts, double price);

    uint64_t start_time() const { return _start_time; }
    uint64_t end_time() const { return _end_time; }
private:
    uint64_t                                 _start_time = 0;
    uint64_t                                 _end_time = 0;
    std::string                              _symbol;
    std::vector<std::pair<uint64_t, double>> _data;
};