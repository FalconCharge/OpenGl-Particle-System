
// Light 1 Info
uniform vec3 u_light1Pos;
uniform float u_light1Range;
uniform vec3 u_light1Color;
uniform vec3 u_light1Attenuation;

// Light 2 Info
uniform vec3 u_light2Pos;
uniform float u_light2Range;
uniform vec3 u_light2Color;
uniform vec3 u_light2Attenuation;

// Light 3 Info
uniform vec3 u_light3Pos;
uniform float u_light3Range;
uniform vec3 u_light3Color;
uniform vec3 u_light3Attenuation;

// Light 4 Info
uniform vec3 u_light4Pos;
uniform float u_light4Range;
uniform vec3 u_light4Color;
uniform vec3 u_light4Attenuation;

// Material properties
uniform sampler2D u_diffuseTex;     // Texture of the Object        NOTE SWAPPIGN THIS WITH COLOR FOR NOW
uniform vec4 color;
uniform vec3 u_specularColor;       // Color of specular
uniform float u_shininess;          // How shiny the object is

// Scene Properties
uniform vec3 u_viewPos;
uniform vec3 u_ambientLight;

// Varyings
in vec3 v_normal;  // Normal at the fragment
in vec4 v_pos;     // Position of the fragment
in vec2 v_uv1;     // Texture coordinates

out vec4 PixelColor;

// Caculates the amount of lighting from a single source
vec3 calc_lighting(vec3 lightPos, float lightRange, vec3 lightColor, vec3 lightAttenuation, vec3 samplePos, vec3 n, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, float shininess)
{
	vec3 lightDir = lightPos - samplePos;
	float distance = length(lightDir);

	if( distance > lightRange )
	{
		return vec3(0,0,0);	
	}
	else
	{
		// normalize it
		lightDir /= distance;
		// Diffuse
		vec3 diffuse = diffuseColor * lightColor * max(0.0,dot(n, lightDir));
		// Specular
		vec3 R = reflect(-lightDir,n);
		vec3 spec = pow(max(dot(R, viewDir), 0.0), shininess) * lightColor * specularColor;
		// Combine the components
		vec3 light = clamp(diffuse + spec, 0, 1);
		// Attenuate the light
		float att = 1.0 / dot(lightAttenuation, vec3(1.0, distance, distance*distance));
		light *= att;

		return light;
	}
}

void main()
{
    //vec3 diffuseMat = texture(u_diffuseTex, v_uv1).rgb;

    vec3 diffuseMat = color.rgb;
	vec3 n = normalize( v_normal.xyz );

	vec3 color = vec3(0,0,0);

	vec3 viewDir = normalize(u_viewPos - v_pos.xyz);

	color += calc_lighting(u_light1Pos.xyz, u_light1Range, u_light1Color, u_light1Attenuation, v_pos.xyz, n, viewDir, diffuseMat, u_specularColor, u_shininess);
	//color += calc_lighting(u_light2Pos.xyz, u_light2Range, u_light2Color, u_light2Attenuation, v_pos.xyz, n, viewDir, diffuseMat, u_specularColor, u_shininess);
	//color += calc_lighting(u_light3Pos.xyz, u_light3Range, u_light3Color, u_light3Attenuation, v_pos.xyz, n, viewDir, diffuseMat, u_specularColor, u_shininess);
	//color += calc_lighting(u_light4Pos.xyz, u_light4Range, u_light4Color, u_light4Attenuation, v_pos.xyz, n, viewDir, diffuseMat, u_specularColor, u_shininess);
	color += u_ambientLight * diffuseMat;

	PixelColor = vec4(color, 1.0);
}