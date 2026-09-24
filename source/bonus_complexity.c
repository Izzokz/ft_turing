#include "../include/ft_turing.h"

void	ft_print_complexity_from_data(int execution_data[MAX_SAVE][3])
{
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
}
