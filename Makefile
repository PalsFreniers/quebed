CC = clang++
CFLAGS = -Wall -Wextra -g -fsanitize=address
LDFLAGS = -fsanitize=address
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
NAME = quebed

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
