SRCS_DIR	= .
SRCS_LIST	= philo.c
SRCS		= $(addprefix $(SRCS_DIR)/, $(SRCS_LIST))

HEADER_DIR	= .
HEADER_LIST	= philo.h
HEADER		= $(addprefix $(HEADER_DIR)/, $(HEADER_LIST))

OBJS_DIR	= ./obj
OBJS_LIST	= $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

CFLAGS		= -Wall -Wextra -Werror -g3 -fsanitize=thread
CC			= cc

NAME		= philo


all	: $(NAME)

$(OBJS_DIR)	:
	mkdir -p $@

$(OBJS_DIR)/%.o	: $(SRCS_DIR)/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -I $(HEADER) -o $@

$(NAME)	: $(OBJS_LIST) $(HEADER)
	$(CC) $(CFLAGS) $(OBJS_LIST) -o $@ -pthread

clean	:
	rm -rf $(OBJS_DIR)

fclean	: clean
	rm -rf $(NAME)

re	: fclean all

.PHONY : all clean fclean re
