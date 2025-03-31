#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int	main(int ac, char **av, char **envp)
{
    struct stat st;
    
    
    printf("HOME_PATH is %s\n", getenv("HOME")); // getenv関数で受け取れる環境変数は./minishellを叩いた時のプロセスのもの？
    
    printf("access_retval = %d\n", access("/usr/bin/ls", F_OK));
    printf("access_retval = %d\n", access("/usr/bin/vs", F_OK));
    
    printf("stat_retval = %d\n", stat("/usr/bin/ls", &st));
    printf("stat_retval = %d\n", stat("/usr/bin/vs", &st));
    printf("error_message: %s\n", strerror(errno));

    printf("error_message: %s\n", strerror(errno));
    printf("error_message: %s\n", strerror(errno));
    return (0);
}