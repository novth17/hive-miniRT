#include "mini_rt.h"

void	set_title(t_minirt *minirt)
{
	static char	title_base[100] = "MiniRay";
	int			status;
	t_string	title;

	title.buf = title_base;
	title.size = sizeof(title_base);
	title.len = sizeof("MiniRay") - 1;
	status = 0;
	status += cat_cstring_to_string(&title, " -- Frame Time: ");
	status += cat_uint_to_str(&title, round(minirt->mlx->delta_time * 1000));
	status += cat_cstring_to_string(&title, " -- Samples: ");
	status += cat_uint_to_str(&title, minirt->scene.camera.samples_per_pixel);
	status += cat_cstring_to_string(&title, " -- Max Bounces: ");
	status += cat_uint_to_str(&title, minirt->scene.camera.max_bounce);
	if (status)
	{
		ft_dprintf(2, "Failed to create title\n"
			"buf_size <%u> len <%u> data <%s>\n",
			title.size, title.len, title.buf);
		mlx_set_window_title(minirt->mlx, "MiniRay -- Info not available");
		return ;
	}
	mlx_set_window_title(minirt->mlx, title.buf);
}
