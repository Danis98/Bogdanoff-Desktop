#pragma once

#include <iostream>
#include <string>

#include <curl/curl.h>

#include "http_util.h"

size_t append_to_buffer(void* ptr, size_t size, size_t nmemb, std::string* data);

class HttpSession
{
public:
    HttpSession(const std::string& base_url)
    : _base_url(base_url)
    {
        if(!curl_global_init_done)
        {
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl_global_init_done = true;
        }

        _curl_handle = curl_easy_init();

        if(!_curl_handle)
        {
            std::cout << "COULD NOT INITIALIZE CURL HANDLE!" << std::endl;
            return;
        }
        
        curl_easy_setopt(_curl_handle, CURLOPT_WRITEFUNCTION, append_to_buffer);
        curl_easy_setopt(_curl_handle, CURLOPT_WRITEDATA, &_buffer);
        curl_easy_setopt(_curl_handle, CURLOPT_HEADERDATA, &_response_headers);
    }

    ~HttpSession()
    {
        curl_easy_cleanup(_curl_handle);
        curl_slist_free_all(_headers);
    }

    bool get(const std::string& endpoint, const std::string& params = "", const std::vector<std::string>& additional_headers = {})
    {
        return _send(endpoint, params, additional_headers);
    }

    bool post(const std::string& endpoint, const std::string& data, const std::vector<std::string>& additional_headers = {})
    {
        curl_easy_setopt(_curl_handle, CURLOPT_POSTFIELDS, data.c_str());
        return _send(endpoint, "", additional_headers);
    }

    std::string_view read_buffer()
    {
        return _buffer;
    }

    std::string_view read_response_headers()
    {
        return _response_headers;
    }

private:
    void _prepare_headers();
    bool _send(std::string endpoint, const std::string& params, const std::vector<std::string>& additional_headers);

    const std::string _base_url = "";

    CURL *_curl_handle;
    curl_slist *_headers = NULL;
    static bool curl_global_init_done;

    std::string _response_headers = "";
    std::string _buffer = "";
};
