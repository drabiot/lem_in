# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/10 13:44:12 by tchartie          #+#    #+#              #
#    Updated: 2026/04/08 00:09:42 by mbirou           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#============ NAME ============#

NAME			= 	lem-in
NAME_B			=	visualizer

#========= COMPILATOR =========#

CC 				= 	cc

#=========== FLAGS ============#

INC				= 	inc/
CFLAGS 			= 	-I$(INC) -Wall -Werror -Wextra -g 

MAKEFLAGS		=	--no-print-directory

LIBMLX = MLX42
LIBS = ./$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

#=========== COLOR ============#

BASE_COLOR 		=	\033[0;39m
GRAY 			=	\033[0;90m
RED 			=	\033[0;91m
GREEN 			=	\033[0;92m
YELLOW 			=	\033[0;93m
BLUE 			=	\033[0;94m
MAGENTA			=	\033[0;95m
CYAN 			=	\033[0;96m
WHITE			=	\033[0;97m

#========== SOURCES ===========#

SRC_DIR 		= 	src/
SRC_NAME 		=	main.c\
					pathCompute.c\
					parser.c \
					sim.c \
					utils.c \
					gnl/get_next_line.c \
					gnl/get_next_line_utils.c

SRC_NAME_B 		=	visualizer.c\
					pathCompute.c\
					parser.c \
					sim.c \
					utils.c \
					gnl/get_next_line.c \
					gnl/get_next_line_utils.c

OBJ_DIR 		=	obj/
OBJ_NAME		=	$(SRC_NAME:.c=.o)
OBJ_NAME_B		=	$(SRC_NAME_B:.c=.o)
OBJ				=	$(patsubst %, $(OBJ_DIR)%, $(OBJ_NAME))
OBJ_B			=	$(patsubst %, $(OBJ_DIR)%, $(OBJ_NAME_B))

all:		$(NAME)

mlx :
	@if ls | grep -q "MLX42"; then \
		clear; \
		echo "\033[32;47;1m** MLX42 already exist **\033[1;m"; \
	else \
		git clone https://github.com/codam-coding-college/MLX42.git; \
		cmake ./MLX42 -B ./MLX42/build; \
		make -C ./MLX42/build --no-print-directory -j4; \
		make --directory ./MLX42/build; \
	fi

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)lem-in successfully compiled! $(BASE_COLOR)"

visualizer : CFLAGS += -I./$(LIBMLX)/include
visualizer:	mlx $(OBJ_B)
	@$(CC) $(CFLAGS) -o $(NAME_B) $(OBJ_B) $(LIBS)
	@echo "$(GREEN)visualizer successfully compiled! $(BASE_COLOR)"

$(OBJ_DIR)%.o:$(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling: $< $(BASE_COLOR)"
	@$(CC) $(CFLAGS) $< -c -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(BLUE)lem-in objects files cleanned! $(BASE_COLOR)"

fclean:		clean
	@rm -f $(NAME)
	@rm -f $(NAME_B)
	@echo "$(CYAN)lem-in executable file cleanned! $(BASE_COLOR)"

libclean:	
	@rm -rf $(LIBMLX)
	@echo "$(CYAN)mlx deleted! $(BASE_COLOR)"


re: 		fclean all

.PHONY :	all clean fclean re