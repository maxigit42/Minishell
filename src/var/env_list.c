#include "minishell.h"

static int	count_env_nodes(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static char	*join_env_pair(t_env *node)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(node->key, "=");
	if (!tmp)
		return (NULL);
	if (node->value)
		full = ft_strjoin(tmp, node->value);
	else
		full = ft_strdup(tmp);
	free(tmp);
	return (full);
}

static void	fill_env_array(char **array, t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		array[i] = join_env_pair(env);
		i++;
		env = env->next;
	}
	array[i] = NULL;
}

char	**env_list_to_array(t_env *env)
{
	char	**array;
	int		count;

	count = count_env_nodes(env);
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	fill_env_array(array, env);
	return (array);
}
