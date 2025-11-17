/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:46:19 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/22 18:32:43 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// expande la entrada
char	*expand_input(char *input)
{
	char	*new_line;
	char 	*temp;
	char	*joined;

	while (!quotes_closed(input))
	{
		new_line = readline("> ");
		temp = ft_strjoin(input, "\n");
		joined = ft_strjoin(temp, new_line);
		free(temp);
		free(input);
		free(new_line);
		input = ft_strdup(joined);
		free(joined);
	}
	return (input);
}

void ft_error(char *str, int quit)
{
	printf("%s\n", str);
	exit(quit);
}

void	free_and_exit(char *str, char **envp, int exit)
{
	int i;

	i = 0;
	while(envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	ft_error(str, exit);
}

void	free_split(char **str)
{
	int i;

	if(!str)
		return;
	i = 0;
	while(str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_list(t_token *head)
{
	t_token	*temp;

	while(head)
	{
		temp = head->next;
		free(head->str);
		free(head);
		head = temp;
	}
}

void	ft_free(char *str, char **envp)
{
	int i;

	i = 0;
	while(envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	printf("%s\n", str);
}

int count_pipe_args(t_token *token)
{
	t_token *current;
	int i;

	i = 0;
	current = token;
	while (current && current->type != PIPE)
	{
		if (current->type == OUTFILE || current->type == INFILE
			|| current->type == APPEND || current->type == HEREDOC)
		{
			current = current->next;
			if (current)
				current = current->next;
		}
		else
		{
			i++;
			current = current->next;
		}
	}
	return(i);
}

void free_args(t_data *data)
{
    int i = 0;

    if (data->cmd)
    {
        while (data->cmd[i])
            free_split(data->cmd[i++]);
        free(data->cmd);
        data->cmd = NULL;
    }

    if (data->pid)
    {
        free(data->pid);
        data->pid = NULL;
    }
}

int count_commands(t_token *token)
{
	t_token *current = token;
	int count = 1;  // empieza en 1 porque sin pipes hay al menos un comando

	while (current)
	{
		if (current->type == PIPE)
			count++;
		current = current->next;
	}
	return count;
}

void free_env_list(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

void free_cmd_array(char ***cmd_array)
{
    int i;
    int j;
    
    if (!cmd_array || !*cmd_array)
        return;
    
    i = 0;
    while (cmd_array[i]) {
        j = 0;
        while (cmd_array[i][j]) {
            free(cmd_array[i][j]);  // Liberar cada string
            j++;
        }
        free(cmd_array[i]);  // Liberar el subarray
        i++;
    }
    free(cmd_array);  // Liberar el array principal
}

int ft_strcmp(const char *s, const char *ss)
{
	int i;

	i = 0;
	while(s[i] || ss[i])
	{
		if(s[i] != ss[i])
			return((unsigned char)s[i] - (unsigned char)ss[i]);
		i++;
	}
	return(0);
}

void	ft_free_list(t_token **token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		free_list(token[i]);
		i++;
	}
	free(token);
}

t_token	**ft_split_token(t_data *data, int n_cmd)
{
	t_token	**subcmd;
	t_token	*current = data->token;
	int		i;

	i = 0;
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

t_token	*ft_token_clone_range(t_token *start, t_token *end)
{
	t_token	*new_head;
	t_token	*new_tail;
	t_token	*new_node;

	new_head = NULL;
	new_tail = NULL;
	while (start != end)
	{
		new_node = malloc(sizeof(t_token));
		if (!new_node)
			ft_error("malloc", 1);
		new_node->str = strdup(start->str);
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
	return (new_head);
}

pid_t	*ft_create_pid(t_data *data)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * (data->pipe + 1));
	if (!pids)
		ft_error("Pid error\n", 1);
	return (pids);
}

void	ft_father(t_data *data, t_env *envp)
{
	if (data->pipe == 0)
		execute_single_cmd(data, envp);
	else if (data->pipe > 0)
	{
		execute_pipes(data, envp);
		free_args(data);
	}
}

void	print_cmds(t_data *data)
{
	int i;

	i = 0;
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

void	free_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

int	find_valid_path(char **paths, char **envp, char **cmd)
{
	char	*temp;
	char	*full_path;
	int		i;

	if (!cmd || !cmd[0])
		return (0);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd[0]);
		free(temp);
		if (!full_path)
			return (0);
		if (access(full_path, X_OK) == 0)
		{
			execve(full_path, cmd, envp);
			perror("execve");
			free(full_path);
			return (0);
		}
		free(full_path);
		i++;
	}
	return (0);
}

char	**get_cmds(t_data *data)
{
	char	**args;
	t_token	*current;
	int		i;

	i = 0;
	current = data->token;
	while (current && current->type == CMD)
	{
		i++;
		current = current->next;
	}
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
		return (NULL);
	i = 0;
	current = data->token;
	while (current && current->type == CMD)
	{
		args[i++] = ft_strdup(current->str);
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

int	get_var_name_len(char *str)
{
	int	len = 0;

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

int has_redirection(t_token *token)
{
    while (token)
    {
        if (token->type == OUTFILE || token->type == INFILE
            || token->type == APPEND || token->type == HEREDOC)
            return (1);
        token = token->next;
    }
    return (0);
}

int builtin_modifies_state(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "cd")
         || !ft_strcmp(cmd, "export")
         || !ft_strcmp(cmd, "unset")
         || !ft_strcmp(cmd, "exit"));
}
