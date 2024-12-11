from bs4 import BeautifulSoup
import json
import requests
import struct
import base64
from websockets.sync.client import connect

# Some symbols like EQQQ.MI do not work in the REST API for some reason, and we gotta do this overengineered thing
# This is slow since Yahoo sometimes just waits forever to send something, especially on non-USEQ symbols
class YahooFinanceStreamPriceSource:
    def __init__(self, credentials):
        pass

    def get_asset_prices(self, assets):
        prices = {asset: 0 for asset in assets}
        read_assets = set()
        with connect("wss://streamer.finance.yahoo.com/?version=2") as websocket:
            sub_message = json.dumps({'subscribe': assets})
            print(sub_message)
            websocket.send(sub_message)
            while len(read_assets) != len(set(assets)):
                message = json.loads(websocket.recv())
                if message['type'] != 'pricing':
                    continue
                encoded = base64.b64decode(message['message'])
                print(encoded)
                symbol, price = self._decode_proto(encoded)
                if symbol not in read_assets:
                    read_assets.add(symbol)
                    prices[symbol] = round(price * 100.0) / 100.0
                
        return prices

    # https://github.com/yahoofinancelive/yliveticker/blob/master/yliveticker/yaticker.proto
    def _decode_proto(self, encoded):
        def read_varint(buffer):
            num = 0
            read_len = 0
            for byte in buffer:
                continuation_bit = int(byte) >> 7
                val = int(byte) & 0x7f
                num = (num << 8) | val
                read_len += 1
                if continuation_bit == 0:
                    break
            return (read_len, num)
        fields = {}
        cursor = 0
        while cursor < len(encoded):
            field_tag = int(encoded[cursor])
            field_idx = field_tag >> 3
            field_type = field_tag & 0x7
            if field_type == 0: # Varint
                read_bytes, value = read_varint(encoded[cursor+1:])
                fields[field_idx] = encoded[cursor+1:cursor+read_bytes+1]
                cursor += read_bytes + 1
            elif field_type == 2: # Length-delimited
                read_bytes, strlen = read_varint(encoded[cursor+1:])
                str_start = cursor+read_bytes+1
                fields[field_idx] = encoded[str_start:str_start+strlen]
                cursor += read_bytes + strlen + 1
            elif field_type == 1: # I64
                value = encoded[cursor+1:cursor+9]
                fields[field_idx] = value
                cursor += 9
            elif field_type == 5: # I32
                value = encoded[cursor+1:cursor+5]
                fields[field_idx] = value
                cursor += 5
            else:
                break
        # We only care about symbol, price
        if 2 not in fields or len(fields[2]) != 4:
            price = 0.0
        else:
            price = struct.unpack('f', fields[2])[0]
        return (fields[1].decode(), price)
