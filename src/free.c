/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:24:12 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/17 14:45:24 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(char *str, char **envp, int exit)
{
	int i;

	i = 0;
	while(envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	ft_error(str, exit);
}

void	free_split(char **str)
{
	int i;

	if(!str)
		return;
	i = 0;
	while(str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_list(t_token *head)
{
	t_token	*temp;

	while(head)
	{
		temp = head->next;
		free(head->str);
		free(head);
		head = temp;
	}
}

void	ft_free(char *str, char **envp)
{
	int i;

	i = 0;
	while(envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	printf("%s\n", str);
}

void free_env_list(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

void	free_cmd_array(char ***cmd_array)
{
	int	i;
	int	j;
		
	if (!cmd_array || !*cmd_array)
		return;
		
	i = 0;
	while (cmd_array[i]) 
	{
		j = 0;
		while (cmd_array[i][j]) {
			free(cmd_array[i][j]);  // Liberar cada string
			j++;
		}
		free(cmd_array[i]);  // Liberar el subarray
		i++;
	}
	free(cmd_array);  // Liberar el array principal
}

void free_args(t_data *data)
{
    int i = 0;

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
