#include <stdio.h>

int main() {
    int a, b;
    printf("Enter first number: ");
    scanf("%d", &a);
    printf("Enter second number: ");
    scanf("%d", &b);
    
    printf("Result: %d + %d = %d\n", a, b, a + b);
    return 0;
}