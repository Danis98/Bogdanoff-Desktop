import requests

from price.price_source import PriceSource

class CoinMarketCapPriceSource(PriceSource):
    def __init__(self, price_cache_path, credentials):
        PriceSource.__init__(self, price_cache_path)
        self._api_key = credentials.get('api_key', '')

    def _get_asset_prices(self, assets):
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
    
    def _get_price_source_tag(self):
        return 'coin_market_cap'
