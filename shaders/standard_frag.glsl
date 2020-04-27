#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 lightColor;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    // full formula: (((far-near) * coord) + near + far) / 2.0
    // we have far = 1, near = 0
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    //Declare a bias to deal with shadow acne
    float bias = 0;
    // Check if face is a front face
    if(dot(Normal, lightPos) < 0)
    {
        float bias = 0.005*tan(acos(dot(Normal, lightPos))); // cosTheta is dot( n,l ), clamped between 0 and 1
        bias = clamp(bias, 0,0.001);
    }
    //Calculating Shadow value
    float shadow = 0.0;
    //Applying PCF with the 9 nearest points
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.15;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

void main() {
    vec3 ambient = 0.3 * lightColor;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = 0.7 * diff * lightColor;

    float shadow = ShadowCalculation(FragPosLightSpace);
    FragColor = vec4(diffuse + (1-shadow) * ambient, 1);
}
