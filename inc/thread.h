/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:29:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 17:17:11 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_H
# define THREAD_H

# ifdef MINIRT_BONUS

#  include <stdatomic.h>
#  include <pthread.h>
#  include <sched.h>
#  include "mini_rt.h"

bool	create_worker_threads(t_minirt *minirt);

# endif

int		get_core_count(void);

#endif
