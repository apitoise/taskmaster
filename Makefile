NAME	=	taskmaster
SRCS	=	prompt/srcs/cmd.c \
		prompt/srcs/hist.c \
		prompt/srcs/in_buff.c \
		prompt/srcs/prompt.c \
		prompt/srcs/term.c \
		core/srcs/main.c \
		core/srcs/action.c \
		core/srcs/clean.c \
		core/srcs/prog.c \
		core/srcs/prog_dic.c \
		core/srcs/unwrap.c \
		core/srcs/monitor.c \
		core/srcs/log.c \
		config/config.c \
		vector/vector.c \
		dict/dict.c
LOGS	=	./log_out \
		./log_err \
		./log.txt
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wextra -Werror

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS) $(LOGS)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
