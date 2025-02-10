# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/28 18:50:23 by rbardet-          #+#    #+#              #
#    Updated: 2025/02/07 17:28:00 by rbardet-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
NAME2 = pipex_bonus

SOURCES =	src/pipex.c \
			src/pipex_utils.c \

SOURCES_BONUS =	src_bonus/pipex_bonus.c \
				src_bonus/pipex_utils_bonus.c \
				src_bonus/here_doc.c \

INCLUDE = -I SOURCES -I libft

LIBFTPATH = LIBFT_V2
LIBFT = $(LIBFTPATH)/libft.a

OBJECTS = $(SOURCES:.c=.o)
OBJECTS2 = $(SOURCES_BONUS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDE)
all: $(NAME)

bonus: $(NAME2)

$(NAME2): $(OBJECTS2) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME2) $(OBJECTS2) $(LIBFT)

$(NAME): $(OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(OBJECTS2)

fclean: clean
	rm -f $(NAME) $(NAME2)

re: fclean all bonus

.PHONY: all clean fclean re bonus
