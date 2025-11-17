/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilline <mwilline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:04:58 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/17 05:21:07 by mwilline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_data *data, t_env **env)
{
	char	**args;

	args = data->cmd[0];
	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(data->token, *env));
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args, data));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, env));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, env));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(*env));
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));
	return (1);
}
