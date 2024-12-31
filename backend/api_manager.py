import requests

class CoinbaseAPIManager:
    BASE_URL = "https://api.coinbase.com/v2"

    def __init__(self, api_id, api_secret, api_passphrase):
        self.api_id = api_id
        self.api_secret = api_secret
        self.api_passphrase = api_passphrase

    def connect(self):
        # Exemple d'un appel pour récupérer le taux de change comme test
        try:
            response = requests.get(f"{self.BASE_URL}/exchange-rates")
            response.raise_for_status()
            data = response.json()
            return data["data"]
        except requests.exceptions.RequestException as e:
            print(f"Error connecting to Coinbase: {e}")
            return None

    def get_account_balance(self):
        # Implémentation pour interroger l'API sur le solde
        # À faire : inclure l'authentification (token/clé API)
        return {"balance": "Simulation Mode"}
