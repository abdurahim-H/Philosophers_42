NAME = philo
CC = cc -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
CFLAGS = -Wall -Wextra -Werror
SRC_DIR = src/
INC_DIR = include/
OBJ_DIR = obj/

# Define colors
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
MAGENTA = \033[1;35m
CYAN = \033[1;36m
ORANGE = \033[1;33m
RESET = \033[0m

SRCS = $(wildcard $(SRC_DIR)*.c)
OBJS = $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)Starting linking process...$(RESET)"
	@echo "$(YELLOW)Linking object files...$(RESET)"
	$(CC) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Linking done. Executable created: $(NAME)$(RESET)"
	@echo "$(CYAN)Linking process finished.$(RESET)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo "$(CYAN)Starting compilation process for $<...$(RESET)"
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
	@echo "$(BLUE)Compiled $< to $@.$(RESET)"
	@echo "$(CYAN)Compilation process for $< finished.$(RESET)"

clean:
	@echo "$(CYAN)Starting cleaning process...$(RESET)"
	@echo "$(MAGENTA)Cleaning object files...$(RESET)"
	@echo "$(ORANGE)"
	rm -rf $(OBJ_DIR)
	@echo "$(RESET)"
	@echo "$(RED)Cleaning done.$(RESET)"
	@echo "$(CYAN)Cleaning process finished.$(RESET)"

fclean: clean
	@echo "$(CYAN)Starting full cleaning process...$(RESET)"
	@echo "$(MAGENTA)Full cleaning...$(RESET)"
	@echo "$(ORANGE)"
	rm -f $(NAME)
	@echo "$(RESET)"
	@echo "$(RED)Full cleaning done. Removed executable: $(NAME)$(RESET)"
	@echo "$(CYAN)Full cleaning process finished.$(RESET)"

re: fclean all

.PHONY: all clean fclean re