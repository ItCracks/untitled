#version 440

in vec2 v2_UV;
in vec3 v3_normal_camera;
in vec3 v3_vertex_world;
in vec3 v3_eyeDirection_camera;
in vec3 v3_vertexToLightDirection_camera;

out vec3 color;

uniform sampler2D Chair_Texture;
uniform mat4 m4_MVP;
uniform mat4 m4_M;
uniform mat4 m4_V;
uniform vec3 v3_light_world;

void main()
{

    /******************* Lighting *******************/

    // These variables should probably better be made uniform in main.cpp.
    vec3 v3_lightColour = vec3(.5, .3, .3);
    float lightPower = 15000.f;
    float materialAmbientCoefficient = .2f;

    vec3 v3_materialDiffusionColour = texture2D(Chair_Texture, v2_UV).rgb;
    vec3 v3_materialAmbientColour = vec3(materialAmbientCoefficient) * v3_materialDiffusionColour;
    vec3 v3_materialSpecularColour = vec3(.3f);

    //Distance from light to vertex, used to calculate light intensity / brightness of the surface.
    float distanceLightVertex = length(v3_light_world - v3_vertex_world);

    vec3 v3_normalizedVertexToLightDirection_camera = normalize(v3_vertexToLightDirection_camera);
    vec3 v3_normalizedNormal_camera = normalize(v3_normal_camera);

    float cos_Normal_VertexToLight = clamp(	dot(v3_normalizedNormal_camera,
						    v3_normalizedVertexToLightDirection_camera), 0, 1);

    vec3 v3_normalizedSightDirection_camera = normalize(v3_eyeDirection_camera);
    vec3 v3_reflectionDirection_camera = reflect    (-v3_normalizedVertexToLightDirection_camera,
						     v3_normalizedNormal_camera);

    float cos_Sight_LightReflectionPath = clamp(dot(v3_normalizedSightDirection_camera,
						    v3_reflectionDirection_camera), 0, 1);

    // Now, Colour = Ambient + Diffusion + Specular
    vec3 v3_finalColour_Diffusion =
	    v3_materialDiffusionColour * v3_lightColour * lightPower * cos_Normal_VertexToLight
	    / pow(distanceLightVertex, 2);

    vec3 v3_finalColour_Specular =
	    v3_materialSpecularColour * v3_lightColour * lightPower
	    * pow(cos_Sight_LightReflectionPath,5) / pow(distanceLightVertex, 2);

    color = v3_materialAmbientColour + v3_finalColour_Diffusion + v3_finalColour_Specular;

    /***************** End: Lighting *****************/

}
