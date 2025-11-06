/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:51:03 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/06 17:53:28 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_echo(char **args)
{
	int i;
	int newline;
	
	i = 1;
	newline = 1;
	if(args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while(args[i])
	{
		ft_putstr_fd(args[i], 1);
		if(args[1 + i])
			ft_putchar_fd(' ', 1);  // creo que no estoy pasando un caracter de espacio y estoy pasando una cadena
		i++;
	}
	if(newline)
		ft_putstr_fd("\n", 1);
	return(0);
}