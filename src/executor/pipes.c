/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:47:45 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/06 18:27:54 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     **create_pipes(int  n_pipes)
{
    int **pipes;
    int i;

    i = 0;
    pipes = malloc(sizeof(int *) * n_pipes);
    while(i < n_pipes)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes)
        {
            while(i-- >= 0)
                free(pipes[i]);
            free(pipes);
            return(NULL);
        }
        if(pipe(pipes[i]) == -1)
        {
            while(i >= 0)
            {
                free(pipes[i]);
                i--;
            }
            free(pipes);
            ft_error("Pipes failed\n", 1);
        } 
        i++;
    }
    return(pipes);
}

void setup_child_pipes(int **pipes, int n_pipes, int index)
{
    int i;
    
    i = 0;
    while (i < n_pipes)
    {
        if (i != index - 1)
            close(pipes[i][0]);
        if (i != index)
            close(pipes[i][1]);
        i++;
    }
    // Si no es el primer comando, conecta su entrada al anterior
    if (index > 0)
        dup2(pipes[index - 1][0], STDIN_FILENO);
    // Si no es el último comando, conecta su salida al siguiente
    if (index < n_pipes)
        dup2(pipes[index][1], STDOUT_FILENO);
}

void close_all_pipes(int **pipes, int n_pipes)
{
    int i;

    i = 0;
    while(i < n_pipes)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

void free_pipes(int **pipes, int n_pipes)
{
    int i;

    i = 0;
    while(i < n_pipes)
    {
        free(pipes[i]);
        i++;
    }
    free(pipes);
}
