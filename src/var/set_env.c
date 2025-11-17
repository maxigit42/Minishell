#include "minishell.h"

static t_env	*find_env_key(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	update_env_value(t_env *node, const char *value)
{
	free(node->value);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = ft_strdup("");
}

static void	add_new_env_node(t_env **env, const char *key, const char *value)
{
	t_env	*new;
	t_env	*curr;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = ft_strdup("");
	new->next = NULL;
	if (!*env)
	{
		*env = new;
		return ;
	}
	curr = *env;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

void	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*node;

	if (!env || !key)
		return ;
	node = find_env_key(*env, key);
	if (node)
		update_env_value(node, value);
	else
		add_new_env_node(env, key, value);
}
