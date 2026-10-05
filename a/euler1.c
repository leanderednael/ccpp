#include <stdio.h>

/* There are (limit - 1) / divisor multiples below limit. Their sum is
   divisor * (1 + 2 + ... + count) = divisor * count * (count + 1) / 2. */
static long long sum_multiples_below(int limit, int divisor) {
  int count = (limit - 1) / divisor;
  return divisor * count * (count + 1) / 2;
}

static long long sum_multiples_of_3_or_5_below(int limit) {
  return sum_multiples_below(limit, 3)
       + sum_multiples_below(limit, 5)
       - sum_multiples_below(limit, 15);
}

int main(void) {
  printf("%lld\n", sum_multiples_of_3_or_5_below(1000));
  return 0;
}
