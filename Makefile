NAME	=	taskmaster
SRCS	=	shell_prompt/srcs/cmd.c \
		shell_prompt/srcs/hist.c \
		shell_prompt/srcs/in_buff.c \
		main.c \
		shell_prompt/srcs/prompt.c \
		shell_prompt/srcs/term.c \
		shell_jobs/action.c \
		shell_jobs/parse_cmd.c \
		utils/srcs/exit_error.c \
		config/config.c \
		vector/vector.c \
		dict/dict.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wextra -Werror

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
