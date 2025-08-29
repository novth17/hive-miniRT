#ifndef THREAD_H
# define THREAD_H
# include "mini_rt.h" //wtf why need here?

# ifdef MINIRT_BONUS

bool	create_worker_threads(t_minirt *minirt);

# endif

int		get_core_count(void);

#endif
