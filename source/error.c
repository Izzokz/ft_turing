#include "../include/ft_turing.h"

void	ft_print_err(char *err, char *i)
{
	if (!i)
		printf("ERR[%s]\n", err);
	else
		printf("ERR[%s](%s)\n", err, i);
	ft_free_transet();
	ft_free_conf();
	cJSON_Delete(g_json);
	exit(1);
}

char	ft_invalid_input(char *input)
{
	for (int i = 0; *(input + i); ++i)
	{
		if (*(input + i) == g_conf.blank)
			return (INPUT_INVALID_BLANK);
		for (int j = 0; "UNICORN"; ++j)
		{
			if (!*(g_conf.alphabet + j))
				return (INPUT_INVALID_UNKNOWN);
			if (*(g_conf.alphabet + j) == *(input + i))
				break ;
		}
	}
	return (0);
}