/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 19:05:18 by tmatis            #+#    #+#             */
/*   Updated: 2021/02/11 13:42:46 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_map
{
	int		width;
	int		height;
	char	c;
	char	**s;
}				t_map;

typedef	struct s_rect
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	c;
}				t_rect;

int			ft_isin(float x, float y, t_rect rect, int border);
int			ft_isborder(float x, float y, t_rect rect);

static	int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static	void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void	ft_freemap(t_map *map)
{
	int	y;

	y = 0;
	while (y < map->height)
	{
		free(map->s[y]);
		y++;
	}
	free(map->s);
}

void	ft_display_map(t_map *map)
{
	int	y;

	y = 0;
	while (y < map->height)
	{
		write(1 , map->s[y], map->width);
		write(1, "\n", 1);
		y++;
	}
}

int		ft_gethead(FILE *file, t_map *map)
{
	int ret;
	int	y;
	int	x;

	ret = fscanf(file, "%i %i %c\n", &map->width, &map->height, &map->c);
	if (ret == 3 && map->width > 0 && map->height > 0 && map->width <= 300 && map->height <= 300)
	{
		map->s = malloc((map->height) * sizeof (char *));
		if (!map->s)
			return (1);
		y = 0;
		while (y < map->height)
		{
			map->s[y] = malloc(map->width * sizeof (char));
			x = 0;
			while (x < map->width)
			{
				map->s[y][x] = map->c;
				x++;
			}
			y++;
		}
		return (0);
	}
	return (1);
}

int			ft_isborder(float x, float y, t_rect rect)
{
	int	r;

	r = 0;
	r += ft_isin(x - 1, y, rect, 0);
	r += ft_isin(x + 1, y, rect, 0);
	r += ft_isin(x, y + 1, rect, 0);
	r += ft_isin(x, y - 1, rect, 0);
	return (r != 4);
}

int			ft_isin(float x, float y, t_rect rect, int border)
{
	if ((x < rect.x) || (y < rect.y) || (y > rect.y + rect.height) || (x > rect.x + rect.width))
		return (0);
	else
	{
		if (border && ft_isborder(x, y, rect))
			return (2);
		else
			return (1);
	}
}

void		ft_handle_char(t_map *map, t_rect rect, float x, float y)
{
	int	r;
	if ((r = ft_isin(x, y, rect, 1)))
	{
		if ((rect.type == 'R') || (r == 2))
			map->s[(int)y][(int)x] = rect.c;
	}
}

int		ft_add_square(t_map *map, t_rect rect)
{
	int	y;
	int	x;

	if (rect.width <= 0.0 || rect.height <= 0.0)
		return (1);
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			ft_handle_char(map, rect, (float)x, (float)y);
			x++;
		}
		y++;
	}
	return (0);
}

int		exec(FILE *file)
{
	t_map	map;
	int		scan;
	t_rect	rect;

	if (ft_gethead(file, &map))
		return (1);
	scan = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.c);
	while (scan == 6)
	{
		if ((rect.type != 'r' && rect.type != 'R') || ft_add_square(&map, rect))
		{
			ft_freemap(&map);
			return (1);
		}
		scan = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.c);
	}
	if (scan == -1)
	{
		ft_display_map(&map);
		ft_freemap(&map);
		return (0);
	}
	else
	{
		ft_freemap(&map);
		return (1);
	}
}

int		main(int argc, char **argv)
{
	FILE	*file;

	(void) argv;
	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (!file || exec(file))
		{
			ft_putstr("Error: Operation file corrupted\n");
			fclose(file);
			return (1);
		}
		fclose(file);
	}
	else
		ft_putstr("Error: argument\n");
	return (0);
}
