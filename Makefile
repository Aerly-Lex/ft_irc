.SILENT:

NAME		:= ircserv
CXX			:= c++
CXXFLAGS	= -std=c++17 #-Wall -Werror -Wextra -std=c++17 -Iincludes

SRC			=	srcs/main.cpp \
				srcs/Server.cpp \
				srcs/User.cpp \
				srcs/Channel.cpp \
				srcs/Utilities.cpp

CMDS		=	srcs/commands/cap.cpp \
				srcs/commands/Commands.cpp \
				srcs/commands/invite.cpp \
				srcs/commands/mode.cpp \
				srcs/commands/nick.cpp \
				srcs/commands/part.cpp \
				srcs/commands/ping.cpp \
				srcs/commands/quit.cpp \
				srcs/commands/join.cpp \
				srcs/commands/topic.cpp

ALL_SRC		= $(SRC) $(CMDS)

OBJ_DIR		= obj/
OBJ			= $(ALL_SRC:srcs/%.cpp=$(OBJ_DIR)%.o)

$(OBJ_DIR)%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
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
	rm -rf $(OBJ_DIR)
	@if [ -d $(OBJ_DIR) ]; then rmdir $(OBJ_DIR); fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test