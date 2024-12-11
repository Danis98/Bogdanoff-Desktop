import base64
import csv
import hashlib
import hmac
import requests
import time

class IBKRAccountClient:
    def __init__(self, credentials):
        csv_path = credentials.get('csv_path', '')
        self._process_csv(csv_path)

    def get_assets(self):
        return self._balances

    def get_account_id(self):
        return 'Interactive Brokers'
    
    def _process_csv(self, csv_path):
        self._balances = {'EUR': 0, 'USD': 0}

        with open(csv_path, "r") as f:
            csv_reader = csv.reader(f)

            for line in csv_reader:
                if len(line) == 0:
                    continue
                if line[0] in ['Deposits & Withdrawals', 'Dividends', 'Withholding Tax']:
                    ccy = line[2]
                    amount = float(line[5])
                    if ccy not in self._balances:
                        self._balances[ccy] = 0
                    self._balances[ccy] += amount
                elif line[0] == 'Trades':
                    asset_class = line[3]
                    ccy = line[4]
                    symbol = line[5]
                    if asset_class == 'Forex':
                        front_leg, back_leg = symbol.split('.')
                        front_amount = float(line[7].replace(',', ''))
                        back_amount = float(line[10].replace(',', ''))
                        eur_fee = float(line[11].replace(',', ''))
                        if front_leg not in self._balances:
                            self._balances[front_leg] = 0
                        if back_leg not in self._balances:
                            self._balances[back_leg] = 0
                        self._balances[front_leg] += front_amount
                        self._balances[back_leg] += back_amount
                        self._balances['EUR'] += eur_fee
                    elif asset_class == 'Stocks':
                        shares = float(line[7].replace(',', ''))
                        notional = float(line[10].replace(',', ''))
                        fee = float(line[11].replace(',', ''))
                        if ccy not in self._balances:
                            self._balances[ccy] = 0
                        if symbol not in self._balances:
                            self._balances[symbol] = 0
                        self._balances[symbol] += shares
                        self._balances[ccy] += notional + fee
                        if self._balances[symbol] == 0:
                            del self._balances[symbol]
                    else:
                        print('Unknown asset class, cannot parse safely')
