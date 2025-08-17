
#include "mini_rt.h"

static void	print_guide_how_to(void)
{
	ft_putstr_fd(
		"\n📌 Favorite Scenes 😘💋\n"
		"   ─────────────────────────────────────────────────────────────\n"
		"   🔹 room_test.rt\n"
		"   🔹 candy_garden_reflect.rt\n"
		"   🔹 smile.rt\n"
		"   ─────────────────────────────────────────────────────────────\n\n"

		"🎮 Controls:\n"
		"   ─────────────────────────────────────────────────────────────\n"
		"   🏁 ESC              ➜ Exit program\n"
		"   🔆 ↑ / ↓            ➜ Brightness + / - (if light available)\n"
		"   💡 L                ➜ Toggle point light\n"
		"   📸 P                ➜ Save image to file\n\n"

		"   🕹️ Camera Movement:\n"
		"   W / S              ➜ Forward / Backward\n"
		"   A / D              ➜ Left / Right\n"
		"   E / Q              ➜ Up / Down\n\n"

		"   🔍 Zoom & Focus:\n"
		"   Scroll             ➜ Zoom in/out (FOV)\n"
		"   Shift + Scroll     ➜ Adjust focus distance (DoF)\n"
		"   Ctrl + Scroll      ➜ Adjust blur strength\n\n"

		"   🔄 Rendering Options:\n"
		"   PageUp / PageDown  ➜ Max bounces + / -\n"
		"   - / =              ➜ Samples per pixel - / +\n\n"
		"   ─────────────────────────────────────────────────────────────\n\n",
		1);
}

void	print_guide(void)
{
	ft_putstr_fd(
		"****************************************************************\n"
		"               		MINI-RT USAGE GUIDE              			\n"
		"****************************************************************\n\n"
		"🚀 How to Run:\n"
		"   ➜ ./miniRT <scene_name.rt>\n\n",
		1);
	print_guide_how_to();
}
