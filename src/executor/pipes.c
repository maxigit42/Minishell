/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:47:45 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/16 20:41:25 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipes_execute(t_data *data, t_env *envp, int **pipes, t_token **com)
{
	int	i;

	i = 0;
	while (i < data->pipe + 1)
	{
		data->pid[i] = fork();
		if (data->pid[i] < 0)
			ft_error("Fork failed\n", 1);
		if (data->pid[i] == 0)
		{
			setup_child_pipes(pipes, data->pipe, i);
			ft_redirection(com[i]);
			execute_cmd(envp, data->cmd[i]);
			exit(1);
		}
		i++;
	}
}

void	execute_pipes(t_data *data, t_env *envp)
{
	int		i;
	int		**pipes;
	t_token	**com;

	pipes = create_pipes(data->pipe);
	data->pid = ft_create_pid(data);
	com = ft_split_token(data, count_commands(data->token));
	pipes_execute(data, envp, pipes, com);
	close_all_pipes(pipes, data->pipe);
	i = 0;
	while (i < data->pipe + 1)
		waitpid(data->pid[i++], NULL, 0);
	free_pipes(pipes, data->pipe);
	ft_free_list(com);
}

int	**create_pipes(int n_pipes)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = malloc(sizeof(int *) * n_pipes);
	while (i < n_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
			ft_error("Pipe failed\n", 1);
		i++;
	}
	return (pipes);
}

void	setup_child_pipes(int **pipes, int n_pipes, int index)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		if (i != index - 1)
			close(pipes[i][0]);
		if (i != index)
			close(pipes[i][1]);
		i++;
	}
	if (index > 0)
		dup2(pipes[index - 1][0], STDIN_FILENO);
	if (index < n_pipes)
		dup2(pipes[index][1], STDOUT_FILENO);
}

void	close_all_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
