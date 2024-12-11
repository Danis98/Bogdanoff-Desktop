import requests
import time

class EtherscanAccountClient:
    def __init__(self, credentials):
        self._addresses = credentials.get('addresses', [])
        self._api_token = credentials.get('token', '')
        self._spam_filter = credentials.get('spam_filter', '')

    def get_assets(self):
        if len(self._addresses) == 0:
            return {}
        balances = {}

        # Query ETH balances
        query = 'https://api.etherscan.io/api?module=account&action=balancemulti&tag=latest'
        query += f'&address={",".join(self._addresses)}'
        query += f'&apikey={self._api_token}'
        response = requests.get(query)
        if response and response.json()['status'] == '1':
            eth_accounts = response.json()['result']
            eth_balances = [float(accounts['balance']) / 1e18 for accounts in eth_accounts] # Amount is in Wei
            balances['ETH'] = sum(eth_balances)

        # Query token balances
        for address in self._addresses:
            # There is no free endpoint to query all balances, we have to recalc from the transactions
            query = 'https://api.etherscan.io/api?module=account&action=tokentx'
            query += f'&address={address}'
            query += f'&apikey={self._api_token}'
            response = requests.get(query)
            if response and response.json()['status'] == '1':
                for tx in response.json()['result']:
                    send_tx = tx['from'] in self._addresses
                    recv_tx = tx['to'] in self._addresses
                    if send_tx and recv_tx:
                        # Filter out self-transfers
                        continue

                    token = tx['tokenSymbol']
                    if token in self._spam_filter:
                        continue

                    amount = float(tx['value']) / (10**int(tx['tokenDecimal']))
                    if token not in balances:
                        balances[token] = 0
                    if send_tx:
                        balances[token] -= amount
                    elif recv_tx:
                        balances[token] += amount
            time.sleep(0.5) # Throttled at 2 calls/s
        
        return balances

    def get_account_id(self):
        return 'Ethereum'