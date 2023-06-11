# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 10:37:44 by ael-hayy          #+#    #+#              #
#    Updated: 2023/05/23 17:53:42 by aaitbelh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserve

FILES = HTTP/Socket.cpp HTTP/HandlGetRequest.cpp HTTP/Client.cpp HTTP/Request.cpp HTTP/Response.cpp HTTP/Server.cpp HTTP/HandDeleteRequest.cpp pars/parsing.cpp

OBJS = $(FILES:.cpp=.o)

CC = clang++
CFLAGS = -std=c++98 -g3 #-fsanitize=address -g
all: $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
# all:
# 	$(CC) $(CFLAGS) $(FILES) -o $(NAME)
	
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re :  fclean all