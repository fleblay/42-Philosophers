SRCS_DIR	= ./
SRCS_LIST	= philo.c
SRCS		= $(addprefix $(SRCS_DIR), $(SRCS_LIST))

HEADER_DIR	= ./
HEADER_LIST	= philo.h
HEADER		= $(addprefix $(HEADER_DIR), $(HEADER_LIST))

OBJS_DIR	= ./obj/
OBJS_LIST	= $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

CFLAGS		= -Wall -Wextra -Werror -g3 -fsanitize=address
CC			= cc

NAME		= philo


all	: $(NAME)

$(OBJS_DIR)%.o	: $(SRCS_DIR)%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -I $(HEADER) -o $@

$(OBJS_DIR)	:
	mkdir $@

$(NAME)	: $(OBJS_LIST) $(HEADER)
	$(CC) $(CFLAGS) $(OBJS_LIST) -o $@

clean	:
	rm -rf philo

fclean	: clean
	rm -rf $(OBJS_DIR)

re	: fclean all

.PHONY : all clean fclean re
