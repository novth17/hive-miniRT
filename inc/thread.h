/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:29:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 17:02:38 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_H
# define THREAD_H
# include "mini_rt.h"

# ifdef MINIRT_BONUS

bool	create_worker_threads(t_minirt *minirt);

# endif

int		get_core_count(void);

#endif
