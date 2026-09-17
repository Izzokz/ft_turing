#include "../include/ft_turing.h"

void	ft_conf(void)
{
	cJSON	*obj = cJSON_GetObjectItem(g_json, "name");
	if (!cJSON_IsString(obj) || !(*obj).valuestring)
		ft_print_err(JSON_INVALID_NAME_ERR, (*obj).valuestring);
	g_conf.name = strdup((*obj).valuestring);
	if (!g_conf.name)
		ft_print_err(ALLOC_ERR, 0);

	obj = cJSON_GetObjectItem(g_json, "alphabet");
	int		arsize;
	if (!cJSON_IsArray(obj) || (arsize = cJSON_GetArraySize(obj)) < 1)
		ft_print_err(JSON_INVALID_ALPHABET_ERR, 0);
	if (!(g_conf.alphabet = malloc(arsize + 1)))
		ft_print_err(ALLOC_ERR, 0);
	int		i = -1;
	char	c;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj) || strlen((*obj).valuestring) != 1)
			ft_print_err(JSON_INVALID_CHARACTER_ALPHABET_ERR, (*obj).valuestring);
		*(g_conf.alphabet + ++i) = c = *((*obj).valuestring);
		for (int x = 0; x < i; ++x)
			if (c == *(g_conf.alphabet + x))
				ft_print_err(JSON_DUP_CHARACTER_ALPHABET_ERR, (*obj).valuestring);
		obj = (*obj).next;
	}
	*(g_conf.alphabet + arsize) = 0;

	obj = cJSON_GetObjectItem(g_json, "blank");
	if (!cJSON_IsString(obj) || !(*obj).valuestring || (strlen((*obj).valuestring) != 1))
		ft_print_err(JSON_INVALID_BLANK_ERR, (*obj).valuestring);
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
		ft_print_err(JSON_BLANK_NOT_IN_ALPHABET_ERR, (*obj).valuestring);

	obj = cJSON_GetObjectItem(g_json, "states");
	if (!cJSON_IsArray(obj) || (g_max_I = cJSON_GetArraySize(obj)) < 1)
		ft_print_err(JSON_INVALID_STATE_LIST_ERR, 0);
	if (!(g_conf.states = calloc((g_max_I + 1), sizeof(void *))))
		ft_print_err(ALLOC_ERR, 0);
	i = -1;
	char	*str;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj))
			ft_print_err(JSON_INVALID_STATE_ERR, 0);
		if (!(*(g_conf.states + ++i) = str = strdup((*obj).valuestring)))
			ft_print_err(ALLOC_ERR, 0);
		for (int x = 0; x < i; ++x)
			if (ft_sequals(str, *(g_conf.states + x)))
				ft_print_err(JSON_DUP_STATE_ERR, (*obj).valuestring);
		obj = (*obj).next;
	}

	obj = cJSON_GetObjectItem(g_json, "initial");
	if (!cJSON_IsString(obj) || !(str = (*obj).valuestring))
		ft_print_err(JSON_INVALID_INITIAL_ERR, (*obj).valuestring);
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
		ft_print_err(JSON_INITIAL_NOT_IN_STATE_LIST_ERR, str);

	obj = cJSON_GetObjectItem(g_json, "finals");
	if (!cJSON_IsArray(obj) || (g_conf.fsize = cJSON_GetArraySize(obj)) < 1)
		ft_print_err(JSON_INVALID_FINAL_LIST_ERR, 0);
	if (!(g_conf.finals = malloc(g_conf.fsize * sizeof(int))))
		ft_print_err(ALLOC_ERR, 0);
	i = -1;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj) || !(str = (*obj).valuestring))
			ft_print_err(JSON_INVALID_FINAL_ERR, (*obj).valuestring);
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
			ft_print_err(JSON_FINAL_NOT_IN_STATE_LIST_ERR, str);
		for (int x = 0; x < i; ++x)
			if (*(g_conf.finals + i) == *(g_conf.finals + x))
				ft_print_err(JSON_DUP_FINAL_ERR, (*obj).valuestring);
		obj = (*obj).next;
	}
}

void	ft_set_transitions(void)
{
	char		**snames = g_conf.states;

	g_transet = calloc(sizeof(void *), g_max_I);
	if (!g_transet)
		ft_print_err(ALLOC_ERR, 0);
	cJSON		*trans = cJSON_GetObjectItem(g_json, "transitions");
	cJSON		*item;
	if (!cJSON_IsObject(trans))
		ft_print_err(JSON_INVALID_TRANS_LIST_ERR, 0);
	int			arsize;
	for (int i = 0; *(snames + i); ++i)
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

		item = cJSON_GetObjectItem(trans, *(snames + i));
		if (!cJSON_IsArray(item) || (arsize = cJSON_GetArraySize(item)) < 1)
			ft_print_err(JSON_INVALID_STATE_TRANS_ERR, *(snames + i));
		if (!(*(g_transet + i) = calloc(arsize + 1, sizeof(int [4]))))
			ft_print_err(ALLOC_ERR, 0);
		item = (*item).child;
		cJSON	*cat;
		char	*str;
		int		not_in_alpha;
		for (int x = 0; item; ++x)
		{
			if (!(*((void **)*(g_transet + i) + x) = malloc(sizeof(int [4]))))
				ft_print_err(ALLOC_ERR, 0);

			cat = cJSON_GetObjectItem(item, "read");
			if (!cat || !cJSON_IsString(cat) || strlen((*cat).valuestring) != 1)
				ft_print_err(JSON_INVALID_TRANS_READ_ERR, (*cat).valuestring);
			not_in_alpha = -1;
			while (*(g_conf.alphabet + ++not_in_alpha))
			{
				if (*(*cat).valuestring == *(g_conf.alphabet + not_in_alpha))
				{
					not_in_alpha = 0;
					break ;
				}
			}
			if (not_in_alpha)
				ft_print_err(JSON_TRANS_READ_NOT_IN_ALPHABET_ERR, (*cat).valuestring);
			**((int **)*(g_transet + i) + x) = *(*cat).valuestring;

			cat = cJSON_GetObjectItem(item, "to_state");
			if (!cat || !cJSON_IsString(cat))
				ft_print_err(JSON_INVALID_TRANS_TO_STATE_ERR, 0);
			str = (*cat).valuestring;
			for (int y = 0; *(snames + y); ++y)
			{
				if (ft_sequals(*(snames + y), str))
				{
					str = 0;
					*(*((int **)*(g_transet + i) + x) + 1) = y;
					break ;
				}
			}
			if (str)
				ft_print_err(JSON_TRANS_TO_STATE_NOT_IN_STATE_LIST_ERR, str);

			cat = cJSON_GetObjectItem(item, "write");
			if (!cat || !cJSON_IsString(cat) || strlen((*cat).valuestring) != 1)
				ft_print_err(JSON_INVALID_TRANS_WRITE_ERR, (*cat).valuestring);
			not_in_alpha = -1;
			while (*(g_conf.alphabet + ++not_in_alpha))
			{
				if (*(*cat).valuestring == *(g_conf.alphabet + not_in_alpha))
				{
					not_in_alpha = 0;
					break ;
				}
			}
			if (not_in_alpha)
				ft_print_err(JSON_TRANS_WRITE_NOT_IN_ALPHABET_ERR, (*cat).valuestring);
			*(*((int **)*(g_transet + i) + x) + 2) = *(*cat).valuestring;

			cat = cJSON_GetObjectItem(item, "action");
			if (!cat || !cJSON_IsString(cat))
				ft_print_err(JSON_INVALID_ACTION_ERR, 0);
			if (ft_sequals("LEFT", (*cat).valuestring))
				*(*((int **)*(g_transet + i) + x) + 3) = 'L';
			else if (ft_sequals("RIGHT", (*cat).valuestring))
				*(*((int **)*(g_transet + i) + x) + 3) = 'R';
			else
				ft_print_err(JSON_UNKNOWN_ACTION_ERR, (*cat).valuestring);
			item = (*item).next;
		}
	}
}