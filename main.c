#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <libc.h>

int main ()
{
    int fd = open("file.txt", 777);
    while (1)
    {
        int Buff = 1000;
        char c[Buff];
        memset(c, 0, Buff);
        printf("\nEnter something : \n");
        int bytes_read = read(0,c, Buff);
        if (strcmp(c, "exit\n") == 0)
            return 0;
        printf("%s\n", c);
        write(fd, c, bytes_read);
    }
}