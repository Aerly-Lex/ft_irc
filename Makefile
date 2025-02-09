.SILENT:

NAME		:= ircserv
CXX			:= c++
CXXFLAGS	= #-Wall -Werror -Wextra -std=c++17 -Iincludes

SRC			=	srcs/main.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Channel.cpp \
				srcs/Utilities.cpp
				# srcs/commands

OBJ_DIR		= obj/
OBJ			= $(SRC:srcs/%.cpp=$(OBJ_DIR)%.o)

$(OBJ_DIR)%.o: srcs/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	# clear
	@echo "\033[32mReady to run the program \"$(NAME)\"\033[0m"

#################################################

all: $(NAME)

test: $(NAME)
	@echo "Starting IRC server..."
	@./ircserv 6667 1234

clean:
	rm -f $(OBJ)
	@if [ -d $(OBJ_DIR) ]; then rmdir $(OBJ_DIR); fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re #test