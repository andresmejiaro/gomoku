NAME = gomoku
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -O3 -flto
SRC = gomoku.c game.c game_render.c game_moves.c \
	game_rules.c game_eval.c negamax.c \
	game_updates.c helpers.c game_score.c
OBJ = $(SRC:.c=.o)
DEES = $(SRC:.c=.d)

LIB_SRC = $(filter-out gomoku.c,$(SRC))
LIB_OBJ = $(LIB_SRC:.c=.o)

TEST_BIN = tests/undo_capture_test

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): tests/undo_capture_test.c $(LIB_OBJ)
	$(CC) $(CFLAGS) tests/undo_capture_test.c $(LIB_OBJ) -o $(TEST_BIN)

clean:
	rm -f $(OBJ) $(DEES)

fclean: clean
	rm -f $(NAME) $(TEST_BIN)

re: fclean all

.PHONY: all clean fclean re test
