NAME =	webserv

INC =	web_server/serverManager.hpp \
		web_server/responseBuilder.hpp \
		web_server/httpparser/requestParser.hpp \
		web_server/configparser/configParser.hpp \
		web_server/configparser/serverConfig.hpp \
		web_server/utilities/utilities.hpp

OBJ =	main.o \
		web_server/serverManager.o \
		web_server/responseBuilder.o \
		web_server/httpparser/requestParser.o \
		web_server/configparser/configParser.o \
		web_server/configparser/serverConfig.o \
		web_server/utilities/utilities.o

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