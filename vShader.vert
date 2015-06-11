#version 440

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 v3_vertex;
layout(location = 1) in vec2 v2_vertexUV;
layout(location = 2) in vec3 v3_normal;

out vec2 v2_UV;
out vec3 v3_normal_camera;
out vec3 v3_vertex_world;
out vec3 v3_eyeDirection_camera;
out vec3 v3_vertexToLightDirection_camera;

uniform sampler2D Chair_Texture;
uniform mat4 m4_MVP;
uniform mat4 m4_M;
uniform mat4 m4_V;
uniform vec3 v3_light_world;

void main(){

	// Output position of the vertices
	gl_Position =  m4_MVP * vec4(v3_vertex,1) ;

	v3_vertex_world = (m4_M * vec4(v3_vertex, 1)).xyz;

	vec3 v3_vertex_camera = (m4_V * vec4(v3_vertex_world,1)).xyz;

	v3_eyeDirection_camera = -1.f * v3_vertex_camera;

	vec3 v3_light_camera = (m4_V * vec4(v3_light_world, 1)).xyz;
	v3_vertexToLightDirection_camera = v3_light_camera - v3_vertex_camera;

	v3_normal_camera = (m4_V * m4_M * vec4(v3_normal,1)).xyz;


	v2_UV = v2_vertexUV;
}

