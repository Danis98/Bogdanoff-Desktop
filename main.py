import argparse
import json

from account.account_client_factory import get_account_client
from price.price_source_factory import get_price_source

def main(args):
    with open(args.config_path, "r") as config_file:
        config = json.load(config_file)
    with open(args.symbol_data_path, "r") as symbol_data_file:
        symbol_data = json.load(symbol_data_file)

    account_clients = []
    for account_client_config in config['account_clients']:
        account_client = get_account_client(account_client_config)
        if account_client is not None:
            account_clients.append(account_client)

    price_sources = {'constant': None}
    for price_source_config in config['price_sources']:
        price_source = get_price_source(price_source_config)
        if price_source is not None:
            price_sources[price_source_config['source']] = price_source

    print(f'Loaded {len(account_clients)} account clients')
    print(f'Loaded {len(price_sources)} price sources')

    account_assets = {client.get_account_id(): client.get_assets() for client in account_clients}
    
    asset_balance = {}
    for account in account_assets:
        for asset in account_assets[account]:
            if asset not in asset_balance:
                asset_balance[asset] = account_assets[account][asset]
            else:
                asset_balance[asset] += account_assets[account][asset]

    symbol_id_reverse_map = {}
    symbol_ids_to_query = {source : [] for source in price_sources}
    for asset in asset_balance:
        if asset not in symbol_data:
            continue
        price_source = symbol_data[asset]['price_source']
        if price_source not in price_sources:
            continue
        symbol_ids_to_query[price_source].append(symbol_data[asset]['symbol_id'])
        reverse_map_key = (price_source, symbol_data[asset]['symbol_id'])
        if reverse_map_key not in symbol_id_reverse_map:
            symbol_id_reverse_map[reverse_map_key] = [asset]
        else:
            symbol_id_reverse_map[reverse_map_key].append(asset)

    asset_prices = {asset: 0.0 for asset in asset_balance}
    for price_source in symbol_ids_to_query:
        if len(symbol_ids_to_query[price_source]) == 0:
            continue
        prices = price_sources[price_source].get_asset_prices(symbol_ids_to_query[price_source])
        for symbol_id in prices:
            # Multiple symbols can map to the same underlying price_source, symbol_id tuple
            assets = symbol_id_reverse_map[(price_source, symbol_id)]
            for asset in assets:
                asset_prices[asset] = prices[symbol_id]

    # Convert to USD. Assume we have ccy prices in here (dirty, I know)
    for asset in asset_prices:
        asset_ccy = symbol_data[asset].get('currency', 'USD')
        asset_prices[asset] *= asset_prices[asset_ccy]

    notional_value = {asset : asset_prices[asset] * asset_balance[asset] for asset in asset_balance}

    total_notional = sum([notional_value[asset] for asset in notional_value])

    asset_class_notional = {}
    for asset in notional_value:
        asset_class = symbol_data[asset]['asset_class']
        if asset_class not in asset_class_notional:
            asset_class_notional[asset_class] = notional_value[asset]
        else:
            asset_class_notional[asset_class] += notional_value[asset]

    print("Asset balances, per account:")
    print(json.dumps(account_assets, indent=2))

    print("Asset balances, total:")
    print(json.dumps(asset_balance, indent=2))

    print("Asset prices:")
    print(json.dumps(asset_prices, indent=2))

    print("Notional asset value:")
    print(json.dumps(notional_value, indent=2))

    print(f"Total notional: ${total_notional:.02f}")

    print("Share of each asset:")
    sorted_assets = sorted([(notional_value[k], k) for k in notional_value], reverse=True)
    for entry in sorted_assets:
        print(f'{entry[1]:10} ${entry[0]:7.2f}\t{entry[0]/total_notional*100:2.02f}%')

    print("Share of each asset class")
    sorted_asset_classes = sorted([(asset_class_notional[k], k) for k in asset_class_notional], reverse=True)
    for entry in sorted_asset_classes:
        print(f'{entry[1]:10} ${entry[0]:7.2f}\t{entry[0]/total_notional*100:2.02f}%')


if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser("Bogdanoff arguments")
    arg_parser.add_argument("-c", "--config-path", help="Configuration file", required=True)
    arg_parser.add_argument("-s", "--symbol-data-path", help="Symbol data file", required=True)
    args = arg_parser.parse_args()
    main(args)