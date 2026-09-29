#ifndef FT_TURING
# define FT_TURING

# include <fcntl.h>  //open
# include <unistd.h>
# include <stdlib.h> //malloc
# include <stdio.h>  //printf
# include <string.h> //strcat,strcpy
# include <signal.h>
# include <math.h>
# include "cjson/cJSON.h"
# include "err_msg.h"

# define PROG_NEXT -1
# define PROG_STOP -2
# define INPUT_INVALID_BLANK 1
# define INPUT_INVALID_UNKNOWN 2
# define MAX_SAVE 69

typedef struct	s_conf
{
	char	*name;
	char	*alphabet;
	char	blank;
	char	**states;
	int		initial;
	int		*finals;
	int		fsize;
}   t_conf;

typedef int		t_edata[MAX_SAVE][3];

extern int		g_prog_state;
extern int		g_max_I;
extern t_conf	g_conf;
extern cJSON	*g_json;
extern void		**g_transet;

void	ft_sig_next(int);
void	ft_sig_stop(int);
void	ft_sig_nihil(int);
void	ft_print_err(const char *const err, const char *const i);
void	ft_free_conf(void);
void	ft_free_transet(void);
char	ft_sequals(const char *const s1, const char *const s2);
char	*ft_read_file(const char *const filename);
char	ft_invalid_input(const char *const input);
void	ft_conf(void);
void	ft_set_transitions(void);
void	ft_print_machine_description(void);
void	ft_print_help(void);
void	ft_tm_compute(char *input, t_edata);
void	ft_print_extra_data(void);
void	ft_print_complexity_from_data(t_edata);

#endif
