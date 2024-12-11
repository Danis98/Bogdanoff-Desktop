from price.coinmarketcap import CoinMarketCapPriceSource
from price.alphavantage import AlphaVantagePriceSource
from price.ecb import ECBPriceSource
from price.yahoo_finance import YahooFinancePriceSource
from price.yahoo_finance_stream import YahooFinanceStreamPriceSource

def get_price_source(account_client_config, price_cache_path):
    if 'source' not in account_client_config:
        return None

    source = account_client_config['source']
    credentials = account_client_config.get('credentials', {})

    print(source)

    if source == 'coin_market_cap':
        return CoinMarketCapPriceSource(price_cache_path, credentials)
    elif source == 'alphavantage':
        return AlphaVantagePriceSource(price_cache_path, credentials)
    elif source == 'ecb':
        return ECBPriceSource(price_cache_path, credentials)
    elif source == 'yahoo_finance':
        return YahooFinancePriceSource(price_cache_path, credentials)
    elif source == 'yahoo_finance_stream':
        return YahooFinanceStreamPriceSource(price_cache_path, credentials)
    else:
        return None