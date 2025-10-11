#version 330 core

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 colour;

uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture2D;

vec3 ambientLight(){
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColour;
    return ambient;
}

vec3 diffuseLight(){
    float diffuseStrength = 0.5f;

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diffuseStrength * diff * lightColour;
    return diffuse;

}

vec3 specularLight(){
    float specularStrength = 1.0f;
    float shininess = 64.0f;

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess); 

    vec3 specular = specularStrength * spec * lightColour;

    return specular;
}

void main()
{
    colour = texture(texture2D, TexCoord) * vec4(ambientLight() + diffuseLight() + specularLight(), 1.0); 
}