/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 17:11:56 by tmatis            #+#    #+#             */
/*   Updated: 2021/02/11 18:22:01 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_map
{
	int	width;
	int	height;
	char c;
	char **matrice;
}				t_map;

typedef	struct	s_circle
{
	char	c;
	float	x;
	float	y;
	float	radius;
	char	color;
}	t_circle;

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(1, str, i);
}

void	ft_free_matrice(t_map *map)
{
	int	y;

	y = 0;
	while (y < map->height)
	{
		free(map->matrice[y]);
		y++;
	}
	free(map->matrice);
}

int		ft_get_head(FILE *file, t_map *map)
{
	int	ret;
	int	y;
	int	x;

	ret = fscanf(file, "%i %i %c\n", &map->width, &map->height, &map->c);
	if (ret == 3 && map->width > 0 && map->height > 0
			&& map->width <= 300 && map->height <= 300)
	{
		map->matrice = malloc(map->height * sizeof (char *));
		if (!map->matrice)
			return (1);
		y = 0;
		while (y < map->height)
		{
			map->matrice[y] = malloc(map->width * sizeof(char));
			x = 0;
			while (x < map->width)
			{
				map->matrice[y][x] = map->c;
				x++;
			}
			y++;
		}
		return (0);
	}
	return (1);
}

void	ft_display_mat(t_map *map)
{
	int	y;

	y = 0;
	while (y < map->height)
	{
		write(1, map->matrice[y], map->width);
		write(1, "\n", 1);
		y++;
	}
}

int		ft_iscirle(t_circle rect, float x, float y)
{
	float	d2;
	float	d;

	d2 = (powf((x - rect.x), 2) + powf((y - rect.y), 2));
	d = sqrtf(d2);
	if (d <= rect.radius)
	{
		if (d - rect.radius > -1.0)
			return (2);
		else
			return (1);
	}
	else
		return (0);
}

void	ft_handle_pix(t_map *map, t_circle rect, float x, float y)
{
	int	ret;

	ret = ft_iscirle(rect, x, y);
	if ((ret && rect.c == 'C') || (ret == 2))
		map->matrice[(int)y][(int)x] = rect.color;
}

void	ft_add_circle(t_map *map, t_circle rect)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			ft_handle_pix(map, rect, (float)x, (float)y);
			x++;
		}
		y++;
	}
}

int		exec(FILE *file, t_map *map)
{
	int		ret;
	t_circle	rect;
	if (ft_get_head(file, map))
		return (1);
	ret = fscanf(file, "%c %f %f %f %c\n", &rect.c, &rect.x, &rect.y, &rect.radius, &rect.color);
	while (ret == 5)
	{
		if (rect.c != 'c' && rect.c != 'C')
		{
			ft_free_matrice(map);
			return (1);
		}
		ft_add_circle(map, rect);
		ret = fscanf(file, "%c %f %f %f %c\n", &rect.c, &rect.x, &rect.y, &rect.radius, &rect.color);
	}
	if (ret == -1)
	{
		ft_display_mat(map);
		ft_free_matrice(map);
		return (0);
	}
	else
	{
		ft_free_matrice(map);
		return (1);
	}
}

int		main(int argc, char **argv)
{
	FILE	*file;
	t_map	map;
	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (!file)
		{
			ft_putstr("Error: Operation file corrupted\n");
			return (1);
		}
		if (exec(file, &map))
		{
			fclose(file);
			ft_putstr("Error: Operation file corrupted\n");
			return (1);
		}
		fclose(file);
	}
	else
	{
		ft_putstr("Error: argument\n");
		return (1);
	}
}
