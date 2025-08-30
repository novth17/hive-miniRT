NAME		= miniRT

CC			= cc
INCLUDE		:= -Iinc -Ilibs/libft/inc -Ilibs/MLX42/include/MLX42

CFLAGS		= $(INCLUDE) -Wall -Wextra -Werror -flto -march=native -O3


LIBFT_DIR := libs/libft
LIBFT := $(LIBFT_DIR)/libft.a

MLX42_DIR := libs/MLX42
MLX42 := $(MLX42_DIR)/build/libmlx42.a

OBJ_DIR		= obj/
SRC_DIR		= src

HEADERS		= 	inc/camera.h \
				inc/error.h  \
				inc/hit.h  \
				inc/image_to_file.h  \
				inc/mini_rt.h  \
				inc/object.h  \
				inc/rt_math.h  \
				inc/task.h  \
				inc/thread.h  \
				inc/types.h

SRC 		=	$(SRC_DIR)/checker/check.c \
				$(SRC_DIR)/control/hook.c \
				$(SRC_DIR)/control/mouse.c \
				$(SRC_DIR)/control/scroll.c \
				$(SRC_DIR)/control/utils.c \
				$(SRC_DIR)/math/color_correction.c \
				$(SRC_DIR)/math/cross_dot_length.c \
				$(SRC_DIR)/math/interpolate.c \
				$(SRC_DIR)/math/normalize.c \
				$(SRC_DIR)/math/quaternion.c \
				$(SRC_DIR)/math/random.c \
				$(SRC_DIR)/math/rt_math.c \
				$(SRC_DIR)/math/square.c \
				$(SRC_DIR)/math/vec2.c \
				$(SRC_DIR)/math/vec3_1.c \
				$(SRC_DIR)/math/vec3_2.c \
				$(SRC_DIR)/math/vec4.c \
				$(SRC_DIR)/parser/attribute/color.c \
				$(SRC_DIR)/parser/attribute/float.c \
				$(SRC_DIR)/parser/attribute/vector.c \
				$(SRC_DIR)/parser/identifier/element.c \
				$(SRC_DIR)/parser/identifier/material.c \
				$(SRC_DIR)/parser/identifier/object.c \
				$(SRC_DIR)/parser/fill_arr.c \
				$(SRC_DIR)/parser/parse_utils.c \
				$(SRC_DIR)/parser/parse.c \
				$(SRC_DIR)/parser/validate.c \
				$(SRC_DIR)/print_message/error.c \
				$(SRC_DIR)/print_message/guide.c \
				$(SRC_DIR)/print_screen/image_to_file_utils.c \
				$(SRC_DIR)/print_screen/image_to_file.c \
				$(SRC_DIR)/render/background.c \
				$(SRC_DIR)/render/camera.c \
				$(SRC_DIR)/render/point_light.c \
				$(SRC_DIR)/render/prepare_to_render.c \
				$(SRC_DIR)/render/ray.c \
				$(SRC_DIR)/render/render.c \
				$(SRC_DIR)/render/sample.c \
				$(SRC_DIR)/shape/cyl_hit.c \
				$(SRC_DIR)/shape/cyl_side_hit.c \
				$(SRC_DIR)/shape/pl_hit.c \
				$(SRC_DIR)/shape/sphere_hit.c \
				$(SRC_DIR)/task_thread/task.c \
				$(SRC_DIR)/task_thread/thread.c \
				$(SRC_DIR)/window_title/string.c \
				$(SRC_DIR)/window_title/title.c \
				$(SRC_DIR)/delete.c \
				$(SRC_DIR)/getters.c \
				$(SRC_DIR)/init.c \
				$(SRC_DIR)/main.c

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

bonus: CFLAGS += -D MINIRT_BONUS -D _GNU_SOURCE
bonus: $(NAME)

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
