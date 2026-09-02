#ifndef T_CONF_H
# define T_CONF_H

typedef struct	s_conf
{
	char	*name;
	char	*alphabet;
	char	blank;
	char	**states;
	char	*initial;
	char	**finals;
}	t_conf;

#endif
