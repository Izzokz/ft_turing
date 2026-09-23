#include "../include/ft_turing.h"

void	ft_print_complexity_from_data(int execution_data[MAX_SAVE][2])
{
	int	i = -1;
	while (++i < MAX_SAVE)
	{
		if (!**(execution_data + i))
			break ;
		printf("EXEC[%d]: %d steps, for %d characters\n", i, *(*(execution_data + i) + 1), **(execution_data + i));
	}
	printf("# DATA SAVED: %d\n", i);
}
