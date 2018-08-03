#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
    int fd1 = creat("output.txt", 0644);
    int a=dup(1);
    printf("%d\n",a);
    dup2(fd1,STDOUT_FILENO);
    printf("works!");
    // close(fd1);
    dup2(STDOUT_FILENO,a);
    printf("SS");
    // close(a);
    return 0;
}
