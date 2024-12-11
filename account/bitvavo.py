import hashlib
import hmac
import requests
import time

class BitvavoAccountClient:
    def __init__(self, credentials):
        self._api_key = credentials.get('api_key', '')
        self._api_secret = credentials.get('secret', '')

    def get_assets(self):
        balances = {}
        query = 'https://api.bitvavo.com/v2/balance'
        timestamp_ms = str(int(time.time() * 1000))
        headers = {
            'Bitvavo-Access-Key': self._api_key,
            'Bitvavo-Access-Timestamp': timestamp_ms,
            'Bitvavo-Access-Signature': self._sign(timestamp_ms, 'GET', '/v2/balance', '')
        }
        result = requests.get(query, headers=headers)
        if result:
            for symbol_balance in result.json():
                balance = float(symbol_balance['available']) + float(symbol_balance['inOrder'])
                if balance == 0:
                    continue
                balances[symbol_balance['symbol']] = balance
        return balances

    def get_account_id(self):
        return 'Bitvavo'
    
    def _sign(self, timestamp, method, url, body):
        to_sign = timestamp + method + url + body
        return hmac.new(self._api_secret.encode('utf-8'), to_sign.encode('utf-8'), hashlib.sha256).hexdigest()