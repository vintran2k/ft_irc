NAME_SERVER		=	server
NAME_CLIENT		=	client
SRCS_SERVER		=	server.cpp
SRCS_CLIENT		=	client.cpp
CC				=	@c++
FLAGS			=	-Wall -Wextra -Werror
OBJDIR_SERVER		=	objs_server
OBJS_SERVER			=	$(addprefix $(OBJDIR_SERVER)/, $(SRCS_SERVER:.cpp=.o))
OBJDIR_CLIENT		=	objs_client
OBJS_CLIENT		=	$(addprefix $(OBJDIR_CLIENT)/, $(SRCS_CLIENT:.cpp=.o))

all					:	$(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER)			:	$(OBJS_SERVER)
				$(CC) $(FLAGS) $(OBJS_SERVER) -o $(NAME_SERVER)
				@echo "\033[1;30m[$(NAME_SERVER)] \033[1;32mcreated !\033[0m"

$(NAME_CLIENT)			:	$(OBJS_CLIENT)
				$(CC) $(FLAGS) $(OBJS_CLIENT) -o $(NAME_CLIENT)
				@echo "\033[1;30m[$(NAME_CLIENT)] \033[1;32mcreated !\033[0m"

${OBJDIR_SERVER}/%.o	:	%.cpp
				@mkdir -p $(OBJDIR_SERVER)
				$(CC) $(FLAGS) -c $< -o $@

${OBJDIR_CLIENT}/%.o	:	%.cpp
				@mkdir -p $(OBJDIR_CLIENT)
				$(CC) $(FLAGS) -c $< -o $@

clean			:
				@rm -rf $(OBJDIR_SERVER) $(OBJDIR_CLIENT)
				@echo "\033[1;30m[$(OBJDIR_SERVER), $(OBJDIR_CLIENT)] : \033[1;31mdeleted !\033[0m"

fclean			:	clean
				@rm -f $(NAME_SERVER) $(NAME_CLIENT)
				@echo "\033[1;30m[$(NAME_SERVER), $(NAME_CLIENT)] \033[1;31mdeleted !\033[0m"

re				:	fclean all

.PHONY			: 	clean fclean all re