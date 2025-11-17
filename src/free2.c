/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilline <mwilline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 17:42:41 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/17 21:33:32 by mwilline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_array(char ***cmd_array)
{
	int	i;
	int	j;

	if (!cmd_array || !*cmd_array)
		return ;
	i = 0;
	while (cmd_array[i])
	{
		j = 0;
		while (cmd_array[i][j])
		{
			free(cmd_array[i][j]);
			j++;
		}
		free(cmd_array[i]);
		i++;
	}
	free(cmd_array);
}

void	free_args(t_data *data)
{
	int	i;

	i = 0;
	if (data->cmd)
	{
		while (data->cmd[i])
			free_split(data->cmd[i++]);
		free(data->cmd);
		data->cmd = NULL;
	}
	if (data->pid)
	{
		free(data->pid);
		data->pid = NULL;
	}
}

void	ft_free_list(t_token **token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		free_list(token[i]);
		i++;
	}
	free(token);
}

void	free_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	free_token(t_parse_token *token)
{
	int	i;

	i = 0;
	while (token[i].str)
	{
		free(token[i].str);
		i++;
	}
	free(token);
}
