NAME		= mini_rt

CC			= cc
INCLUDE		:= -Iinc -Ilibs/libft/inc -Ilibs/MLX42/include/MLX42
CFLAGS		= $(INCLUDE) -Wall -Wextra -g #-Werror

LIBFT_DIR := libs/libft
LIBFT := $(LIBFT_DIR)/libft.a

MLX42_DIR := libs/MLX42
MLX42 := $(MLX42_DIR)/build/libmlx42.a

OBJ_DIR		= obj/
SRC_DIR		= src

HEADERS		= 	inc/error.h \
				inc/mini_rt.h \
				inc/object.h \
				inc/rt_math_hien.h

SRC 		=	$(SRC_DIR)/main.c \
				$(SRC_DIR)/init.c \
				$(SRC_DIR)/error.c \
				$(SRC_DIR)/delete.c \
				$(SRC_DIR)/parser/parse.c \
				$(SRC_DIR)/parser/validate.c \
				$(SRC_DIR)/parser/fill_arr.c \
				$(SRC_DIR)/parser/attribute/color.c \
				$(SRC_DIR)/parser/attribute/float.c \
				$(SRC_DIR)/parser/attribute/vector.c \
				$(SRC_DIR)/parser/identifier/element.c \
				$(SRC_DIR)/parser/identifier/object.c \
				$(SRC_DIR)/math/rt_math.c

OBJS		= $(addprefix $(OBJ_DIR), $(SRC:$(SRC_DIR)/%.c=%.o))

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)

$(MLX42):
	@if [ ! -d "$(MLX42_DIR)" ]; then \
		cd libs && git clone https://github.com/codam-coding-college/MLX42.git; \
	fi
	@cmake $(MLX42_DIR) -B $(MLX42_DIR)/build && $(MAKE) -C $(MLX42_DIR)/build -j4

$(LIBFT):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(MLX42) $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX42) -ldl -lglfw -pthread -lm -o $(NAME)
	@echo "\033[0;32mminiRT built with MLX42 successfully ✅\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
	@echo "\033[0;32mminiRT cleaned successfully ✅\033[0m"

fclean:
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)
	@rm -rf $(MLX_DIR)/build
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "\033[0;32mminiRT cleaned successfully. Executable '$(NAME)' removed. ✅\033[0m"

re: fclean all

.PHONY: all clean fclean re
