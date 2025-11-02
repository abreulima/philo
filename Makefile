NAME    := philo
CC      := cc
CFLAGS	:=
CLIBS	:= -lpthread
CFILES	:= philo.c
OBJS	:= $(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CLIBS) -c $(CFILES)

run:
	./philo

clean:
	rm $(NAME)

fclean: clean
	rm $(OBJS)
