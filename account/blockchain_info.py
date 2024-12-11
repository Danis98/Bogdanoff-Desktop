import requests

class BlockChainInfoAccountClient:
    def __init__(self, credentials):
        self._addresses = credentials.get('addresses', [])

    def get_assets(self):
        if len(self._addresses) == 0:
            return {}
        query = f'https://blockchain.info/multiaddr?active={"|".join(self._addresses)}'
        response = requests.get(query)
        if response:
            balances = response.json()['addresses']
            balances = [balance['final_balance'] / 1e8 for balance in balances] # Amount is in Satoshis
            return {'BTC': sum(balances)}

    def get_account_id(self):
        return 'Bitcoin'