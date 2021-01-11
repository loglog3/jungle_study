#include <stdio.h>

// 클래스처럼 생각하자

int main()
{

  typedef struct stack
  {
    char name[20];
    int age;
    char address[100];
  } P;

  P p1 = {"고길동", 40, "서울시 도봉구"};
  struct stack p2 = {"이승민", 27, "경기도 고양시"};

  printf("%s", p1.name);
  return 0;
}