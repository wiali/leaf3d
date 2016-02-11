#version 330 core

#define NR_MAX_LIGHTS 16

/* STRUCTS ********************************************************************/

struct Material {
    vec3    ambient;
    vec3    diffuse;
    vec3    specular;
    float   shininess;
};

struct Light {
    int     type;
    vec3    position;
    vec3    direction;
    vec4    color;
    float   kc;
    float   kl;
    float   kq;
};

/* INPUTS *********************************************************************/

// Data from vertex shader.
in vec3 o_position;
in vec3 o_normal;
in vec3 o_tangent;
in vec3 o_bitangent;
in vec2 o_texcoord0;

/* UNIFORMS *******************************************************************/

// Flags.
uniform bool        u_enableSpecularMap;
uniform bool        u_enableNormalMap;

// Diffuse map.
uniform sampler2D   u_diffuseMap;
uniform sampler2D   u_specularMap;
uniform sampler2D   u_normalMap;

// Camera position.
uniform vec3        u_cameraPos;

// Material and lights.
uniform Material    u_material;
uniform int         u_lightNr;
uniform Light       u_light[NR_MAX_LIGHTS];
uniform vec4        u_ambientColor;

/* UTILS **********************************************************************/

// Returns intensity of reflected ambient lighting.
vec3 ambientLighting()
{
   return u_material.ambient * u_ambientColor.xyz * u_ambientColor.w;
}

// Returns intensity of diffuse reflection.
vec3 diffuseLighting(
    in vec3 diffuse,
    in vec4 color,
    in vec3 normalDirection,
    in vec3 surfaceToLightDirection
)
{
   // Calculation as for Lambertian reflection.
   float diffuseTerm = clamp(dot(normalDirection, surfaceToLightDirection), 0, 1) ;
   return diffuse * color.rgb * color.a * diffuseTerm;
}

// Returns intensity of specular reflection.
vec3 specularLighting(
    in vec3 specular,
    in float shininess,
    in vec4 color,
    in vec3 normalDirection,
    in vec3 surfaceToLightDirection,
    in vec3 surfaceToCameraDirection
)
{
    vec3 halfVector = normalize(surfaceToLightDirection + surfaceToCameraDirection);
    float dotProduct = dot(normalDirection, halfVector);
    float specularTerm = 0;
    // Avoid specular reflections on back faces.
    if (dotProduct > 0)
        specularTerm = pow(dotProduct, shininess);
    return specular * (color.rgb * color.a * specularTerm);
}

// Returns light attenuation based on distance.
float lightingAttenuation(
    in float kc,
    in float kl,
    in float kq,
    in float surfaceToLightDistance
)
{
    return 1.0f / (kc + kl * surfaceToLightDistance + kq * (surfaceToLightDistance * surfaceToLightDistance));
}

/* MAIN ***********************************************************************/

void main()
{
    vec3 normal = o_normal;
    vec4 diffuse = texture(u_diffuseMap, o_texcoord0);
    vec4 specular = diffuse;

    // Specular mapping.
    if (u_enableSpecularMap)
        specular = texture(u_specularMap, o_texcoord0);

    // Normal mapping.
    if (u_enableNormalMap)
    {
        // Calculate fragment bump normal using TBN matrix.
        mat3 TBN = mat3(o_tangent, o_bitangent, o_normal);
        normal = texture(u_normalMap, o_texcoord0).rgb;
        normal = TBN * normalize(normal * 2.0 - 1.0);
    }

    // Blinn-Phong lighting components.
    vec3 Iamb = ambientLighting();
    vec3 Idif = vec3(0);
    vec3 Ispe = vec3(0);

    // Iterate over all lights.
    for (int i = 0; i < min(u_lightNr, NR_MAX_LIGHTS); i++)
    {
        vec3    surfaceToLight = u_light[i].position - o_position;
        float   attenuation = 1.0f;

        // Directional light.
        if (u_light[i].type == 0)
            surfaceToLight = -u_light[i].direction;

        // Normalize vectors after interpolation.
        float   surfaceToLightDistance = length(surfaceToLight);
        vec3    surfaceToLightDirection = normalize(surfaceToLight);
        vec3    surfaceToCameraDirection = normalize(u_cameraPos - o_position);
        vec3    lightDirection = normalize(-u_light[i].direction);

        // Point light.
        if (u_light[i].type == 1)
        {
            // Calculate attenuation based on distance.
            attenuation = lightingAttenuation(
                u_light[i].kc,
                u_light[i].kl,
                u_light[i].kq,
                surfaceToLightDistance
            );
        }

        // Spotlight.
        else if (u_light[i].type == 2)
        {
            float theta         = dot(surfaceToLightDirection, lightDirection);
            float epsilon       = u_light[i].kc - u_light[i].kl;
            attenuation         = clamp((theta - u_light[i].kl) / epsilon, 0.0, 1.0);
        }

        Idif += attenuation * diffuseLighting(
            u_material.diffuse,
            u_light[i].color,
            normal,
            surfaceToLightDirection
        );

        Ispe += attenuation * specularLighting(
            u_material.specular,
            u_material.shininess,
            u_light[i].color,
            normal,
            surfaceToLightDirection,
            surfaceToCameraDirection
        );
    }

    // Combine all components into the final color.
    vec4 ambientColor = diffuse * vec4(Iamb, 1.0f);
    vec4 diffuseColor = diffuse * vec4(Idif, 1.0f);
    vec4 specularColor = specular * vec4(Ispe, 1.0f);

    // Final fragment color.
    gl_FragColor = ambientColor + diffuseColor + specularColor;
}