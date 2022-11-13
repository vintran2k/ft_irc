NAME			=	ircserv

SRCS			=	main.cpp					\
					network/Socket.cpp			\
					network/Server.cpp			\
					network/Client.cpp			\
					irc/Irc.cpp					\
					irc/User.cpp				\
					irc/Channel.cpp				\
					irc/ircUtils.cpp			\
					irc/commands/AWAY.cpp		\
					irc/commands/DIE.cpp		\
					irc/commands/INVITE.cpp		\
					irc/commands/JOIN.cpp		\
					irc/commands/KICK.cpp		\
					irc/commands/KILL.cpp		\
					irc/commands/LIST.cpp		\
					irc/commands/MODE.cpp		\
					irc/commands/MOTD.cpp		\
					irc/commands/NAMES.cpp		\
					irc/commands/NICK.cpp		\
					irc/commands/NOTICE.cpp		\
					irc/commands/OPER.cpp		\
					irc/commands/PART.cpp		\
					irc/commands/PASS.cpp		\
					irc/commands/PING.cpp		\
					irc/commands/PRIVMSG.cpp	\
					irc/commands/QUIT.cpp		\
					irc/commands/TOPIC.cpp		\
					irc/commands/USER.cpp		\
					irc/commands/WHO.cpp


CC				=	@c++
FLAGS			=	-Wall -Wextra -Werror -std=c++98 -g #// -g a enlever
OBJDIR			=	objs
OBJS			=	$(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))


$(OBJDIR)/%.o	:	%.cpp
				@mkdir -p $(OBJDIR)/network
				@mkdir -p $(OBJDIR)/irc
				@mkdir -p $(OBJDIR)/irc/commands
				$(CC) $(FLAGS) -c $< -o $@


$(NAME)			:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)
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