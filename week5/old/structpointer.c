#include <stdio.h>

struct Point
{
  int x;
  int y;
};
int main()
{
  struct Point pt = {100, 200};
  struct Point *ppt = &pt;
  printf("pt.x = %d : pt.y = %d\n", pt.x, pt.y);
  printf("pt.x = %d : pt.y = %d\n", ppt->x, ppt->y);

  pt.x = 300;
  pt.y = 400;

  printf("pt.x = %d : pt.y = %d\n", pt.x, pt.y);

  return 0;
}
