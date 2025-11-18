/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: biniesta <biniesta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:37:47 by biniesta          #+#    #+#             */
/*   Updated: 2025/11/18 18:55:35 by biniesta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_var_name_len(char *str)
{
	int	len;

	len = 0;
	if (str[len] == '?')
		return (1);
	if (!ft_isalpha(str[len]) && str[len] != '_')
		return (0);
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

char	*get_var_name(char *str, int len)
{
	char	*name;

	if (len <= 0)
		return (NULL);
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str, len + 1);
	return (name);
}

int	check_quotes(char *str, t_data *data)
{
	int	i;
	int	single;
	int	doble;

	single = 0;
	doble = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && doble == 0)
			single = !single;
		else if (str[i] == '"' && single == 0)
			doble = !doble;
		i++;
	}
	if (single || doble)
	{
		data->exit_status = 2;
		ft_putstr_fd("syntax error: unclosed quotes\n", 2);
		return (0);
	}
	return (1);
}
