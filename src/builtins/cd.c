/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilline <mwilline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 04:06:55 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/17 05:21:13 by mwilline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_token *token, t_env *env)
{
	char	*path;

	if (token->next && token->next->type == CMD)
		path = token->next->str;
	else
		path = get_env_value_list(env, "HOME");
	if (!path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
