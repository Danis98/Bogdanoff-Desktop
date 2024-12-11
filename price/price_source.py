from datetime import datetime

class PriceSource:
    def __init__(self, price_cache_path):
        self._price_cache_path = price_cache_path

        self._price_cache = {}
        with open(price_cache_path, 'r') as cache:
            cache_lines = cache.read().strip().split('\n')
            for line in cache_lines:
                if len(line) == 0:
                    continue
                price_date, price_source_tag, symbol, price = line.split(',')
                if self._get_price_source_tag() != price_source_tag:
                    continue
                if symbol not in self._price_cache:
                    self._price_cache[symbol] = {}
                self._price_cache[symbol][price_date] = float(price)

    def get_asset_prices(self, assets):
        price_date = datetime.today().strftime('%Y-%m-%d')
        assets_to_query = []
        prices = {}
        for asset in assets:
            if asset not in self._price_cache:
                self._price_cache[asset] = {}
            if price_date not in self._price_cache[asset]:
                assets_to_query.append(asset)
            else:
                prices[asset] = self._price_cache[asset][price_date]

        cur_prices_dict = self._get_asset_prices(assets_to_query)
        prices.update(cur_prices_dict)

        with open(self._price_cache_path, 'a') as cache_file:
            for asset in cur_prices_dict:
                self._price_cache[asset][price_date] = cur_prices_dict[asset]
                cache_file.write(f'{price_date},{self._get_price_source_tag()},{asset},{cur_prices_dict[asset]}\n')
        
        return prices
    
    def _get_price_source_tag(self):
        return ''