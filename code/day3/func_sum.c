#include <stdio.h>
void sum(int num1, int num2){
    int sum = num1 + num2;
    printf("%d", sum);
}
int main()
{
    sum(20,30);
    return 0;
}