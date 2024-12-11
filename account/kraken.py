import base64
import hashlib
import hmac
import requests
import time

class KrakenAccountClient:
    def __init__(self, credentials):
        self._api_key = credentials.get('api_key', '')
        self._api_secret = credentials.get('secret', '')

    def get_assets(self):
        balances = {}
        query = 'https://api.kraken.com/0/private/Balance'
        timestamp_ms = int(time.time() * 1000)
        headers = {
            'API-Key': self._api_key,
            'API-Sign': self._sign('/0/private/Balance', f'nonce={str(timestamp_ms)}', str(timestamp_ms))
        }
        body = {
            'nonce': timestamp_ms
        }
        result = requests.post(query, data=body, headers=headers)
        if result and len(result.json()['error']) == 0:
            for symbol in result.json()['result']:
                balance = float(result.json()['result'][symbol])
                if balance == 0:
                    continue
                balances[symbol] = balance
        return balances

    def get_account_id(self):
        return 'Kraken'
    
    def _sign(self,url, body, timestamp):
        to_sign = url.encode('utf-8') + hashlib.sha256((timestamp + body).encode('utf-8')).digest()
        digest = hmac.new(base64.b64decode(self._api_secret), to_sign, hashlib.sha512).digest()
        return base64.b64encode(digest).decode()