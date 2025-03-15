NAME	=	minishell
SRCS	=	main.c

OBJS	=	$(SRCS:.c=.o)
CC		=	cc 
CFLAGS	=	-Wall -Wextra -Werror

$(NAME) : $(OBJS)
	make -C libft
	$(CC) -o $(NAME) $(OBJS) libft/libft.a -lreadline

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
#	make fclean -C libft/lib
	make -C libft fclean

fclean:	clean
	rm -f $(NAME)

norm:
#	norminette minishell.h
	norminette $(SRCS)

re:	fclean	all

.PHONY:all	clean	fclean	re
