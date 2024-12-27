##
# Minishell
#
# @file
# @version 0.1

unittest_executor:
	cmake -S . -B build && \
	cmake --build build && \
	./build/run_tests_executor

compile_commands:
	cmake -S . -B build && \


# end
