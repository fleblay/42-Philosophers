SRCS_DIR	= .
SRCS_LIST	= philo.c ft_satoi.c ft_putstr_fd.c init_data.c destroy_data.c\
			launch_philo.c philo_routine.c time_utils.c print_utils.c meal.c\
			fork_utils.c philo_routine_utils.c


SRCS		= $(addprefix $(SRCS_DIR)/, $(SRCS_LIST))

HEADER_DIR	= .
HEADER_LIST	= philo.h
HEADER		= $(addprefix $(HEADER_DIR)/, $(HEADER_LIST))

OBJS_DIR	= ./obj
OBJS_LIST	= $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

#CFLAGS		= -Wall -Wextra -Werror -g3 -fsanitize=thread
CFLAGS		= -Wall -Wextra -Werror -g3

ifeq ($(MAKECMDGOALS), debug)
CFLAGS		+= -D DEBUG=1
endif

CC			= gcc
NAME		= philo_bonus

all	: $(NAME)
debug : $(NAME)

$(OBJS_DIR)	:
	mkdir -p $@

$(OBJS_DIR)/%.o	: $(SRCS_DIR)/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -I $(HEADER_DIR) -pthread -o $@

$(NAME)	: $(OBJS_LIST) $(HEADER)
	$(CC) $(CFLAGS) $(OBJS_LIST) -o $@ -pthread

clean	:
	rm -rf $(OBJS_DIR)

fclean	: clean
	rm -rf $(NAME)

re	: fclean all

.PHONY : all clean fclean re
