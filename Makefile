NAME =	webserv

INC =	basic_server/tcpServer.hpp basic_server/httpparser/requestParser.hpp basic_server/responseBuilder.hpp

OBJ =	basic_server/server.o basic_server/tcpServer.o basic_server/httpparser/requestParser.o basic_server/responseBuilder.o

CC =	clang++

CCFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CCFLAGS) $(OBJ) -o $@

%.o: %.cpp $(INC)
		$(CC) -c $(CCFLAGS) -o $@ $<

clean:
		@rm -rf $(OBJ)

fclean: clean
		@rm -rf $(NAME)

re:
		@make fclean
		@make all

.PHONY: all clean fclean re