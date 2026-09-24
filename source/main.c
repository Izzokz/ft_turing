#include "../include/ft_turing.h"

int		g_prog_state = 0;
int		g_max_I;
t_conf	g_conf = {0, 0, 0, 0, 0, 0, 0};
cJSON	*g_json = 0;
void	**g_transet = 0;

int	main(int ac, char *av[])
{
	if (ac == 1)
		ft_print_err(NO_ARG_ERR, 0);
	if (ac == 2)
	{
		if (ft_sequals(*++av, "--help") || ft_sequals(*av, "-h"))
			ft_print_help();
		else
			ft_print_err(NOT_ENOUGH_ARG_ERR, 0);
	}

	signal(SIGINT, ft_sig_stop);
	signal(SIGQUIT, ft_sig_stop);

	char	*file_read = ft_read_file(*++av);
	if (g_prog_state)
		goto end;
	if (!file_read)
		ft_print_err(READ_FILE_ERR, 0);
	g_json = cJSON_Parse(file_read);
	free(file_read);
	if (!g_json)
		ft_print_err(INVALID_JSON_ERR, 0);
	if (g_prog_state)
	{
		cJSON_Delete(g_json);
		goto end;
	}

	ft_conf();
	if (!g_prog_state)
		ft_set_transitions();
	cJSON_Delete(g_json);
	g_json = 0;

	if (g_prog_state)
		goto end;

	signal(SIGINT, ft_sig_nihil);
	signal(SIGQUIT, ft_sig_nihil);
	ft_print_machine_description();
	signal(SIGINT, ft_sig_stop);
	signal(SIGQUIT, ft_sig_stop);
	ft_print_extra_data();

	if (g_prog_state)
		goto end;

	t_edata		execution_data;
	for (int i = 0; i < MAX_SAVE; ++i)
	{
		**(execution_data + i) = 0;
		*(*(execution_data + i) + 1) = 0;
		*(*(execution_data + i) + 2) = 0;
	}
	signal(SIGINT, ft_sig_next);
	while (*++av && g_prog_state != PROG_STOP)
	{
		g_prog_state = 0;
		ac = ft_invalid_input(*av);
		if (ac == INPUT_INVALID_BLANK)
			printf("\"%s\" is Invalid (One character is a blank).\n\n", *av);
		else if (ac == INPUT_INVALID_UNKNOWN)
			printf("\"%s\" is Invalid (One character is not in the alphabet).\n\n", *av);
		else
			ft_tm_compute(*av, execution_data);
	}
	signal(SIGINT, ft_sig_stop);

	if (g_prog_state != PROG_STOP)
		ft_print_complexity_from_data(execution_data);

end:
	ft_free_conf();
	ft_free_transet();
}
