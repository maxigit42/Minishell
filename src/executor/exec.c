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

void	execute_single_cmd(t_data *data, t_env *envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		ft_error("Fork failed\n", 1);
	if (pid == 0)
	{
		ft_redirection(data->token);
		execute_cmd(envp, data->cmd[0]);
		exit(127);
	}
	waitpid(pid, &status, 0);
	data->exit_status = WEXITSTATUS(status);
	free_args(data);
}

int	execute_cmd(t_env *env, char **cmd)
{
	char	*full_envp;
	char	**paths;
	char	**envp_array;

	if (!env)
		return (printf("⛔ envp es NULL\n"));
	full_envp = get_env_value_list(env, "PATH");
	if (!full_envp)
		return (printf("⛔ No se encontró PATH en envp\n"));
	paths = ft_split(full_envp, ':');
	if (!paths)
		return (1);
	envp_array = env_list_to_array(env);
	if (!envp_array)
	{
		free_split(paths);
		return (1);
	}
	if (!find_valid_path(paths, envp_array, cmd))
		printf("command not found\n");
	free_split(paths);
	free_split(envp_array);
	return (1);
}
