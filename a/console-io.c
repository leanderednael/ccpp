#include <stdio.h>

int main() {
  int val1 = 0;
  int val2 = 0;

  printf("Enter value: ");
  scanf("%d", &val1);
  printf("Enter another value: ");
  scanf("%d", &val2);
  printf("The sum of %d and %d is %d\n", val1, val2, val1 + val2);

  return 0;
}
