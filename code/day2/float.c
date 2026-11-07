#include <stdio.h>

int main()
{
    //float 单精度小数 精确度小数点后六位 windows占4个字节
    float a = 3.14F;
    printf("%.1f", a);//保留一位
    return 0;
    //double 双精度小数 winidows占8个字节 后面是%lf 可以用sizeof测量
}