/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:57:32 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/25 20:10:03 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
#define ERRORS_H

#define RED     "\033[31m"
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"

#define ERROR_COLOR 	RED "Invalid color format (commas and [0,255])" RESET
#define ERROR_COORD 	RED "Invalid coordinates for vector" RESET
#define ERROR_FLOAT 	RED "Invalid float" RESET
#define ERROR_NORM  	RED "Component must be in range [-1,1]" RESET
#define ERROR_MALLOC	RED "Malloc failed" RESET
#define	UNK_ELEMENT		RED "Unknown element of element id:" RESET


#endif
