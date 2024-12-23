NAME = libft.a

SRC = 

OBJS = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror

CC = cc

# **************************************************************************** #
# char                                                                         #
# **************************************************************************** #
SRC += ft_isalpha.c
SRC += ft_isdigit.c
SRC += ft_isalnum.c
SRC += ft_isascii.c
SRC += ft_isprint.c
SRC += ft_toupper.c
SRC += ft_tolower.c
# **************************************************************************** #
# str                                                                          #
# **************************************************************************** #
SRC += ft_strlen.c
SRC += ft_strlcat.c
SRC += ft_strlcpy.c
SRC += ft_strdup.c
SRC += ft_strchr.c
SRC += ft_strrchr.c
SRC += ft_strjoin.c
SRC += ft_strtrim.c
SRC += ft_strnstr.c
SRC += ft_substr.c
SRC += ft_strmapi.c
SRC += ft_strncmp.c
SRC += ft_striteri.c
# **************************************************************************** #
# mem                                                                          #
# **************************************************************************** #
SRC += ft_memcmp.c
SRC += ft_bzero.c
SRC += ft_calloc.c
SRC += ft_memset.c
SRC += ft_memcpy.c
SRC += ft_memchr.c
SRC += ft_memmove.c
# **************************************************************************** #
# conversions                                                                  #
# **************************************************************************** #
SRC += ft_itoa.c
SRC += ft_atoi.c
# **************************************************************************** #
# put                                                                          #
# **************************************************************************** #
SRC += ft_putnbr_fd.c
SRC += ft_putchar_fd.c
SRC += ft_putstr_fd.c
SRC += ft_putendl_fd.c
# **************************************************************************** #
# other                                                                        #
# **************************************************************************** #
SRC += ft_split.c



all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re