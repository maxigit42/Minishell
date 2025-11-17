/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 21:28:44 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/17 05:32:02 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_exit_status(t_data *data, char **str)
{
	char	*exit_extatus;
	char	*result;

	exit_extatus = ft_itoa(data->exit_status);
	result = ft_strjoin(*str, exit_extatus);
	free(exit_extatus);
	free(*str);
	*str = result;
	return (2);
}

static int	handle_var(t_data *data, char **str, char *tmp)
{
	char	*val;
	char	*result;
	int		var_len;
	char	*name;

	var_len = get_var_name_len(tmp);
	if (var_len == 0)
	{
		val = ft_strdup("$");
		result = ft_strjoin(*str, val);
		free(*str);
		free(val);
		*str = result;
		return (1);
	}
	name = get_var_name(tmp, var_len);
	val = get_env_value_list(data->env, name);
	if (!val)
		val = "";
	result = ft_strjoin(*str, val);
	free(name);
	free(*str);
	*str = result;
	return (var_len + 1);
}

char	*expand_variables(char *str, t_data *data)
{
	int		i;
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	if (!result || !str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			if (str[i + 1] == '?')
				i += handle_exit_status(data, &result);
			else
				i += handle_var(data, &result, &str[i + 1]);
			continue ;
		}
		char c[2] = { str[i], '\0' };
		tmp = ft_strjoin(result, c);
		free(result);
		result = tmp;
		i++;
	}
	return (result);
}
