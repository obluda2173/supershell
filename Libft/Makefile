NAME = libft.a

SRC = 

BONUS = 

OBJS = $(SRC:.c=.o)

OBJS_B = $(BONUS:.c=.o)

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
SRC += ft_strndup.c
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
# **************************************************************************** #
# bonus                                                                        #
# **************************************************************************** #
BONUS += ft_lstlast.c
BONUS += ft_lstnew.c
BONUS += ft_lstmap.c
BONUS += ft_lstsize.c
BONUS += ft_lstadd_back.c
BONUS += ft_lstadd_front.c
BONUS += ft_lstiter.c
BONUS += ft_lstdelone.c
BONUS += ft_lstclear.c

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

bonus: $(OBJS) $(OBJS_B)
	ar rc $(NAME) $(OBJS) $(OBJS_B)
	ranlib $(NAME)


clean:
	rm -f $(OBJS) $(OBJS_B)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all bonus clean fclean re