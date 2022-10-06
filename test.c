#include <stdio.h>


#define SIZE 32

int main(int argc, char const *argv[])
{
    char buf[SIZE];


    int res = snprintf(buf, SIZE, "%s", "");

    printf("'%s': %d\n", buf, res);



    return 0;
}
