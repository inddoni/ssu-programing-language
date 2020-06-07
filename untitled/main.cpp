#include <iostream>

int main() {
    int arr[] = {100, 200, 300};
    int *p = NULL;
    p = arr;

    printf("%d\n", *p+1);
    printf("%d\n", *(p+1));
    printf("%d\n", *p++);
    printf("%d\n", (*p)++);
    printf("%d\n", *++p);
    printf("%d\n", ++*p);

    return 0;
}