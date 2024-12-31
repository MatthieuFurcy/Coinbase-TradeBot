#include <vector>
#include <iostream>
extern "C" {
    struct TradeAction {
        double price;
        int quantity;
        bool is_buy; // true = buy, false = sell
    };

    TradeAction* process_strategy(double* prices, int size, int& num_actions) {
        static TradeAction actions[10]; // Limitation à 10 actions
        num_actions = 0;

        for (int i = 1; i < size; ++i) {
            if (prices[i] < prices[i - 1]) {
                actions[num_actions++] = {prices[i], 1, true};
            } else if (prices[i] > prices[i - 1]) {
                actions[num_actions++] = {prices[i], 1, false};
            }
        }
        return actions;
    }
}
