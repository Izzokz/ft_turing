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




#endif