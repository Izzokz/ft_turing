NAME = ft_turing

LIB = lib/libcjson.a lib/libcjson_utils.a
INCLUDES = -Iinclude/cjson

SRCS = main.c
OBJS = $(SRCS:.s=.o)
# FILES = $(OBJS)

GCC= gcc -Wall -Werror -Wextra -g

all : $(NAME)

$(NAME) : $(OBJS)
	@$(GCC) $(OBJS) $(LIB) -no-pie -o $@

%.o: %.c
	@$(GCC) $(INCLUDES) -c $< -o $@

# clean :
# 	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all test clean fclean re
