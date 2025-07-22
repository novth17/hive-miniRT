
#include "../test_stuff/rt_math.h"
#include <cstdint>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>

#include "MLX42.h"

// these are placeholder limits for now
// should look at what numbers make sense later
#define MIN_HIT_DIST 0.001f
#define TOLERANCE	 0.0001f

/*

typedef struct
{
	float		aspect_ratio;
	uint32_t	samples_per_pixel; // how many rays per pixel
	uint32_t	max_bounce; // 1 or 2 for mandatory
	t_v3		view_point; // where we are looking from
	t_v3		direction; // where we are looking towards;
	float		fov; //
} t_camera;


typedef struct s_image
{
	uint32_t width;
	uint32_t height;
	uint32_t *pixels;
} t_image;

typedef struct plane
{
	t_v3	normal;
	float	d;
	t_v3 	color;
} t_plane;

typedef struct
{
	t_plane *arr;
	uint32_t count;
} t_planes;

typedef struct
{
	t_v3	position;
	float	radius;
	t_v3 	color;
} t_sphere;

typedef struct
{
	t_sphere *arr;
	uint32_t count;
} t_spheres;

typedef struct
{
	t_v3 a;
	t_v3 b;
	float radius;
	//@TODO implement;
} t_cylinder;

typedef struct
{
	t_cylinder *arr;
	uint32_t count;
} t_cylinders;

typedef struct s_context
{
	t_camera	*cam;
	t_planes	*planes;
	t_spheres	*spheres;
	t_cylinders	*cylinders;
	mlx_image_t *image;
} t_context;

typedef struct
{
	bool  did_hit;
	float distance;
	t_v3  point;
	t_v3  normal;
} t_hit;

typedef struct
{
	t_v3 origin;
	t_v3 direction;
} t_ray;

*/


// everything under this is definetely going to be re done somewhere else

#pragma pack(push, 1)
typedef struct bitmap_header
{
	//header header
	uint16_t file_type;
	uint32_t file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t bitmap_offset;
	//info_header
	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t color_planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t size_of_bitmap;
	int32_t horizontal_ppm;
	int32_t vertical_ppm;
	uint32_t colors_used;
	uint32_t colors_important;
} bitmap_header;
#pragma pack(pop)

typedef struct image_u32
{
	uint32_t width;
	uint32_t height;
	uint32_t *pixels;
} image_u32;

static
uint32_t get_total_pixel_size(image_u32 image)
{
	uint32_t result = image.width * image.height * sizeof(*image.pixels);
	return (result);
}

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static
image_u32 allocate_image(uint32_t width, uint32_t height)
{
	image_u32 image = {};
	image.width = width;
	image.height = height;

	uint32_t  output_pixel_size = get_total_pixel_size(image);
	image.pixels = (uint32_t *)malloc(output_pixel_size);
	if (image.pixels == NULL)
		perror("allocate_image:");
	return (image);
}

static
void write_image(image_u32 image, char *filename)
{
	uint32_t output_pixel_size = get_total_pixel_size(image);

	bitmap_header header = {};
	header.file_type = 0x4D42;
	header.file_size = sizeof(header) + output_pixel_size;
	header.bitmap_offset = sizeof(header);
	header.size = sizeof(header) - 14; // full header - sizeof 1st 5 elements
	header.width = image.width;
	header.height =  image.height;
	header.color_planes = 1;
	header.bits_per_pixel = 32;
	header.compression = 0;
	header.size_of_bitmap = output_pixel_size;
	header.horizontal_ppm = 0;
	header.vertical_ppm = 0;
	header.colors_used = 0;
	header.colors_important = 0;

	int fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd != -1)
	{
		write(fd, &header, sizeof(header));
		write(fd, image.pixels, output_pixel_size);
		close(fd);
	}
}



static float ExactLinearTosRGB(float L);


typedef struct
{
	float	aspect_ratio; //
	int32_t image_width;
	int32_t image_height;

	float focal_length;
	float viewport_height;
	float viewport_width;
	t_v3 camera_center;

	t_v3 viewport_u;
	t_v3 viewport_v;

	t_v3 pixel_delta_u;
	t_v3 pixel_delta_v;

	t_v3 viewport_upper_left;
	t_v3 pixel00_loc;
} t_camera;

#define IMAGE_WIDTH 1280.0f
#define IMAGE_HEIGHT 720.0f

#define v3_sub v3_sub_v3

static inline
t_v3	viewport_top_left(const t_v3 camera_center, const t_v3 viewport_u, const t_v3 viewport_v, const float focal_length)
{
	t_v3	result;

	result = v3_sub(camera_center, v3(0, 0, focal_length));
	result = v3_sub(result, v3_div_f32(viewport_u, 2.0f));
	result = v3_sub(result, v3_div_f32(viewport_v, 2.0f));
	return (result);
}

static inline
t_v3	pixel00_location(const t_v3 viewport_upper_left, const t_v3 pixel_delta_u, const t_v3 pixel_delta_v)
{
	t_v3	result;

	result = v3_add_v3(pixel_delta_u, pixel_delta_v);
	result = f32_mul_v3(0.5, result);
	result = v3_add_v3(viewport_upper_left, result);
	return (result);
}


typedef struct
{
	t_v3 origin;
	t_v3 direction;
} t_ray;

static inline
t_ray init_ray(t_camera *cam, int32_t x, int32_t y)
{

}

static inline
t_camera init_cam( void )
{
	t_camera cam;

	cam.aspect_ratio = IMAGE_WIDTH / IMAGE_HEIGHT;
	cam.image_width = IMAGE_WIDTH;
	cam.image_height = (int32_t)(cam.image_width / cam.aspect_ratio);
	if (cam.image_height < 1)
		cam.image_height = 1;

	cam.focal_length = 1.0f;
	cam.viewport_height = 2.0f;
	cam.viewport_width = cam.viewport_height * ((float)(cam.image_width) / cam.image_height);
	cam.camera_center = v3(0, 0, 0);

	cam.viewport_u = v3(cam.viewport_width, 0, 0);
	cam.viewport_v = v3(0, -cam.viewport_height, 0);

	cam.pixel_delta_u = v3_div_f32(cam.viewport_u, (float)cam.image_width);
	cam.pixel_delta_v = v3_div_f32(cam.viewport_v, (float)cam.image_height);

	// t_v3 viewport_upper_left = v3_sub_v3(camera_center, (v3_sub_v3(v3_sub_v3(v3(0, 0, focal_length), v3_div_f32(viewport_u, 2.0f)), v3_div_f32(viewport_v, 2.0f))));
	// t_v3 viewport_upper_left = v3_sub_v3(camera_center, v3(0, 0, focal_length));
	// viewport_upper_left = v3_sub(viewport_upper_left, )
	cam.viewport_upper_left = viewport_top_left(cam.camera_center, cam.viewport_u, cam.viewport_v, cam.focal_length);

	// t_v3 pixel00_loc = v3_add_v3(viewport_upper_left, f32_mul_v3(0.5f, (v3_add_v3(pixel_delta_u, pixel_delta_v))));
	cam.pixel00_loc = pixel00_location(cam.viewport_upper_left, cam.pixel_delta_u, cam.pixel_delta_v);
}

int main(void)
{

	float	aspect_ratio = IMAGE_WIDTH / IMAGE_HEIGHT;
	int32_t image_width = IMAGE_WIDTH;
	int32_t image_height = (int32_t)(image_width / aspect_ratio);
	if (image_height < 1)
		image_height = 1;

	float focal_length = 1.0f;
	float viewport_height = 2.0f;
	float viewport_width = viewport_height * ((float)(image_width) / image_height);
	t_v3 camera_center = v3(0, 0, 0);

	t_v3 viewport_u = v3(viewport_width, 0, 0);
	t_v3 viewport_v = v3(0, -viewport_height, 0);

	t_v3 pixel_delta_u = v3_div_f32(viewport_u, (float)image_width);
	t_v3 pixel_delta_v = v3_div_f32(viewport_v, (float)image_height);

	// t_v3 viewport_upper_left = v3_sub_v3(camera_center, (v3_sub_v3(v3_sub_v3(v3(0, 0, focal_length), v3_div_f32(viewport_u, 2.0f)), v3_div_f32(viewport_v, 2.0f))));
	// t_v3 viewport_upper_left = v3_sub_v3(camera_center, v3(0, 0, focal_length));
	// viewport_upper_left = v3_sub(viewport_upper_left, )
	t_v3 viewport_upper_left = viewport_top_left(camera_center, viewport_u, viewport_v, focal_length);

	// t_v3 pixel00_loc = v3_add_v3(viewport_upper_left, f32_mul_v3(0.5f, (v3_add_v3(pixel_delta_u, pixel_delta_v))));
	t_v3 pixel00_loc = pixel00_location(viewport_upper_left, pixel_delta_u, pixel_delta_v);

	image_u32 image;


	t_camera cam;

	cam = init_cam();

	int32_t y = 0;
	while (y < image_height)
	{
		int32_t x = 0;
		while (x < image_width)
		{
			t_v3 pixel_center =
			t_ray ray;
			ray.origin =
		}
	}



}

static
float ExactLinearTosRGB(float L)
{
	float S;

	if (L < 0.0f)
	{
		L = 0.0f;
	}

	if (L > 1.0f)
	{
		L = 1.0f;
	}

	S = L * 12.92;
	if (L > 0.0031308)
	{
		S = 1.055F*pow(L, 1.0f/2.4f) - 0.055f;
	}
	return (S);
}
