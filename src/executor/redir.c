/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilline <mwilline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 00:14:49 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/29 00:14:49 by mwilline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_father(t_data *data, t_env *envp)
{
    if(data->pipe == 0)
        execute_single_cmd(data, envp);
    else if(data->pipe > 0)
    {
        execute_pipes(data, envp);
        free_args(data);
    }
}


void    execute_single_cmd(t_data *data, t_env *envp)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
        ft_error("Fork failed\n", 1);

    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        ft_redirection(data->token);
        execute_cmd(envp, data->cmd[0]);
        exit(127);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        data->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        data->exit_status = 128 + WTERMSIG(status);
    
    free_args(data);
}

void    execute_pipes(t_data *data, t_env *envp)
{
    int i;
    int **pipes;
    int n_cmd;
    t_token **com;
    int status;

    n_cmd = data->pipe + 1;
    pipes = create_pipes(data->pipe);
    if(!pipes)
    {
        ft_putstr_fd("minishell: pipe creation failed\n", 2);
        return;
    }
    i = 0;
    data->pid = ft_create_pid(data);
    com = ft_split_token(data, count_commands(data->token));
    while(i < n_cmd)
    {
        data->pid[i] = fork();
        if(data->pid[i] < 0)
            ft_error("Fork failed\n", 1);
        if(data->pid[i] == 0)
        {
            signal(SIGINT, SIG_DFL);    
            signal(SIGQUIT, SIG_DFL);
            setup_child_pipes(pipes, data->pipe, i);
            ft_redirection(com[i]);
            execute_cmd(envp, data->cmd[i]);
            exit(127);
        }
        i++;
    }
    close_all_pipes(pipes, data->pipe);
    i = 0;
    while(i < n_cmd)
    {
        waitpid(data->pid[i], &status, 0);
        if(i == n_cmd - 1)  // Solo el último comando actualiza exit_status
        {
            if (WIFEXITED(status))
                data->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                data->exit_status = 128 + WTERMSIG(status);
        }
        i++;
    }
    free_pipes(pipes, data->pipe);
    ft_free_list(com);
}

void    ft_free_list(t_token **token)
{
    int i;

    i = 0;
    while(token[i])
    {
        free_list(token[i]);
        i++;
    }
    free(token);
}

t_token *ft_token_clone_range(t_token *start, t_token *end)
{
    t_token *new_head = NULL;
    t_token *new_tail = NULL;

    while (start != end)
    {
        t_token *new_node = malloc(sizeof(t_token));
        if (!new_node)
            ft_error("malloc", 1);

        new_node->str = ft_strdup(start->str);
        if (!new_node->str)
            ft_error("strdup", 1);

        new_node->type = start->type;
        new_node->next = NULL;

        if (!new_head)
            new_head = new_node;
        else
            new_tail->next = new_node;

        new_tail = new_node;
        start = start->next;
    }
    return new_head;
}

t_token **ft_split_token(t_data *data, int n_cmd)
{
    t_token **subcmd;
    t_token *current = data->token;
    int i = 0;

    subcmd = malloc(sizeof(t_token *) * (n_cmd + 1));
    if (!subcmd)
        ft_error("malloc", 1);

    while (i < n_cmd && current)
    {
        t_token *start = current;
        // Avanzar hasta el siguiente pipe o hasta NULL
        while (current && current->type != PIPE)
            current = current->next;

        // Clonar desde start hasta current (exclusivo)
        subcmd[i] = ft_token_clone_range(start, current);

        // Si current es pipe, avanzar para el siguiente segmento
        if (current && current->type == PIPE)
            current = current->next;

        i++;
    }
    subcmd[i] = NULL;
    return subcmd;
}

void    ft_redirection(t_token *token)
{
    t_token     *current;
    int     fd;

    current = token;

    while(current)
    {
        if ((current->type == INFILE || current->type == OUTFILE 
            || current->type == APPEND || current->type == HEREDOC) && !current->next)
        {
            ft_putstr_fd("minishell: syntax error: expected filename after redirection\n", 2);
            exit(2);
        }

        if (current->type == INFILE)
        {
            fd = open(current->next->str, O_RDONLY);
            if (fd < 0)
            {
                perror("minishell");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if(current->type == OUTFILE)
        {
            fd = open(current->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(fd < 0)
            {
                perror("minishell");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if(current->type == APPEND)
        {
            fd = open(current->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("minishell");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if(current->type == HEREDOC)
        {
            fd = ft_here_doc(current->next->str);
            if(fd > 0)
            {
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if(fd == -1)
                exit(1);
        }
        current = current->next;
    }
}

pid_t    *ft_create_pid(t_data *data)
{
    pid_t   *pids;

    pids = malloc(sizeof(pid_t) * (data->pipe + 1));
    if(!pids)
        ft_error("Pid error\n", 1);
    return(pids);
}

void	print_cmds(t_data *data)
{
	int i = 0;
	
	if (!data || !data->cmd)
	{
		printf("⛔ data o data->cmd es NULL\n");
		return;
	}
	while (data->cmd[i])
	{
		printf("🔹 Comando %d:\n", i);
		int j = 0;
		while (data->cmd[i][j])
		{
			printf("  cmd[%d][%d] = \"%s\"\n", i, j, data->cmd[i][j]);
			j++;
		}
		i++;
	}
}
