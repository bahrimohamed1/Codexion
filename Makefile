CC = cc
NAME = codexion
CFLAGS = -Wall -Wextra -Werror
FILES = codexion.c parser.c time.c
OBJS = $(FILES:.c=.o)

all: $(OBJS)
	$(CC) $(FILES) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all