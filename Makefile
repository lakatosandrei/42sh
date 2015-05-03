NAME = ft_select

SRCS =	main.c

OBJS = $(SRCS:.c=.o)

CC = g++

FLAGS = -Wall -Wextra -Werror

TERMCAP = -ltermcap -lncurses

CFLAG = -c

CDEBUG = -g

CNAME = -o

RM = rm

RM_T = -f

all:$(NAME)

$(NAME):
	make -C ./libft
	$(CC) $(FLAGS) $(CFLAG) $(SRCS) $(TERMCAP)
	$(CC) $(FLAGS) $(CNAME) $(NAME) $(OBJS) $(CDEBUG) $(TERMCAP)

clean:
	$(RM) $(RM_T) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:fclean $(NAME)
