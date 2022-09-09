#include <stdio.h>

static double valutaKurs = 0.9991;

double valutaOmregner(double input) {
  double euro = input * valutaKurs;
  return euro;
}

int main() {
  double input;
  printf("Indtast dit beløb i $ du gerne vil have omregnet: \n");
  scanf("%lf", &input);
  printf("%.2lf $ svarer til %.2lf € \n", input, valutaOmregner(input));
  return 0;
}
