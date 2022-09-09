#include <stdio.h>

#define COLOR_BOLD "\e[1m"
#define COLOR_GREEN "\e[32m"
#define COLOR_RED "\e[31m"
#define COLOR_NONE "\e[m"
#define COLOR_ITALIC "\e[3m"

int mitArray[] = {11, 12, 13, 14, 15, 16, 17, 18};
int arrayLength = sizeof(mitArray) / sizeof(mitArray[0]);

void findTarget(int array[], int target, int swap) {

  for (int i = 0; i < arrayLength; i++) {
    if (array[i] == target) {
      printf(COLOR_BOLD COLOR_GREEN
             "Tallet %d er i den angivet liste\n" COLOR_NONE,
             array[i]);
      printf(COLOR_ITALIC "Der udføres nu et swap af %d til %d\n" COLOR_NONE,
             array[i], swap);
      array[i] = swap;

      printf(COLOR_RED "Arrayen er nu: " COLOR_NONE);
      for (int j = 0; j < arrayLength; j++) {
        printf("%d ", array[j]);
      }
      printf("\n");
    }
  }
}

int main() {
  /* array, target, swap  */
  findTarget(mitArray, 14, 55);
  return 0;
}
