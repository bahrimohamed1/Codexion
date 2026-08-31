NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c \
	parser.c \
	time.c \
	simulation.c \
	simulation_run.c \
	coders.c \
	coder_actions.c \
	dongles.c \
	dongle_request.c \
	dongle_wait.c \
	dongles_acquire.c \
	dongles_release.c \
	heap.c \
	heap_utils.c \
	heap_pop.c \
	scheduler.c \
	logger.c \
	monitor.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(SRCS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all