.SILENT:
.PHONY: all clean fclean re

NAME		=	minishell

# Compilation
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -I$(INC)
LDFLAGS		=	-lreadline


# Libft
LIBFT_DIR	=	libft/
LIBFT		=	libft.a
LIBFT_PATH	=	$(LIBFT_DIR)$(LIBFT)

# Sources
SRC_DIR		=	srcs/
SRCS		=	main.c \
				utils/init.c \
				utils/utils.c \
				utils/free_utils.c \
				utils/wrap_function.c \
				utils/ft_readline.c \
				parsing/parsing.c \
				parsing/parse_utils.c \
				parsing/parse_cmd_node.c \
				parsing/parse_error.c \
				parsing/parse_wildcard.c \
				parsing/parse_redir.c \
				tokenize/tokenize.c \
				tokenize/tk_utils.c \
				tokenize/h_double_q.c \
				tokenize/h_redir_pipes.c \
				tokenize/h_str_flag_env.c \
				built-ins/env.c \
				built-ins/dir.c \
				built-ins/echo.c \
				built-ins/env_utils.c \
				built-ins/dir_utils.c \
				built-ins/exit.c \
				execution/exec_utils.c \
				execution/exec_error.c \
				execution/exec_redir.c \
				execution/exec_ope.c \
				execution/exec.c \
				signals/signals.c \
				debug/print_tokens.c \
				debug/print_tree.c

SRCS_PATH	=	$(addprefix $(SRC_DIR), $(SRCS))

# Objects
OBJ_DIR		=	objs/
OBJS		=	$(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

# Includes
INC			=	includes/

# Rules
all: lib $(NAME) Makefile

$(NAME): $(OBJS) $(LIBFT_PATH)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LIBFT_PATH)

lib:
	make -sC $(LIBFT_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		make clean -sC $(LIBFT_DIR)
		rm -rf $(OBJ_DIR)

fclean: clean
		rm $(NAME) $(LIBFT_PATH)
		make fclean -sC $(LIBFT_DIR)

re: fclean all
