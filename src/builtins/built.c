/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:04:58 by mwilline          #+#    #+#             */
/*   Updated: 2025/10/28 14:37:12 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_builtin(t_data *data, t_env **env)
{
    char **args = data->cmd[0];

    if (!args || !args[0])
        return (0);

    if (ft_strncmp(args[0], "cd", 2) == 0)
        return builtin_cd(data->token, *env);
    if (ft_strncmp(args[0], "echo", 4) == 0)
        return builtin_echo(args);
    if (ft_strncmp(args[0], "pwd", 3) == 0)
        return builtin_pwd();
    if (ft_strncmp(args[0], "export", 6) == 0)
        return builtin_export(args, env);
    if (ft_strncmp(args[0], "unset", 5) == 0)
        return builtin_unset(args, env);
    if (ft_strncmp(args[0], "env", 3) == 0)
        return builtin_env(*env);
    if (ft_strncmp(args[0], "exit", 4) == 0)
        return builtin_exit(args);
    return (1);
}
