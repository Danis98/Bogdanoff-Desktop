from bs4 import BeautifulSoup
import json
import requests
import re
from websockets.sync.client import connect

class YahooFinancePriceSource:
    def __init__(self, credentials):
        pass

    def get_asset_prices(self, assets):
        prices = {asset: 0 for asset in assets}
        for asset in assets:
            print(asset)
            response = requests.get(f'https://finance.yahoo.com/quote/{asset}/', allow_redirects=True)
            if response:
                soup = BeautifulSoup(response.content, features="html.parser")
                if asset == 'EQQQ.MI':
                    with open('eqqq.txt', 'w') as f:
                        f.write(soup.prettify())
                regex = re.compile(f'https://query1.finance.yahoo.com/v10/finance/quoteSummary/{asset}.*')
                found = soup.find_all('script', {'data-url': regex})
                if len(found) == 0:
                    continue
                symbol_info = json.loads(found[0].get_text())['body']
                symbol_info = json.loads(symbol_info)
                prices[asset] = symbol_info['quoteSummary']['result'][0]['price']['regularMarketPrice']['raw']
            else:
                print("ERROR")
                print(response.content)

        return prices
