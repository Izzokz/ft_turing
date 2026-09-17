#include "../include/ft_turing.h"

void	ft_print_help(void)
{
	write(1, "usage: ft_turing [-h] jsonfile input\n\npositional arguments:\n\tjsonfile\t\t\tjson description of the machine\n\tinput\t\t\t\tinput of the machine\n\noptional arguments:\n\t-h, --help\t\t\tshow this help message and exit\n", 202);
	exit(0);
}

void	ft_print_machine_description(void)
{
	printf("####\n## %s\n####\n# Alphabet: [", g_conf.name);
	for (int i = 0; *(g_conf.alphabet + i); ++i)
	{
		printf("%c", *(g_conf.alphabet + i));
		if (*(g_conf.alphabet + i + 1))
			printf(", ");
	}

	printf("]\n# States: [");
	for (int i = 0; *(g_conf.states + i); ++i)
	{
		printf("%s", *(g_conf.states + i));
		if (*(g_conf.states + i + 1))
			printf(", ");
	}

	printf("]\n# Initial: %s\n# Finals: [", *(g_conf.states + g_conf.initial));
	for (int i = 0; i < g_conf.fsize; ++i)
	{
		printf("%s", *(g_conf.states + *(g_conf.finals + i)));
		if (i + 1 < g_conf.fsize)
			printf(", ");
	}

	printf("]\n# Transitions:\n");
	for (int i = 0; i < g_max_I; ++i)
	{
		if (!*(g_transet + i))
			continue ;
		for (int j = 0; *((int **)*(g_transet + i) + j); ++j)
			printf("%s[%c] => %s; writes %c; goes %c\n", *(g_conf.states + i), **((int **)*(g_transet + i) + j), *(g_conf.states + *(*((int **)*(g_transet + i) + j) + 1)), *(*((int **)*(g_transet + i) + j) + 2), *(*((int **)*(g_transet + i) + j) + 3));
	}
	printf("\n");
}


void	ft_tm_compute(char *input)
{
	char	*tape = strdup(input);
	if (!tape)
		ft_print_err(ALLOC_ERR, 0);

	printf("\"%s\"\n", tape);
	int		i = 0;
	int		state = g_conf.initial;
	int		c;
	int		write;
	int		next_state;
	int		move;
	while ("UNICORN")
	{
		if (i == -1)
		{
			if (!(input = malloc(strlen(tape) + 2)))
			{
				free(tape);
				ft_print_err(ALLOC_ERR, 0);
			}
			strcpy(input + 1, tape);
			*input = g_conf.blank;
			free(tape);
			tape = input;
			i = 0;
		}

		else if (!*(tape + i))
		{
			if (!(input = malloc(i + 2)))
			{
				free(tape);
				ft_print_err(ALLOC_ERR, 0);
			}
			strcpy(input, tape);
			*(input + i) = g_conf.blank;
			*(input + i + 1) = 0;
			free(tape);
			tape = input;
		}

		printf("<");
		for (int x = 0; *(tape + x); ++x)
		{
			if (x == i)
				printf("\033[41m%c\033[0m", *(tape + x));
			else
				printf("%c", *(tape + x));
		}

		if (!*(g_transet + state))
		{
			printf("> - %s", *(g_conf.states + state));
			break ;
		}

		c = *(tape + i);
		for (int x = 0; *((int **)*(g_transet + state) + x); ++x)
		{
			if (c == **((int **)*(g_transet + state) + x))
			{
				c = x;
				break ;
			}
			else if (!*((int **)*(g_transet + state) + x + 1))
				c = -1;
		}
		if (c < 0)
		{
			printf("> - UNKOWN TRANSITION");
			break ;
		}

		write = *(*((int **)*(g_transet + state) + c) + 2);
		next_state = *(*((int **)*(g_transet + state) + c) + 1);
		move = *(*((int **)*(g_transet + state) + c) + 3);
		printf("> - %s[%c] => %s; writes %c; goes %c\n", *(g_conf.states + state), **((int **)*(g_transet + state) + c), *(g_conf.states + next_state), write, move);

		*(tape + i) = write;
		i += (move == 'R') ? 1 : -1;
		state = next_state;
	}
	free(tape);
	printf("\n\n");
}