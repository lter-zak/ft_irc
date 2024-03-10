NONE='\033[0m'
BLUE = '\x1b[36m'
YELLOW= '\x1b[32m'
MAGENTA ='\x1b[35m'

NAME		= ircserv
SRCS    	= $(shell find . -name "*.cpp")
OBJS		= $(SRCS:.cpp=.o)
HEADER		= $(shell find . -name "*.hpp")
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 #-fsanitize=address
RM			= rm -f
#CPP			= c++


all: $(NAME)

$(NAME) : $(OBJS)
		@echo $(NONE)$(BLUE) "\n\t\t🤹 Compiling $(NAME)... \n" $(NONE)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
		@echo $(NONE)$(BLUE)"\n\t\t😱 Compiled   $(NAME)\n"$(NONE)

clean	:
		@echo $(YELLOW) "\n\t\t🙀 Removing object files...\n" $(NONE)
		$(RM) $(OBJS)

fclean	: 	clean
			@echo $(MAGENTA) "\n\t\t🙀 Removing $(NAME)...\n" $(NONE)
			$(RM) $(NAME)  
			rm -rf ~/LIbrary/Caches

re		:fclean all
.PHONY: all clean fclean re