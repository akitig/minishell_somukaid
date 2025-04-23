#include <unistd.h>
#include "../libft/libft.h"

int main(int ac, char **ag, char **envp)
{
    char    **split;

    split = ft_split(ag[1], ' ');
    execve(split[0], split, envp);
    return (0);
}