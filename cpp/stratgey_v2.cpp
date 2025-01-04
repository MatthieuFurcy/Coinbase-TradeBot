#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <curl/curl.h>
#include <nlohmann/json.hpp>  // Inclure la bibliothèque JSON moderne pour C++

using json = nlohmann::json;

// Fonction pour gérer la réponse de la requête
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Calcul du RSI
double calculateRSI(const std::vector<double>& prices, int period) {
    double gain = 0.0;
    double loss = 0.0;

    for (int i = 1; i < period; ++i) {
        double change = prices[i] - prices[i - 1];
        if (change > 0) {
            gain += change;
        } else {
            loss -= change;
        }
    }

    double avg_gain = gain / period;
    double avg_loss = loss / period;

    for (int i = period; i < prices.size(); ++i) {
        double change = prices[i] - prices[i - 1];
        if (change > 0) {
            gain = change;
            loss = 0;
        } else {
            gain = 0;
            loss = -change;
        }

        avg_gain = ((avg_gain * (period - 1)) + gain) / period;
        avg_loss = ((avg_loss * (period - 1)) + loss) / period;

        if (avg_loss == 0) return 100;

        double rs = avg_gain / avg_loss;
        double rsi = 100 - (100 / (1 + rs));
        return rsi;
    }
    return 0.0;
}

// Calcul du MACD
void calculateMACD(const std::vector<double>& prices, int fastLength, int slowLength, int signalLength, std::vector<double>& macd, std::vector<double>& signal) {
    std::vector<double> emaFast, emaSlow;

    for (int i = 0; i < prices.size(); ++i) {
        double emaValFast = 0.0;
        double emaValSlow = 0.0;

        if (i >= fastLength - 1) {
            double multiplierFast = 2.0 / (fastLength + 1);
            double sumFast = 0.0;
            for (int j = i - fastLength + 1; j <= i; ++j) {
                sumFast += prices[j];
            }
            emaFast.push_back(sumFast * multiplierFast);
        }

        if (i >= slowLength - 1) {
            double multiplierSlow = 2.0 / (slowLength + 1);
            double sumSlow = 0.0;
            for (int j = i - slowLength + 1; j <= i; ++j) {
                sumSlow += prices[j];
            }
            emaSlow.push_back(sumSlow * multiplierSlow);
        }

        if (emaFast.size() > 0 && emaSlow.size() > 0) {
            double macdValue = emaFast.back() - emaSlow.back();
            macd.push_back(macdValue);
        }

        if (macd.size() >= signalLength) {
            double signalVal = 0.0;
            for (int j = macd.size() - signalLength; j < macd.size(); ++j) {
                signalVal += macd[j];
            }
            signal.push_back(signalVal / signalLength);
        }
    }
}

// Fonction pour appeler l'API Coinbase Pro (pour récupérer le solde et passer des ordres)
void callCoinbaseAPI(const std::string& endpoint, const std::string& api_key, const std::string& api_secret, const std::string& api_passphrase, const std::string& method, const json& data = {}) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    struct curl_slist* headers = NULL;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Construire l'URL de l'API
        std::string url = "https://api.pro.coinbase.com" + endpoint;

        // Ajouter les headers d'authentification
        headers = curl_slist_append(headers, ("CB-ACCESS-KEY: " + api_key).c_str());
        headers = curl_slist_append(headers, ("CB-ACCESS-SIGN: " + api_secret).c_str());
        headers = curl_slist_append(headers, ("CB-ACCESS-TIMESTAMP: " + std::to_string(time(NULL))).c_str());
        headers = curl_slist_append(headers, ("CB-ACCESS-PASSPHRASE: " + api_passphrase).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.dump().c_str());
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Afficher la réponse de l'API
            std::cout << "Response: " << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    // Exemple d'API Key et autre (vous devez utiliser vos propres clés)
    std::string api_key = "YOUR_API_KEY";
    std::string api_secret = "YOUR_API_SECRET";
    std::string api_passphrase = "YOUR_API_PASSPHRASE";

    // Exemple de données de prix (vous récupérez ces données depuis une source en temps réel)
    std::vector<double> prices = { 100.0, 102.0, 101.5, 104.0, 103.5, 106.0, 107.0, 110.0, 109.5, 112.0 };

    // Calcul des indicateurs RSI et MACD
    double rsi = calculateRSI(prices, 14);
    std::vector<double> macd, signal;
    calculateMACD(prices, 12, 26, 9, macd, signal);

    // Conditions de trading (par exemple, acheter si RSI < 30 et MACD haussier)
    bool buyCondition = (rsi < 30.0) && (macd.back() > signal.back());

    // Si la condition d'achat est remplie, passer un ordre (exemple d'ordre de futures avec effet de levier)
    if (buyCondition) {
        std::cout << "Achat condition remplie. Envoi d'ordre de trading..." << std::endl;
        
        // Exemple d'ordre d'achat
        json order_data = {
            {"product_id", "BTC-USD"},
            {"side", "buy"},
            {"price", "11000.00"},
            {"size", "0.1"},
            {"type", "limit"},
            {"post_only", true}
        };

        callCoinbaseAPI("/orders", api_key, api_secret, api_passphrase, "POST", order_data);
    }

    return 0;
}
