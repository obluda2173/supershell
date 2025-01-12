# **************************************************************************** #
# CONFIG                                                                       #
# **************************************************************************** #
NAME		=	minishell

# Compiler and flags
CC 			=	@cc
CFLAGS		=	-Wall -Wextra -Werror
LIBS 		= 	-lreadline -Llibft -lft

# Debugging
FSANITIZE = -fsanitize=address

# Directories and source files
SRC_DIR		=	src
OBJ_DIR		=	obj
BUILD_DIR	=	build
LIBFT_DIR	=	libft

INCLUDES 	:= 	-Ilibft -Iinclude

RUN_DIR		=	run
PRS_RDIR	=	src/parser
SRC_FILES 	:= $(wildcard src/*.c) $(wildcard src/lexer/*.c) $(wildcard src/parser/*.c)
OBJ_FILES 	:= 	$(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

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
SUCCESS     =   @echo "$(GREEN)minishell compiled successfully$(COLOR)"
S_OBJS      =   @echo "$(BLUE)Cleaning of objects completed$(COLOR)"
S_NAME      =   @echo "$(BLUE)Full clean completed$(COLOR)"

# **************************************************************************** #
# RULES                                                                        #
# **************************************************************************** #
all:		$(NAME)
$(NAME):	libft $(OBJ_FILES)
			$(CC) $(FLAGS) $(OBJ_FILES) $(INCLUDES) $(FSANITIZE) run/main.c -o $(NAME) $(LIBS)
			$(SUCCESS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/lexer
	@mkdir -p $(OBJ_DIR)/parser

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# **************************************************************************** #
# RULES - test                                                                  #
# **************************************************************************** #

unittest:
	cmake -S . -B build && \
	cmake --build build && \
	./build/run_unittests

pytest: $(NAME)
	@make FSANITIZE=-fsanitize=address && \
	python3 -m pytest 

# **************************************************************************** #
# PHONY                                                                        #
# **************************************************************************** #

libft:
	@make bonus -C $(LIBFT_DIR)

clean:
	$(MAKE) -C libft $@
	@rm -f $(OBJ_FILES)
	$(S_OBJS)

fclean:		clean
			$(MAKE) -C libft $@
			rm -f $(NAME)
			$(S_NAME)
			rm -rf $(OBJ_DIR)
			rm -rf $(BUILD_DIR)

re: 		fclean all

print_srcs:
	@echo $(SRC_FILES)

print_objs:
	@echo $(OBJ_FILES)

compile_commands:
	cmake -S . -B build -DBUILD_TEST=ON -DBUILD_MINISHELL=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && \
	mv build/compile_commands.json ./compile_commands.json


.PHONY:		all clean fclean re libft
