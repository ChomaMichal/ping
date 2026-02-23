
CC = clang

FLAGS = -Wall -Wextra -g

SRC = main.c \
	  parsing.c \
	  utils.c\
	  mesages.c\
	  sockets.c
OBJ = $(SRC:.c=.o)

NAME = ft_ping

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

install:
	mkdir -p  $(DESTDIR)/bin
	cp $(NAME) $(DESTDIR)/bin

%.o: %.c
	$(CC) $(FLAGS) -c $<


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

run: all
	./ft_ping
