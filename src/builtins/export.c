/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: runo <runo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:46:21 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/14 10:41:24 by runo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_display(t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (tmp->value)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
	return (0);
}

int	builtin_export(char **args, t_env **env)
{
	int		i;
	char	*sep;
	char	*key;
	char	*value;

	i = 1;
	if (!args[i])
		return (env_display(env));
	while (args[i])
	{
		sep = ft_strchr(args[i], '=');
		if (sep)
		{
			key = ft_substr(args[i], 0, sep - args[i]);
			value = ft_strdup(sep + 1);
			set_env_value(env, key, value);
			free(key);
			free(value);
		}
		else
			set_env_value(env, args[i], NULL);
		i++;
	}
	return (0);
}

