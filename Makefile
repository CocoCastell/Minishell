.SILENT:
.PHONY: all clean fclean re

NAME		=	minishell

# Compilation
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra

# Libft
LIBFT_DIR	=	libft/
LIBFT		=	libft/libft.a

# Sources
SRC_DIR		=	srcs
SRC			=	main.c \

# Includes
INC			=


# Rules
all: $(LIBFT) $(NAME)

$(LIBFT):
		make -sC $(LIBFT_DIR)

clean:

fclean: clean
		rm -f $(NAME)
		make fclean -sC $(LIBFT_DIR)

re: fclean all
