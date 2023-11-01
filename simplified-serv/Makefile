
### Program ###
NAME		=	webserv

### Compiler ###
CC			=	c++

ifeq ($(shell uname), Darwin)
# Max
CFLAGS		=	-Wall -Werror -Wextra -g -o0 -DDEBUG -std=c++98# --pedantic
#-Werror
else
# Ranja and Lukas
CFLAGS		=	-Wall -Werror -Wextra -g -std=c++98
endif

### Directories (Sources) ###
SRC_DIR		=	src/
OBJ_DIR		=	obj/
INC_DIR		=	include/

### Control Codes ###
RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
UP			=	\033[A
CUT			=	\033[K

### Source Files ###
# SRC_FILES	=	main.cpp TestServer.cpp ClientSocket.cpp ListeningSocket.cpp RespondMessage.cpp

# OBJ_FILES	=	$(SRC_FILES:.cpp=.o)
SOURCES := $(wildcard $(SRC_DIR)*.cpp) 
OBJECTS := $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SOURCES)) 

### Paths ###
# SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))
# OBJ			=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

### All Rule ###
all: $(NAME)

### Compile The Executable ###
# $(NAME): $(OBJ)
$(NAME): $(OBJECTS)
	@echo "$(YELLOW)Compiling [$(NAME)]...$(RESET)"
# 	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ $^
	@echo "$(GREEN)Finished [$(NAME)]$(RESET)"
	@echo "please delete me" > www/delete_dir/delete_me.txt
	@mkdir -p tmp


### Compile Objects ###
$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling [$@]...$(RESET)"
#   @$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ -c $<
	@$(CC) $(CFLAGS) -I $(INC_DIR) -I $(SRC_DIR) -o $@ -c $<
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished [$@]$(RESET)"
	@printf "$(UP)$(CUT)"

### clean Rule ###
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
	rm -rf $(OBJ_DIR); \
	echo "$(BLUE)Deleting all objects from /webserv...$(RESET)"; else \
	echo "No objects to remove from /webserv."; \
	fi;

### fclean Rule ###
fclean: clean
	@if [ -f "$(NAME)" ]; then \
	rm -f $(NAME); \
	echo "$(BLUE)Deleting $(NAME) from webserv...$(RESET)"; else \
	echo "No Executable to remove from webserv."; \
	fi;

e:	$(NAME)
	./$(NAME) nothing

### re Rule ###
re: fclean all

### phony ###
.PHONY: all clean fclean re