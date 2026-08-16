NAME = gomoku
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -O3
SRC = gomoku.c game.c game_render.c game_moves.c \
	game_rules.c game_eval.c negamax.c \
	game_updates.c helpers.c game_score.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
