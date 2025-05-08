/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:53:12 by sluterea          #+#    #+#             */
/*   Updated: 2025/05/06 20:08:26 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/get_next_line_bonus.h"
# include "../libft/includes/ft_printf_bonus.h"
# include "../libft/includes/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stddef.h>

/**************************/
/********  DEFINES  *******/
/**************************/

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define WHITE "\033[1;37m"
# define BLACK "\033[1;30m"
# define BL "\033[94m"
# define DEF "\033[0m"

/**************************/
/*********  ENUMS  ********/
/**************************/

/* Token enum */
enum e_type
{
	COMMAND,
	ARGUMENT,
	AND,
	OR,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	FLAG,
	UNKNOWN,
	O_BRACKET,
	C_BRACKET,
	WILDCARD,
	LITERAL
};

/* Tree enum */
enum e_node_type
{
	BRACKET_NODE,
	PIPE_NODE,
	AND_NODE,
	OR_NODE,
	CMD_NODE
};

/**************************/
/******* STRUCTURES *******/
/**************************/

typedef struct s_env_var_h
{
	char	*var;
	char	*str;
	int		j;
}	t_env_h;

/* Token Structures */

typedef struct s_h_str
{
	char	*part;
	char	*joined;
	char	*tmp;
	int		starts_with_quote;
	int		is_first_char;
}	t_hstr;

typedef struct s_var_content
{
	char	*value;
	int		len;
}	t_var_content;

typedef struct s_token
{
	char			*str;
	enum e_type		type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_parse2
{
	int				len;
	int				curr_pos;
	int				last_pos;
	int				count;
	int				flag;
	int				error;
	int				diff;
	char			*delimiters;
	int				empty;
	int				is_first_tk;
	int				prev_heredoc;
	int				is_dquote;
}	t_parse2;

typedef struct s_handle_double
{
	char	*str;
	char	*temp;
	char	*env_value;
	char	*char_str;
}	t_h_d;

typedef struct s_handle_double_ns
{
	char	*str;
	char	*temp;
	char	*env_value;
	char	*char_str;
	int		pos;
}	t_ns;

/* Exec structs */
typedef struct s_exec_cmd_split
{
	pid_t	pid;
	int		status;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);
}	t_ec;

/* Heredoc */
typedef struct s_handle_heredoc
{
	int		fd[2];
	pid_t	pid;
	int		status;
	char	*line;
	bool	quoted;
	char	*clean_delim;
}	t_here;

/* Tree Structures */
typedef struct s_cmd_node
{
	char	*command;
	char	**args;
	char	*heredoc;
	char	*append;
	int		redir_in;
	int		redir_out;
	int		redir_err;
}	t_cmd_node;

typedef struct s_node
{
	enum e_node_type	type;
	t_cmd_node			command;
	int					error;
	struct s_node		*left;
	struct s_node		*right;
}	t_node;

/* Tree parsing struct */
typedef struct s_parsing_save_lines
{
	t_node		*left;
	t_node		*op_node;
	int			token_type;
	t_node		*right;
}	t_p_norm;

/* Parsing split */
typedef struct s_split_parse
{
	size_t	index;
	size_t	count;
	char	**arr;
	size_t	i;
	size_t	len;
	size_t	j;
}	t_split_p;

/* Shell struct */
typedef struct s_shell
{
	char					**env;
	t_token					**tk;
	t_node					*tree;
	char					*input;
	char					*dir;
	int						error;
	int						std[2];
}	t_shell;

/* Echo struct */
typedef struct s_echo
{
	size_t		i;
	char		*tmp;
	char		*new_tmp;
	int			new_line;
}	t_echo;

/* Export Struct */
typedef struct s_export_vars
{
	char	**new_env;
	int		i;
	int		j;
	char	*v;
	int		append;
}	t_export_vars;

typedef struct s_expand_env
{
	char	*var;
	char	*str;
	char	*new_str;
	int		j;
	int		count;
	char	*env_var;
	int		prev_type;
}	t_expand;

/* Env struct */
typedef struct s_unset
{
	size_t		i;
	size_t		j;
	char		*v;
	size_t		len;
	char		**env_cpy;
}	t_unset;

typedef struct s_append
{
	size_t		i;
	char		*v;
	size_t		l;
	char		*ap;
	char		*tmp;
	char		*n;
	int			err;
}	t_append;

/* Dir struct */
typedef struct s_setdir
{
	char	*set_new;
	char	*set_old;
	char	*tmp;
}	t_sd;

/* Main */
typedef struct s_main
{
	t_shell	sh;
	t_token	*tk_cpy;
	t_node	*tree_ptr;
}	t_main;

/**************************/
/******** TOKENIZE ********/
/**************************/

/* Tokenise */
t_token			**tokenize(char *line, t_shell *sh);
t_token			*add_token(t_token **tokens, char *str, enum e_type type);
void			h_del(char *line, t_parse2 *parse, t_token **tk);

/* Handle pipes/redirs/and/bracket */
int				handle_pipe(char *line, t_parse2 *parse, t_token **tokens);
int				handle_and(char *line, t_parse2 *parse, t_token **tokens);
int				hand_r_in(char *line, t_parse2 *parse, t_token **tokens);
int				hand_r_out(char *line, t_parse2 *parse, t_token **tokens);
int				handle_brackets(char *line, t_parse2 *parse, t_token **tokens);

/* Handle  str/flag/literal/var */
int				handle_str(char *line, t_parse2 *p, t_token **tk, t_shell *sh);

/* Tk utils */
enum e_type		check_type(t_token *token);
void			get_diff_curr_len(t_parse2 *parse);
void			update_current_position(t_parse2 *parse);
int				find_repeated_delimiter(char *l, char c, int i);
char			*ft_add_quotes(const char *input);
char			*ft_char_to_str(char c);

/* Handle quotes*/
char			*h_literal(char *l, t_parse2 *p);
char			*h_d_quotes(char *l, t_parse2 *p, t_shell *sh);

/* Tk expand env uils*/
void			handle_specials(t_expand *ex, t_shell *sh, char del, char *l);
void			handle_valid_case(t_expand *ex, t_shell *sh, char *line);
void			handle_else(t_expand *ex, char *l, t_parse2 *p);
void			init_expand_struct(t_expand *ex);
char			*handle_env_var_inline(char *l, t_shell *sh, t_parse2 *p);

/**************************/
/******** PARSING *********/
/**************************/

/* Parsing */
t_node			*parsing(t_token **token, t_shell *sh);
t_node			*parse_bracket(t_token **token, t_shell *sh);
t_node			*parse_command(t_token **token, t_shell *sh);
t_node			*parse_pipe(t_token **token, t_shell *sh);

/* Parse cmd node */
void			put_cmd_arg(char **cmd, t_token **token, char **arg_line);
t_node			*return_cmd_node(char *ag, t_cmd_node cmd, t_node *cmd_node);
t_node			*parse_command(t_token **tk, t_shell *sh);

/* Parse utils */
int				is_part_of_cmd(t_token *token);
int				make_node(t_node **node, enum e_node_type type);
int				ask_end_cmd(t_token **tk, t_shell *sh);
void			init_cmd_data(t_cmd_node *cmd);
char			**ft_split_parse(char const *s);
void			free_str(char **s, int i);

/* Parse error */
int				init_parse_error(t_token **tk);
t_node			*synthax_error(t_node *node, int flag, t_token **token);
t_node			*cmd_err(int flag, t_node *node, char **arg_ln, t_token **tk);
t_node			*error_malloc(t_node *node);

/* Parse redir */
int				handle_redir(t_cmd_node *c, t_token **tk, t_shell *sh, int fd);
int				check_redir(t_token **tk, t_shell *sh, t_cmd_node *cmd);

/* Wildcard */
char			*parse_wildcard(char *wildcard_str, bool is_first_wcard);
void			put_files(char **files, const char *new_file);
int				is_matching(char *match_str, const char *file_name);

char			*handle_d_q_here(char *line, t_shell *sh);
char			*handle_env_var_h(char *line, int *i, t_shell *sh);

/**************************/
/********* UTILS **********/
/**************************/

/* Init */
char			*init_prompt(char *curr_dir, t_shell *sh);
void			init_sh(t_shell *sh, char *env[]);
char			*get_ls_color(void);

/* Utils */
void			print_banner(void);
void			ft_sorpresa(void);

/* Free */
void			free_tokens(t_token **tokens);
void			free_sh(t_shell *sh);
void			free_tree(t_node *node);
void			free_cmd(t_cmd_node *cmd);
int				free_multiple(int amount, char *f, char *s, char *t);

/* Wrap functions */
void			free_wrap(void *str);

/**************************/
/******* BUILT-INS ********/
/**************************/

/* Env variable */
char			*ft_getenv(char **env, char *var);
int				ft_update_env(t_shell *sh, char *var);
int				ft_export(t_shell *sh, char *var);
int				ft_unset(t_shell *sh, char *var);
int				ft_env(char **args, t_shell *sh);
int				ft_append_env(t_shell *sh, char *var);
int				ft_handle_export(t_shell *sh, char **vars);
int				ft_handle_unset(t_shell *sh, char **vars);

/* Env utils */
char			**ft_create_env(char **env);
int				verify_var(t_shell *sh, char *v);
char			*split_var_name(char *var);
int				ft_arrlen(char **arr);
int				init_unset_vars(t_unset *u, t_shell *sh, char *var);
int				ft_isinenv(char **env, char *var);
int				is_valid_var_name(char *str);
int				print_export(t_shell *sh);
int				init_export_vars(t_export_vars *ex, t_shell *sh, char *var);

/* Dir */
char			*ft_getpwd(void);
int				ft_cd(char *path, char **args, t_shell *sh);
int				ft_pwd(t_shell *sh);

/* Dir utils */
int				set_dir_error(char *path, char *old_pwd);
int				set_dir_path(t_shell *sh, char *old_pwd, char *new);
int				set_dir_prev(t_shell *sh, char *curr_pwd);
int				set_dir_home(t_shell *sh, char *curr_pwd);

/* Echo */
int				ft_echo(char **args);

/* Exit */
int				ft_exit(t_node *tree, t_shell *sh, int *error);

/**************************/
/******** EXECUTION *******/
/**************************/

/* Exec */
void			recursive_exec(t_node **cmd_tree, t_shell *sh, bool in_pipe);
int				exec_command(t_node *cmd_tree, t_shell *sh, bool in_pipe);
void			check_and_exec(t_node *cmd_tree, t_shell *sh);
int				exec_builtin(t_node *tree, t_shell *sh, int error);

/* Exec command */
void			exec_and(t_node **cmd_tree, t_shell *sh, bool in_pipe);
void			exec_or(t_node **cmd_tree, t_shell *sh, bool in_pipe);
void			exec_pipe(t_node **cmd_tree, t_shell *sh, bool in_pipe);

/* Utils */
void			*get_path(char *command, char **envp);
void			*find_path(char **path, char *command);
int				is_builtin(char *command);
int				change_in_out(t_node *cmd_tree, int is_builtin);
bool			is_quoted(char *s);
char			*strip_quotes(char *s);

/* Exec error */
void			exit_error(char *msg, int status);
void			free_all(t_shell *sh, int pipe_fd[2], char *msg, bool in_pipe);

/* Exec heredoc */
int				handle_heredoc(char *args, t_shell *sh);
void			heredoc_loop(int pipe_fd[2], char *hd, bool q, t_shell *sh);

/**************************/
/******** SIGNALS *********/
/**************************/

int				manage_signals(void);
void			handle_fork_sig(int signum);
void			sig_heredoc(int sig);
void			signal_handler_2(int sig);

/**************************/
/********* DEBUG *********/
/**************************/

void			print_tokens(t_token **result);
void			print_tree(t_node *node, int depth);

#endif
