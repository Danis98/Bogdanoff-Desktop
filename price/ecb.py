from io import BytesIO
import requests
from zipfile import ZipFile

class ECBPriceSource:
    def __init__(self, credentials):
        self._rates = self._get_latest_rates()

    def get_asset_prices(self, assets):
        prices = {asset: 0 for asset in assets}
        for asset in assets:
            if asset in self._rates:
                prices[asset] = self._rates[asset]
        return prices
    
    def _get_latest_rates(self):
        rates = {'EUR': 1.0}
        response = requests.get('http://www.ecb.europa.eu/stats/eurofxref/eurofxref-hist.zip')
        if response:
            unzipped = ZipFile(BytesIO(response.content))
            csv_unzipped = unzipped.open('eurofxref-hist.csv')
            header = csv_unzipped.readline().decode().strip()
            latest = csv_unzipped.readline().decode().strip()
            ccys = header.split(',')
            eurusd = 1
            for idx, value in enumerate(latest.split(',')):
                if idx == 0 or len(value) == 0:
                    continue
                ccy = ccys[idx]
                if value == 'N/A':
                    continue
                if ccy == 'USD':
                    eurusd = float(value)
                else:
                    rates[ccy] = float(value)
            for ccy in rates:
                rates[ccy] = eurusd / rates[ccy]
            rates['USD'] = 1.0
        return rates
