ANTLR_FLAGS = -no-listener -visitor -Werror -Dlanguage=Cpp -long-messages
GENERATED_DIR = ./generated

grammar: $(wildcard *.g4)
	antlr4 $(ANTLR_FLAGS) Behavior.g4 -o $(GENERATED_DIR)