# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/10 13:44:12 by tchartie          #+#    #+#              #
#    Updated: 2026/03/10 14:59:40 by tchartie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#============ NAME ============#

NAME			= 	lem-in

#========= COMPILATOR =========#

CC 				= 	cc

#=========== FLAGS ============#

INC				= 	inc/
CFLAGS 			= 	-I$(INC) -Wall -Werror -Wextra -g

MAKEFLAGS		=	--no-print-directory

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
SRC_NAME 		=	main.c \
					parser.c \
					gnl/get_next_line.c \
					gnl/get_next_line_utils.c

OBJ_DIR 		=	obj/
OBJ_NAME		=	$(SRC_NAME:.c=.o)
OBJ				=	$(patsubst %, $(OBJ_DIR)%, $(OBJ_NAME))

all:		$(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)lem-in successfully compiled! $(BASE_COLOR)"

$(OBJ_DIR)%.o:$(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling: $< $(BASE_COLOR)"
	@$(CC) $(CFLAGS) $< -c -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(BLUE)lem-in objects files cleanned! $(BASE_COLOR)"

fclean:		clean
	@rm -f $(NAME)
	@echo "$(CYAN)ft_shmup executable file cleanned! $(BASE_COLOR)"


re: 		fclean all

.PHONY :	all clean fclean re