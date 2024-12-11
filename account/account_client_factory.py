from account.blockchain_info import BlockChainInfoAccountClient
from account.etherscan import EtherscanAccountClient
from account.bitvavo import BitvavoAccountClient
from account.coinex import CoinexAccountClient
from account.kraken import KrakenAccountClient
from account.interactive_brokers import IBKRAccountClient

def get_account_client(account_client_config):
    if 'venue' not in account_client_config:
        return None

    venue = account_client_config['venue']
    credentials = account_client_config.get('credentials', {})

    if venue == 'blockchain_info':
        return BlockChainInfoAccountClient(credentials)
    elif venue == 'etherscan':
        return EtherscanAccountClient(credentials)
    elif venue == 'bitvavo':
        return BitvavoAccountClient(credentials)
    elif venue == 'coinex':
        return CoinexAccountClient(credentials)
    elif venue == 'kraken':
        return KrakenAccountClient(credentials)
    elif venue == 'interactive_brokers':
        return IBKRAccountClient(credentials)
    else:
        return None