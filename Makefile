NAME = webserve

FILES = main.cpp

OBJS = $(FILES:.cpp=.o)

CC = clang++
CFLAGS = -Wall -Werror -Wextra -std=c++98


all:$(NAME)
$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re : fclean clean all