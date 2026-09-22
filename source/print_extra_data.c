#include "../include/ft_turing.h"
#include <limits.h>

#define MOVE_ERR -1 // MALLOC ERROR OR NO MORE TRANSITION OR USED TO DEFINE UNCALLED STATES
#define SET_MOVES (INT_MAX - 1)
#define DELETE_MOVES INT_MAX
#define LOOP 1
#define RUN_END INT_MIN // LIM FOR RUN_PATH OR USED TO DEFINE FINAL STATES

static int	*ft_create_moves_sheet(void)
{
	int	*m = malloc(g_max_I * sizeof(int));

	if (m)
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

static inline void	ft_complete_data(int *const state_nature, const int *const *const run_list)
{
	for (int i = 0, x, y; i < g_max_I; ++i)
	{
		if (*(state_nature + i))
			continue ;
		x = -1;
		while (*(run_list + ++x))
		{
			y = -1;
			while (*(*(run_list + x) + ++y) != RUN_END)
				if (*(*(run_list + x) + y) == i)
					break ;
			if (*(*(run_list + x) + y) != RUN_END)
				break ;
		}
		if (!*(run_list + x))
			*(state_nature + i) = MOVE_ERR;
	}
}

void	ft_print_extra_data(void)
{
	if (!*(g_transet + g_conf.initial))
	{
		printf("/!\\ INITIAL == FINAL /!\\\n\n");
		return ;
	}

	if (ft_set_moves() == MOVE_ERR)
		ft_print_err(ALLOC_ERR, "print_extra_data.c");

	int				*state_nature = calloc(sizeof(int), g_max_I);
	if (!state_nature)
	{
		ft_free_moves();
	}

	int				**run_list = calloc(sizeof(int *), 1);
	if (!run_list)
	{
		free(state_nature);
		ft_free_moves();
		ft_print_err(ALLOC_ERR, "print_extra_data.c");
	}

	int				run = 0;
	int				depth = 0;
	int				state = g_conf.initial;
	int				*last_run_path = NULL;
	char			last_run_saved = 0;
	int				total_runs = 0;
	int				move;
	char			at_least_one_final = 0;
	while (depth > -1)
	{
		int			*current_run_path = malloc(sizeof(int) * (depth + 2));
		if (!current_run_path)
		{
			ft_free_run_list(run_list);
			free(state_nature);
			ft_free_moves();
			ft_print_err(ALLOC_ERR, "print_extra_data.c");
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
			state = *(current_run_path + depth);
			move = ft_get_next_move(state);
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
				free(state_nature);
				ft_free_moves();
				ft_print_err(ALLOC_ERR, "print_extra_data.c");
			}
			for (int i = 0; i <= depth; ++i)
				*(current_run_path + i) = *(tmp + i);
			free(tmp);
			move = *(current_run_path + depth + 1) = *(*((int **)*(g_transet + state) + move) + 1);
			*(current_run_path + depth + 2) = RUN_END;

			if (*(state_nature + move) == LOOP)
				break ;

			if (!*(g_transet + move))
			{
				at_least_one_final = 1;
				*(state_nature + move) = RUN_END;
				break ;
			}

			int		i = -1;
			while (++i <= depth)
				if (*(current_run_path + i) == move)
					break ;
			if (i <= depth)
			{
				*(state_nature + move) = LOOP;
				break ;
			}

			++depth;
		}

		char	eligible = 1;

		if (move != MOVE_ERR)
		{
			for (int x = 0, i; x < run; ++x)
			{
				i = -1;
				while (*(*(run_list + x) + ++i) != RUN_END)
					if (*(current_run_path + i) == RUN_END || *(*(run_list + x) + i) != *(current_run_path + i))
						break ;
				if (*(*(run_list + x) + i) == RUN_END || (*(current_run_path + i) == RUN_END && *(*(run_list + x) + i - 1) == *(current_run_path + i - 1)))
				{
					eligible = 0;
					break ;
				}
			}
		}
		else
			eligible = 0;
		if (eligible)
		{
			int	**tmp = run_list;
			if (!(run_list = malloc(sizeof(int *) * (run + 2))))
			{
				free(current_run_path);
				ft_free_run_list(tmp);
				free(state_nature);
				ft_free_moves();
				ft_print_err(ALLOC_ERR, "print_extra_data.c");
			}
			for (int x = 0; x < run; ++x)
				*(run_list + x) = *(tmp + x);
			free(tmp);
			*(run_list + run) = current_run_path;
			*(run_list + run + 1) = 0;
			++run;
			last_run_saved = 1;
		}
		last_run_path = current_run_path;
		++total_runs;
	}

	if (!last_run_saved)
		free(last_run_path);
	ft_free_moves();

	ft_complete_data(state_nature, (const int *const *const)run_list);
	printf("# TOTAL RUNS: %d\n# STATES NATURE:\n", total_runs);
	for (int i = 0; i < g_max_I; ++i)
	{
		if (*(state_nature + i) == MOVE_ERR)
			printf("[%s]: NEVER CALLED\n", *(g_conf.states + i));
		else if (*(state_nature + i) == RUN_END)
			printf("[%s]: FINAL\n", *(g_conf.states + i));
		else if (*(state_nature + i))
			printf("[%s]: LOOP\n", *(g_conf.states + i));
		else
			printf("[%s]: BASIC STATE\n", *(g_conf.states + i));
	}
	if (!at_least_one_final)
		printf("/!\\ NO FINAL FOUND /!\\\n");
	printf("\n# POSSIBLE PATHS:\n");

	for (int i = 0, j; *(run_list + i); ++i)
	{
		int			*actual_path = *(run_list + i);
		j = -1;
		while (*(actual_path + ++j) != RUN_END)
		{
			char	*state = *(g_conf.states + *(actual_path + j));
			if (!j)
				printf("[(%s) > ", state);
			else if (*(*(run_list + i) + j + 1) == RUN_END)
				printf("(%s)]", state);
			else
				printf("(%s) > ", state);
		}
		if (*(state_nature + *(actual_path + j - 1)) == LOOP)
			printf(" >>LOOP<<\n");
		else
			printf(" >>FINAL<<\n");
	}
	printf("\n");

	free(state_nature);
	ft_free_run_list(run_list);
}
