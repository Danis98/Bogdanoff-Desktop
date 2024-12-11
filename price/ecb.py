from io import BytesIO
import requests
from zipfile import ZipFile

from price.price_source import PriceSource

class ECBPriceSource(PriceSource):
    def __init__(self, price_cache_path, credentials):
        PriceSource.__init__(self, price_cache_path)
        self._rates = self._get_latest_rates()

    def _get_asset_prices(self, assets):
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
    
    def _get_price_source_tag(self):
        return 'ecb'
