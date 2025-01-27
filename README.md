Bogdanoff
======
This should track various assets and their $ value

## How to set up
The program expects a `settings.json` file, example below

```
{
    "price_sources":
    [
        {
            "source": "coin_market_cap",
            "credentials":
            {
                "api_key": "XXXXXXXXXXXXXXXX"
            }
        },
        {
            "source": "ecb"
        },
    ],
    "account_clients":
    [
        {
            "venue": "coinex",
            "credentials":
            {
                "access_id": "XXXXXXXXXXXXXX",
                "secret": "XXXXXXXXXXXXXXXXX"
            }
        },
        {
            "venue": "etherscan",
            "credentials":{
                "token": "XXXXXXXXXXXXXX",
                "addresses":
                [
                    "XXXXXXXXXXXXXXXXXXXXXX"
                ],
                "spam_filter":
                [
                    "SOME SPAM TOKEN NAME"
                ]
            }
        },
        {
            "venue": "interactive_brokers",
            "credentials":
            {
                "csv_path": "PATH_TO_TRADES_CSV"
            }
        }
    ]
}
```

### Components

#### Price
Price components are an interface for querying a website/API about asset prices

Supported prices:
|Name|Asset Class|
|----|-----------|
|AlphaVantage|EQUITY|
|CoinMarketCap|CRYPTO|
|ECB|FX|
|Yahoo Finance|EQUITY|

#### Account Clients
Account clients look up balances and transactions for the specified accounts.
Each site/API needs its own client usually

Supported clients:
|Name|Asset Class|
|----|-----------|
|Bitvavo|CRYPTO|
|Blockchain.info|CRYPTO|
|CoinEx|CRYPTO|
|Etherscan|CRYPTO|
|Interactive Brokers|EQUITY|
|Kraken|CRYPTO|