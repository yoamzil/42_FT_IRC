CXX			=	c++
CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98
NAME		=	ircserv
SRCS		=	src/main.cpp \
				src/Server.cpp \
				src/client_channel/Client.cpp \
				src/client_channel/Channel.cpp \
				src/Commands.cpp \
				src/client_channel/Authentications.cpp \
				src/client_channel/ModeUser.cpp \

HFS			=	include/Server.hpp \
				include/client_channel/Client.hpp \
				include/client_channel/Channel.hpp \
				include/Commands.hpp \
				include/client_channel/Authentications.hpp \
				include/client_channel/ModeUser.hpp \
				

OBJS		=	$(SRCS:.cpp=.o)

%.o			:	%.cpp $(HFS)
				$(CXX) $(CXXFLAGS) -c $< -o $@

all			:	$(NAME)

$(NAME)		:	$(OBJS) $(HFS)
				$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean		:
				rm -f $(OBJS)

fclean		:	clean
				rm -f $(NAME)

re			:	fclean all

.PHONY		:	clean fclean all re