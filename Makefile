# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afomin <afomin@student.42kl.edu.my         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/10 17:25:59 by afomin            #+#    #+#              #
#    Updated: 2026/03/10 17:26:01 by afomin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := codexion
CC          := cc
FLAGS       := -Wall -Wextra -Werror -pthread

INIT_DIR      := Initialization
CODEXION_DIR  := Codexion
MODELS_DIR    := Models
UTILS_DIR     := Utils

INIT_SRCS     := initialization.c validation.c cleanup.c
CODEXION_SRCS := codexion.c scheduler.c
MODELS_SRCS   := coder.c queue.c
UTILS_SRCS    := utils.c logger.c pthread.c

SRCS := main.c \
        $(addprefix $(INIT_DIR)/, $(INIT_SRCS)) \
        $(addprefix $(CODEXION_DIR)/, $(CODEXION_SRCS)) \
        $(addprefix $(MODELS_DIR)/, $(MODELS_SRCS)) \
        $(addprefix $(UTILS_DIR)/, $(UTILS_SRCS))

OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re