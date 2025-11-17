#include "minishell.h"

void	redir_cmd(t_data *data, t_env *envp)
{
	char *cmd;

	cmd = data->cmd[0][0];
	if (data->token->type == BUILTIN
            && builtin_modifies_state(cmd)
            && !has_redirection(data->token))
	{
		data->exit_status = execute_builtin(data, &data->env);
	}
	else
		execute_single_cmd(data, envp);
}

int alloc_cmd_array(t_data *data)
{
	data->cmd = malloc(sizeof(char **) * (data->pipe + 2));
	if(!data->cmd)
		return(0);
	return(1);
}

char	**alloc_subcmd(t_token *current)
{
	int		arg_count;
	char	**subcmd;

	arg_count = count_pipe_args(current);
	if (arg_count <= 0)
		arg_count = 1;
	subcmd = malloc(sizeof(char *) * (arg_count + 1));
	return (subcmd);
}

void	fill_subcmd(t_token **cur, char **subcmd)
{
	int	j;

	j = 0;
	while (*cur && (*cur)->type != PIPE)
	{
		if ((*cur)->type == OUTFILE || (*cur)->type == INFILE
			|| (*cur)->type == APPEND || (*cur)->type == HEREDOC)
		{
			*cur = (*cur)->next;
			if (*cur)
				*cur = (*cur)->next;
			continue;
		}
		subcmd[j++] = ft_strdup((*cur)->str);
		*cur = (*cur)->next;
	}
	subcmd[j] = NULL;
}
