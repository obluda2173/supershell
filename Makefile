# **************************************************************************** #
# CONFIG                                                                       #
# **************************************************************************** #
NAME		=	minishell

# Compiler and flags
CC 			=	@cc
CFLAGS		=	-Wall -Wextra -Werror
LIBS 		= 	-lreadline -Llibft -lft

# Debugging
FSANITIZE	=	-fsanitize=address

# Directories and source files
SRC_DIR		=	src
OBJ_DIR		=	obj
BUILD_DIR	=	build
LIBFT_DIR	=	libft

INCLUDES 	:= 	-Ilibft -Iinclude

RUN_DIR		=	run
SRC_FILES 	= 	src/lexer/format_input_1.c src/lexer/format_input_2.c src/lexer/lexer.c src/lexer/syntax_check_1.c \
				src/lexer/syntax_check_2.c src/lexer/tokenize_1.c src/lexer/tokenize_2.c src/lexer/tokenize_3.c \
				src/parser/parser_2.c src/parser/parser_arguments.c src/parser/parser_free.c src/parser/parser_helpers.c src/parser/parser_modify_tokens.c \
				src/parser/parser_node_creation.c src/parser/parser_paranthesis.c src/parser/parser_redirections.c src/parser/parser_teardowns.c src/parser/parser.c \
				src/main/data.c src/main/env_var.c src/main/helpers.c src/main/heredoc_1.c src/main/heredoc_2.c \
				src/main/heredoc_3.c src/main/heredoc.c src/main/input.c src/main/readline.c src/main/repl.c src/main/signals.c \
				src/executor/execute_cmd_1.c src/executor/execute_cmd_2.c src/executor/execute_cmd.c src/executor/execute_helpers_1.c \
				src/executor/execute_helpers.c src/executor/execute_pipeline.c src/executor/execute.c src/executor/executor_prepare_argv.c \
				src/executor/builtins/cd_1.c src/executor/builtins/cd_2.c src/executor/builtins/echo.c src/executor/builtins/env.c src/executor/builtins/exit.c \
				src/executor/builtins/export_1.c src/executor/builtins/export_2.c src/executor/builtins/export_3.c src/executor/builtins/pwd.c src/executor/builtins/unset.c \
				src/executor/expansions/expand_arguments_1.c src/executor/expansions/expand_arguments.c src/executor/expansions/expand_redirections_1.c src/executor/expansions/expand_redirections_2.c \
				src/executor/expansions/expand_redirections.c src/executor/expansions/expand_string_2.c src/executor/expansions/expand_string.c src/executor/expansions/handle_dollar_1.c \
				src/executor/expansions/handle_dollar_2.c src/executor/expansions/handle_double_quotes_1.c src/executor/expansions/handle_double_quotes.c src/executor/expansions/handle_utils.c \
				src/executor/expansions/sort_arguments.c src/executor/expansions/wildcards_dir_entries.c src/executor/expansions/wildcards_patterns.c

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
	@mkdir -p $(OBJ_DIR)/main
	@mkdir -p $(OBJ_DIR)/lexer
	@mkdir -p $(OBJ_DIR)/parser
	@mkdir -p $(OBJ_DIR)/executor
	@mkdir -p $(OBJ_DIR)/executor/builtins
	@mkdir -p $(OBJ_DIR)/executor/expansions

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# **************************************************************************** #
# RULES - test                                                                  #
# **************************************************************************** #

unittest:
	cmake -S . -B build && \
	cmake --build build && \
	./build/run_unittests

pytest-all: pytest pytest-valgrind

pytest: $(NAME)
	@make FSANITIZE=-fsanitize=address && \
	touch tests/end_to_end_tests/test_files/no_perm.txt && \
	chmod 000 tests/end_to_end_tests/test_files/no_perm.txt && \
	python3 -m pytest tests/end_to_end_tests/without_valgrind/

pytest-valgrind: $(NAME)
	@make FSANITIZE= && \
	touch tests/end_to_end_tests/test_files/no_perm.txt && \
	chmod 000 tests/end_to_end_tests/test_files/no_perm.txt && \
	python3 -m pytest tests/end_to_end_tests/with_valgrind/


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
