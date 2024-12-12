#include <stdio.h>
#include <stdlib.h>

// Define initial size of the prices array
#define initial_size 100

int processQuery(int *start, int *end, int index, int **prices); // Processes a query to find max profit and loss
int processPrice(int price, int *index, int *size,
                 int **prices); // Processes each price and manages dynamic array expansion
void
countLossProf(int start, int end, int *prices); // Counts the maximum profit and maximum loss within the specified range


int processPrice(int price, int *index, int *size, int **prices) {
    if (price < 0) {
        printf("Nespravny vstup.\n");
        return 0;
    }
    if (*index < *size) {
        (*prices)[(*index)++] = price;
    } else {
        // Resize the array when it's full, increase size exponentially
        int new_size = *size + (*size > 100 ? *size : *size / 2);
        int *tmp = (int *) realloc(*prices, new_size * sizeof(**prices));
        if (tmp == NULL) return 0;
        *prices = tmp;
        *size = new_size;
        (*prices)[(*index)++] = price;
    }
    return 1;
}

void countLossProf(int start, int end, int *prices) {
    int max_profit = 0, max_loss = 0, start_p = start,
            end_p = start, start_l = start, end_l = start,
            min_price = prices[start], max_price = prices[start], min_index = start, max_index = start;


    for (int i = start; i <= end; i++) {
        // Find the minimum price in the range
        if (prices[i] < min_price) {
            min_price = prices[i];
            min_index = i;
        }

        // Calculate current profit
        int cur_profit = prices[i] - min_price;
        if (cur_profit > max_profit) {
            max_profit = cur_profit;
            start_p = min_index;
            end_p = i;
        }

        // Find the maximum price in the range
        cur_profit = prices[i] - max_price;
        if (prices[i] > max_price) {
            max_price = prices[i];
            max_index = i;
        }

        // Calculate current loss
        if (cur_profit < max_loss) {
            max_loss = cur_profit;
            start_l = max_index;
            end_l = i;
        }
    }

    if (max_profit > 0) printf("Nejvyssi zisk: %d (%d - %d)\n", max_profit, start_p, end_p);
    else printf("Nejvyssi zisk: N/A\n");

    if (max_loss < 0) printf("Nejvyssi ztrata: %d (%d - %d)\n", (-1 * max_loss), start_l, end_l);
    else printf("Nejvyssi ztrata: N/A\n");
}

int processQuery(int *start, int *end, int index, int **prices) {
    if ((*start >= 0) && (*start <= *end) && (*end < index)) {
        countLossProf(*start, *end, *prices);
    } else {
        printf("Nespravny vstup.\n");
        return 0;
    }
    return 1;
}

int main() {

    int size = initial_size;
    int *prices = (int *) malloc(size * sizeof(*prices));
    if (!prices) return 1;

    int price, start, end, index = 0, input;

    printf("Ceny, hledani:\n");

    while (1) {
        if ((input = scanf(" + %d", &price)) == 1) {
            if (!processPrice(price, &index, &size, &prices)) {
                free(prices);
                return 1;
            }
        } else if ((input = scanf(" ? %d %d", &start, &end)) == 2) {
            if (!processQuery(&start, &end, index, &prices)) {
                free(prices);
                return 1;
            }
        } else if (input == EOF && index != 0) break;
        else {
            printf("Nespravny vstup.\n");
            free(prices);
            return 1;
        }
    }

    free(prices);
    return 0;
}
