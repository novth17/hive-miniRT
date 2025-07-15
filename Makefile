NAME		= miniRT
CC			= cc
CFLAGS		= -Wextra -Wall -Werror

OBJ_DIR		= obj/
SRC_DIR		= src

INCLUDES	= -I ./inc
HEADERS		= inc/miniRT.h

SRC 		= 	main.c

OBJS		= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(NAME): $(OBJS) $(HEADERS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[0;32mminiRT built successfully ✅\033[0m"

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[0;32mminiRT cleaned successfully ✅\033[0m"

fclean: clean
	@rm -f $(NAME) $(LIBFT)

re: fclean all

.PHONY: all clean fclean re
