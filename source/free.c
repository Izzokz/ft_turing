#include "../include/ft_turing.h"

void	ft_free_conf(void)
{
	void	*cp;
	free(g_conf.name);
	free(g_conf.alphabet);
	if (g_conf.states)
	{
		cp = g_conf.states;
		while (*(g_conf.states))
		{
			free(*(g_conf.states));
			++(g_conf.states);
		}
		free(cp);
	}
	free(g_conf.finals);
}

void	ft_free_transet(void)
{
	if (g_transet)
	{
		for (int i = 0; i < g_max_I; ++i)
		{
			if (*(g_transet + i))
			{
				for (int j = 0; *((void **)*(g_transet + i) + j); ++j)
					free(*((void **)*(g_transet + i) + j));
				free(*(g_transet + i));
			}
		}
		free(g_transet);
	}
}