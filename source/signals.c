#include "../include/ft_turing.h"

void	ft_sig_next(int sig)
{
	(void) sig;
	g_prog_state = PROG_NEXT;
}

void	ft_sig_stop(int sig)
{
	(void) sig;
	g_prog_state = PROG_STOP;
}

void	ft_sig_nihil(int sig)
{
	(void) sig;
	ft_free_transet();
	ft_free_conf();
	cJSON_Delete(g_json);
	exit(0);
}
