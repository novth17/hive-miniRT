# include "mini_rt.h"

int validate_input(int argc, char *filename)
{
	size_t len;

	if (argc != 2)
	{
		ft_dprintf(2, "Error: Argument must be 1 as a file [fname].rt!\n");
		return (FAIL);
	}
	len = ft_strlen(filename);
	if (len <= 3)
	{
		ft_dprintf(2, "Error: Wrong file name format ");
		ft_dprintf(2,"| Usage: ./mini_rt [fname].rt\n");
		return (FAIL);
	}
	if (ft_strcmp(filename + len - 3, ".rt") != 0)
	{
		ft_dprintf(2, "Error: File must end with .rt ");
		ft_dprintf(2, "| Usage: ./mini_rt [fname].rt\n");
		return (FAIL);
	}
	return (SUCCESS);
}

bool is_in_range_vec3(t_vec3 vector)
{
	return (
		vector.x >= -1.0 && vector.x <= 1.0 &&
		vector.y >= -1.0 && vector.y <= 1.0 &&
		vector.z >= -1.0 && vector.z <= 1.0
	);
}

bool check_comma_and_move(char **str, bool *is_valid)
{
	if (!*is_valid || **str != ',')
	{
		*is_valid = false;
		return false;
	}
	(*str)++;
	return true;
}
