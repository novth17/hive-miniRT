/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 23:08:46 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 23:08:48 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define RED     "\033[31m"
# define RESET   "\033[0m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BOLD    "\033[1m"

# define ERROR_COLOR "Invalid color format (commas and [0,255])"
# define ERROR_COORD "Invalid coordinates for vector"
# define ERROR_FLOAT "Invalid float"
# define ERROR_IN_RANGE "Component must be in range [-1,1]"
# define ERROR_IN_RANGE2 "Component must be in range [0,1]"
# define ERROR_FOV "FOV: not in range [0,180]"
# define ERROR_NORM "Not a normalized vector"
# define ERROR_EMPTY "File is empty or no camera detected!"
# define ERROR_NOT_EXIST "No such file exists"
# define ERROR_MALLOC "Malloc failed"
# define UNK_ELEMENT "Unknown element of element id:"

#endif
