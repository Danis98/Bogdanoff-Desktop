import requests

from price.price_source import PriceSource

class AlphaVantagePriceSource(PriceSource):
    def __init__(self, price_cache_path, credentials):
        PriceSource.__init__(self, price_cache_path)
        self._api_key = credentials.get('api_key', '')

    def _get_asset_prices(self, assets):
        prices = {asset: 0 for asset in assets}
        for asset in assets:
            request = 'https://www.alphavantage.co/query?function=GLOBAL_QUOTE'
            request += f'&symbol={asset}'
            request += f'&apikey={self._api_key}'
            response = requests.get(request)
            print(response.json())
            if response and 'Global Quote' in response.json():
                prices[asset] = float(response.json()['Global Quote']['05. price'])
        return prices
    
    def _get_price_source_tag(self):
        return 'alpha_vantage'