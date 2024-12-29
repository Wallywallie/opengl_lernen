#version 330 core
    out vec4 FragColor;

    in vec3 ourColor;
    in vec3 FragPos;
    in vec3 Normal;

    


    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform mat4 normalMatrix;
    uniform vec3 viewerPos;
    



    
    void main(){
        vec3 objColor = vec3( 1.0, 0.5, 0.31);

        float ambientStrength = 0.5;
        float specularStrength = 0.5;
        float shininess = 32;

        vec3 ambient = ambientStrength * lightColor;

        vec3 norm = normalize(mat3(normalMatrix )* Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        vec3 viewDir = normalize(viewerPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;

        vec3 result = (ambient + diffuse + specular) * objColor;


        FragColor = vec4(result, 1.0);
    }