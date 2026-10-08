#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>

enum { COIN_COUNT = 8 };

static const int coin_values[COIN_COUNT] = {200, 100, 50, 20, 10, 5, 2, 1};
static const char *coin_descriptions[COIN_COUNT] = {
    "big and thick, with finely corrugated outer edge",
    "smaller than 50 and thick, with partly corrugated outer edge",
    "big and thick, with corrugated outer edge",
    "bigger and thick, with a few dents on the outer edge",
    "small and thick, with corrugated outer edge",
    "copper = lightweight, bigger, with smooth outer edge",
    "copper = lightweight, a little bigger with a groove in the outer edge",
    "copper = lightweight, small with smooth outer edge"};

static int read_value(int *value) {
  printf("Enter Euro-cent value: ");
  if (scanf("%d", value) != 1 || *value < 0) {
    fprintf(stderr, "Please enter a non-negative whole number of cents.\n");
    return 0;
  }
  return 1;
}

/* Finds a minimum-coin solution with dynamic programming and stores the
   number of each denomination in counts.
   Greedy can fail for other coin systems: with denominations 4, 3, and 1,
   greedy makes 6 as 4 + 1 + 1 (3 coins), while the optimum is 3 + 3 (2 coins).
   The Euro denominations used here happen to work with greedy, but DP also
   finds the optimum without relying on that property. */
static int calculate_minimum_coins(int value, const int coin_values[],
                                   size_t coin_count, int counts[]) {
  /* Check before forming the allocation size: value + 1 in int could
     overflow, and multiplying by sizeof could overflow size_t. */
  size_t entries = (size_t)value + 1;
  if (entries > SIZE_MAX / sizeof(int)) {
    return -1;
  }

  int *minimum = malloc(entries * sizeof(*minimum));
  int *last_coin = malloc(entries * sizeof(*last_coin));
  if (minimum == NULL || last_coin == NULL) {
    free(minimum);
    free(last_coin);
    return -1;
  }

  minimum[0] = 0;
  last_coin[0] = -1;
  for (size_t amount = 1; amount < entries; amount++) {
    minimum[amount] = INT_MAX;
    last_coin[amount] = -1;
    for (size_t i = 0; i < coin_count; i++) {
      int coin = coin_values[i];
      if ((size_t)coin <= amount && minimum[amount - (size_t)coin] != INT_MAX &&
          minimum[amount - (size_t)coin] + 1 < minimum[amount]) {
        minimum[amount] = minimum[amount - coin] + 1;
        last_coin[amount] = (int)i;
      }
    }
  }

  for (size_t i = 0; i < coin_count; i++) {
    counts[i] = 0;
  }
  for (size_t amount = (size_t)value; amount > 0;
       amount -= (size_t)coin_values[last_coin[amount]]) {
    counts[last_coin[amount]]++;
  }

  int result = minimum[value];
  free(minimum);
  free(last_coin);
  return result;
}

static void print_coin_table(int value, int total_coins, const int counts[]) {
  printf("\n%-15s %-15s %s\n", "Number of coins", "Value of coin", "Description");
  printf("---------------------------------------------------------------\n");

  for (size_t i = 0; i < COIN_COUNT; i++) {
    int number = counts[i];
    if (number > 0) {
      printf("%-15d %-12d ct %s\n", number, coin_values[i], coin_descriptions[i]);
    }
  }

  if (value == 0) {
    printf("No coins are needed for 0 ct.\n");
  } else {
    printf("Total: %d coin%s for %d ct.\n", total_coins,
           total_coins == 1 ? "" : "s", value);
  }
}

int main(void) {
  int value;
  int counts[COIN_COUNT];

  if (!read_value(&value)) {
    return 1;
  }

  int total_coins = calculate_minimum_coins(value, coin_values, COIN_COUNT, counts);
  if (total_coins < 0) {
    fprintf(stderr, "Unable to allocate memory for the calculation.\n");
    return 1;
  }

  print_coin_table(value, total_coins, counts);

  return 0;
}
