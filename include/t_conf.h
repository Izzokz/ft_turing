#ifndef T_CONF_H
# define T_CONF_H

typedef struct	s_conf
{
	char	*name;
	char	*alphabet;
	char	blank;
	char	**states;
	int		initial;
	int		*finals;
	int		fsize;
}	t_conf;

#endif
