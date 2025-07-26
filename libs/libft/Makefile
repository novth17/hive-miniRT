# Name of the library
NAME = libft.a

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Source and Object Files
SOURCES = src/ft_is/ft_isalnum.c \
          src/ft_is/ft_isalpha.c \
          src/ft_is/ft_isascii.c \
          src/ft_is/ft_isdigit.c \
          src/ft_is/ft_isprint.c \
		  src/ft_is/ft_isspace.c \
		  src/ft_is/ft_is_only_whitespace.c \
          src/ft_lst/ft_lstadd_back_bonus.c \
          src/ft_lst/ft_lstadd_front_bonus.c \
          src/ft_lst/ft_lstclear_bonus.c \
          src/ft_lst/ft_lstdelone_bonus.c \
          src/ft_lst/ft_lstiter_bonus.c \
          src/ft_lst/ft_lstlast_bonus.c \
          src/ft_lst/ft_lstmap_bonus.c \
          src/ft_lst/ft_lstnew_bonus.c \
          src/ft_lst/ft_lstsize_bonus.c \
          src/ft_mem/ft_bzero.c \
          src/ft_mem/ft_calloc.c \
          src/ft_mem/ft_memchr.c \
          src/ft_mem/ft_memcmp.c \
          src/ft_mem/ft_memcpy.c \
          src/ft_mem/ft_memmove.c \
          src/ft_mem/ft_memset.c \
          src/ft_dprintf/ft_print_char.c \
          src/ft_dprintf/ft_print_hex_lower.c \
          src/ft_dprintf/ft_print_hex_pointer.c \
          src/ft_dprintf/ft_print_hex_upper.c \
          src/ft_dprintf/ft_print_number.c \
          src/ft_dprintf/ft_print_string.c \
          src/ft_dprintf/ft_dprintf.c \
          src/ft_put/ft_putchar_fd.c \
          src/ft_put/ft_putendl_fd.c \
          src/ft_put/ft_putnbr_fd.c \
          src/ft_put/ft_putstr_fd.c \
          src/ft_string/ft_split.c \
          src/ft_string/ft_strchr.c \
          src/ft_string/ft_strdup.c \
          src/ft_string/ft_striteri.c \
          src/ft_string/ft_strjoin.c \
          src/ft_string/ft_strlcat.c \
          src/ft_string/ft_strlcpy.c \
          src/ft_string/ft_strlen.c \
          src/ft_string/ft_strmapi.c \
          src/ft_string/ft_strncmp.c \
		  src/ft_string/ft_strcmp.c \
          src/ft_string/ft_strnstr.c \
          src/ft_string/ft_strrchr.c \
          src/ft_string/ft_strtrim.c \
          src/ft_string/ft_substr.c \
		  src/ft_string/ft_count_2d.c \
          src/ft_to/ft_atoi.c \
		  src/ft_to/ft_atof.c \
		  src/ft_to/ft_atol.c \
          src/ft_to/ft_itoa.c \
          src/ft_to/ft_tolower.c \
          src/ft_to/ft_toupper.c \
		  src/ft_string/ft_free_2d.c \
		  src/ft_string/ft_free_3d.c \
		  src/get_next_line/get_next_line_utils.c \
		  src/get_next_line/get_next_line.c

# Adjust paths for object files
OBJS = $(SOURCES:.c=.o)

# Default target
all: $(NAME)

# Rule to build the library
$(NAME): $(OBJS)
	@ar crs $(NAME) $(OBJS)
	@echo "\033[0;33mLibrary $(NAME) created successfully! ✅\033[0m"

# Rule to build object files
%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< to create $@" > /dev/null
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	@rm -f $(OBJS)
	@echo "\033[0;33mLibft clean success: All object files removed. ✅\033[0m"

# Full clean target
fclean: clean
	@rm -f $(NAME)
	@echo "\033[0;33mLibft fclean success: Library $(NAME) removed. ✅\033[0m"


# Recompile everything
re: fclean all

# Phony targets

