#version 410 core
#define MAX_POINT_LIGHTS 10

in vec3 fragPos;
in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;
uniform bool dayTime;

//struct PointLight {
  //  vec3 position;
 //   vec3 color;
 ///   float constant;
 //   float linear;
//    float quadratic;
//};

//uniform int numPointLights;                      // Number of point lights
//uniform PointLight pointLightss[MAX_POINT_LIGHTS];  // Array of point lights

uniform vec3 spotLightPosition;
uniform vec3 spotLightColor;
uniform float spotLightConstant;
uniform float spotLightLinear;
uniform float spotLightQuadratic;
uniform float spotLightIntensity;
uniform float spotLightMaxDistance;
uniform vec3 spotLightDirection;
uniform float spotLightCutOff;

uniform vec3 spotLightPosition2;
uniform vec3 spotLightColor2;
uniform float spotLightConstant2;
uniform float spotLightLinear2;
uniform float spotLightQuadratic2;
uniform float spotLightIntensity2;
uniform float spotLightMaxDistance2;
uniform vec3 spotLightDirection2;
uniform float spotLightCutOff2;

uniform vec3 spotLightPosition3;
uniform vec3 spotLightColor3;
uniform float spotLightConstant3;
uniform float spotLightLinear3;
uniform float spotLightQuadratic3;
uniform float spotLightIntensity3;
uniform float spotLightMaxDistance3;
uniform vec3 spotLightDirection3;
uniform float spotLightCutOff3;

uniform vec3 flashlightPosition;
uniform vec3 flashlightColor;
uniform float flashlightConstant;
uniform float flashlightLinear;
uniform float flashlightQuadratic;
uniform float flashlightIntensity;
uniform float flashlightMaxDistance;
uniform vec3 flashlightDirection;
uniform float flashlightCutOff;


uniform vec3 pointLightPosition1;
uniform vec3 pointLightColor1;
uniform float pointLightConstant1;
uniform float pointLightLinear1;
uniform float pointLightQuadratic1;
uniform float pointLightIntensity1;
uniform float pointLightMaxDistance1;

uniform vec3 pointLightPosition2;
uniform vec3 pointLightColor2;
uniform float pointLightConstant2;
uniform float pointLightLinear2;
uniform float pointLightQuadratic2;
uniform float pointLightIntensity2;
uniform float pointLightMaxDistance2;

uniform vec3 pointLightPosition3;
uniform vec3 pointLightColor3;
uniform float pointLightConstant3;
uniform float pointLightLinear3;
uniform float pointLightQuadratic3;
uniform float pointLightIntensity3;
uniform float pointLightMaxDistance3;

uniform vec3 pointLightPosition4;
uniform vec3 pointLightColor4;
uniform float pointLightConstant4;
uniform float pointLightLinear4;
uniform float pointLightQuadratic4;
uniform float pointLightIntensity4;
uniform float pointLightMaxDistance4;

uniform vec3 pointLightPosition5;
uniform vec3 pointLightColor5;
uniform float pointLightConstant5;
uniform float pointLightLinear5;
uniform float pointLightQuadratic5;
uniform float pointLightIntensity5;
uniform float pointLightMaxDistance5;


uniform vec3 pointLightPosition6;
uniform vec3 pointLightColor6;
uniform float pointLightConstant6;
uniform float pointLightLinear6;
uniform float pointLightQuadratic6;
uniform float pointLightIntensity6;
uniform float pointLightMaxDistance6;

uniform vec3 pointLightPosition7;
uniform vec3 pointLightColor7;
uniform float pointLightConstant7;
uniform float pointLightLinear7;
uniform float pointLightQuadratic7;
uniform float pointLightIntensity7;
uniform float pointLightMaxDistance7;

uniform vec3 pointLightPosition8;
uniform vec3 pointLightColor8;
uniform float pointLightConstant8;
uniform float pointLightLinear8;
uniform float pointLightQuadratic8;
uniform float pointLightIntensity8;
uniform float pointLightMaxDistance8;

uniform vec3 pointLightPosition9;
uniform vec3 pointLightColor9;
uniform float pointLightConstant9;
uniform float pointLightLinear9;
uniform float pointLightQuadratic9;
uniform float pointLightIntensity9;
uniform float pointLightMaxDistance9;

uniform vec3 pointLightPosition10;
uniform vec3 pointLightColor10;
uniform float pointLightConstant10;
uniform float pointLightLinear10;
uniform float pointLightQuadratic10;
uniform float pointLightIntensity10;
uniform float pointLightMaxDistance10;

uniform vec3 pointLightPosition11;
uniform vec3 pointLightColor11;
uniform float pointLightConstant11;
uniform float pointLightLinear11;
uniform float pointLightQuadratic11;
uniform float pointLightIntensity11;
uniform float pointLightMaxDistance11;

uniform vec3 pointLightPosition12;
uniform vec3 pointLightColor12;
uniform float pointLightConstant12;
uniform float pointLightLinear12;
uniform float pointLightQuadratic12;
uniform float pointLightIntensity12;
uniform float pointLightMaxDistance12;

uniform vec3 pointLightPosition13;
uniform vec3 pointLightColor13;
uniform float pointLightConstant13;
uniform float pointLightLinear13;
uniform float pointLightQuadratic13;
uniform float pointLightIntensity13;
uniform float pointLightMaxDistance13;

uniform vec3 pointLightPosition14;
uniform vec3 pointLightColor14;
uniform float pointLightConstant14;
uniform float pointLightLinear14;
uniform float pointLightQuadratic14;
uniform float pointLightIntensity14;
uniform float pointLightMaxDistance14;

uniform vec3 pointLightPosition15;
uniform vec3 pointLightColor15;
uniform float pointLightConstant15;
uniform float pointLightLinear15;
uniform float pointLightQuadratic15;
uniform float pointLightIntensity15;
uniform float pointLightMaxDistance15;

uniform vec3 pointLightPosition16;
uniform vec3 pointLightColor16;
uniform float pointLightConstant16;
uniform float pointLightLinear16;
uniform float pointLightQuadratic16;
uniform float pointLightIntensity16;
uniform float pointLightMaxDistance16;

uniform vec3 pointLightPosition17;
uniform vec3 pointLightColor17;
uniform float pointLightConstant17;
uniform float pointLightLinear17;
uniform float pointLightQuadratic17;
uniform float pointLightIntensity17;
uniform float pointLightMaxDistance17;

uniform vec3 pointLightPosition18;
uniform vec3 pointLightColor18;
uniform float pointLightConstant18;
uniform float pointLightLinear18;
uniform float pointLightQuadratic18;
uniform float pointLightIntensity18;
uniform float pointLightMaxDistance18;

uniform vec3 pointLightPosition19;
uniform vec3 pointLightColor19;
uniform float pointLightConstant19;
uniform float pointLightLinear19;
uniform float pointLightQuadratic19;
uniform float pointLightIntensity19;
uniform float pointLightMaxDistance19;

uniform vec3 pointLightPosition20;
uniform vec3 pointLightColor20;
uniform float pointLightConstant20;
uniform float pointLightLinear20;
uniform float pointLightQuadratic20;
uniform float pointLightIntensity20;
uniform float pointLightMaxDistance20;

uniform vec3 pointLightPosition21;
uniform vec3 pointLightColor21;
uniform float pointLightConstant21;
uniform float pointLightLinear21;
uniform float pointLightQuadratic21;
uniform float pointLightIntensity21;
uniform float pointLightMaxDistance21;

uniform vec3 pointLightPosition22;
uniform vec3 pointLightColor22;
uniform float pointLightConstant22;
uniform float pointLightLinear22;
uniform float pointLightQuadratic22;
uniform float pointLightIntensity22;
uniform float pointLightMaxDistance22;

uniform vec3 pointLightPosition23;
uniform vec3 pointLightColor23;
uniform float pointLightConstant23;
uniform float pointLightLinear23;
uniform float pointLightQuadratic23;
uniform float pointLightIntensity23;
uniform float pointLightMaxDistance23;

uniform float fogDensity;



//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform bool fogActive;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 1f;
float shininess = 32.0f;

void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	vec3 normalEye = normalize(fNormal);	
	
	vec3 lightDirN = normalize(lightDir);
	
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	ambient = ambientStrength * lightColor;
	
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;

	vec3 lightDirLocal = pointLightPosition1-fragPos;
	float distance = length(lightDirLocal);
	float attenuation = 0.0f;
	if(distance < pointLightMaxDistance1)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant1 + pointLightLinear1 * distance + pointLightQuadratic1 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor1 * pointLightIntensity1);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor1 * pointLightIntensity1);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity1);
	}

	lightDirLocal = pointLightPosition2- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance2)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant2 + pointLightLinear2 * distance + pointLightQuadratic2 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor2 * pointLightIntensity2);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor2 * pointLightIntensity2);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity2);
	}
	

	lightDirLocal = pointLightPosition3- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance3)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant3 + pointLightLinear3 * distance + pointLightQuadratic3 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor3 * pointLightIntensity3);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor3 * pointLightIntensity3);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity3);
	}

	lightDirLocal = pointLightPosition4- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance4)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant4 + pointLightLinear4 * distance + pointLightQuadratic4 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor4 * pointLightIntensity4);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor4 * pointLightIntensity4);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity4);
	}

	lightDirLocal = pointLightPosition5- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance5)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant5 + pointLightLinear5 * distance + pointLightQuadratic5 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor5 * pointLightIntensity5);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor5 * pointLightIntensity5);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity5);
	}

	lightDirLocal = pointLightPosition6- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance6)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant6 + pointLightLinear6 * distance + pointLightQuadratic6 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor6 * pointLightIntensity6);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor6 * pointLightIntensity6);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity6);
	}

	lightDirLocal = pointLightPosition7- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance7)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant7 + pointLightLinear6 * distance + pointLightQuadratic7 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor7 * pointLightIntensity7);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor7 * pointLightIntensity7);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity7);
	}

	lightDirLocal = pointLightPosition8- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance8)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant8 + pointLightLinear8 * distance + pointLightQuadratic8 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor8 * pointLightIntensity8);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor8 * pointLightIntensity8);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity8);
	}

	lightDirLocal = pointLightPosition9- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance9)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant9 + pointLightLinear9 * distance + pointLightQuadratic9 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor9 * pointLightIntensity9);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor9 * pointLightIntensity9);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity9);
	}

	lightDirLocal = pointLightPosition10- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance10)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant10 + pointLightLinear10 * distance + pointLightQuadratic10 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor10 * pointLightIntensity10);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor10 * pointLightIntensity10);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity10);
	}

	lightDirLocal = pointLightPosition11- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance11)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant11 + pointLightLinear11 * distance + pointLightQuadratic11 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor11 * pointLightIntensity11);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor11 * pointLightIntensity11);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity11);
	}

	lightDirLocal = pointLightPosition12- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance12)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant12 + pointLightLinear12 * distance + pointLightQuadratic12 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor12 * pointLightIntensity12);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor12 * pointLightIntensity12);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity12);
	}

	lightDirLocal = pointLightPosition13- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance13)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant13 + pointLightLinear13 * distance + pointLightQuadratic13 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor13 * pointLightIntensity13);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor13 * pointLightIntensity13);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity13);
	}

	lightDirLocal = pointLightPosition14- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance14)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant14 + pointLightLinear14 * distance + pointLightQuadratic14 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor14 * pointLightIntensity14);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor14 * pointLightIntensity14);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity14);
	}

	lightDirLocal = pointLightPosition15- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance15)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant15 + pointLightLinear15 * distance + pointLightQuadratic15 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor15 * pointLightIntensity15);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor15 * pointLightIntensity15);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity15);
	}

	lightDirLocal = pointLightPosition16- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance16)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant16 + pointLightLinear16 * distance + pointLightQuadratic16 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor16 * pointLightIntensity16);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor16 * pointLightIntensity16);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity16);
	}

	lightDirLocal = pointLightPosition17- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance17)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant17 + pointLightLinear17 * distance + pointLightQuadratic17 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor17 * pointLightIntensity17);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor17 * pointLightIntensity17);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity17);
	}

	lightDirLocal = pointLightPosition18- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance18)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant18 + pointLightLinear18 * distance + pointLightQuadratic18 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor18 * pointLightIntensity18);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor18 * pointLightIntensity18);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity18);
	}

	lightDirLocal = pointLightPosition19- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance19)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant19 + pointLightLinear19 * distance + pointLightQuadratic19 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor19 * pointLightIntensity19);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor19 * pointLightIntensity19);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity19);
	}

	lightDirLocal = pointLightPosition20- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance20)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant20 + pointLightLinear20 * distance + pointLightQuadratic20 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor20 * pointLightIntensity20);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor20 * pointLightIntensity20);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity20);
	}

	lightDirLocal = pointLightPosition21- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance21)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant21 + pointLightLinear21 * distance + pointLightQuadratic21 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor21 * pointLightIntensity21);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor21 * pointLightIntensity21);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity21);
	}

	lightDirLocal = pointLightPosition22- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance22)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant22 + pointLightLinear22 * distance + pointLightQuadratic22 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor22 * pointLightIntensity22);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor22 * pointLightIntensity22);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity22);
	}

	lightDirLocal = pointLightPosition23- fragPos;
	distance = length(lightDirLocal);
	if(distance < pointLightMaxDistance23)
	{
		lightDirLocal = normalize(lightDirLocal);
		attenuation = 1.0f / (pointLightConstant23 + pointLightLinear23 * distance + pointLightQuadratic23 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * pointLightColor23 * pointLightIntensity23);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * pointLightColor23 * pointLightIntensity23);
		ambient += (attenuation * ambientStrength * lightColor * pointLightIntensity23);
	}

	//spotlight

	lightDirLocal = spotLightPosition- fragPos;
	distance = length(lightDirLocal);
	lightDirLocal = normalize(lightDirLocal);
	float SpotFactor = dot(lightDirLocal, spotLightDirection);

	if(distance < spotLightMaxDistance && SpotFactor > spotLightCutOff)
	{
		float spotLightAngleAttenuation = (1.0f - (1.0f - SpotFactor) / (1.0f - spotLightCutOff));
		attenuation = 1.0f / (spotLightConstant + spotLightLinear * distance + spotLightQuadratic * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * spotLightColor * spotLightIntensity* spotLightAngleAttenuation);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * spotLightColor * spotLightIntensity * spotLightAngleAttenuation);
		ambient += (attenuation * ambientStrength * lightColor * spotLightIntensity * spotLightAngleAttenuation);
	}

	lightDirLocal = spotLightPosition2- fragPos;
	distance = length(lightDirLocal);
	lightDirLocal = normalize(lightDirLocal);
	SpotFactor = dot(lightDirLocal, spotLightDirection2);

	if(distance <spotLightMaxDistance2 && SpotFactor > spotLightCutOff2)
	{
		float spotLightAngleAttenuation = (1.0f - (1.0f - SpotFactor) / (1.0f - spotLightCutOff2));
		attenuation = 1.0f / (spotLightConstant2 + spotLightLinear2 * distance + spotLightQuadratic2 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * spotLightColor2 * spotLightIntensity2* spotLightAngleAttenuation);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * spotLightColor2 * spotLightIntensity2 * spotLightAngleAttenuation);
		ambient += (attenuation * ambientStrength * lightColor * spotLightIntensity2 * spotLightAngleAttenuation);
	}

	lightDirLocal = spotLightPosition3- fragPos;
	distance = length(lightDirLocal);
	lightDirLocal = normalize(lightDirLocal);
	SpotFactor = dot(lightDirLocal, spotLightDirection3);

	if(distance <spotLightMaxDistance3 && SpotFactor > spotLightCutOff3)
	{
		float spotLightAngleAttenuation = (1.0f - (1.0f - SpotFactor) / (1.0f - spotLightCutOff3));
		attenuation = 1.0f / (spotLightConstant3 + spotLightLinear3 * distance + spotLightQuadratic3 * (distance * distance));
		diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * spotLightColor3 * spotLightIntensity3* spotLightAngleAttenuation);
		reflection = reflect(-lightDirLocal, normalEye);
		specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
		specular += (attenuation*specularStrength * specCoeff * spotLightColor3 * spotLightIntensity3 * spotLightAngleAttenuation);
		ambient += (attenuation * ambientStrength * lightColor * spotLightIntensity3 * spotLightAngleAttenuation);
	}

lightDirLocal = flashlightPosition- fPosEye.xyz;
distance = length(lightDirLocal);
lightDirLocal = normalize(lightDirLocal);
SpotFactor = dot(lightDirLocal, flashlightDirection);

if(distance < flashlightMaxDistance && SpotFactor > flashlightCutOff)
{
	attenuation = 1.0f / (flashlightConstant + flashlightLinear * distance + flashlightQuadratic * (distance * distance));
	float spotLightAngleAttenuation = (1.0f - (1.0f - SpotFactor) / (1.0f - flashlightCutOff));
	diffuse += (attenuation* max(dot(normalEye, lightDirLocal), 0.0f) * flashlightColor * flashlightIntensity* spotLightAngleAttenuation);
	reflection = reflect(-lightDirLocal, normalEye);
	specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular += (attenuation*specularStrength * specCoeff * flashlightColor * flashlightIntensity * spotLightAngleAttenuation);
	ambient += (attenuation * ambientStrength * lightColor * flashlightIntensity * spotLightAngleAttenuation);
}


//	for (int i = 0; i < numPointLights; i++) {
 //       vec3 lightDir = pointLightss[i].position - fPosEye.xyz;
 //       float distance = length(lightDir);
 //       lightDir = normalize(lightDir);
 //       float attenuation = 1.0 / (pointLightss[i].constant + pointLightss[i].linear * distance + pointLightss[i].quadratic * (distance * distance));
//        diffuse += max(dot(normalEye, lightDir), 0.0) * pointLightss[i].color * attenuation;
//        reflection = reflect(-lightDir, normalEye);
 //       specCoeff = pow(max(dot(viewDirN, reflection), 0.0), shininess);
 //       specular += specularStrength * specCoeff * pointLightss[i].color * attenuation;
  //  }
}

float computeShadow()
{
	vec3 shadowCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	shadowCoords = 0.5f * shadowCoords + 0.5f;

	float closestDepth = texture(shadowMap, shadowCoords.xy).r;
	float currentDepth = shadowCoords.z;
	float depthBias = 0.001f;
	float shadow = clamp(currentDepth - depthBias > closestDepth ? 1.0f : 0.0f, 0.0f, 1.0f);

	// Handle the case where the fragment is outside the light frustum
	if (shadowCoords.z > 1.0f)
		shadow = 0.0f;

	return shadow;

}

float computeFog()
{
	 float fragmentDistance = length(fPosEye);
	 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

	 return clamp(fogFactor, 0.0f, 1.0f);
}


void main() 
{
	computeLightComponents();

	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;
	float shadow = computeShadow();

	//shadow = shadow * dayTime + 0.0f*(1-dayTime);

	if(!dayTime)
	{
		shadow = 0.0f;
	}

	vec3 color = min((ambient + (1.0f - shadow) * diffuse) + (1.0f - shadow) * specular, 1.0f);
    
	if(fogActive)
	{	
		float fogFactor = computeFog();
		vec4 fogColor;
		if(dayTime)
		{
			fogColor = vec4(0.65f, 0.6f, 0.564f, 1.0f);
		}
		else
		{
			fogColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
		}

		fogColor = vec4(0.65f, 0.6f, 0.564f, 1.0f);
		vec3 fogColorRGB = fogColor.rgb;
		fColor = mix(vec4(fogColorRGB, 1.0f), vec4(color, 1.0), fogFactor);
	}
	else
	{
		fColor = vec4(color, 1.0f);
	}

}
