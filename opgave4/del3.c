#include <stdio.h>

#define COLOR_BOLD "\e[1m"
#define COLOR_GREEN "\e[32m"
#define COLOR_RED "\e[31m"
#define COLOR_NONE "\e[m"
#define COLOR_ITALIC "\e[3m"

int mitArray[] = {3, 1, 7, 4, 9, 10, 6};
int arrayLength = sizeof(mitArray) / sizeof(mitArray[0]);

void bubbleSort(int array[]) {
  int temp;

  for (int x; x < arrayLength - 1; x++) {
    for (int y; y < arrayLength - 1; y++) {
      if (array[y] > array[y + 1]) {
        temp = array[y];
        array[y] = array[y + 1];
        array[y + 1] = temp;
      }
    }
  }

  for (int i = 0; i < arrayLength; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main() {
  bubbleSort(mitArray);
  return 0;
}
