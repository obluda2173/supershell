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
	cmake -S . -B build -DBUILD_EXECUTOR=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && \
	mv build/compile_commands.json ./compile_commands.json

# end
