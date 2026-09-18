#include "../include/ft_turing.h"

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

	char	*file_read = ft_read_file(*++av);
	if (!file_read)
		ft_print_err(READ_FILE_ERR, 0);
	g_json = cJSON_Parse(file_read);
	free(file_read);
	if (!g_json)
		ft_print_err(INVALID_JSON_ERR, 0);

	ft_conf();
	ft_set_transitions();
	cJSON_Delete(g_json);
	g_json = 0;

	ft_print_machine_description();
	while (*++av)
	{
		ac = ft_invalid_input(*av);
		if (ac == INPUT_INVALID_BLANK)
			printf("\"%s\" is Invalid (One character is a blank).\n\n", *av);
		else if (ac == INPUT_INVALID_UNKNOWN)
			printf("\"%s\" is Invalid (One character is not in the alphabet).\n\n", *av);
		else
			ft_tm_compute(*av);
	}

	ft_free_conf();
	ft_free_transet();
}



/*
main
 │
 ├── read_file()              pure-ish boundary
 │
 ├── parse_config()
 │      │
 │      └── Config
 │
 ├── build_transitions()
 │      │
 │      └── TransitionTable
 │
 ├── validate_input()
 │      │
 │      └── Valid / Invalid
 │
 └── run_machine()
        │
        └── MachineResult
*/
