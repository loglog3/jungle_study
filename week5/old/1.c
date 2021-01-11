#include <stdio.h>
#include <stdlib.h>

int main()
{
  int a = 4;
  int b = 7;

  int **tmp = (int **)malloc(2 * sizeof(int *));
  tmp[0] = &a;
  tmp[1] = &b;

  printf("%d, %d", tmp[0], *tmp[0]);

  int arr[50];
  arr[0] = 1;
  arr[1] = 2;
  printf("%p\n", arr, *(arr+1));
}