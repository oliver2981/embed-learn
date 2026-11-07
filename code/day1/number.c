#include <stdio.h>

int main()
{
    /* 整型   %d
       实型   %f
       字符   %c
       字符串 %s */
    int a = 10, c = 30;//变量在使用前一定要赋值
    int b = 20;
    printf("a+b=%d\n", a + b); //a+b=30
    printf("%f\n", 1.93);              // 1.930000
    printf("我今天真的%s\n", "cool");   // 我今天真的cool
    printf("%d", 010);//8 2进制的话就是0b开头 16进制的话就是0x开头
    return 0;
}