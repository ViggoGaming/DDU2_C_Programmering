#include <stdio.h>

#define COLOR_BOLD "\e[1m"
#define COLOR_GREEN "\e[32m"
#define COLOR_RED "\e[31m"
#define COLOR_NONE "\e[m"
#define COLOR_ITALIC "\e[3m"

static int tal = 15;

int main() {
  int input;
  printf(COLOR_BOLD "Velkommen til 'gæt et tal'!\n" COLOR_NONE);
  printf(COLOR_BOLD "Du skal gætte et tal mellem 1-20\n" COLOR_NONE);

  while (input != tal) {
    printf(COLOR_ITALIC "Indtast dit gæt: \n" COLOR_NONE);
    scanf("%d", &input);

    if (input > tal) {
      printf(COLOR_RED "Øv, dit gæt var for højt, prøv igen\n" COLOR_NONE);
    } else if (input < tal) {
      printf(COLOR_RED "Øv, dit gæt var for lavt, prøv igen\n" COLOR_NONE);
    }
  }

  printf(COLOR_BOLD COLOR_GREEN "Tillykke, du gættede det rigtige tal %d\n",
         tal);
  return 0;
}
