/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:04:58 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/17 14:42:48 by biniesta         ###   ########.fr       */
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

int	builtin_modifies_state(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit"));
}

int	is_builtin(const char *str)
{
	if (!str)
		return (0);
	return (ft_strcmp(str, "echo") == 0
		|| ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "pwd") == 0
		|| ft_strcmp(str, "export") == 0
		|| ft_strcmp(str, "unset") == 0
		|| ft_strcmp(str, "env") == 0
		|| ft_strcmp(str, "exit") == 0);
}
