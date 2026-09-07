#include <fcntl.h> //open
#include <unistd.h>
#include <stdlib.h> //malloc
#include <stdio.h> //printf
#include <string.h> //strcat,strcpy
#include "include/cjson/cJSON.h"
#include "include/t_conf.h"

int		g_max_I = -1;
t_conf	g_conf = {0, 0, 0, 0, 0, 0, 0};
cJSON	*g_json = 0;
void	**g_transet = 0;

static char	ft_sequals(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (0);
	i = -1;
	while (*(s1 + ++i) || *(s2 + i))
		if (*(s1 + i) != *(s2 + i))
			break ;
	return (*(s1 + i) == *(s2 + i));
}

void	ft_print_help(void)
{
	write(1, "usage: ft_turing [-h] jsonfile input\n\npositional arguments:\n\tjsonfile\t\t\tjson description of the machine\n\tinput\t\t\t\tinput of the machine\n\noptional arguments:\n\t-h, --help\t\t\tshow this help message and exit\n", 202);
	exit(0);
}

static void	ft_free_conf(void)
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

static void	ft_free_transet(void)
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

static void	ft_print_err(void)
{
	ft_free_transet();
	ft_free_conf();
	cJSON_Delete(g_json);
	printf("ERR\n");
	exit(1);
}

static char	*ft_read_file(char *filename)
{
	int		fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	char	*file = malloc(1);
	if (!file)
		ft_print_err();
	*file = 0;
	char	*cpfile;
	int		file_size = 0;
	int		ret;
	char	buff[1000];
	while ("UNICORN")
	{
		ret = read(fd, buff, 999);
		if (ret < 0)
		{
			free(file);
			ft_print_err();
		}
		cpfile = file;
		if (!(file = malloc((file_size += ret) + 1)))
		{
			free(cpfile);
			ft_print_err();
		}
		strcpy(file, cpfile);
		free(cpfile);
		*(buff + ret) = 0;
		strcat(file, buff);
		if (ret < 999)
			break ;
	}
	return (file);
}

static void	ft_conf(void)
{
	cJSON	*obj = cJSON_GetObjectItem(g_json, "name");
	if (!cJSON_IsString(obj) || !(*obj).valuestring)
		ft_print_err();
	g_conf.name = strdup((*obj).valuestring);
	if (!g_conf.name)
		ft_print_err();

	obj = cJSON_GetObjectItem(g_json, "alphabet");
	int		arsize;
	if (!cJSON_IsArray(obj) || (arsize = cJSON_GetArraySize(obj)) < 1 || !(g_conf.alphabet = malloc(arsize + 1)))
		ft_print_err();
	int		i = -1;
	char	c;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj) || strlen((*obj).valuestring) != 1)
			ft_print_err();
		*(g_conf.alphabet + ++i) = c = *((*obj).valuestring);
		for (int x = 0; x < i; ++x)
			if (c == *(g_conf.alphabet + x))
				ft_print_err();
		obj = (*obj).next;
	}
	*(g_conf.alphabet + arsize) = 0;

	obj = cJSON_GetObjectItem(g_json, "blank");
	if (!cJSON_IsString(obj) || !(*obj).valuestring || (strlen((*obj).valuestring) != 1))
		ft_print_err();
	g_conf.blank = c = *((*obj).valuestring);
	for (int x = 0; *(g_conf.alphabet + x); ++x)
	{
		if (c == *(g_conf.alphabet + x))
		{
			c = 0;
			break ;
		}
	}
	if (c)
		ft_print_err();

	obj = cJSON_GetObjectItem(g_json, "states");
	if (!cJSON_IsArray(obj) || (arsize = cJSON_GetArraySize(obj)) < 2 || !(g_conf.states = calloc((arsize + 1), sizeof(void *))))
		ft_print_err();
	i = -1;
	char	*str;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj) || !(*(g_conf.states + ++i) = str = strdup((*obj).valuestring)))
			ft_print_err();
		for (int x = 0; x < i; ++x)
			if (ft_sequals(str, *(g_conf.states + x)))
				ft_print_err();
		obj = (*obj).next;
	}

	obj = cJSON_GetObjectItem(g_json, "initial");
	if (!cJSON_IsString(obj) || !(str = (*obj).valuestring))
		ft_print_err();
	for (int x = 0; *(g_conf.states + x); ++x)
	{
		if (ft_sequals(str, *(g_conf.states + x)))
		{
			str = 0;
			g_conf.initial = x;
			break ;
		}
	}
	if (str)
		ft_print_err();

	obj = cJSON_GetObjectItem(g_json, "finals");
	if (!cJSON_IsArray(obj) || (g_conf.fsize = cJSON_GetArraySize(obj)) < 1 || !(g_conf.finals = malloc(g_conf.fsize * sizeof(int))))
		ft_print_err();
	i = -1;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj) || !(str = (*obj).valuestring))
			ft_print_err();
		for (int x = 0; *(g_conf.states + x); ++x)
		{
			if (ft_sequals(str, *(g_conf.states + x)))
			{
				str = 0;
				*(g_conf.finals + ++i) = x;
				break ;
			}
		}
		if (str)
			ft_print_err();
		for (int x = 0; x < i; ++x)
			if (*(g_conf.finals + i) == *(g_conf.finals + x))
				ft_print_err();
		obj = (*obj).next;
	}
}

static void	ft_set_transitions(void)
{
	char		**snames = g_conf.states;

	while (*(snames + ++g_max_I))
		;
	if (!g_max_I || g_conf.initial > g_max_I)
		ft_print_err();

	g_transet = calloc(sizeof(void *), g_max_I);
	if (!g_transet)
		ft_print_err();
	cJSON		*trans = cJSON_GetObjectItem(g_json, "transitions");
	cJSON		*item;
	if (!cJSON_IsObject(trans))
		ft_print_err();
	int			arsize;
	for (int i = 0; *(g_conf.states + i); ++i)
	{
		char	is_final = 0;
		for (int j = 0; j < g_conf.fsize; ++j)
		{
			if (*(g_conf.finals + j) == i)
			{
				is_final = 1;
				break ;
			}
		}
		if (is_final)
			continue ;

		item = cJSON_GetObjectItem(trans, *(g_conf.states + i));
		if (!cJSON_IsArray(item) || (arsize = cJSON_GetArraySize(item)) < 1 || !(*(g_transet + i) = calloc(arsize + 1, sizeof(int [4]))))
			ft_print_err();
		item = (*item).child;
		cJSON	*cat;
		char	*str;
		for (int x = 0; item; ++x)
		{
			if (!(*((void **)*(g_transet + i) + x) = malloc(sizeof(int [4]))))
				ft_print_err();
			cat = cJSON_GetObjectItem(item, "read");
			if (!cat || !cJSON_IsString(cat) || strlen((*cat).valuestring) != 1)
				ft_print_err();
			**((int **)*(g_transet + i) + x) = *(*cat).valuestring;

			cat = cJSON_GetObjectItem(item, "to_state");
			if (!cat || !cJSON_IsString(cat))
				ft_print_err();
			str = (*cat).valuestring;
			for (int y = 0; *(g_conf.states + y); ++y)
			{
				if (ft_sequals(*(g_conf.states + y), str))
				{
					str = 0;
					*(*((int **)*(g_transet + i) + x) + 1) = y;
					break ;
				}
			}
			if (str)
				ft_print_err();

			cat = cJSON_GetObjectItem(item, "write");
			if (!cat || !cJSON_IsString(cat) || strlen((*cat).valuestring) != 1)
				ft_print_err();
			*(*((int **)*(g_transet + i) + x) + 2) = *(*cat).valuestring;

			cat = cJSON_GetObjectItem(item, "action");
			if (!cat || !cJSON_IsString(cat))
				ft_print_err();
			if (ft_sequals("LEFT", (*cat).valuestring))
				*(*((int **)*(g_transet + i) + x) + 3) = 'L';
			else if (ft_sequals("RIGHT", (*cat).valuestring))
				*(*((int **)*(g_transet + i) + x) + 3) = 'R';
			else
				ft_print_err();
			item = (*item).next;
		}
	}
}

static void	ft_print_machine_description(void)
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

static char	ft_invalid_input(char *input)
{
	for (int i = 0; *(input + i); ++i)
	{
		if (*(input + i) == g_conf.blank)
			return (1);
		for (int j = 0; "UNICORN"; ++j)
		{
			if (!*(g_conf.alphabet + j))
				return (1);
			if (*(g_conf.alphabet + j) == *(input + i))
				break ;
		}
	}
	return (0);
}

void	ft_tm_compute(char *input)
{
	char	*tape = strdup(input);
	if (!tape)
		ft_print_err();

	printf("\"%s\"\n", tape);
	int		i = 0;
	int		state = g_conf.initial;
	int		c;
	int		write;
	int		next_state;
	int		move;
	while ("UNICORN")
	{
		if (!*(tape + i))
		{
			if (!(input = malloc(i + 2)))
				ft_print_err();
			strcpy(input, tape);
			*(input + i) = g_conf.blank;
			*(input + i + 1) = 0;
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
		if (i < 0)
		{
			printf("TAPE LOST");
			break ;
		}
		state = next_state;
	}
	free(tape);
	printf("\n\n");
}

int	main(int ac, char *av[])
{
	if (ac == 1)
		ft_print_err();
	if (ac == 2)
	{
		if (ft_sequals(*++av, "--help") || ft_sequals(*av, "-h"))
			ft_print_help();
		else
			ft_print_err();
	}

	char	*file_read = ft_read_file(*++av);
	if (!file_read)
		ft_print_err();
	g_json = cJSON_Parse(file_read);
	free(file_read);
	if (!g_json)
		ft_print_err();

	ft_conf();
	ft_set_transitions();
	cJSON_Delete(g_json);
	g_json = 0;

	ft_print_machine_description();
	while (*++av)
	{
		if (ft_invalid_input(*av))
			printf("\"%s\" is Invalid.\n\n", *av);
		else
			ft_tm_compute(*av);
	}

	ft_free_conf();
	ft_free_transet();
}

// name  str
// alphabet [c, c, c, c]
// blank c
// states [str, str, str]
// initial str
// finals [str]

// TRANSITION: {
//     stat[1]
//     {
//         [alphabe[], states[], states[], ]]
//         [alphabe[], states[], states[], ]]
//         [alphabe[], states[], states[], ]]
//     }
//     ...
// }

// transition = states[i]

// [transition[alphabe[], states[], states[], ]]
