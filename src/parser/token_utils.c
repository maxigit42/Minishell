/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:32:50 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/17 14:51:12 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	**ft_split_token(t_data *data, int n_cmd)
{
	t_token	**subcmd;
	t_token	*current;
	t_token	*start;
	int		i;

	i = 0;
	subcmd = malloc(sizeof(t_token *) * (n_cmd + 1));
	current = data->token;
	if (!subcmd)
		ft_error("malloc", 1);
	while (i < n_cmd && current)
	{
		start = current;
		while (current && current->type != PIPE)
			current = current->next;
		subcmd[i] = ft_token_clone_range(start, current);
		if (current && current->type == PIPE)
			current = current->next;
		i++;
	}
	subcmd[i] = NULL;
	return (subcmd);
}

t_token	*ft_token_clone_range(t_token *start, t_token *end)
{
	t_token	*new_head;
	t_token	*new_tail;
	t_token	*new_node;

	new_head = NULL;
	new_tail = NULL;
	while (start != end)
	{
		new_node = malloc(sizeof(t_token));
		if (!new_node)
			ft_error("malloc", 1);
		new_node->str = strdup(start->str);
		if (!new_node->str)
			ft_error("strdup", 1);
		new_node->type = start->type;
		new_node->next = NULL;
		if (!new_head)
			new_head = new_node;
		else
			new_tail->next = new_node;
		new_tail = new_node;
		start = start->next;
	}
	return (new_head);
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

void	put_lstback(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!head || !new)
		return ;
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
