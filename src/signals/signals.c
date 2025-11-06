/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilline <mwilline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:08:59 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/08 19:39:35 by mwilline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    set_signal(void)
{
    signal(SIGINT, ctrl_c);
    signal(SIGQUIT, SIG_IGN);
}

void ctrl_c(int sig) 
{
    g_signal = sig;
	ssize_t bruno;
    bruno = write(STDERR_FILENO, "\n", 1);
	if(!bruno)
		perror("Error");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

// void    ctrl_slash(int  sig)
// {
//     (void)sig;
// 	ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
// 	rl_replace_line("", 0);

// }