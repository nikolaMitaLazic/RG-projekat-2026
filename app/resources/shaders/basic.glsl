//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    bool enabled;
    vec3 position;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform DirectionalLight directionalLight;
uniform SpotLight flashlight;
uniform PointLight pointLights[2];
uniform vec3 viewPosition;

void main() {
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragPos);

    vec3 lightDirection = normalize(-directionalLight.direction);
    vec3 ambient = directionalLight.ambient * color;

    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = directionalLight.diffuse * diffuseStrength * color;

    vec3 reflectedDirection = reflect(-lightDirection, normal);
    float specularStrength = pow(max(dot(viewDirection, reflectedDirection), 0.0), 32.0);
    vec3 specular = directionalLight.specular * specularStrength;

    vec3 result = ambient + diffuse + specular;

    for (int i = 0; i < 2; i++) {
        PointLight pointLight = pointLights[i];
        lightDirection = normalize(pointLight.position - FragPos);

        float distance = length(pointLight.position - FragPos);
        float attenuation = 1.0 / (pointLight.constant
            + pointLight.linear * distance
            + pointLight.quadratic * distance * distance);

        ambient = pointLight.ambient * color;

        diffuseStrength = max(dot(normal, lightDirection), 0.0);
        diffuse = pointLight.diffuse * diffuseStrength * color;

        reflectedDirection = reflect(-lightDirection, normal);
        specularStrength = pow(max(dot(viewDirection, reflectedDirection), 0.0), 32.0);
        specular = pointLight.specular * specularStrength;

        result += (ambient + diffuse + specular) * attenuation;
    }

    if (flashlight.enabled) {
        lightDirection = normalize(flashlight.position - FragPos);

        float distance = length(flashlight.position - FragPos);
        float attenuation = 1.0 / (flashlight.constant
            + flashlight.linear * distance
            + flashlight.quadratic * distance * distance);

        float theta = dot(lightDirection, normalize(-flashlight.direction));
        float epsilon = flashlight.innerCutoff - flashlight.outerCutoff;
        float intensity = clamp((theta - flashlight.outerCutoff) / epsilon, 0.0, 1.0);

        ambient = flashlight.ambient * color;

        diffuseStrength = max(dot(normal, lightDirection), 0.0);
        diffuse = flashlight.diffuse * diffuseStrength * color;

        reflectedDirection = reflect(-lightDirection, normal);
        specularStrength = pow(max(dot(viewDirection, reflectedDirection), 0.0), 32.0);
        specular = flashlight.specular * specularStrength;

        result += (ambient + diffuse + specular) * attenuation * intensity;
    }

    FragColor = vec4(result, 1.0);
}