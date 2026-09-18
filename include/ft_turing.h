#ifndef FT_TURING
# define FT_TURING

# include <fcntl.h>  //open
# include <unistd.h>
# include <stdlib.h> //malloc
# include <stdio.h>  //printf
# include <string.h> //strcat,strcpy
# include "cjson/cJSON.h"
# include "err_msg.h"

# define INPUT_INVALID_BLANK 1
# define INPUT_INVALID_UNKNOWN 2

typedef struct s_conf
{
	char	*name;
	char	*alphabet;
	char	blank;
	char	**states;
	int		initial;
	int		*finals;
	int		fsize;
}   t_conf;

extern int		g_max_I;
extern t_conf	g_conf;
extern cJSON	*g_json;
extern void		**g_transet;

void	ft_print_err(char *err, char *i);
void	ft_free_conf(void);
void	ft_free_transet(void);
char	ft_sequals(char *s1, char *s2);
char	*ft_read_file(char *filename);
char	ft_invalid_input(char *input);
void	ft_conf(void);
void	ft_set_transitions(void);
void	ft_print_machine_description(void);
void	ft_print_help(void);
void	ft_tm_compute(char *input);
void	ft_print_complexity(void);

#endif
