/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:58:11 by mwilline          #+#    #+#             */
/*   Updated: 2025/11/06 18:18:34 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_exit(char **args)
{
    int status;

    status = 0;

    ft_putstr_fd("exit\n", 1);
    if (args[1])
    {
        if (!ft_isdigit_str(args[1]))
        {
            ft_putstr_fd("minishell: exit: ", 2);
            ft_putstr_fd(args[1], 2);
            ft_putstr_fd(": numeric argument required\n", 2);
            exit(2);  
        }
        if (args[2]) // demasiados argumentos
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            return (1);
        }
        status = ft_atoi(args[1]);
    }
    exit(status);
}


int ft_isdigit_str(char *str)
{
    int i = 0;

    if (!str || !*str)
        return (0);

    // permitir signo opcional al inicio
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}
