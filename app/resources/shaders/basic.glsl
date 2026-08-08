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

uniform sampler2D texture_diffuse1;
uniform DirectionalLight directionalLight;
uniform vec3 viewPosition;

void main() {
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(-directionalLight.direction);

    vec3 ambient = directionalLight.ambient * color;

    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = directionalLight.diffuse * diffuseStrength * color;

    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 reflectedDirection = reflect(-lightDirection, normal);
    float specularStrength = pow(max(dot(viewDirection, reflectedDirection), 0.0), 32.0);
    vec3 specular = directionalLight.specular * specularStrength;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}