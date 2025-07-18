NAME = libftprintf.a

CC = cc

CFLAG = -Wall -Wextra -Werror

SOURCES =	ft_print_char.c \
			ft_print_hex_lower.c \
			ft_print_hex_pointer.c \
			ft_print_hex_upper.c \
			ft_print_number.c \
			ft_print_string.c \
			ft_printf.c

OBJS = $(SOURCES:%.c=%.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@ar crs $(NAME) $(OBJS)
	@echo "Library $(NAME) created successfully!"

bonus: $(OBJS) $(OBJS_BONUS)
	@ar crs $(NAME) $(OBJS) $(OBJS_BONUS)
	@echo "Library $(NAME) with bonus objects created successfully!"

%.o: %.c
	@echo "Now compiling $< to make $@"
	@$(CC) $(CFLAG) -c $< -o $@

clean:
	@rm -f $(OBJS) $(OBJS_BONUS)
	@echo "All object files removed."

fclean: clean
	rm -f $(NAME)
	@echo "Library $(NAME) removed."

re: fclean all