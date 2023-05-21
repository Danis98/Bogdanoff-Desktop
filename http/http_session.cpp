#include "http/http_session.h"

size_t append_to_buffer(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

bool HttpSession::curl_global_init_done = false;

bool HttpSession::_send(std::string endpoint, const std::string& params, const std::vector<std::string>& additional_headers)
{
    _response_headers.clear();
    _buffer.clear();

    std::string full_url = concatenate_endpoint(_base_url, endpoint);
    if(params.size() != 0)
        full_url += "?" + params;

    curl_easy_setopt(_curl_handle, CURLOPT_URL, full_url.c_str());
    _prepare_headers();
    for(const std::string& header : additional_headers)
        _headers = curl_slist_append(_headers, header.c_str());
    curl_easy_setopt(_curl_handle, CURLOPT_HTTPHEADER, _headers);

    CURLcode result = curl_easy_perform(_curl_handle);
    if(result != CURLE_OK)
        std::cout << "[HttpSession] Could not perform request to " << endpoint << "! Error: " << result << std::endl;

    return result == CURLE_OK;
}

void HttpSession::_prepare_headers()
{
    curl_slist_free_all(_headers);
    _headers = NULL;
    _headers = curl_slist_append(_headers, "Content-Type: application/json");
    _headers = curl_slist_append(_headers, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) Gecko/20100101 Firefox/15.0.1");
}
