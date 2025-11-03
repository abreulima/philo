NAME    := philo
CC      := cc
CFLAGS	:=
<<<<<<< HEAD
CLIBS	:= 
=======
CLIBS	:= -lpthread
>>>>>>> 68e3288 (up)
CFILES	:= philo.c
OBJS	:= $(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CLIBS) -c $(CFILES)

<<<<<<< HEAD
=======
run:
	./philo
>>>>>>> 68e3288 (up)

clean:
	rm $(NAME)

fclean: clean
	rm $(OBJS)
<<<<<<< HEAD


re: fclean all
	
run:
	./philo

=======
>>>>>>> 68e3288 (up)
