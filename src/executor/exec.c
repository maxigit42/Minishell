/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilline <mwilline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 03:30:59 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/29 03:30:59 by mwilline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     find_valid_path(char **paths, char **envp, char **cmd)
{
    char    *temp;
    char    *full_path;
    int i;

    if (!cmd || !cmd[0])
        return 0;
    i = 0;
    while(paths[i])
    {
        temp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(temp, cmd[0]);
        free(temp);
        if(!full_path)
            return(0);
        if(access(full_path, X_OK) == 0)
        {
            execve(full_path, cmd, envp);
            perror("execve");
            free(full_path);
            return (0);
        }
        free(full_path);
        i++;
    }
    return(0);
}
static int  is_path(char *cmd)
{
    if (!cmd)
        return (0);			// funcion nueva
    if (cmd[0] == '/' || cmd[0] == '.')
        return (1);
    return (0);
}

static void execute_direct_path(char **cmd, char **envp_array)
{
    if (access(cmd[0], F_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd[0], 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        exit(127);
    }
    if (access(cmd[0], X_OK) != 0)							//funcion nueva
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd[0], 2);
        ft_putstr_fd(": Permission denied\n", 2);
        exit(126);
    }
    execve(cmd[0], cmd, envp_array);
    perror("execve");
    exit(126);
}

void   execute_cmd(t_env *env, char **cmd)
{
    char    *full_envp;
    char    **paths;
    char    **envp_array;

    if (!cmd || !cmd[0])
        return;

    envp_array = env_list_to_array(env);
    if(!envp_array)
        return;

    /* Si es ruta absoluta o relativa, ejecutar directamente */
    if (is_path(cmd[0]))
    {
        execute_direct_path(cmd, envp_array);
        free_split(envp_array);
        return;
    }

    /* Buscar en PATH */
    full_envp = get_env_value_list(env, "PATH");
    if (!full_envp)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd[0], 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        free_split(envp_array);
        exit(127);
    }

    paths = ft_split(full_envp, ':');    //modificaciones en esta funcion
    if(!paths)
    {
        free_split(envp_array);
        return;
    }

    if(!find_valid_path(paths, envp_array, cmd))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        free_split(paths);
        free_split(envp_array);
        exit(127);
    }
    free_split(paths);
    free_split(envp_array);
}

char **get_cmds(t_data *data)
{
    char **args;
    t_token *current;
    int i;

    i = 0;
    current = data->token;
    while (current && current->type == CMD)
    {
        i++;    
        current = current->next;
    }
    args = malloc(sizeof(char *) * (i + 1));
    if (!args)
        return NULL;
    i = 0;
    current = data->token;
    while (current && current->type == CMD)
    {
        args[i++] = ft_strdup(current->str);
        current = current->next;
    }
    args[i] = NULL;
    return(args);
}