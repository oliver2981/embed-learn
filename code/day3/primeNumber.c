/*#include <stdio.h>

int main(void)
{
    int i=2;
    int prime;
    scanf("%d", &prime);
    int judge=1;

    for(;i<prime;i++){
        if(prime%i==0){
            judge=0;
            break;
        }      
    }
    if(judge==1){
        printf("prime");
    }
    return 0;
}*/

#include <stdio.h>

int main(void)
{
    int n;
    scanf("%d", &n);

    int is_prime = 1;   // 先假设是素数

    if (n <= 1) {
        is_prime = 0;
    } else {
        for (int i = 2; i < n; i++) {
            if (n % i == 0) {
                is_prime = 0;
                break;      // 找到一个因子就够了
            }
        }
    }

    if (is_prime)
        printf("%d 是素数\n", n);
    else
        printf("%d 不是素数\n", n);

    return 0;
}
    