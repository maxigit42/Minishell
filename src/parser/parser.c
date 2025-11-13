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
    {
        free(new);
        ft_error(MALLOC_ERR, 0);
    }
    new->str = ft_strdup(str);
    new->type = 0;
    new->infile = 0;
    new->outfile = 0;
    new->heredoc = 0;
    new->append = 0;
    new->next = NULL;
    return(new);
}

void    get_token_type(char *arg, t_token *token, t_data *data)
{
    if(ft_strncmp(arg, ">>", 2) == 0)
    {
        token->append++;
        token->type = APPEND;
    }
    else if(ft_strncmp(arg, "<<", 2) == 0)
    {
        token->heredoc++;
        token->type = HEREDOC;
    }
    else if(ft_strncmp(arg, ">", 1) == 0)
    {
        token->outfile++;
        token->type = OUTFILE;
    }
    else if(ft_strncmp(arg, "<", 1) == 0)
    {
        token->infile++;
        token->type = INFILE;
    }
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

void split_arg(char *args, t_data *data)
{
    char **expanded_array;
    int i;
    t_token *new;
    t_parse_token *tokens_quote;

    if (!args || !*args)
        return;
	// quite la verificacion q hacias aqui para replicar el comportamiento bash

    // 2️⃣ Tokenizar respetando las comillas
    tokens_quote = split_with_quotes(args);
    if (!tokens_quote)
        return;

    // 3️⃣ Expandir variables de entorno ($VAR, $?, etc)
    expanded_array = process_tokens(tokens_quote, data);
    if (!expanded_array)
        return;

    // for(int x = 0; expanded_array[x]; x++)
    //     printf("Varaible expandidas: %s\n", expanded_array[x]);
    // 4️⃣ Crear lista enlazada de tokens y asignar tipos
    i = 0;
    while (expanded_array[i])
    {
        new = ft_token_new(expanded_array[i]);
        if (!new)
        {
            free_split(expanded_array);
            free(tokens_quote);
            free_list(data->token);
            data->token = NULL;
            return;
        }
        put_lstback(&(data->token), new);
        get_token_type(expanded_array[i], new, data);
        i++;
    }

    free_token(tokens_quote);
    // 5️⃣ Liberar arrays temporales
    free_split(expanded_array);
}

void free_token(t_parse_token *token)
{
    int i;

    i = 0;
    while(token[i].str)
    {
        free(token[i].str);
        i++;
    }
    free(token);
}

int is_builtin(const char *str)
{
    if (!str) return 0;
    return (
        ft_strcmp(str, "echo") == 0 ||
        ft_strcmp(str, "cd") == 0 ||
        ft_strcmp(str, "pwd") == 0 ||
        ft_strcmp(str, "export") == 0 ||
        ft_strcmp(str, "unset") == 0 ||
        ft_strcmp(str, "env") == 0 ||
        ft_strcmp(str, "exit") == 0
    );
}