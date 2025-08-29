/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:29:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 16:29:27 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_H
# define THREAD_H
# include "mini_rt.h" //wtf why need here?

# ifdef MINIRT_BONUS

bool	create_worker_threads(t_minirt *minirt);

# endif

int		get_core_count(void);

#endif
