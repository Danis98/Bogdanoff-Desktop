from price.coinmarketcap import CoinMarketCapPriceSource
from price.alphavantage import AlphaVantagePriceSource
from price.ecb import ECBPriceSource
from price.yahoo_finance import YahooFinancePriceSource
from price.yahoo_finance_stream import YahooFinanceStreamPriceSource

def get_price_source(account_client_config):
    if 'source' not in account_client_config:
        return None

    source = account_client_config['source']
    credentials = account_client_config.get('credentials', {})

    if source == 'coinmarketcap':
        return CoinMarketCapPriceSource(credentials)
    elif source == 'alphavantage':
        return AlphaVantagePriceSource(credentials)
    elif source == 'ecb':
        return ECBPriceSource(credentials)
    elif source == 'yahoo_finance':
        return YahooFinancePriceSource(credentials)
    elif source == 'yahoo_finance_stream':
        return YahooFinanceStreamPriceSource(credentials)
    else:
        return None