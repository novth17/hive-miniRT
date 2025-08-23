#ifndef THREAD_H
# define THREAD_H
# include "mini_rt.h"

int		get_core_count(void);
bool	create_worker_threads(t_minirt *minirt);

#endif
