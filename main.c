#include <fcntl.h> //open
#include <unistd.h>
#include <stdlib.h> //malloc
#include <stdio.h> //printf
#include <string.h> //strcat,strcpy
#include "include/cjson/cJSON.h"
#include "include/t_conf.h"

int		g_I = 0;
int		g_max_I = -1;
t_conf	g_conf = {0, 0, 0, 0, 0, 0};
cJSON	*g_json = 0;

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

/*static void	**ft_set_transitions(t_conf *json)
{
	char	**snames = (*json).states;

	while (*(snames + ++g_max_I))
		;
	if (!g_max_I)
		return (0);

	void	**transet = malloc(sizeof(void *) * g_max_I);
	if (!transet)
		return (0);
}*/

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
	free(g_conf.initial);
	if (g_conf.finals)
	{
		cp = g_conf.finals;
		while (*(g_conf.finals))
		{
			free(*(g_conf.finals));
			++(g_conf.finals);
		}
		free(cp);
	}
}

void	ft_print_err(void)
{
	ft_free_conf();
	cJSON_Delete(g_json);
	write(2, "ERR\n", 4);
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
		if (strlen((*obj).valuestring) != 1)
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
		if (!(*(g_conf.states + ++i) = str = strdup((*obj).valuestring)))
			ft_print_err();
		for (int x = 0; x < i; ++x)
			if (ft_sequals(str, *(g_conf.states + x)))
				ft_print_err();
		obj = (*obj).next;
	}

	obj = cJSON_GetObjectItem(g_json, "initial");
	if (!cJSON_IsString(obj) || !(*obj).valuestring)
		ft_print_err();
	g_conf.initial = strdup((*obj).valuestring);
	if (!g_conf.initial)
		ft_print_err();

	obj = cJSON_GetObjectItem(g_json, "finals");
	if (!cJSON_IsArray(obj) || (arsize = cJSON_GetArraySize(obj)) < 1 || !(g_conf.finals = calloc((arsize + 1), sizeof(void *))))
		ft_print_err();
	i = -1;
	obj = (*obj).child;
	while (obj)
	{
		if (!(*(g_conf.finals + ++i) = str = strdup((*obj).valuestring)))
			ft_print_err();
		for (int x = 0; x < i; ++x)
			if (ft_sequals(str, *(g_conf.finals + x)))
				ft_print_err();
		for (int x = 0; *(g_conf.states + x); ++x)
		{
			if (ft_sequals(str, *(g_conf.states + x)))
			{
				str = 0;
				break ;
			}
		}
		if (str)
			ft_print_err();
		obj = (*obj).next;
	}
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
	cJSON_Delete(g_json);
	ft_free_conf();
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
