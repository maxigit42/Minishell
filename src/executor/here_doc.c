#include "minishell.h"


//  revisar que al hacer un heredoc con un comando invalido redirigido a un outfile el 
// mensaje del error se escribe en el archivo y no en la terminal 


static void ft_heredoc_loop(char *limit, int fd)
{
    char *line;

    while(1)
    {
        line = readline("> ");
        if(!line || ft_strcmp(limit, line) == 0)
        {
            free(line);
            close(fd);
            exit(EXIT_SUCCESS);
        }
        ft_putstr_fd(line, fd);
        ft_putchar_fd('\n', fd);
        free(line);
    }
}

int ft_here_doc(char *limit)
{
    pid_t pid;
    int fd[2];

    if(pipe(fd) < 0)
        ft_error("minishell: failed to open pipe", 1); // mejorar la gestion de errores
    pid = fork();
    if(pid < 0)
        ft_error("Error", 1);
    if(pid == 0)
    {
        close(fd[0]);
        ft_heredoc_loop(limit, fd[1]);
    }
    return(waitpid(-1, NULL, 0), close(fd[1]), fd[0]);
}
