/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximo <maximo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:07:42 by mwilline          #+#    #+#             */
/*   Updated: 2025/07/22 21:05:58 by maximo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <libft.h>
#include "message.h"
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_env{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

typedef struct s_parse_token
{
	char *str;
	int in_single_quote;
	int in_double_quote;
} t_parse_token;

typedef struct s_token
{
	int				type;
	char			*str;
	
	int				infile;
	int				outfile;
	int				heredoc;
	int				append; 
	
	struct t_data	*data;
	struct s_token	*next;
}	t_token;

typedef struct t_data
{
	t_token *token;
	t_env	*env;
	pid_t	*pid;

	int		pipe;
	int		exit_status;//

	char	***cmd;
	char	**single_cmd;
} t_data;

void	execute_cmd(t_env *env, char **cmd);
int		find_valid_path(char **paths, char **envp, char **cmd);
char	*get_envp(char **envp);
char	**get_cmds(t_data *data);
int		ft_here_doc(char *limit);

// redir
void	ft_father(t_data *data, t_env *envp);
pid_t	*ft_create_pid(t_data *data);
void	execute_pipes(t_data *data, t_env *envp);
void	execute_single_cmd(t_data *data, t_env *envp);
void	ft_redirection(t_token *token);
t_token	**ft_split_token(t_data *data, int n_cmd);
t_token	*ft_token_clone_range(t_token *start, t_token *end);

// pipes
void	setup_child_pipes(int **pipes, int n_pipes, int index);
int		**create_pipes(int n_pipes);
void	close_all_pipes(int **pipes, int n_pipes);
void	free_pipes(int **pipes, int n_pipes);

// parse
void	split_arg(char *args, t_data *data);
void	put_lstback(t_token **head, t_token *new);
void	get_token_type(char *arg, t_token *token, t_data *data);
t_token	*ft_token_new(char *str);
int		is_builtin(const char *str);

// quote_handle
int		quotes_closed(char *s);
t_parse_token *split_with_quotes(char *str);
char	*expand_variables(char *str, t_data *data);
char **process_tokens(t_parse_token *tokens, t_data *data);

// utils
void	ft_error(char *str, int quit);
void	free_split(char **str);
void	free_and_exit(char *str, char **envp, int exit);
void	free_list(t_token *head);
void	ft_free(char *str, char **envp);
int		count_pipe_args(t_token *token);
void	free_args(t_data *data);
int		count_commands(t_token *token);
void	ft_free_list(t_token **token);
void	free_env_list(t_env *env);
void	free_cmd_array(char ***cmd_array);
int		ft_strcmp(const char *s, const char *ss);
void free_token(t_parse_token *token);
char	*expand_input(char *input);
int		get_var_name_len(char *str);
char	*get_var_name(char *str, int len);
int builtin_modifies_state(char *cmd);
int has_redirection(t_token *token);

// signs
void	set_signal(void);
void	ctrl_c(int sig);

// var
char	**copy_envp(char **envp);
char	*get_env_value_list(t_env *env, const char *key);
t_env	*create_env_node(char *entry);
t_env	*envp_to_list(char **envp);
void	set_env_value(t_env **env, const char *key, const char *value);
char	**env_list_to_array(t_env *env);
void	unset_env_value(t_env **env, const char *key);

// build
int		execute_builtin(t_data *data, t_env **env);
int		builtin_cd(t_token *token, t_env *env);
int		builtin_echo(char **args, t_data *data);
int		builtin_env(t_env *env);
int		builtin_exit(char **args);
int		ft_isdigit_str(char *str);
int		builtin_export(char **args, t_env **env);
int		builtin_pwd(void);
int		builtin_unset(char **args, t_env **env);

void	mini_init(t_data *data, t_env *envp);
void	ft_take_cmd(t_data *data);
void	take_single_cmd(t_data *data);
void	print_cmds(t_data *data);

void	redir_cmd(t_data *data, t_env *envp);
int alloc_cmd_array(t_data *data);
char	**alloc_subcmd(t_token *current);
void	fill_subcmd(t_token **cur, char **subcmd);

//exe_utils
int	handle_path_command(char **cmd, char **envp_array);
void	handle_no_path(char **cmd, char **envp_array);
void	handle_path_search(char **paths, char **envp_array, char **cmd);

//exec
int	is_path(char *cmd);
void	execute_direct_path(char **cmd, char **envp_array);




#endif