import requests

import json

class CoinMarketCapPriceSource:
    def __init__(self, credentials):
        self._api_key = credentials.get('api_key', '')

    def get_asset_prices(self, assets):
        prices = {asset: 0 for asset in assets}
        headers = {
            'X-CMC_PRO_API_KEY': self._api_key
        }
        response = requests.get(f' https://pro-api.coinmarketcap.com/v2/cryptocurrency/quotes/latest?slug={",".join(assets)}', headers=headers)
        if response:
            for symbol in response.json()['data']:
                coin_info = response.json()['data'][symbol]
                slug = coin_info['slug']
                price = float(coin_info['quote']['USD']['price'] or '0.0')
                prices[slug] = price
        return prices
