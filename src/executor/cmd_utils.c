/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:47:31 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/17 14:31:17 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmds(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	if (!data || !data->cmd)
	{
		printf("⛔ data o data->cmd es NULL\n");
		return ;
	}
	while (data->cmd[i])
	{
		printf("🔹 Comando %d:\n", i);
		j = 0;
		while (data->cmd[i][j])
		{
			printf("  cmd[%d][%d] = \"%s\"\n", i, j, data->cmd[i][j]);
			j++;
		}
		i++;
	}
}

char	**get_cmds(t_data *data)
{
	char	**args;
	t_token	*current;
	int		i;

	i = 0;
	current = data->token;
	while (current && current->type == CMD)
	{
		i++;
		current = current->next;
	}
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
		return (NULL);
	i = 0;
	current = data->token;
	while (current && current->type == CMD)
	{
		args[i++] = ft_strdup(current->str);
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

int	count_commands(t_token *token)
{
	t_token	*current;
	int		count;

	current = token;
	count = 1;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		current = current->next;
	}
	return (count);
}
