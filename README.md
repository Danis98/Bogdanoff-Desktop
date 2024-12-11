Bogdanoff
======
This should track various assets and their $ value

## How to set up
The program expects a `settings.json` file, example below

```
{
    "prices":
    [
        {
            "source": "coingecko",
            "enabled": "true"
        }
    ],
    "clients":
    [
        {
            "venue": "coinex",
            "enabled": "true",
            "price": "coingecko",
            "access_id": "[ACCESS_ID]",
            "secret": "[SECRET]"
        },
        {
            "venue": "etherscan",
            "enabled": "true",
            "price": "coingecko",
            "token": "[TOKEN]",
            "addresses":
            [
                "[ADDRESS]"
            ]
        },
        {
            "venue": "blockchain_info",
            "enabled": "true",
            "price": "coingecko",
            "addresses":
            [
                "[ADDRESS]"
            ]
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
|CoinGecko|CRYPTO|

#### Account Clients
Account clients look up balances and transactions for the specified accounts.
Each site/API needs its own client usually

Supported clients:
|Name|Asset Class|
|----|-----------|
|CoinEx|CRYPTO|
|Etherscan|CRYPTO|
|Blockchain.info|CRYPTO|