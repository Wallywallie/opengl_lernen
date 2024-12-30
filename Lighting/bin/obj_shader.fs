#version 330 core
    out vec4 FragColor;

    in vec3 ourColor;
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoords;

    


    uniform vec3 lightColor;
    uniform mat4 normalMatrix;
    uniform vec3 viewerPos;

    uniform float debugOutput;

    struct PointLight{
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

 
    #define NR_POINT_LIGHTS 4
    uniform PointLight pointLights[NR_POINT_LIGHTS];

    struct DirLight {
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };
    uniform DirLight dirlight;

    struct Material{

        sampler2D diffuse;
        sampler2D specular;
        float shininess;
    };

    uniform Material material;


    vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
    vec3 calcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);

    
    
    void main(){


        vec3 viewDir = normalize(viewerPos - FragPos);
        vec3 norm = normalize(mat3(normalMatrix )* Normal);
        vec3 result = calcDirLight(dirlight, norm, viewDir);
        //for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        //    result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        //}
        //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  
        


        FragColor = vec4(result, 1.0);
    }

    vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        
        return (ambient + diffuse + specular);
    }

    vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        //环境光
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        //漫反射
        
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff  * light.diffuse * vec3(texture(material.diffuse, TexCoords));
        //镜面反射
        
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

        vec3 result =  (ambient + diffuse + specular) * attenuation;    
        return result;    
    }