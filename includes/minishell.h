/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:53:12 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/05 18:04:39 by cochatel         ###   ########.fr       */
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

/**************************/
/********  DEFINES  *******/
/**************************/

# define RED "\033[1;38;2;255;0;0m"
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
	WILDCARD
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

/* Token Structures */

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
}	t_parse2;

typedef struct s_handle_double
{
	char	*str;
	char	*temp;
	char	*env_value;
	char	*char_str;
}	t_h_d;

/* Tree Structures */
typedef struct s_cmd_node
{
	char	*command;
	char	**args;
	char	*redir_in;
	char	*redir_out;
	char	*heredoc;
	char	*append;
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

/* Shell struct */
typedef struct s_shell
{
	char					**env;
	t_token					**tk;
	t_node					*tree;
	char					*input;
	char					*dir;
	int						error;
}	t_shell;

/* Echo struct */
typedef struct s_echo
{
	size_t		i;
	char		*tmp;
	char		*new_tmp;
	int			new_line;
}	t_echo;

/* Env struct */
typedef struct s_unset
{
	size_t		i;
	size_t		j;
	char		*v;
	size_t		len;
	char		**env_cpy;
}	t_unset;

/* Dir struct */
typedef struct s_setdir
{
	char	*set_new;
	char	*set_old;
	char	*tmp;
}	t_sd;

/**************************/
/******** TOKENIZE ********/
/**************************/

/* Tokenise */
t_token			**tokenize(char *line, t_shell *sh);
t_token			*add_token(t_token **tokens, char *str, enum e_type type);
void			h_del(char *line, t_parse2 *parse, t_token **tk, t_shell *sh);

/* Handle pipes/redirs/and/bracket */
int				handle_pipe(char *line, t_parse2 *parse, t_token **tokens);
int				handle_and(char *line, t_parse2 *parse, t_token **tokens);
int				hand_r_in(char *line, t_parse2 *parse, t_token **tokens);
int				hand_r_out(char *line, t_parse2 *parse, t_token **tokens);
int				handle_brackets(char *line, t_parse2 *parse, t_token **tokens);

/* Handle double quotes */
void			handle_wildcard(char *line, t_parse2 *parse, t_token **tokens);
int				handle_d_q(char *line, t_parse2 *p, t_token **tk, t_shell *sh);
char			*handle_env_var_quote(char *line, t_parse2 *parse, t_shell *sh);

/* Handle  str/flag/literal/var */
void			handle_str(char *line, t_parse2 *parse, t_token **tokens);
int				h_env_var(char *line, t_parse2 *p, t_token **tk, t_shell *sh);
int				handle_flag(char *line, t_parse2 *parse, t_token **tokens);
int				handle_literal(char *line, t_parse2 *parse, t_token **tokens);

/* Tk utils */
enum e_type		check_type(t_token *token);
void			get_diff_curr_len(t_parse2 *parse);
void			update_current_position(t_parse2 *parse);
char			find_repeated_pattern(const char *line);
int				find_repeated_delimiter(char *l, char c);

/**************************/
/******** PARSING *********/
/**************************/

/* Parsing */
t_node			*parsing(t_token **token);
t_node			*parse_bracket(t_token **token);
t_node			*parse_command(t_token **token);
t_node			*parse_pipe(t_token **token);

/* Parse cmd node */
void			put_cmd_arg(char **cmd, t_token **token, char **arg_line);
t_node			*return_cmd_node(char *ag, t_cmd_node cmd, t_node *cmd_node);

/* Parse utils */
int				is_part_of_cmd(t_token *token);
int				make_node(t_node **node, enum e_node_type type);
void			init_cmd_data(t_cmd_node *cmd);

/* Parse error */
int				init_parse_error(t_token **tk);
t_node			*synthax_error(t_node *node, int flag, t_token **token);
t_node			*cmd_err(int flag, t_node *node, char **arg_ln, t_token **tk);
t_node			*error_malloc(t_node *node);

// Parse redir
void			handle_redir(t_cmd_node *cmd, t_token **tk);

/* Wildcard */
char			*parse_wildcard(char *wildcard_str, bool is_first_wcard);
int			is_matching(char *match_str, const char *file_name);
void			put_files(char **files, const char *new_file);

/**************************/
/********* UTILS **********/
/**************************/

/* Init */
char			*init_prompt(char *curr_dir, t_shell *sh);
void			init_sh(t_shell *sh, char *env[]);
char			*get_ls_color(void);

/* Utils */
void			print_banner(void);

/* Free */
void			free_tokens(t_token **tokens);
void			free_sh(t_shell *sh);
void			free_tree(t_node *node);
void			free_cmd(t_cmd_node *cmd);
int				free_multiple(int amount, char *f, char *s, char *t);

/* Wrap functions */
void			free_wrap(void *str);

/* Readline */
char 			*ft_readline(char *prompt, t_shell *sh);


/**************************/
/******* BUILT-INS ********/
/**************************/

/* Env variable */
char			*ft_getenv(char **env, char *var);
int				ft_update_env(t_shell *sh, char *var);
int				ft_export(t_shell *sh, char *var);
int				ft_unset(t_shell *sh, char *var);
int				ft_env(char **args, t_shell *sh);

/* Env utils */
char			**ft_create_env(char **env);
int				verify_var(t_shell *sh, char *v);
char			*split_var_name(char *var);
int				ft_arrlen(char **arr);
int				init_unset_vars(t_unset *u, t_shell *sh, char *var);

/* Dir */
char			*ft_getpwd(void);
char			*ft_getpwd_s(void);
int				ft_cd(char *path, t_shell *sh);

/* Dir utils */
int				set_dir_error(t_shell *sh, char *path, char *old_pwd);
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
void			exec_command(t_node *cmd_tree, t_shell *sh, bool in_pipe);
void			check_and_exec(t_node *cmd_tree, t_shell *sh);
int				exec_builtin(t_node *tree, t_shell *sh);

/* Exec command */
void			exec_and(t_node **cmd_tree, t_shell *sh, bool in_pipe);
void			exec_or(t_node **cmd_tree, t_shell *sh, bool in_pipe);
void			exec_pipe(t_node **cmd_tree, t_shell *sh);

/* Utils */
void			*get_path(char *command, char **envp);
void			*find_path(char **path, char *command);
void			ft_sorpresa(void);
int				is_builtin(char *command);

/* Exec error */
void			exit_error(char *msg, int status);

/* Exec redirection */
void			handle_heredoc(char *args);
void			heredoc_loop(int pipe_fd[2], char *hd);
void			exec_redir(t_node *cmd_tree);

/**************************/
/******** SIGNALS *********/
/**************************/

int				manage_signals(void);

/**************************/
/********* DEBUG *********/
/**************************/

void			print_tokens(t_token **result);
void			print_tree(t_node *node, int depth);

/**************************/
/********* GLOBAL *********/
/**************************/
extern volatile sig_atomic_t	g_signal_received;

#endif
