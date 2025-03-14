 #include <unistd.h>
 #include <stdio.h>
 #include <string.h>
 #include <errno.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>

 int    main(void)
 {
    int fd;

    printf("0:%d\n", isatty(0));
    printf("1:%d\n", isatty(1));
    printf("2:%d\n", isatty(2));
    printf("3:%d\n", isatty(3));
    printf("err_str:%s\n", strerror(errno));
    fd = open("readline.c", O_RDONLY);
    printf("%d\n", fd);
    printf("3:%d\n", isatty(3));
 //printf("err_str:%s\n", strerror(0));
    printf("err_str:%s\n", strerror(errno));
    return (0);
 }
