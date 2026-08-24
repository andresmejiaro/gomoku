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
TEST_BIN2 = tests/recursive_undo_test
TEST_BIN3 = tests/score_oracle_test
TEST_BIN4 = tests/capture_potential_test

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN) $(TEST_BIN2) $(TEST_BIN3) $(TEST_BIN4)
	./$(TEST_BIN)
	./$(TEST_BIN2)
	./$(TEST_BIN3)
	./$(TEST_BIN4)

$(TEST_BIN): tests/undo_capture_test.c $(LIB_OBJ)
	$(CC) $(CFLAGS) tests/undo_capture_test.c $(LIB_OBJ) -o $(TEST_BIN)

$(TEST_BIN2): tests/recursive_undo_test.c $(LIB_OBJ)
	$(CC) $(CFLAGS) tests/recursive_undo_test.c $(LIB_OBJ) -o $(TEST_BIN2)

$(TEST_BIN3): tests/score_oracle_test.c $(LIB_OBJ)
	$(CC) $(CFLAGS) tests/score_oracle_test.c $(LIB_OBJ) -o $(TEST_BIN3)

$(TEST_BIN4): tests/capture_potential_test.c $(LIB_OBJ)
	$(CC) $(CFLAGS) tests/capture_potential_test.c $(LIB_OBJ) -o $(TEST_BIN4)

clean:
	rm -f $(OBJ) $(DEES)

fclean: clean
	rm -f $(NAME) $(TEST_BIN) $(TEST_BIN2) $(TEST_BIN3) $(TEST_BIN4)

re: fclean all

.PHONY: all clean fclean re test
