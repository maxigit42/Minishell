/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 04:27:55 by mwilline          #+#    #+#             */
/*   Updated: 2025/10/29 00:24:55 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_envp(char **envp)
{
	char	**copy;
	int i;

	i = 0;
	while(envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if(!copy)
		return(NULL);
	i = 0;
	while(envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if(!copy)
			return(NULL);
		i++;
	}
	copy[i] = NULL;
	return(copy);
}

t_env *create_env_node(char	*entry)
{
	t_env *new;
	char *sep;

	new = malloc(sizeof(t_env));
	if(!new)
		return(NULL);
	sep = ft_strchr(entry, '=');
	if(sep)                               //crea un nodo con cada variable global
	{
		new->key = ft_substr(entry, 0, sep - entry);
		new->value =  ft_strdup(sep + 1);
	}
	else
	{
		new->key = ft_strdup(entry);
		new->value = NULL;
	}
	new->next = NULL;
	return(new);
}

t_env	*envp_to_list(char **envp)
{
	t_env *head;
	t_env *curr;
	t_env *new;
								
	head = NULL;			//pasa todas las variables globales para crear un nodo de cada una
	curr = NULL;
	while(*envp)
	{
		new = create_env_node(*envp);
		if(!new)
			ft_error("Error\n", 1);
		if(!head)
			head = new;
		else
			curr->next = new;
		curr = new;
		envp++;
	}
	return(head);
}

char	*get_env_value_list(t_env *env, const char *key)
{
	int i;

	i = ft_strlen(key);
	while(env)
	{
		if(ft_strncmp(env->key, key, i) == 0)
			return(env->value);
		env = env->next;
	}
	return(NULL);
}

void	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env *curr = *env;

	while(curr)
	{
		if(ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(value);
			return;
		}
		curr = curr->next;
	}
	t_env *new;
	new = malloc(sizeof(t_env));
	if(!new)
		return;
	new->key = ft_strdup(key);
	if(value)
		new->value = ft_strdup(value);
	else
		new->value = ft_strdup("");
	new->next = NULL;
	if(!*env)
	{
		*env = new;
		return;
	}
	curr = *env;
    while (curr->next)
        curr = curr->next;
    curr->next = new;
}

void	unset_env_value(t_env **env, const char *key)
{
	t_env *curr;
	t_env *prev;
	
	curr = *env;
	prev = NULL;
	while(curr)
	{
		if(ft_strncmp(curr->key, key, ft_strlen(curr->key)) == 0)
		{
			if(prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	
}

char **env_list_to_array(t_env *env)
{
    int count = 0;
    t_env *temp = env;
    char **array;
    int i;
	char *key_equal;

    while (temp)
    {
        count++;
        temp = temp->next;
    }

    array = malloc(sizeof(char *) * (count + 1));
    if (!array)
        return (NULL);

    temp = env;
    i = 0;
    while (temp)
    {
        if (temp->value)
        {
            key_equal = ft_strjoin(temp->key, "=");
            array[i] = ft_strjoin(key_equal, temp->value);
            free(key_equal);
        }
        else
        {
            array[i] = ft_strjoin(temp->key, "=");
        }
        i++;
        temp = temp->next;
    }
    array[i] = NULL;
    return (array);
}

