#include <stdio.h>

void hexConverter(int input) {
  printf("%d i decimal svarer til %X i hexadecimal\n", input, input);
}

int main() {
  double input;
  printf("Indtast den decimal værdi du gerne vil have konverteret til "
         "hexadecimal: \n");
  scanf("%lf", &input);
  hexConverter(input);
  return 0;
}
