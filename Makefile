NAME			=	ircserv
SRCS			=	main.cpp		\
					Socket.cpp		\
					Server.cpp		\
					Client.cpp		\
					Irc.cpp			\
					User.cpp		\
					ircUtils.cpp	\
					Channel.cpp
CC				=	@c++
FLAGS			=	-Wall -Wextra -Werror -std=c++98 -g
OBJDIR			=	objs
OBJS			=	$(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))


all					:	$(NAME)

$(NAME)			:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)
				@echo "\033[1;30m[$(NAME)] \033[1;32mcreated !\033[0m"

${OBJDIR}/%.o	:	%.cpp
				@mkdir -p $(OBJDIR)
				$(CC) $(FLAGS) -c $< -o $@


clean			:
				@rm -rf $(OBJDIR)
				@echo "\033[1;30m[$(OBJDIR)] : \033[1;31mdeleted !\033[0m"

fclean			:	clean
				@rm -f $(NAME)
				@echo "\033[1;30m[$(NAME)] \033[1;31mdeleted !\033[0m"

re				:	fclean all

.PHONY			: 	clean fclean all re