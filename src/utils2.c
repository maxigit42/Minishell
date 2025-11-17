/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 04:58:28 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/17 05:10:54 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	put_lstback(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!head || !new)
		return;
	if (!*head)
	{
		*head = new;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_token	*ft_token_new(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
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
	return (new);
}

int	is_builtin(const char *str)
{
	if (!str)
		return (0);
	return (ft_strcmp(str, "echo") == 0
		|| ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "pwd") == 0
		|| ft_strcmp(str, "export") == 0
		|| ft_strcmp(str, "unset") == 0
		|| ft_strcmp(str, "env") == 0
		|| ft_strcmp(str, "exit") == 0);
}