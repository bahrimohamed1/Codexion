CC = cc
NAME = codexion
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS = codexion.c parser.c time.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(SRCS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all