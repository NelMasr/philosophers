NAME	=	philo

SRCS	=	srcs/main.c		\
			srcs/init.c		\
			srcs/sim.c		\
			srcs/utils.c	\
			srcs/time.c		\
			srcs/routine.c	\

OBJS	=	${SRCS:.c=.o}

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -pthread
DEBUG	=	-g

.c.o:
			${CC} ${CFLAGS} ${DEBUG} -c $< -o ${<:.c=.o}

all:
			$(MAKE) -j $(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(DEBUG) -o $(NAME) $(OBJS)

clean:
			rm -rf ${OBJS}

fclean:		clean
			rm  -rf ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
