NAME = ft_turing

LIB = lib/libcjson.a lib/libcjson_utils.a
INCLUDES = -Iinclude/cjson

SRCS = source/main.c source/error.c  source/free.c  source/read_file.c  source/utils.c
OBJS = $(SRCS:source/%.c=obj/%.o)
FILES = obj

GCC= gcc -Wall -Werror -Wextra -g

all : $(NAME)

$(NAME) : $(OBJS)
	@$(GCC) $(OBJS) $(LIB) -no-pie -o $@

obj/%.o: source/%.c
	@mkdir -p obj
	@$(GCC) $(INCLUDES) -c $< -o $@ 

clean :
	@rm -f $(OBJS)
	@rm -rf $(FILES)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
