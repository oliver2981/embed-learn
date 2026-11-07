#include <stdio.h>
int main()
{
    long C = 1000L;//L要加 不然会存在隐式转换
    printf("%ld\n", C);
    printf("%zu\n", sizeof(C));
    printf("%zu\n", sizeof(long));
    //定义一个无符号整数
    unsigned int a = 100;
    printf("%u", a);
    return 0;
}