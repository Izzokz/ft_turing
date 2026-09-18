#include "../include/ft_turing.h"
#include <limits.h>

#define MOVE_ERR -1 // MALLOC ERROR OR NO MORE TRANSITION
#define SET_MOVES 1
#define RESET_MOVE 2
#define DELETE_MOVES INT_MAX
#define RUN_END INT_MIN

static int	*ft_create_moves_sheet(void)
{
	int	*m = malloc(g_max_I * sizeof(int));

	if (!m)
		return (0);
	for (int i = 0; i < g_max_I; ++i)
		*(m + i) = -1;
	return (m);
}

static int	ft_move(int state, int op)
{
	static int 	*current_move_by_state = NULL;
	if (op == DELETE_MOVES)
	{
		free(current_move_by_state);
		current_move_by_state = 0;
		return (0);
	}
	if (op == SET_MOVES)
	{
		current_move_by_state = ft_create_moves_sheet();
		if (!current_move_by_state)
			return (MOVE_ERR);
		return (0);
	}

	if (!*((int **)*(g_transet + state) + *(current_move_by_state + state) + 1))
		return (MOVE_ERR);
	return (++*(current_move_by_state + state));
}

static int	ft_get_next_move(int state)
{
	return (ft_move(state, 0));
}

static int	ft_set_moves(void)
{
	return (ft_move(0, SET_MOVES));
}

static void	ft_free_moves(void)
{
	ft_move(0, DELETE_MOVES);
}

static inline void	ft_free_run_list(int **run_list)
{
	for (int i = 0; *(run_list + i); ++i)
		free(*(run_list + i));
	free(run_list);
}

void	ft_print_complexity(void)
{
	if (!*(g_transet + g_conf.initial))
	{
		printf("# Complexity: O(1)\n\n");
		return ;
	}

	if (ft_set_moves() == MOVE_ERR)
		ft_print_err(ALLOC_ERR, "bonus_complexity.c");

	int				*state_is_loop = calloc(sizeof(int), g_max_I);
	if (!state_is_loop)
	{
		ft_free_moves();
	}

	int				**run_list = calloc(sizeof(int *), 1);
	if (!run_list)
	{
		free(state_is_loop);
		ft_free_moves();
		ft_print_err(ALLOC_ERR, "bonus_complexity.c");
	}

	int				run = 0;
	int				depth = 0;
	int				state = g_conf.initial;
	int				*last_run_path = NULL;
	int				last_run_saved = 0;
	int				all_run = 0;
	while (depth > -1)
	{
		int			*current_run_path = malloc(sizeof(int) * (depth + 2));
		if (!current_run_path)
		{
			ft_free_run_list(run_list);
			free(state_is_loop);
			ft_free_moves();
			ft_print_err(ALLOC_ERR, "bonus_complexity.c");
		}
		*current_run_path = g_conf.initial;
		*(current_run_path + depth + 1) = RUN_END;
		for (int i = 1; i <= depth; ++i)
			*(current_run_path + i) = *(last_run_path + i);
		if (!last_run_saved)
			free(last_run_path);
		last_run_saved = 0;

		while ("UNICORN")
		{
			if (run)
				state = *(current_run_path + depth);
			int		move = ft_get_next_move(state);
			if (move == MOVE_ERR)
			{
				--depth;
				break ;
			}

			int		*tmp = current_run_path;
			if (!(current_run_path = malloc(sizeof(int) * (depth + 3))))
			{
				free(tmp);
				ft_free_run_list(run_list);
				free(state_is_loop);
				ft_free_moves();
				ft_print_err(ALLOC_ERR, "bonus_complexity.c");
			}
			for (int i = 0; i <= depth; ++i)
				*(current_run_path + i) = *(tmp + i);
			free(tmp);
			move = *(current_run_path + depth + 1) = *(*((int **)*(g_transet + state) + move) + 1);
			*(current_run_path + depth + 2) = RUN_END;

			if (!*(g_transet + move))
				break ;

			int		i = -1;
			while (++i <= depth)
				if (*(current_run_path + i) == move)
					break ;
			if (i <= depth)
			{
				*(state_is_loop + move) = 1;
				break ;
			}

			++depth;
		}

		if (depth >= 0)
		{
			char	eligible = 1;

			int		i = -1;
			for (int x = 0; x < run; ++x)
			{
				i = -1;
				while (*(*(run_list + x) + ++i) != RUN_END)
					if (*(current_run_path + i) == RUN_END || *(*(run_list + x) + i) != *(current_run_path + i))
						break ;
				if (*(*(run_list + x) + i) == RUN_END || ((*(*(run_list + x) + i + 1) == RUN_END) && (*(current_run_path + i) == RUN_END) && (*(*(run_list + x) + i) == *(current_run_path + i - 1))))
				{
					eligible = 0;
					break ;
				}
			}

			if (eligible)
			{
				int	**tmp = run_list;
				if (!(run_list = malloc(sizeof(int *) * (run + 2))))
				{
					free(current_run_path);
					ft_free_run_list(tmp);
					free(state_is_loop);
					ft_free_moves();
					ft_print_err(ALLOC_ERR, "bonus_complexity.c");
				}
				for (int x = 0; x < run; ++x)
					*(run_list + x) = *(tmp + x);
				free(tmp);
				*(run_list + run) = current_run_path;
				*(run_list + run + 1) = 0;
				++run;
				last_run_saved = 1;
			}
		}
		last_run_path = current_run_path;
		++all_run;
	}
	if (!last_run_saved)
		free(last_run_path);
	ft_free_moves();

	printf("# ALL RUN: %d\n# LOOPS:\n", all_run);
	for (int i = 0; i < g_max_I; ++i)
	{
		if (*(state_is_loop + i))
			printf("STATE[%d]: TRUE\n", i);
		else
			printf("STATE[%d]: FALSE\n", i);
	}
	printf("\n");

	for (int i = 0; *(run_list + i); ++i)
	{
		for (int j = 0; *(*(run_list + i) + j) != RUN_END; ++j)
		{
			if (!j)
				printf("[%d, ", *(*(run_list + i) + j));
			else if (*(*(run_list + i) + j + 1) == RUN_END)
				printf("%d]\n", *(*(run_list + i) + j));
			else
				printf("%d, ", *(*(run_list + i) + j));
		}
	}
	printf("\n");

	//int	max_n = 0;
	// CALCULATE FROM RUN HISTORY
	free(state_is_loop);
	ft_free_run_list(run_list);
}
