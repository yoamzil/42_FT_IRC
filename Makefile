CXX			=	c++
CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98
NAME		=	ircserv
SRCS		=	src/main.cpp \
				src/Server.cpp \
				src/Client.cpp \
				src/Channel.cpp \
				src/Commands.cpp \

HFS			=	include/Server.hpp \
				include/Client.hpp \
				include/Channel.hpp \
				include/Commands.hpp \
				

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