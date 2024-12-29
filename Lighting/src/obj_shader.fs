#version 330 core
    out vec4 FragColor;

    in vec3 ourColor;
    in vec3 FragPos;
    in vec3 Normal;

    


    uniform vec3 lightColor;


    uniform mat4 normalMatrix;
    uniform vec3 viewerPos;

    struct Light{
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };
    uniform Light light;

    struct Material{
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    uniform Material material;
    
    
    void main(){
        vec3 objColor = vec3( 1.0, 0.5, 0.31);


        //环境光
        vec3 ambient = light.ambient * material.ambient;
        //漫反射
        vec3 norm = normalize(mat3(normalMatrix )* Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * material.diffuse * light.diffuse;
        //镜面反射
        vec3 viewDir = normalize(viewerPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * light.specular * material.specular;

        vec3 result = ambient + diffuse + specular;


        FragColor = vec4(result, 1.0);
    }