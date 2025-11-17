/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:06:53 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/22 21:19:12 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0;

void	start_args(char **argv, int argc, t_data *data, char **envp)
{
	(void)argc;
	(void)argv;
	data->token = NULL;
	data->env = envp_to_list(envp);
	data->exit_status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;

	start_args(argv, argc, &data, envp);
	set_signal();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		data.token = NULL;
		data.pipe = 0;
		if (input[0])
		{
			split_arg(input, &data);
			mini_init(&data, data.env);
			if (data.token)
				free_list(data.token);
		}
		free(input);
	}
	free_env_list(data.env);
	return (0);
}

void	mini_init(t_data *data, t_env *envp)
{
	if (!data->token)
		return ;
	ft_take_cmd(data);
	if (data->pipe == 0)
		redir_cmd(data, envp);
	else
	{
		execute_pipes(data, envp);
		free_args(data);
	}
	if (data->cmd)
	{
		free_cmd_array(data->cmd);
		data->cmd = NULL;
	}
	if (data->token)
	{
		free_list(data->token);
		data->token = NULL;
	}
}

void	ft_take_cmd(t_data *data)
{
	t_token	*current;
	int	i;
	char	**subcmd;

	if (!alloc_cmd_array(data))
		return ;
	current = data->token;
	i = 0;
	while (current)
	{
		subcmd = alloc_subcmd(current);
		if (!subcmd)
			return (free_cmd_array(data->cmd), (void)(data->cmd = NULL));
		fill_subcmd(&current, subcmd);
		data->cmd[i++] = subcmd;
		if (current && current->type == PIPE)
			current = current->next;
	}
	data->cmd[i] = NULL;
}
