NAME    := philo
CC      := cc
CFLAGS	:=
CLIBS	:= 
CFILES	:= philo.c
OBJS	:= $(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CLIBS) -c $(CFILES)


clean:
	rm $(NAME)

fclean: clean
	rm $(OBJS)


re: fclean all
	
run:
	./philo

