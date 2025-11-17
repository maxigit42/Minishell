/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilline <mwilline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+ 	+#+           */
/*   Created: 2025/07/29 00:14:49 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/29 00:14:49 by mwilline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	handle_infile(t_token *current)
{
	int fd;

	fd = open(current->next->str, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		ft_error("Failed to open input file\n", 1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_outfile(t_token *current)
{
	int fd;

	fd = open(current->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		ft_error("open outfile\n", 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_append(t_token *current)
{
	int fd;

	fd = open(current->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		ft_error("open append", 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_heredoc(t_token *current)
{
	int fd;

	fd = ft_here_doc(current->next->str);
	if (fd > 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	close(fd);
	return (0);
}

void	ft_redirection(t_token *token)
{
	t_token *current;

	current = token;
	while (current)
	{
		if ((current->type == INFILE || current->type == OUTFILE
				|| current->type == APPEND || current->type == HEREDOC)
			&& !current->next)
			ft_error("Syntax error: expected filename after redirection\n", 1);
		if (current->type == INFILE)
			handle_infile(current);
		else if (current->type == OUTFILE)
			handle_outfile(current);
		else if (current->type == APPEND)
			handle_append(current);
		else if (current->type == HEREDOC)
			handle_heredoc(current);
		current = current->next;
	}
}
