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
INIT_DIR := Initialization
CODEXION_DIR := Codexion
MODELS_DIR := Models
UTILS_DIR := Utils

INIT_SRCS := app.c validation.c
CODEXION_SRCS := codexion.c scheduler.c
MODELS_SRCS := coder.c queue.c
UTILS_SRCS := utils.c logger.c

INIT_FILES := $(addprefix $(INIT_DIR)/,$(INIT_SRCS))
CODEXION_FILES := $(addprefix $(CODEXION_DIR)/,$(CODEXION_SRCS))
MODELS_FILES := $(addprefix $(MODELS_DIR)/,$(MODELS_SRCS))
UTILS_FILES := $(addprefix $(UTILS_DIR)/,$(UTILS_SRCS))

all: $(NAME)

$(NAME): main.c $(CODEXION_FILES) $(INIT_FILES) $(MODELS_FILES) $(UTILS_FILES)
	$(CC) $(FLAGS) main.c $(CODEXION_FILES) $(INIT_FILES) $(MODELS_FILES) $(UTILS_FILES) -o $(NAME)

clean:
	rm -f $(NAME)

.PHONY: all clean