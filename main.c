#include <fcntl.h> //open
#include <unistd.h>
#include <stdlib.h> //malloc
#include <stdio.h> //printf
#include <string.h> //strcat,strcpy
#include "include/cjson/cJSON.h"

int	g_I = 0;
int	g_max_I = -1;

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

void	ft_print_err(void)
{
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
	cJSON	*json = cJSON_Parse(file_read);
	free(file_read);
	cJSON_Delete(json);
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
