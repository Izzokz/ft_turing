#include "../include/ft_turing.h"

#define POS(x) (x < 0 ? -x : x)
#define EPSILON .15
#define GET_MIN_RANGE(x) ((x * EPSILON) < 2 ? 2 : (x * EPSILON))

static char	ft_is_constant(t_edata execution_data)
{
	int	goal = *(*execution_data + 1);
	int	range = GET_MIN_RANGE(goal);
	for (int i = 1; i < MAX_SAVE; ++i)
	{
		if (!*(*(execution_data + i) + 2))
			break ;
		if (POS((goal - *(*(execution_data + i) + 1))) > range)
			return (0);
	}
	return (1);
}

static char	ft_is_logarithmic(t_edata execution_data)
{
	return (**execution_data);
}

static char	ft_is_linear(t_edata execution_data)
{
	int		min_ref = 0b01111111111111111111111111111111;
	int		min_size = 0b01111111111111111111111111111111;
	int		max_ref = 0;
	int		max_size = 0;
	for (int i = 0; i < MAX_SAVE; ++i)
	{
		if (!*(*(execution_data + i) + 2))
			break ;
		if (min_size > **(execution_data + i))
		{
			min_ref = *(*(execution_data + i) + 1);
			min_size = **(execution_data + i);
		}
		if (max_size < **(execution_data + i))
		{
			max_ref = *(*(execution_data + i) + 1);
			max_size = **(execution_data + i);
		}
	}

	for (int i = 1; i < MAX_SAVE; ++i)
	{
		if (!*(*(execution_data + i) + 2))
			break ;
		int	goal = (**(execution_data + i) - min_size) * (max_ref - min_ref) / (max_size - min_size) + min_ref;
		int	range = GET_MIN_RANGE(goal);
		if (POS((goal - *(*(execution_data + i) + 1))) > range)
			return (0);
	}
	return (1);
}

static char	ft_is_linearithmic(t_edata execution_data)
{
	return (**execution_data);
}

static char	ft_is_quadratic(t_edata execution_data)
{
	return (**execution_data);
}

static char	ft_is_exponential(t_edata execution_data)
{
	return (**execution_data);
}

static char	ft_is_factorial(t_edata execution_data)
{
	return (**execution_data);
}

void	ft_print_complexity_from_data(t_edata execution_data)
{
	printf("####\n## COMPLEXITY CALCULATION\n####\n");

	int	i = 0;
	for (int x = 0; x < MAX_SAVE; ++x)
	{
		if (!*(*(execution_data + x) + 2))
			break ;
		*(*(execution_data + x) + 1) = *(*(execution_data + x) + 1) / *(*(execution_data + x) + 2);
		i += *(*(execution_data + x) + 2);
		printf("EXEC[%d]: %d av. steps, for %d characters (%d executions)\n", x, *(*(execution_data + x) + 1), **(execution_data + x), *(*(execution_data + x) + 2));
	}
	printf("# DATA SAVED: %d\n", i);

	if (!*(*(execution_data + 1) + 2))
	{
		printf("/!\\ NOT ENOUGH \"EXEC\" SAVED TO CALCULATE TIME COMPLEXITY /!\\\n");
		return ;
	}

	if (!*(*(execution_data + 10) + 2))
		printf("/!\\ LESS THAN TEN \"EXEC\" SAVED : TIME COMPLEXITY CALCULATION SHOULD NOT BE ACCURATE /!\\\n");

	if (ft_is_constant(execution_data))
		printf("\n# O(1): POSITIVE\n# O(logn): ");
	else
		printf("\n# O(1): NEGATIVE\n# O(logn): ");
	if (ft_is_logarithmic(execution_data))
		printf("POSITIVE\n# O(n): ");
	else
		printf("NEGATIVE\n# O(n): ");
	if (ft_is_linear(execution_data))
		printf("POSITIVE\n# O(nlogn): ");
	else
		printf("NEGATIVE\n# O(nlogn): ");
	if (ft_is_linearithmic(execution_data))
		printf("POSITIVE\n# O(n^2): "); // See for n^x, instead.
	else
		printf("NEGATIVE\n# O(n^2): ");
	if (ft_is_quadratic(execution_data))
		printf("POSITIVE\n# O(2^n): ");
	else
		printf("NEGATIVE\n# O(2^n): ");
	if (ft_is_exponential(execution_data))
		printf("POSITIVE\n# O(n!): ");
	else
		printf("NEGATIVE\n# O(n!): ");
	if (ft_is_factorial(execution_data))
		printf("POSITIVE\n");
	else
		printf("NEGATIVE\n");
}
