#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Uses the Sieve of Eratosthenes, growing the search range if necessary.

• List numbers: Write down all integers from 2 up to your chosen limit n.

• Find the first prime: Start at 2, the first and smallest prime number.

• Cross out multiples: Mark all proper multiples of 2 (4, 6, 8, etc.) as
composite.

• Repeat: Move to the next unmarked number (3), keep it as a prime, and cross
out its multiples.

• Stop condition: Continue this process only up to the square root of n
(sqrt{n}).

• Result: All remaining unmarked numbers in the list are prime numbers.
*/
static int find_nth_prime(size_t target) {
  size_t limit =
      200000; /* A comfortable initial bound for the 10,001st prime. */

  for (;;) {
    bool* is_prime = calloc(limit + 1, sizeof(*is_prime));
    if (is_prime == NULL) {
      return 0;
    }

    for (size_t number = 2; number <= limit; number++) {
      is_prime[number] = 1;
    }

    for (size_t prime = 2; prime <= limit / prime; prime++) {
      if (is_prime[prime]) {
        for (size_t multiple = prime * prime; multiple <= limit;
             multiple += prime) {
          is_prime[multiple] = 0;
        }
      }
    }

    size_t prime_count = 0;
    for (size_t number = 2; number <= limit; number++) {
      if (is_prime[number] && ++prime_count == target) {
        free(is_prime);
        return (int)number;
      }
    }

    free(is_prime);
    if (limit > (size_t)-1 / 2 - 1) {
      return 0;
    }
    limit *= 2;
  }
}

int main(void) {
  int prime = find_nth_prime(10001);
  if (prime == 0) {
    fprintf(stderr, "Unable to find the requested prime.\n");
    return 1;
  }

  printf("%d\n", prime);
  return 0;
}
