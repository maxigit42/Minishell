NAME = minishell

# Directorios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
LIBFT_DIR = libft

# Archivos fuente (todos los .c en src/ y subcarpetas directas)
SRCS = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)

# Archivos objeto (ej: src/signals/signal.c → obj/signals/signal.o)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Includes y libft
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)/libft.a

# Compilador y flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g -O1

# Regla principal
all: $(LIBFT) $(NAME)

# Compilar el ejecutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

# Regla para compilar .c → .o, y crear carpeta si no existe
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilar libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Limpiar objetos
clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

# Limpiar todo
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

# Recompilar todo
re: fclean all

.PHONY: all clean fclean re