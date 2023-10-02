#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;
layout(rgba32f, binding = 1) uniform image2D randomImg;
// layout(rgba32f, binding = 2) uniform image2D randomImg;
// layout(rgba32f, binding = 3) uniform image2D randomImg;

void main()
{

    vec4 pixel = vec4(0.0, 0.0, 1.0, 1.0);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    vec4 pixel2 = imageLoad(randomImg, pixel_coords);

    pixel.x = pixel2.x;
    pixel.y = pixel2.y;
    pixel.z = pixel2.z;

    
	imageStore(screen, pixel_coords, pixel);
}
