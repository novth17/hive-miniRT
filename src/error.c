# include "mini_rt.h"

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
