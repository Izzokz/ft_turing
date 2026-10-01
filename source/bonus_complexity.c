#include "../include/ft_turing.h"

#define POS(x) ((x) < 0 ? -(x) : (x))
#define EPSILON .1L
#define GET_MIN_RANGE(x) ((x) * EPSILON < 2.L ? 2.L : (x) * EPSILON)
#define TARGET_RANGE .9L

/*
* WHY SOME TESTS ARE NEGATIVE WHILE THE <epc> MATCHES (n^x)?
	* Because individual tests fail if one case is not in range while estimating the polynomial degree work with a mean.
	* Imagine if (min == .78) and (max == .98) : fails (n^2) because <min> not in the range but <epc> is in range of (2.0 +/- 10%).
	* That is why <epc> is useful.
*/

static char	ft_is_constant(t_edata execution_data, int max)
{
	long double	goal = *(*execution_data + 1);
	long double	range = GET_MIN_RANGE(goal);
	while (--max)
		if (POS(goal - *(*(execution_data + max) + 1)) > range)
			return (0);
	return (1);
}

static char	ft_is_logarithmic(t_edata execution_data, int max, long double max_ref, long double max_size)
{
	long double	factor = max_ref / logl(max_size);
	long double	goal;
	long double	range;
	while (--max > 1)
	{
		goal = factor * logl(**(execution_data + max));
		range = GET_MIN_RANGE(goal);
		if (POS(goal - *(*(execution_data + max) + 1)) > range)
			return (0);
	}
	return (1);
}

static char	ft_is_linear(t_edata execution_data, int max, long double max_ref, long double max_size)
{
	long double	factor = max_ref / max_size;
	long double	goal;
	long double	range;
	while (--max > -1)
	{
		goal = factor * **(execution_data + max);
		range = GET_MIN_RANGE(goal);
		if (POS(goal - *(*(execution_data + max) + 1)) > range)
			return (0);
	}
	return (1);
}

static char	ft_is_linearithmic(t_edata execution_data, int max, long double max_ref, long double max_size)
{
	long double	factor = max_ref / (max_size * logl(max_size));
	long double	goal;
	long double	range;
	while (--max > -1)
	{
		goal = factor * **(execution_data + max) * logl(**(execution_data + max));
		range = GET_MIN_RANGE(goal);
		if (POS(goal - *(*(execution_data + max) + 1)) > range)
			return (0);
	}
	return (1);
}

static char	ft_is_quadratic(t_edata execution_data, int max, long double max_ref, long double max_size)
{
	long double	factor = max_ref / (max_size * max_size);
	long double	goal;
	long double	range;
	while (--max > -1)
	{
		goal = factor * **(execution_data + max) * **(execution_data + max);
		range = GET_MIN_RANGE(goal);
		if (POS(goal - *(*(execution_data + max) + 1)) > range)
			return (0);
	}
	return (1);
}

static char	ft_is_exponential(t_edata execution_data, int max, long double max_ref, long double max_size, long double min_ref, long double min_size)
{
	long double	max_log = logl(max_ref);
	long double	min_log = logl(min_ref);
	long double	slope = (max_log - min_log) / (max_size - min_size);
	long double	goal;
	long double	range;
	while (--max > -1)
	{
		goal = min_log + slope * (**(execution_data + max) - min_size);
		range = logl(1L + EPSILON);
		if (POS(goal - logl(*(*(execution_data + max) + 1))) > range)
			return (0);
	}
	return (1);
}

static char	ft_is_factorial(t_edata execution_data, int max, long double max_ref, long double max_size)
{
	long double	factor = logl(max_ref) - lgammal(max_size + 1L);
	long double	goal;
	long double	range;
	while (--max > -1)
	{
		goal = factor + lgammal(**(execution_data + max) + 1L);
		range = logl(1L + EPSILON);
		if (POS(goal - logl(*(*(execution_data + max) + 1))) > range)
			return (0);
	}
	return (1);
}

static long double	ft_estimate_polynomial_complexity(t_edata execution_data, int max)
{
	long double		sum_x = 0L;
	long double		sum_y = 0L;
	long double		sum_xy = 0L;
	long double		sum_xx = 0L;
	long double		x;
	long double		y;
	int				i = -1;
	while (++i < max)
	{
		x = logl(**(execution_data + i));
		y = logl(*(*(execution_data + i) + 1));
		sum_x += x;
		sum_y += y;
		sum_xy += x * y;
		sum_xx += x * x;
	}

	long double		degree = (max * sum_xy - sum_x * sum_y) / (max * sum_xx - sum_x * sum_x);
	if (POS(degree) < EPSILON)
		return (POS(degree));

	long double		mean_y = sum_y / max;
	long double		sum2_total = 0L;
	long double		sum2_res = 0L;
	long double		goal;
	while (--i > -1)
	{
		x = logl(**(execution_data + i));
		y = logl(*(*(execution_data + i) + 1));
		goal = (sum_y - degree * sum_x) / max + degree * x;
		sum2_total += (y - mean_y) * (y - mean_y);
		sum2_res += (y - goal) * (y - goal);
	}

	if (!sum2_total)
		return (-1L);

	if ((1L - sum2_res / sum2_total) < TARGET_RANGE)
		return (-1L);

	return (degree);
}

void	ft_print_complexity_from_data(t_edata execution_data)
{
	printf("####\n## COMPLEXITY CALCULATION\n####\n");

	int			i = 0;
	int			max = -1;
	int			max_ref;
	int			max_size = 0;
	int			min_ref;
	int			min_size = 0b01111111111111111111111111111111;
	while (++max < MAX_SAVE && *(*(execution_data + max) + 2))
	{
		int		*tmp = *(execution_data + max);
		*(tmp + 1) = *(tmp + 1) / *(tmp + 2);
		i += *(tmp + 2);
		printf("EXEC[%d]: %d av. steps, for %d characters (%d executions)\n", max, *(tmp + 1), *tmp, *(tmp + 2));
		if (*tmp < min_size)
		{
			min_ref = *(tmp + 1);
			min_size = *tmp;
		}
		if (*tmp > max_size)
		{
			max_ref = *(tmp + 1);
			max_size = *tmp;
		}
	}
	printf("# DATA SAVED: %d\n", i);

	if (!*(*(execution_data + 1) + 2))
	{
		printf("/!\\ NOT ENOUGH \"EXEC\" SAVED TO CALCULATE TIME COMPLEXITY /!\\\n");
		return ;
	}

	if (!*(*(execution_data + 10) + 2))
		printf("/!\\ LESS THAN TEN \"EXEC\" SAVED : TIME COMPLEXITY CALCULATION SHOULD NOT BE ACCURATE /!\\\n");

	char		*is_const = "";
	if (ft_is_constant(execution_data, max))
	{
		printf("\n# O(1): POSITIVE\n# O(logn): ");
		is_const = " (However, it is constant so consider it NEGATIVE)";
	}
	else
		printf("\n# O(1): NEGATIVE\n# O(logn): ");
	if (ft_is_logarithmic(execution_data, max, max_ref, max_size))
		printf("POSITIVE%s\n# O(n): ", is_const);
	else
		printf("NEGATIVE\n# O(n): ");
	if (ft_is_linear(execution_data, max, max_ref, max_size))
		printf("POSITIVE%s\n# O(nlogn): ", is_const);
	else
		printf("NEGATIVE\n# O(nlogn): ");
	if (ft_is_linearithmic(execution_data, max, max_ref, max_size))
		printf("POSITIVE%s\n# O(n^2): ", is_const);
	else
		printf("NEGATIVE\n# O(n^2): ");
	if (ft_is_quadratic(execution_data, max, max_ref, max_size))
		printf("POSITIVE%s\n# O(2^n): ", is_const);
	else
		printf("NEGATIVE\n# O(2^n): ");
	if (ft_is_exponential(execution_data, max, max_ref, max_size, min_ref, min_size))
		printf("POSITIVE%s\n# O(n!): ", is_const);
	else
		printf("NEGATIVE\n# O(n!): ");
	if (ft_is_factorial(execution_data, max, max_ref, max_size))
		printf("POSITIVE%s\n", is_const);
	else
		printf("NEGATIVE\n");

	long double	epc = ft_estimate_polynomial_complexity(execution_data, max);
	if (epc > -1L)
		printf("\n# ESTIMATED POLYNOMIAL COMPLEXITY: O(n^%.2Lf)\n", epc);
}
