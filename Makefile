NAME		= miniRT

CC			= cc
INCLUDE := -Iinc -Ilibs/libft/inc -Ilibs/MLX42/include/MLX42
CFLAGS  = $(INCLUDE) -Wall -Wextra -Werror

LIBFT_DIR := libs/libft
LIBFT := $(LIBFT_DIR)/libft.a

MLX42_DIR := libs/MLX42
MLX42 := $(MLX42_DIR)/build/libmlx42.a

OBJ_DIR		= obj/
SRC_DIR		= src

HEADERS		= inc/miniRT.h

SRC 		= $(SRC_DIR)/main.c
OBJS		= $(addprefix $(OBJ_DIR), $(SRC:$(SRC_DIR)/%.c=%.o))

$(OBJ_DIR)%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)

$(MLX42):
	@if [ ! -d "$(MLX42_DIR)" ]; then \
		cd libs && git clone https://github.com/codam-coding-college/MLX42.git; \
	fi
	@cmake $(MLX42_DIR) -B $(MLX42_DIR)/build && $(MAKE) -C $(MLX42_DIR)/build -j4

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(MLX42) $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX42) -ldl -lglfw -pthread -lm -o $(NAME)
	@echo "\033[0;32mminiRT built with MLX42 successfully ✅\033[0m"

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(MLX_DIR)/build
	@echo "\033[0;32mminiRT cleaned successfully ✅\033[0m"

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
