# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 10:37:44 by ael-hayy          #+#    #+#              #
#    Updated: 2023/03/22 16:03:55 by ael-hayy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserve

FILES = Socket.cpp Client.cpp main.cpp Request.cpp Response.cpp Server.cpp

OBJS = $(FILES:.cpp=.o)

CC = clang++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3 -fsanitize=address


all:$(NAME)
$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re : fclean clean all