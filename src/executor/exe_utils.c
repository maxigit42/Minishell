/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:57:35 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/17 14:35:43 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_valid_path(char **paths, char **envp, char **cmd)
{
	char	*temp;
	char	*full_path;
	int		i;

	if (!cmd || !cmd[0])
		return (0);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd[0]);
		free(temp);
		if (!full_path)
			return (0);
		if (access(full_path, X_OK) == 0)
		{
			execve(full_path, cmd, envp);
			perror("execve");
			free(full_path);
			return (0);
		}
		free(full_path);
		i++;
	}
	return (0);
}

int	count_pipe_args(t_token *token)
{
	t_token	*current;
	int		i;

	i = 0;
	current = token;
	while (current && current->type != PIPE)
	{
		if (current->type == OUTFILE || current->type == INFILE
			|| current->type == APPEND || current->type == HEREDOC)
		{
			current = current->next;
			if (current)
				current = current->next;
		}
		else
		{
			i++;
			current = current->next;
		}
	}
	return (i);
}

pid_t	*ft_create_pid(t_data *data)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * (data->pipe + 1));
	if (!pids)
		ft_error("Pid error\n", 1);
	return (pids);
}
