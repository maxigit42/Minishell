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

t_token     *ft_token_new(char *str)
{
    t_token  *new;
    
    new = malloc(sizeof(t_token));
    if(!new)
        return(NULL);
    new->str = ft_strdup(str);
    if(!new->str)
    {
        free(new);
        return(NULL);
    }
    new->type = 0;
    new->next = NULL;
    return(new);
}

void    get_token_type(char *arg, t_token *token, t_data *data)
{
    if(ft_strncmp(arg, ">>", 2) == 0)
        token->type = APPEND;
    else if(ft_strncmp(arg, "<<", 2) == 0)
        token->type = HEREDOC;
    else if(ft_strncmp(arg, ">", 1) == 0)
        token->type = OUTFILE;
    else if(ft_strncmp(arg, "<", 1) == 0)
        token->type = INFILE;
    else if(ft_strncmp(arg, "|", 1) == 0)
    {
        data->pipe++;
        token->type = PIPE;
    }
    else if(is_builtin(arg))
        token->type = BUILTIN;
    else
        token->type = CMD;
}


void    put_lstback(t_token **head, t_token *new)
{
    t_token *temp;

    if(!head || !new)
        return;
    if(!*head)
    {
        *head = new;
        return;
    }
    temp = *head;
    while(temp->next)
        temp = temp->next;
    temp->next = new;
}

void    split_arg(char *args, t_data *data)
{
    char **token_array;
    char **original_array;
    int i;
    t_token *new;

    i = 0;
    if(!check_quotes(args))
    {
        data->exit_status = 2;
        return;
    }
    token_array = split_with_quotes(args);
    if (!token_array)
        return ;
    original_array = token_array;
    token_array = process_tokens(token_array, data);
    if(!token_array)
        return;
    while(token_array[i])
    {
        new = ft_token_new(token_array[i]);
        if(!new)
        {
            free_split(token_array);
            free_list(data->token);
            data->token = NULL;
            return;
        }
        put_lstback(&(data->token), new);
        get_token_type(token_array[i], new, data);
        i++;
    }
    free_split(token_array);
}

int	is_builtin(const char *str)
{
	if (!str)
		return (0);
	return (
		ft_strcmp(str, "echo") == 0
		|| ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "pwd") == 0
		|| ft_strcmp(str, "export") == 0
		|| ft_strcmp(str, "unset") == 0
		|| ft_strcmp(str, "env") == 0
		|| ft_strcmp(str, "exit") == 0
	);
}
