#pragma once

#include <map>
#include <string>

#include "json/json.hpp"

std::string concatenate_endpoint(const std::string& url, const std::string& endpoint);

class GetParams
{
public:
    GetParams(const json& params_json);

    std::string to_string() const;
private:
    // std::map since order matters sometimes
    std::map<std::string, std::string> _params;
};