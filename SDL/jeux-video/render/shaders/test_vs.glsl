#version 430 

layout (location = 0) in vec3 pos;

out vec2 vTexCoord;
// attribute vec4 VPosition;

uniform mat4 uProjectionMatrix; // The ortho stuff, dont touch this
uniform mat4 uViewMatrix; // This changes the camera's position
// uniform mat4 umodelMatrix;

void main()
{
  gl_Position = uProjectionMatrix * uViewMatrix * vec4(pos, 1.0);
};