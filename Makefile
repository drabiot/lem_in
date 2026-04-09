# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/10 13:44:12 by tchartie          #+#    #+#              #
#    Updated: 2026/04/09 15:43:43 by tchartie         ###   ########.fr        #
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

LIBRARIES_DIR = libs/
GLFW = $(LIBRARIES_DIR)glfw/
CGLM = $(LIBRARIES_DIR)cglm/
VEC = $(LIBRARIES_DIR)vector/
LIBS = -ldl -lglfw -pthread -lm $(GLFW)build/src/libglfw3.a
GLAD = ../libs/glad/glad.c
VECTOR = ../libs/vector/vec.c
GLADO = $(patsubst %, $(OBJ_DIR)%, $(GLAD:.c=.o))
VECO = $(patsubst %, $(OBJ_DIR)%, $(VECTOR:.c=.o))

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

$(LIBRARIES_DIR):
	mkdir -p $(LIBRARIES_DIR)

glfw: $(LIBRARIES_DIR)
	@if [ ! -d "$(GLFW)" ]; then \
		echo "$(DARK_PINK)Directory $(GLFW) does not exist. Cloning the repository...$(BASE_COLOR)"; \
        git clone https://github.com/glfw/glfw.git $(GLFW); \
    else \
       echo "$(GREEN)glfw Found, no need to pull$(BASE_COLOR)"; \
	fi
	@if [ ! -f "$(GLFW)build/src/libglfw3.a" ]; then \
		cmake $(GLFW) -B $(GLFW)/build; \
		make -C $(GLFW)/build --no-print-directory -j$(nproc) ;\
	fi

glad: $(LIBRARIES_DIR)
	@if ls libs | grep -q "glad"; then\
  		echo "$(GREEN)glad is here$(BASE_COLOR)";\
  	else \
		echo "$(RED)Downloading glad$(BASE_COLOR)";\
		mkdir $(LIBRARIES_DIR)glad;\
		cd $(LIBRARIES_DIR)glad;\
		mkdir glad;\
		cd glad;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.h --output glad.h;\
		cd ..;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.c --output glad.c;\
		echo "\$(GREEN)Downloaded glad$(BASE_COLOR)";\
	fi

cglm: $(LIBRARIES_DIR)
	@if [ ! -d "$(CGLM)" ]; then \
		echo "$(DARK_PINK)Directory $(CGLM) does not exist. Cloning the repository...$(BASE_COLOR)"; \
        git clone https://github.com/recp/cglm.git $(CGLM); \
		cd $(CGLM); \
		mkdir build; \
		cd build; \
		cmake ..; \
		make; \
    else \
       echo "$(GREEN)CGLM Found, no need to pull$(BASE_COLOR)"; \
	fi

vector: $(LIBRARIES_DIR)
	@if [ ! -d "$(VEC)" ]; then \
		echo "$(DARK_PINK)Directory $(VEC) does not exist. Cloning the repository...$(BASE_COLOR)"; \
        git clone https://github.com/Mashpoe/c-vector.git $(VEC); \
		cd $(VEC); \
    else \
       echo "$(GREEN)Vector Found, no need to pull$(BASE_COLOR)"; \
	fi

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)lem-in successfully compiled! $(BASE_COLOR)"

visualizer:	CFLAGS += -I$(LIBRARIES_DIR)/glad -I$(GLFW)include  -I$(CGLM)include -I$(VEC)
visualizer:	glad glfw cglm vector $(GLADO) $(VECO) $(OBJ_B)
	@$(CC) $(CFLAGS) -o $(NAME_B) $(OBJ_B) $(GLADO) $(VECO) $(LIBS)
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
	@rm -rf $(LIBRARIES_DIR)
	@echo "$(CYAN)libs deleted! $(BASE_COLOR)"


re: 		fclean all

.PHONY :	all clean fclean re