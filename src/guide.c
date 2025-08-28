/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guide.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:23:42 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:59:31 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	print_guide_how_to(void)
{
	ft_putstr_fd(
		"🎮 Controls:\n"
		"   ─────────────────────────────────────────────────────────────\n"
		"   🏁 ESC              ➜ Exit program\n"
		"   🔆 ↑ / ↓            ➜ Brightness + / - (if light available)\n"
		"   💡 L                ➜ Toggle point light\n"
		"   📸 P                ➜ Save image to file\n\n"
		"   🕹️  Camera Movement:\n"
		"   W / S               ➜ Forward / Backward\n"
		"   A / D               ➜ Left / Right\n"
		"   E / Q               ➜ Up / Down\n"
		"   ← / →               ➜ Slower / Faster movement\n"
		"   R                   ➜ Reset to original camera position\n\n"
		"   🔍 Zoom & Focus:\n"
		"   Scroll              ➜ Zoom in/out (FOV)\n"
		"   Shift + Scroll      ➜ Adjust focus distance (DoF)\n"
		"   Ctrl + Scroll       ➜ Adjust blur strength\n\n"
		"   🔄 Rendering Options:\n"
		"   PageUp / PageDown   ➜ Max bounces + / -\n"
		"   - / =               ➜ Samples per pixel - / +\n\n"
		"   ─────────────────────────────────────────────────────────────\n\n",
		1);
}

void	print_guide(void)
{
	ft_putstr_fd(
		"****************************************************************\n"
		"               	     MINI-RT USAGE GUIDE                    \n"
		"****************************************************************\n\n"
		"🚀 How to Run:\n"
		"   ➜ ./miniRT <scene_name.rt>\n\n"
		"\n📌 Favorite Scenes 😘💋\n"
		"   ─────────────────────────────────────────────────────────────\n"
		"   🔹 mirror_room.rt\n"
		"   🔹 candy_garden_reflect.rt\n"
		"   🔹 smile.rt\n"
		"   🔹 lollipops.rt\n"
		"   🔹 ladder.rt\n"
		"   ─────────────────────────────────────────────────────────────\n\n",
		1);
	print_guide_how_to();
}
