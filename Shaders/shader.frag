#version 330 core

in vec2 TexCoord;
out vec4 colour;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 ambientLight()
{
        float ambientStrength = 0.3;
        vec3 ambient = ambientStrength * lightColour;
        return ambient;
}

vec3 diffuseLight() {
        float diffuseStrength = 0.5;
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diffuseStrength * diff * lightColour;
        return diffuse;
}

vec3 specularLight() {
        float specularStrength = 0.8;
        float shininess = 32.0;
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);

        vec3 reflectDir = reflect(-lightDir, norm);
        vec3 viewDir = normalize(viewPos - FragPos);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColour;
        return specular;
}

void main()
{
        colour = texture(texture1, TexCoord) * vec4(ambientLight() + diffuseLight() + specularLight(), 1.0);
}