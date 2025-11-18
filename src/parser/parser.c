/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:15:44 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/21 17:26:50 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_redirs(char *arg, t_token *token)
{
	if (ft_strncmp(arg, ">>", 2) == 0)
	{
		token->append++;
		return (APPEND);
	}
	if (ft_strncmp(arg, "<<", 2) == 0)
	{
		token->heredoc++;
		return (HEREDOC);
	}
	if (ft_strncmp(arg, ">", 1) == 0)
	{
		token->outfile++;
		return (OUTFILE);
	}
	if (ft_strncmp(arg, "<", 1) == 0)
	{
		token->infile++;
		return (INFILE);
	}
	return (-1);
}

static int	check_pipe_builtin(char *arg, t_data *data)
{
	if (ft_strncmp(arg, "|", 1) == 0)
	{
		data->pipe++;
		return (PIPE);
	}
	if (is_builtin(arg))
		return (BUILTIN);
	return (CMD);
}

void	get_token_type(char *arg, t_token *token, t_data *data)
{
	int	type;

	type = check_redirs(arg, token);
	if (type == -1)
		type = check_pipe_builtin(arg, data);
	token->type = type;
}

static int	add_token_to_list(char *arg, t_data *data, t_parse_token *tq,
			char **exp)
{
	t_token	*new;

	new = ft_token_new(arg);
	if (!new)
	{
		free_split(exp);
		free_token(tq);
		free_list(data->token);
		data->token = NULL;
		return (0);
	}
	put_lstback(&(data->token), new);
	get_token_type(arg, new, data);
	return (1);
}

void	split_arg(char *args, t_data *data)
{
	char			**exp;
	int				i;
	t_parse_token	*tq;

	if (!check_quotes(args, data))
		return ;
	if (!args || !*args)
		return ;
	tq = split_with_quotes(args);
	if (!tq)
		return ;
	exp = process_tokens(tq, data);
	if (!exp)
		return ;
	i = 0;
	while (exp[i])
	{
		if (!add_token_to_list(exp[i], data, tq, exp))
			return ;
		i++;
	}
	free_token(tq);
	free_split(exp);
}
