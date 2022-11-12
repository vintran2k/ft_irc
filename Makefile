NAME			=	ircserv


MAIN			=	main.cpp
SRCS_NETWORK	=	Socket.cpp		\
					Server.cpp		\
					Client.cpp
SRCS_IRC		=	Irc.cpp			\
					User.cpp		\
					Channel.cpp		\
					ircUtils.cpp
SRCS_CMD		=	AWAY.cpp


CC				=	@c++
FLAGS			=	-Wall -Wextra -Werror -std=c++98 -g #// -g a enlever
OBJDIR			=	objs
OBJS			=	$(MAIN:.cpp=.o)									\
					$(addprefix network/, $(SRCS_NETWORK:.cpp=.o))	\
					$(addprefix irc/, $(SRCS_IRC:.cpp=.o))		\
					$(addprefix irc/commands/, $(SRCS_CMD:.cpp=.o))


.cpp.o			:
				$(CC) $(FLAGS) -c $< -o $@


$(NAME)			:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)
				@mkdir -p $(OBJDIR)
				@mv $(OBJS) $(OBJDIR)
				@echo "\033[1;30m[$(NAME)] \033[1;32mcreated !\033[0m"

all				:	$(NAME)

clean			:
				@rm -rf $(OBJDIR)
				@echo "\033[1;30m[$(OBJDIR)] : \033[1;31mdeleted !\033[0m"

fclean			:	clean
				@rm -f $(NAME)
				@echo "\033[1;30m[$(NAME)] \033[1;31mdeleted !\033[0m"

re				:	fclean all

.PHONY			: 	clean fclean all re