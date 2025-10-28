/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:46:21 by mwilline          #+#    #+#             */
/*   Updated: 2025/10/29 00:27:05 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_export(char **args, t_env **env)
{
	int i;

	i = 1;
	if(!args[i])
	{
		t_env *tmp = *env;
		while(tmp)
		{
			if(tmp->value)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s\n", tmp->key);
			tmp = tmp->next;
		}
		return(0);
	}
	while (args[i])
    {
        char *sep = ft_strchr(args[i], '=');
        if (sep)
        {
            char *key = ft_substr(args[i], 0, sep - args[i]);
            char *value = ft_strdup(sep + 1);
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
