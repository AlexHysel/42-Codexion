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

NAME = codexion
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread
INIT_FILES = Initialization/app.c Initialization/validation.c
CODEXION_FILES = Codexion/codexion.c Codexion/scheduler.c
MODELS_FILES = Models/coder.c Models/queue.c Models/table.c
UTILS_FILES = Utils/utils.c

all: $(NAME)

$(NAME): main.c $(CODEXION_FILES) $(INIT_FILES) $(MODELS_FILES) $(UTILS_FILES)
	$(CC) $(FLAGS) main.c $(CODEXION_FILES) $(INIT_FILES) $(MODELS_FILES) $(UTILS_FILES) -o $(NAME)

clean:
	rm $(NAME)