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

int main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;
	(void)argc;
	(void)argv;
	data.token = NULL;
	data.env = envp_to_list(envp);
	data.exit_status = 0;

	set_signal();
	while(1)
	{
		input = readline("minishell$ ");
		if(!input)
			break;
		if(*input)
			add_history(input);
		data.token = NULL;
		data.pipe = 0;
		if(input[0])
		{
		split_arg(input, &data);
		mini_init(&data, data.env);
		if (data.token)
    		free_list(data.token);
		}
		free(input);
	}
	free_env_list(data.env);
	return(0);
}

void mini_init(t_data *data, t_env *envp)
{
    ft_take_cmd(data);
    if (!data->token)
        return;

    /* SOLO 1 COMANDO (SIN PIPES) */
    if (data->pipe == 0)
    {
        char *cmd = data->cmd[0][0];

        /* Builtin en el padre SOLO si modifica estado y NO hay redirecciones */
        if (data->token->type == BUILTIN
            && builtin_modifies_state(cmd)
            && !has_redirection(data->token))
        {
            data->exit_status = execute_builtin(data, &data->env);
        }
        else
        {
            /* ➜ builtin con redirección o comando normal → HIJO */
            execute_single_cmd(data, envp);
        }
    }
    else
    {
        /* MÚLTIPLES COMANDOS (PIPES) */
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
	int		i;
	int		j;
	char	**subcmd;
	int		arg_count;

	if (!data || !data->token)
		return;

	data->cmd = malloc(sizeof(char **) * (data->pipe + 2));
	if (!data->cmd)
		return;
	i = 0;
	current = data->token;

	while (current)
	{
		j = 0;
		arg_count = count_pipe_args(current);
		if (arg_count <= 0)
			arg_count = 1;
		subcmd = malloc(sizeof(char *) * (arg_count + 1));
		if (!subcmd)
			return (free_cmd_array(data->cmd), (void)(data->cmd = NULL));

		while (current && current->type != PIPE)
		{
			// Saltar redirecciones
			if (current->type == OUTFILE || current->type == INFILE
				|| current->type == APPEND || current->type == HEREDOC)
			{
				current = current->next;
				if (current)
					current = current->next;
				continue;
			}
			subcmd[j++] = ft_strdup(current->str);
			current = current->next;
		}
		subcmd[j] = NULL;
		data->cmd[i++] = subcmd;

		if (current && current->type == PIPE)
			current = current->next;
	}
	data->cmd[i] = NULL;
}

