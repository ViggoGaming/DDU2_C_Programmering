#include <stdio.h>

int mitArray[] = {11, 12, 13, 14, 15, 16, 17, 18};
int arrayLength = sizeof(mitArray) / sizeof(mitArray[0]);

void findTarget(int array[], int target) {

  for (int i = 0; i < arrayLength; i++) {
    if (array[i] == target) {
      printf("Tallet %d er i den angivet liste\n", array[i]);
    }
  }
}

int main() {
  findTarget(mitArray, 14);
  return 0;
}
