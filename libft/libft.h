/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:26:34 by erian             #+#    #+#             */
/*   Updated: 2025/01/05 17:09:47 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_dllist
{
	void			*content;
	struct s_dllist	*next;
	struct s_dllist	*prev;
}					t_dllist;

/* ************************************************************************** */
/* char                                                                     * */
/* ************************************************************************** */
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);
/* ************************************************************************** */
/* str                                                                      * */
/* ************************************************************************** */
size_t				ft_strlen(const char *str);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
char				*ft_strdup(const char *src);
char				*ft_strndup(const char *s, size_t n);
char				*ft_strchr(const char *str, int chr);
char				*ft_strrchr(const char *str, int chr);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strnstr(const char *s1, const char *s2, size_t len);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
/* ************************************************************************** */
/* mem                                                                      * */
/* ************************************************************************** */
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t count, size_t size);
void				*ft_memset(void *str, int c, size_t len);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memchr(const void *str, int chr, size_t nbr);
void				*ft_memmove(void *dst, const void *src, size_t len);
/* ************************************************************************** */
/* conversions                                                              * */
/* ************************************************************************** */
char				*ft_itoa(int n);
int					ft_atoi(const char *str);
/* ************************************************************************** */
/* put                                                                      * */
/* ************************************************************************** */
void				ft_putnbr_fd(int n, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
/* ************************************************************************** */
/* other                                                                    * */
/* ************************************************************************** */
char				**ft_split(char const *s, char c);
bool				ft_isspace(char c);
/* ************************************************************************** */
/* bonus                                                                    * */
/* ************************************************************************** */
t_list				*ft_lstlast(t_list *lst);
t_list				*ft_lstnew(void *content);
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
int					ft_lstsize(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new_node);
void				ft_lstadd_front(t_list **lst, t_list *new_node);
void				ft_lstiter(t_list *lst, void (*f)(void *));
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
/* ************************************************************************** */
/* double linked list
 * */
/* ************************************************************************** */
t_dllist			*ft_dllstfirst(t_dllist *lst);
t_dllist			*ft_dllstlast(t_dllist *lst);
t_dllist			*ft_dllstnew(void *content);
t_dllist			*ft_dllstmap(t_dllist *lst, void *(*f)(void *),
						void (*del)(void *));
int					ft_dllstsize(t_dllist *lst);
void				ft_dllstadd_back(t_dllist **lst, t_dllist *new_node);
void				ft_dllstadd_front(t_dllist **lst, t_dllist *new_node);
void				ft_dllstiter(t_dllist *lst, void (*f)(void *));
void				ft_dllstdelone(t_dllist *lst, void (*del)(void *));
void				ft_dllstclear(t_dllist **lst, void (*del)(void *));
#endif
