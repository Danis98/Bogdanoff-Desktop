import requests

class AlphaVantagePriceSource:
    def __init__(self, credentials):
        self._api_key = credentials.get('api_key', '')

    def get_asset_prices(self, assets):
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