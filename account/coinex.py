import hashlib
import hmac
import requests
import time

class CoinexAccountClient:
    def __init__(self, credentials):
        self._api_key = credentials.get('access_id', '')
        self._api_secret = credentials.get('secret', '')

    def get_assets(self):
        balances = {}
        query = 'https://api.coinex.com/v2/assets/spot/balance'
        timestamp_ms = str(int(time.time() * 1000))
        headers = {
            'X-COINEX-KEY': self._api_key,
            'X-COINEX-TIMESTAMP': timestamp_ms,
            'X-COINEX-SIGN': self._sign('GET', '/v2/assets/spot/balance', '', timestamp_ms)
        }
        result = requests.get(query, headers=headers)
        if result and result.json()['code'] == 0:
            for symbol_balance in result.json()['data']:
                balance = float(symbol_balance['available']) + float(symbol_balance['frozen'])
                if balance == 0:
                    continue
                balances[symbol_balance['ccy']] = balance
        return balances

    def get_account_id(self):
        return 'Coinex'
    
    def _sign(self, method, url, body, timestamp):
        to_sign = method + url + body + timestamp
        return hmac.new(self._api_secret.encode('utf-8'), to_sign.encode('utf-8'), hashlib.sha256).hexdigest().lower()