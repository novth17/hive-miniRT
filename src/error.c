# include "mini_rt.h"

static void print_usage(char **tokens)
{
	if (ft_strcmp(tokens[0], "cy") == 0)
	{
		ft_dprintf(2,GREEN"Usage: cy <center:x,y,z> <axis:x,y,z> <diameter> "
					"<height> <R,G,B> [<diffuse> <specular> <emitter>]\n"RESET);
	}
	else if (ft_strcmp(tokens[0], "sp") == 0)
	{
		ft_dprintf(2, GREEN"Usage: sp <center:x,y,z> <diameter> <R,G,B> "
					"[<diffuse> <specular> <emitter>]\n"RESET);
	}
	else if (ft_strcmp(tokens[0], "pl") == 0)
	{
		ft_dprintf(2, GREEN"Usage: pl <point:x,y,z> <normal:x,y,z> <R,G,B> "
					"[<diffuse> <specular> <emitter>]\n"RESET);
	}
}

void	exit_error(t_minirt *minirt,  char *msg)
{
	ft_dprintf(2, RED"Error\n%s\n", msg);
	delete_minirt(minirt);
	exit(FAIL);
}

int print_error(const char *msg, const char *value)
{
	if (value)
		ft_dprintf(2, RED"Error\n%s: %s\n", msg, value);
	else
		ft_dprintf(2, RED"Error\n%s\n", msg);
	return (FAIL);
}

int check_id_args_count(char **tokens, const char *id, int expected)
{
	if (ft_count_2d(tokens) != expected)
	{
		ft_dprintf(2, "%s: Number of arguments must be %d!\n", id, expected);
		return (FAIL);
	}
	return (SUCCESS);
}

int check_obj_args_count(char **tokens, const char *id, int min, int max)
{
	int count = ft_count_2d(tokens);

	if (count == max + 1 && ft_strcmp(tokens[max], "checker") == 0)
		return (SUCCESS);

	if (count < min || count > max)
	{
		ft_dprintf(2, RED"%s: Number of obj arguments must be between %d and %d, or %d with 'checker'!\n"RESET,
			id, min, max, max + 1);
		print_usage(tokens);
		return (FAIL);
	}
	return (SUCCESS);
}



