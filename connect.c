#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gomoku.h"


#define ROWS 6
#define COLUMNS 7

void	print_board(char **board)
{
	int	row;
	int	column;

	printf("\n");
	row = 0;
	while (row < ROWS)
	{
		column = 0;
		while (column < COLUMNS)
		{
			printf("| %c ", board[row][column]);
			column++;
		}
		printf("|\n");
		row++;
	}
	printf("  1   2   3   4   5   6   7\n\n");
}


char	**create_board(void)
{
	char	**board;
	int		row;

	board = malloc(ROWS * sizeof(char *));
	if (board == NULL)
		return (NULL);
	row = 0;
	while (row < ROWS)
	{
		board[row] = malloc(COLUMNS * sizeof(char));
		if (board[row] == NULL)
		{
			while (row > 0)
			{
				row--;
				free(board[row]);
			}
			free(board);
			return (NULL);
		}
		row++;
	}
	return (board);
}

void	free_board(char **board)
{
	int	row;

	if (board == NULL)
		return ;
	row = 0;
	while (row < ROWS)
	{
		free(board[row]);
		row++;
	}
	free(board);
}

char	**copy_board(char **board)
{
	char	**newboard;
	int		row;

	newboard = create_board();
	if (newboard == NULL)
		return (NULL);
	row = 0;
	while (row < ROWS)
	{
		memcpy(newboard[row], board[row], COLUMNS * sizeof(char));
		row++;
	}
	return (newboard);
}

void	clear_board(char **board)
{
	int	row;
	int	column;

	row = 0;
	while (row < ROWS)
	{
		column = 0;
		while (column < COLUMNS)
		{
			board[row][column] = ' ';
			column++;
		}
		row++;
	}
}

int	place_piece(char **board, int column, char player)
{
	int	row;

	if (column < 0 || column >= COLUMNS)
		return (0);
	row = ROWS - 1;
	while (row >= 0)
	{
		if (board[row][column] == ' ')
		{
			board[row][column] = player;
			return (1);
		}
		row--;
	}
	return (0);
}

int	place_piece_numeric(char **board, int column, int player){
	char pc = 'O';
	if (player ==  1)
		pc = 'X';
	return place_piece(board, column,pc);

}

int	four_in_a_row(char **board, int row, int column,
		int row_step, int column_step, char player)
{
	int	count;

	count = 0;
	while (count < 4)
	{
		if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS)
			return (0);
		if (board[row][column] != player)
			return (0);
		row = row + row_step;
		column = column + column_step;
		count++;
	}
	return (1);
}

int	score_single(char **board, int row, int column,
		int row_step, int column_step, char player)
{
	int	count;
	int	spaces;
	int	open_ends;
	int	check_row;
	int	check_column;

	if (board[row][column] != player)
		return (0);
	count = 0;
	spaces = 0;
	open_ends = 0;
	check_row = row;
	check_column = column;
	while (check_row >= 0 && check_row < ROWS
		&& check_column >= 0 && check_column < COLUMNS
		&& board[check_row][check_column] == player)
	{
		count++;
		check_row -= row_step;
		check_column -= column_step;
	}
	if (check_row >= 0 && check_row < ROWS
		&& check_column >= 0 && check_column < COLUMNS
		&& board[check_row][check_column] == ' ')
	{
		open_ends++;
		while (check_row >= 0 && check_row < ROWS
			&& check_column >= 0 && check_column < COLUMNS
			&& board[check_row][check_column] == ' ')
		{
			spaces++;
			check_row -= row_step;
			check_column -= column_step;
		}
	}
	check_row = row + row_step;
	check_column = column + column_step;
	while (check_row >= 0 && check_row < ROWS
		&& check_column >= 0 && check_column < COLUMNS
		&& board[check_row][check_column] == player)
	{
		count++;
		check_row += row_step;
		check_column += column_step;
	}
	if (check_row >= 0 && check_row < ROWS
		&& check_column >= 0 && check_column < COLUMNS
		&& board[check_row][check_column] == ' ')
	{
		open_ends++;
		while (check_row >= 0 && check_row < ROWS
			&& check_column >= 0 && check_column < COLUMNS
			&& board[check_row][check_column] == ' ')
		{
			spaces++;
			check_row += row_step;
			check_column += column_step;
		}
	}
	if (count + spaces < 4)
		return (0);
	if (open_ends == 0)
		return (0);
	if (open_ends == 2)
		return (count * 2);
	return (count);
}


int max(int a, int b){
	return (a)*(a>b) +b*(a<=b);
}


int	score_total(char **board, char player)
{
	int	row;
	int	column;
	int score, a, b, c, d;


	row = 0;
	score = -1000000;
	while (row < ROWS)
	{
		column = 0;
		while (column < COLUMNS)
		{
			a = score_single(board, row, column, 0, 1, player);
			b = score_single(board, row, column, 1, 0, player);
			c =score_single(board, row, column, 1, 1, player);
			d =score_single(board, row, column, 1, -1, player);
			score = max(score,a);
			score = max(score,b);
			score = max(score,c);
			score = max(score,d);
			column++;
		}
		row++;
	}
	if (score >= 7)
		score = 1000;
	return (score);
}



int	has_won(char **board, char player)
{
	int	row;
	int	column;

	row = 0;
	while (row < ROWS)
	{
		column = 0;
		while (column < COLUMNS)
		{
			if (four_in_a_row(board, row, column, 0, 1, player)
				|| four_in_a_row(board, row, column, 1, 0, player)
				|| four_in_a_row(board, row, column, 1, 1, player)
				|| four_in_a_row(board, row, column, 1, -1, player))
				return (1);
			column++;
		}
		row++;
	}
	return (0);
}


int	evaluate(char **board)
{
	if (has_won(board, 'X'))
		return (10000);
	if (has_won(board, 'O'))
		return (-10000);
	return (score_total(board, 'X') - score_total(board, 'O'));
}
int gameended(char **board ){
	if(has_won(board,'X') || has_won(board,'O'))
		return 1;
	
	for (int r = 0; r < ROWS; r++){
		for (int c = 0; c < COLUMNS; c++){
			if (board[r][c] == ' ')
				return 0;
		}
	}
	return 1;
}


int negamax_v0(int depth, char **board, int player,int root){
	char **tempboard;
	if (depth == 0 || gameended(board))
		return player*evaluate(board);
	int score = -10000;
	int ps;
	for (int m = 0; m < 7; m++){
		tempboard = copy_board(board);
		if (tempboard == NULL)
			return (score);
		int valid = place_piece_numeric(tempboard,m,player);
		if (valid)
		{
			ps = -negamax_v0(depth - 1, tempboard, -player, 0);
			score = max(score, ps);
			if (root)
				printf("play: %d score: %d\n", m + 1, ps);
		}
		free_board(tempboard);
	}
	return score;
}







int	main(void)
{
	char	player;
	int		column;
	int		turns;
	char 	**board;

	board = create_board();
	if (board == NULL)
	{
		printf("Could not allocate board.\n");
		return (1);
	}
	clear_board(board);
	player = 'X';
	int nplayer = 1;
	turns = 0;
	while (turns < ROWS * COLUMNS)
	{
		print_board(board);
		printf("Player %c, choose a column (1-7): \n", player);
		negamax_v0(2, board, nplayer ,1);
		if (scanf("%d", &column) != 1)
		{
			printf("Invalid input. Game ended.\n");
			free_board(board);
			return (1);
		}
		if (!place_piece(board, column - 1, player))
		{
			printf("That column is full or invalid. Try again.\n");
			continue ;
		}
		turns++;
		printf("score X: %d, score O: %d",
			score_total(board,'X'),score_total(board,'O'));
		if (has_won(board, player))
		{
			print_board(board);
			printf("Player %c wins!\n", player);
			free_board(board);
			return (0);
		}
		if (player == 'X')
			player = 'O';
		else
			player = 'X';
		nplayer = -1*nplayer;
	}
	print_board(board);
	printf("The board is full. It is a draw!\n");
	free_board(board);
	return (0);
}
