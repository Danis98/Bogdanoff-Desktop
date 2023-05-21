#include "http_util.h"

std::string concatenate_endpoint(const std::string& url, const std::string& endpoint)
{
    std::string res(url);
    
    int slash_ctr = url[url.size() - 1] == '/';
    slash_ctr    += endpoint[0] == '/';

    if(slash_ctr == 0)
        res += '/';
    else if(slash_ctr == 2)
        res.pop_back();
    
    res += endpoint;

    return res;
}

GetParams::GetParams(const json& params_json)
{
    for(auto& it : params_json.items())
    {
        _params[it.key()] = it.value();
    }
}

std::string GetParams::to_string() const
{
    std::string params_str = "";
    for(auto& it : _params)
    {
        if(params_str.size() != 0)
            params_str += '&';
        params_str += it.first + "=" + it.second;
    }
    return params_str;
}