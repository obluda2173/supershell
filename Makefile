# **************************************************************************** #
# CONFIG                                                                       #
# **************************************************************************** #
NAME		=	minishell

# Compiler and flags
CC 			=	@cc
RM			=	@rm -f
FLAGS		=	-Wall -Wextra -Werror -lreadline

# Directories and source files
SRCDIR		=	src
SRC			=	$(SRCDIR)/main.c \
				$(SRCDIR)/parser.c

INCLDIR		=	include
LIBFTDIR	=	$(INCLDIR)/Libft

OBJS		=	$(SRC:.c=.o)

# Libraries
LIBS		=	$(LIBFTDIR)/libft.a

# **************************************************************************** #
# COLORS                                                                       #
# **************************************************************************** #
BLUE        =   \033[0;94m
RED         =   \033[0;91m
GREEN       =   \033[0;92m
YELLOW      =   \033[0;33m
CYAN        =   \033[0;96m
COLOR       =   \033[0m

# Messages
SUCCESS     =   @echo "$(GREEN)pipex compiled successfully$(COLOR)"
S_OBJS      =   @echo "$(BLUE)Cleaning of objects completed$(COLOR)"
S_NAME      =   @echo "$(BLUE)Full clean completed$(COLOR)"

# **************************************************************************** #
# RULES                                                                        #
# **************************************************************************** #
all:		$(NAME)
$(NAME):	$(OBJS)
			@make -sC $(LIBFTDIR)
			$(CC) $(FLAGS) $(OBJS) $(LIBS) -o $(NAME)
			$(SUCCESS)

clean:
			$(RM) $(OBJS)
			@make --no-print-directory -sC $(LIBFTDIR) clean
			$(S_OBJS)

fclean:		clean
			$(RM) $(NAME)
			@make --no-print-directory -sC $(LIBFTDIR) fclean
			$(S_NAME)

re: 		fclean all

# **************************************************************************** #
# PHONY                                                                        #
# **************************************************************************** #
.PHONY:		all clean fclean re