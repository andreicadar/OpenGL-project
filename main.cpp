//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright   2016 CGIS. All rights reserved.
//

#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#define MAX_POINT_LIGHTS 30

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>

int glWindowWidth = 1400;
int glWindowHeight = 1000;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 16384;
const unsigned int SHADOW_HEIGHT = 16384;

glm::mat4 model;
GLuint modelLoc;
GLuint modelLoc2;
glm::mat4 view;
GLuint viewLoc;
GLuint viewLoc2;
glm::mat4 projection;
GLuint projectionLoc;
GLuint projectionLoc2;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
GLuint normalMatrixLoc2;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

gps::Camera myCamera(
	glm::vec3(0.0f, 0.7f, 29.0f),
	glm::vec3(0.0f, 0.0f, 4000),
	glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.08;

bool pressedKeys[1024];
float angleY = 0.0f;
GLfloat lightAngle;
bool canCrouch = true;
bool inFrontOfMonitor = false;
bool canPressV = true;
bool canPressT = true;

bool inCameraMode = false;
bool flashLightOn = false;
bool wireframeView = false;
glm::vec3 oldCameraPosition;
glm::vec3 oldCameraTarget;

gps::Model3D nanosuit;
gps::Model3D postLamps;
gps::Model3D ground;
gps::Model3D pathway;
gps::Model3D rock;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D fireplace;
gps::Model3D lightPostMetalPart;
gps::Model3D lightPostGlassPart;
gps::Model3D house;
gps::Model3D TV;
gps::Model3D woodenTable;
gps::Model3D securityCamera;
gps::Model3D houseLightBulbs;
gps::Model3D wetFloorSign;
gps::Model3D rubberDuck;

gps::Shader myCustomShader;
gps::Shader emissiveShader;
gps::Shader blurShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;
gps::Shader skyboxShader;
gps::Shader frameBuffer;

GLuint shadowMapFBO;
GLuint hdrFBO;
GLuint depthMapTexture;
unsigned int colorBuffers[2];
unsigned int pingpongFBO[2];
unsigned int pingpongBuffer[2];

int cameraAnimPosition = 0;

gps::SkyBox mySkyBox;

bool showDepthMap;
bool fogActive = false;
bool canPressF = true;
bool canPressG = true;
bool canPressX = true;
bool lastCameraState = false;
int daytime = 1;

glm::vec3 duckPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 tranlationVector = glm::vec3(0.0f, 0.0f, 0.0f);
void changeDayTime();
bool duckDirection = true;

double lastFrameTime;
float deltaTime = 0.0f;
float animationSpeed = 0.01f;
float elapsedAnimTime = 0.0f;
bool canUpdateSkyBox = true;

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float constant;
    float linear;
    float quadratic;
	float intensity;
	float maxDistance;
};

PointLight pointLights[MAX_POINT_LIGHTS];

struct SpotLight {
	glm::vec3 position;
	glm::vec3 color;
	float constant;
	float linear;
	float quadratic;
	float intensity;
	float maxDistance;
	glm::vec3 direction;
	float cutOff;
};

SpotLight flashlight;


GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void updateDeltaTime()
{
	double currentFrameTime = glfwGetTime();
	deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
	lastFrameTime = currentFrameTime;
}

void processCameraAnim()
{
	//std::cout << cameraAnimPosition << std::endl;
	switch (cameraAnimPosition)
	{
	case 1:
		if (elapsedAnimTime < 1.0f)
		{
			elapsedAnimTime += animationSpeed * deltaTime; // deltaTime should be the time elapsed since the last frame
			std::cout<< glm::mix(glm::vec3(0.0f, 10.7f, 15.0f), glm::vec3(0.0f, 10.7f, 35.0f), elapsedAnimTime).x << std::endl;
			myCamera.setCameraPosition(glm::mix(glm::vec3(0.0f, 10.7f, 35.0f), glm::vec3(18.0f, 10.7f, 35.0f), elapsedAnimTime));
			
			myCamera.rotate(-25.0f, glm::mix(-90.0f, -130.0f, elapsedAnimTime));

			myCustomShader.useShaderProgram();
			lightColor = glm::mix(glm::vec3(0.45f, 0.45f, 0.45f), glm::vec3(0.0f, 0.0f, 0.0f), elapsedAnimTime);
			glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
			if (elapsedAnimTime > 0.5f && canUpdateSkyBox)
			{
				std::vector<const GLchar*> faces;
				faces.push_back("skybox/galaxy+X.tga");
				faces.push_back("skybox/galaxy-X.tga");
				faces.push_back("skybox/galaxy+Z.tga");
				faces.push_back("skybox/galaxy-Z.tga");
				faces.push_back("skybox/galaxy+Y.tga");
				faces.push_back("skybox/galaxy-Y.tga");
				mySkyBox.Load(faces);
				canUpdateSkyBox = false;
			}
		}
		else
		{
			canUpdateSkyBox = true;
			cameraAnimPosition++;
			elapsedAnimTime = 0.0f;
			animationSpeed = 0.15f;
		}
		break;

	case 2:
		if (elapsedAnimTime < 1.0f)
		{
			elapsedAnimTime += animationSpeed * deltaTime; // deltaTime should be the time elapsed since the last frame
			myCamera.setCameraPosition(glm::mix(glm::vec3(18.0f, 10.7f, 35.0f), glm::vec3(18.0f, 10.7f, 0.0f), elapsedAnimTime));
			myCamera.rotate(-25.0f, glm::mix(-130.0f, -180.0f, elapsedAnimTime));
			
			myCustomShader.useShaderProgram();
			lightColor = glm::mix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), elapsedAnimTime);
			glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));

			if (elapsedAnimTime > 0.5f && canUpdateSkyBox)
			{
				std::vector<const GLchar*> faces;

				faces.push_back("skybox/right.tga");
				faces.push_back("skybox/left.tga");
				faces.push_back("skybox/top.tga");
				faces.push_back("skybox/bottom.tga");
				faces.push_back("skybox/back.tga");
				faces.push_back("skybox/front.tga");
				mySkyBox.Load(faces);
				canUpdateSkyBox = false;
			}
		}
		else
		{
			cameraAnimPosition++;
			elapsedAnimTime = 0.0f;
			animationSpeed = 0.22f;
		}
		break;
	case 3:
		if (elapsedAnimTime < 1.0f)
		{
			elapsedAnimTime += animationSpeed * deltaTime; // deltaTime should be the time elapsed since the last frame
			myCamera.setCameraPosition(glm::mix(glm::vec3(18.0f, 10.7f, 0.0f), glm::vec3(-16.0, 0.7f, 0.65f), elapsedAnimTime));
			myCamera.rotate(glm::mix(-25.0f, 0.0f, elapsedAnimTime), glm::mix(-180.0f, -90.0f, elapsedAnimTime));

			if (elapsedAnimTime > 0.5f && canUpdateSkyBox)
			{
				std::vector<const GLchar*> faces;

				faces.push_back("skybox/right.tga");
				faces.push_back("skybox/left.tga");
				faces.push_back("skybox/top.tga");
				faces.push_back("skybox/bottom.tga");
				faces.push_back("skybox/back.tga");
				faces.push_back("skybox/front.tga");
				mySkyBox.Load(faces);
				canUpdateSkyBox = false;
			}
		}
		else
		{
			cameraAnimPosition++;
			elapsedAnimTime = 0.0f;
			animationSpeed = 0.01f;
		}
		break;
	}

}

void processMovement()
{
	processCameraAnim();
	if (cameraAnimPosition == 0)
	{
		myCamera.setCameraPosition(glm::vec3(0.0f, 10.7f, 35.0f));
		//myCamera.setCameraTarget(glm::vec3(-1000.0f, -1000.0f, 1000.0f));
		myCamera.rotate(-25.0f, -90.0f);
		animationSpeed = 0.15f;
		elapsedAnimTime = 0.0f;
		cameraAnimPosition++;
		lastFrameTime = glfwGetTime();
	}
	//sceneStart 18.0
	else if(cameraAnimPosition == 4){
		if (pressedKeys[GLFW_KEY_Q]) {
			angleY -= 1.0f;
		}

		if (pressedKeys[GLFW_KEY_E]) {
			angleY += 1.0f;
		}

		if (pressedKeys[GLFW_KEY_J]) {
			lightAngle -= 1.0f;
		}

		if (pressedKeys[GLFW_KEY_L]) {
			lightAngle += 1.0f;
		}
		if (pressedKeys[GLFW_KEY_G] && canPressG == true)
		{
			fogActive = !fogActive;
			myCustomShader.useShaderProgram();
			glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "fogActive"), fogActive);
			canPressG = false;
		}
		else if (!pressedKeys[GLFW_KEY_G])
		{
			canPressG = true;
		}

		if (pressedKeys[GLFW_KEY_F] && canPressF == true)
		{
			flashLightOn = !flashLightOn;
			flashlight.intensity = flashLightOn == true ? 50.0f : 0.0f;
			myCustomShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightIntensity"), flashlight.intensity);
			canPressF = false;

		}
		else if (!pressedKeys[GLFW_KEY_F])
		{
			canPressF = true;
		}

		if (pressedKeys[GLFW_KEY_X] && canPressX == true)
		{
			//daytime = !daytime;
			myCustomShader.useShaderProgram();
			changeDayTime();
			canPressX = false;
		}
		else if (!pressedKeys[GLFW_KEY_X])
		{
			canPressX = true;
		}


		if (pressedKeys[GLFW_KEY_W]) {
			myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
			std::cout << myCamera.getCameraPosition().x << " " << myCamera.getCameraPosition().y << " " << myCamera.getCameraPosition().z << std::endl;

		}

		if (pressedKeys[GLFW_KEY_S]) {
			myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
		}

		if (pressedKeys[GLFW_KEY_A]) {
			myCamera.move(gps::MOVE_LEFT, cameraSpeed);
		}

		if (pressedKeys[GLFW_KEY_D]) {
			myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
		}

		if (pressedKeys[GLFW_KEY_LEFT_SHIFT]) {
			//cameraSpeed = 0.16f; -ecran mare
			cameraSpeed = 0.08f;
		}
		else
		{
			//cameraSpeed = 0.08f;
			cameraSpeed = 0.04f;
		}

		if (pressedKeys[GLFW_KEY_LEFT_CONTROL] && canCrouch)
		{
			myCamera.changeCrouchState();
			canCrouch = false;
		}
		else if (!pressedKeys[GLFW_KEY_LEFT_CONTROL])
		{
			canCrouch = true;
		}

		if (myCamera.getCameraPosition().x > -15.0f && myCamera.getCameraPosition().x < -12.0f && myCamera.getCameraPosition().z > -17.5f && myCamera.getCameraPosition().z < -11.5f && inFrontOfMonitor == false)
		{
			inFrontOfMonitor = true;
			std::cout << "You are in the house";
		}
		else if (myCamera.getCameraPosition().x < -15.0f || myCamera.getCameraPosition().x > -12.0f || myCamera.getCameraPosition().z < -17.5f || myCamera.getCameraPosition().z > -11.5f)
		{
			inFrontOfMonitor = false;
			//gltDeleteText(text);
		}

		if (pressedKeys[GLFW_KEY_V] && (inFrontOfMonitor == true || inCameraMode) && canPressV)
		{
			inCameraMode = !inCameraMode;
			canPressV = false;
			if (inCameraMode)
			{
				oldCameraPosition = myCamera.getCameraPosition();
				oldCameraTarget = myCamera.getCameraTarget();
				myCamera.setCameraPosition(glm::vec3(18.4346f, 6.0f, -28.15f));
				myCamera.setCameraTarget(glm::vec3(1000.0f, 300.3f, -1200.0f));
				myCamera.changeToCameraView();
				//myCamera = new gps::Camera(glm::vec3(18.4346f, 6.0f, -28.15f), glm::vec3(0.0f, 0.3f, 33.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				myCamera.changeSensitivity(0.01f);
			}
			else
			{
				myCamera.setCameraPosition(oldCameraPosition);
				myCamera.setCameraTarget(oldCameraTarget);
				myCamera.changeToNormalView();
				myCamera.changeSensitivity(0.2f);
			}
			myCamera.changeSecurityCameraMode();
		}
		else if (!pressedKeys[GLFW_KEY_V])
		{
			canPressV = true;
		}

		if (pressedKeys[GLFW_KEY_T] && canPressT) {

			wireframeView = !wireframeView;

			if (wireframeView) {

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			canPressT = false;
		}
		else if (!pressedKeys[GLFW_KEY_T]) {
			canPressT = true;
		}
	}


}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (cameraAnimPosition == 4)//trebe schimbat
	{
		if (!inCameraMode)
		{
			static double lastX = xpos;
			static double lastY = ypos;
			float xOffset = xpos - lastX;
			float yOffset = lastY - ypos;  // Inverted y-axis for typical camera controls

			lastX = xpos;
			lastY = ypos;
			if (inCameraMode == lastCameraState)
			{
				myCamera.processMouseMovement(xOffset, yOffset, true);
				float pitch = myCamera.getPitch();
				//float yaw = myCamera.getYaw();
				//std::cout << pitch;
				//std::cout<< yaw << std::endl;
				//lashlight.direction = glm::vec3(0.0f, 0.0f, pitch/90);

				myCustomShader.useShaderProgram();
				glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightDirection"), 1, glm::value_ptr(flashlight.direction));

			}
		}
		else
		{
			static double lastXCameraPos = xpos;
			//static double lastYCameraPos = ypos;

			float xOffset = xpos - lastXCameraPos;
			//float yOffset = lastYCameraPos - ypos;  // Inverted y-axis for typical camera controls

			lastXCameraPos = xpos;
			//lastYCameraPos = ypos;
			if (inCameraMode == lastCameraState)
				myCamera.processMouseMovement(xOffset, 0, true);
		}
		lastCameraState = inCameraMode;
	}
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	//for sRBG framebuffer
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	//for antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (__APPLE__)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	nanosuit.LoadModel("objects/nanosuit/nanosuit.obj");
	//Erock.LoadModel("objects/abc/rock.obj");
	postLamps.LoadModel("objects/postLamps/postLamps.obj");
	ground.LoadModel("objects/ground/ground.obj");
	pathway.LoadModel("objects/pathway/pathway.obj");
	lightCube.LoadModel("objects/cube/cube.obj");
	screenQuad.LoadModel("objects/quad/quad.obj");
	fireplace.LoadModel("objects/firePlace/fireplace.obj");
	lightPostMetalPart.LoadModel("objects/lightPostMetalPart/lightPostMetalPart.obj");
	lightPostGlassPart.LoadModel("objects/lightPostGlassPart/lightPostGlassPart.obj");
	house.LoadModel("objects/house/house.obj");
	TV.LoadModel("objects/TV/TV.obj");
	woodenTable.LoadModel("objects/woodenTable/woodenTable.obj");
	securityCamera.LoadModel("objects/securityCamera/securityCamera.obj");
	houseLightBulbs.LoadModel("objects/houseLightBulbs/houseLightBulbs.obj");
	wetFloorSign.LoadModel("objects/wetFloorSign/wetFloorSign.obj");
    rubberDuck.LoadModel("objects/rubberDuck/rubberDuck.obj");
}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();

	emissiveShader.loadShader("shaders/emissive.vert", "shaders/emissive.frag");
	emissiveShader.useShaderProgram();

	blurShader.loadShader("shaders/frameBuffer.vert", "shaders/blur.frag");
	blurShader.useShaderProgram();

	glUniform1i(glGetUniformLocation(blurShader.shaderProgram, "screenTexture"), 0);

	frameBuffer.loadShader("shaders/frameBuffer.vert", "shaders/frameBuffer.frag");
	frameBuffer.useShaderProgram();

	glUniform1i(glGetUniformLocation(frameBuffer.shaderProgram, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(frameBuffer.shaderProgram, "bloomTexture"), 1);



	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	depthMapShader.loadShader("shaders/shadow.vert", "shaders/shadow.frag");
	depthMapShader.useShaderProgram();
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();

}

void initUniforms() {
	myCustomShader.useShaderProgram();

	SpotLight spotLight;
	spotLight.position = glm::vec3(-25.282f, 3.0f, 25.274f);
	spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.constant = 1.0f;
	spotLight.linear = 0.55;
	spotLight.quadratic = 1.24;
	spotLight.intensity = 50;
	spotLight.maxDistance = 10.0f;
	spotLight.direction = glm::vec3(0.0f, 1.0f, 0.0f);
	spotLight.cutOff = glm::cos(glm::radians(30.5f));

	SpotLight spotLight2;
	spotLight2.position = glm::vec3(-27.214f, 3.0f, 17.4724f);
	spotLight2.color = glm::vec3(1.0f, 0.0f, 0.0f);
	spotLight2.constant = 1.0f;
	spotLight2.linear = 0.55;
	spotLight2.quadratic = 1.24;
	spotLight2.intensity = 50;
	spotLight2.maxDistance = 10.0f;
	spotLight2.direction = glm::vec3(0.0f, 1.0f, 0.0f);
	spotLight2.cutOff = glm::cos(glm::radians(20.5f));

	SpotLight spotLight3;
	spotLight3.position = glm::vec3(-27.214f, 3.0f, 2.0f);
	spotLight3.color = glm::vec3(0.294f, 0.858f, 0.05f);
	spotLight3.constant = 1.0f;
	spotLight3.linear = 0.55;
	spotLight3.quadratic = 1.24;
	spotLight3.intensity = 50;
	spotLight3.maxDistance = 10.0f;
	spotLight3.direction = glm::vec3(0.0f, 1.0f, 0.0f);
	spotLight3.cutOff = glm::cos(glm::radians(20.5f));

	flashlight.position = glm::vec3(0.0f, 0.0f, 0.0f);
	flashlight.color = glm::vec3(1.0f, 1.0f, 1.0f);
	flashlight.constant = 1.0f;
	flashlight.linear = 0.55;
	flashlight.quadratic = 1.24;
	flashlight.intensity = 0;
	flashlight.maxDistance = 1000.0f;
	flashlight.direction = glm::vec3(0.0f, 0.0f, 1.0f);
	flashlight.cutOff = glm::cos(glm::radians(10.0f));


	int numPointLights = 2;

	pointLights[0].position = glm::vec3(4.46f, 0.85f, 23.45f);
	pointLights[0].color = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[0].constant = 1.0f;
	pointLights[0].linear = 0.7;
	pointLights[0].quadratic = 1;
	pointLights[0].intensity = 15;
	pointLights[0].maxDistance = 7.0f;

	pointLights[1].position = glm::vec3(8.53f, 5.53f, -18.62f);  // Example position for point light 2
	pointLights[1].color = glm::vec3(0.788f, 0.329f, 0.062f);      // Example color for point light 2
	pointLights[1].constant = 1.0f;
	pointLights[1].linear = 0.35f;
	pointLights[1].quadratic = 0.44f;
	pointLights[1].intensity = 0;
	pointLights[1].maxDistance = 13.0f;

	pointLights[2].position = glm::vec3(8.53f, 0.15f, -18.62f);  // Example position for point light 2
	pointLights[2].color = glm::vec3(0.941f, 0.325f, 0.235f);      // Example color for point light 2
	pointLights[2].constant = 1.0f;
	pointLights[2].linear = 0.35f;
	pointLights[2].quadratic = 0.44f;
	pointLights[2].intensity = 0;
	pointLights[2].maxDistance = 13.0f;

	pointLights[3].position = glm::vec3(-4.0f, 4.0f, 25.66f);  // Example position for point light 2
	pointLights[3].color = glm::vec3(100.0f, 100.0f, 100.0f);      // Example color for point light 2
	pointLights[3].constant = 1.0f;
	pointLights[3].linear = 0.55f;
	pointLights[3].quadratic = 2.2f;
	pointLights[3].intensity = 20;
	pointLights[3].maxDistance = 2.0f;

	pointLights[4].position = glm::vec3(-4.16f, 4.0f, 20.823f);  // Example position for point light 2
	pointLights[4].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[4].constant = 1.0f;
	pointLights[4].linear = 0.55f;
	pointLights[4].quadratic = 2.2f;
	pointLights[4].intensity = 20;
	pointLights[4].maxDistance = 7.0f;

	pointLights[5].position = glm::vec3(-4.20f, 4.0f, 16.412f);  // Example position for point light 2
	pointLights[5].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[5].constant = 1.0f;
	pointLights[5].linear = 0.55f;
	pointLights[5].quadratic = 2.2f;
	pointLights[5].intensity = 20;
	pointLights[5].maxDistance = 7.0f;


	//incepe rogvaiv
	pointLights[6].position = glm::vec3(-19.7f, 2.5f, -16.212f);  // Example position for point light 2
	pointLights[6].color = glm::vec3(1.0f, 0.0f, 0.0f);      // Example color for point light 2
	pointLights[6].constant = 1.0f;
	pointLights[6].linear = 2.0f;
	pointLights[6].quadratic = 4.0f;
	pointLights[6].intensity = 25;
	pointLights[6].maxDistance = 4.0f;

	pointLights[7].position = glm::vec3(-18.5f, 2.5f, -16.212f);  // Example position for point light 2
	pointLights[7].color = glm::vec3(0.721f, 0.372f, 0.5f);      // Example color for point light 2
	pointLights[7].constant = 1.0f;
	pointLights[7].linear = 2.0f;
	pointLights[7].quadratic = 4.0f;
	pointLights[7].intensity = 25;
	pointLights[7].maxDistance = 4.0f;

	pointLights[8].position = glm::vec3(-18.5f, 2.5f, -16.212f);  // Example position for point light 2
	pointLights[8].color = glm::vec3(0.839f, 0.878f, 0.043f);      // Example color for point light 2
	pointLights[8].constant = 1.0f;
	pointLights[8].linear = 2.0f;
	pointLights[8].quadratic = 4.0f;
	pointLights[8].intensity = 25;
	pointLights[8].maxDistance = 4.0f;

	pointLights[9].position = glm::vec3(-16.0f, 2.5f, -16.212f);  // Example position for point light 2
	pointLights[9].color = glm::vec3(0.0f, 1.0f, 0.0f);      // Example color for point light 2
	pointLights[9].constant = 1.0f;
	pointLights[9].linear = 2.0f;
	pointLights[9].quadratic = 4.0f;
	pointLights[9].intensity = 25;
	pointLights[9].maxDistance = 4.0f;

	pointLights[10].position = glm::vec3(-15.0f, 2.5f, -16.212f);  // Example position for point light 2
	pointLights[10].color = glm::vec3(0.0f, 0.0f, 1.0f);      // Example color for point light 2
	pointLights[10].constant = 1.0f;
	pointLights[10].linear = 2.0f;
	pointLights[10].quadratic = 4.0f;
	pointLights[10].intensity = 25;
	pointLights[10].maxDistance = 4.0f;

	pointLights[11].position = glm::vec3(-14.0f, 2.5f, -16.212f);  // Example position for point light 2
	pointLights[11].color = glm::vec3(0.176f, 0.047f, 0.811f);      // Example color for point light 2
	pointLights[11].constant = 1.0f;
	pointLights[11].linear = 2.0f;
	pointLights[11].quadratic = 4.0f;
	pointLights[11].intensity = 25;
	pointLights[11].maxDistance = 4.0f;

	pointLights[12].position = glm::vec3(-12.6f, 2.5f, -16.212f);  // Example position for point light 2
	pointLights[12].color = glm::vec3(0.345f, 0.278f, 0.678f);      // Example color for point light 2
	pointLights[12].constant = 1.0f;
	pointLights[12].linear = 2.0f;
	pointLights[12].quadratic = 4.0f;
	pointLights[12].intensity = 25;
	pointLights[12].maxDistance = 4.0f;
	//se termina rogvaiv

	pointLights[13].position = glm::vec3(-4.15f, 4.0f, 11.530);  // Example position for point light 2
	pointLights[13].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[13].constant = 1.0f;
	pointLights[13].linear = 0.55f;
	pointLights[13].quadratic = 2.2f;
	pointLights[13].intensity = 20;
	pointLights[13].maxDistance = 7.0f;

	pointLights[14].position = glm::vec3(-4.17f, 4.0f, 7.25f);  // Example position for point light 2
	pointLights[14].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[14].constant = 1.0f;
	pointLights[14].linear = 0.55f;
	pointLights[14].quadratic = 2.2f;
	pointLights[14].intensity = 20;
	pointLights[14].maxDistance = 7.0f;

	pointLights[15].position = glm::vec3(-4.22f, 4.0f, 2.55);  // Example position for point light 2
	pointLights[15].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[15].constant = 1.0f;
	pointLights[15].linear = 0.55f;
	pointLights[15].quadratic = 2.2f;
	pointLights[15].intensity = 20;
	pointLights[15].maxDistance = 7.0f;

	//incepe partea dreapta a luminilor
	pointLights[16].position = glm::vec3(4.5f, 4.0f, 25.254);  // Example position for point light 2
	pointLights[16].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[16].constant = 1.0f;
	pointLights[16].linear = 0.55f;
	pointLights[16].quadratic = 2.2f;
	pointLights[16].intensity = 20;
	pointLights[16].maxDistance = 7.0f;

	pointLights[17].position = glm::vec3(4.5f, 4.0f, 20.537);  // Example position for point light 2
	pointLights[17].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[17].constant = 1.0f;
	pointLights[17].linear = 0.55f;
	pointLights[17].quadratic = 2.2f;
	pointLights[17].intensity = 20;
	pointLights[17].maxDistance = 7.0f;

	pointLights[18].position = glm::vec3(4.45f, 4.0f, 16.384);  // Example position for point light 2
	pointLights[18].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[18].constant = 1.0f;
	pointLights[18].linear = 0.55f;
	pointLights[18].quadratic = 2.2f;
	pointLights[18].intensity = 20;
	pointLights[18].maxDistance = 7.0f;

	pointLights[19].position = glm::vec3(4.33f, 4.0f, 11.4);  // Example position for point light 2
	pointLights[19].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[19].constant = 1.0f;
	pointLights[19].linear = 0.55f;
	pointLights[19].quadratic = 2.2f;
	pointLights[19].intensity = 20;
	pointLights[19].maxDistance = 7.0f;

	pointLights[20].position = glm::vec3(4.33f, 4.0f, 7.25f);  // Example position for point light 2
	pointLights[20].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[20].constant = 1.0f;
	pointLights[20].linear = 0.55f;
	pointLights[20].quadratic = 2.2f;
	pointLights[20].intensity = 20;
	pointLights[20].maxDistance = 7.0f;

	pointLights[21].position = glm::vec3(4.33f, 4.0f, 2.40f);  // Example position for point light 2
	pointLights[21].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[21].constant = 1.0f;
	pointLights[21].linear = 0.55f;
	pointLights[21].quadratic = 2.2f;
	pointLights[21].intensity = 20;
	pointLights[21].maxDistance = 7.0;

	pointLights[22].position = glm::vec3(-25.822f, 4.0f, 25.274f);  // Example position for point light 2
	pointLights[22].color = glm::vec3(0.858f, 0.811f, 0.627f);      // Example color for point light 2
	pointLights[22].constant = 1.0f;
	pointLights[22].linear = 0.55f;
	pointLights[22].quadratic = 2.2f;
	pointLights[22].intensity = 10;
	pointLights[22].maxDistance = 7.0f;


	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightPosition"), 1, glm::value_ptr(spotLight.position));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightColor"), 1, glm::value_ptr(spotLight.color));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightConstant"), spotLight.constant);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightLinear"), spotLight.linear);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightQuadratic"), spotLight.quadratic);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightIntensity"), spotLight.intensity);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightMaxDistance"), spotLight.maxDistance);
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightDirection"), 1, glm::value_ptr(spotLight.direction));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightCutOff"), spotLight.cutOff);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightPosition2"), 1, glm::value_ptr(spotLight2.position));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightColor2"), 1, glm::value_ptr(spotLight2.color));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightConstant2"), spotLight2.constant);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightLinear2"), spotLight2.linear);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightQuadratic2"), spotLight2.quadratic);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightIntensity2"), spotLight2.intensity);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightMaxDistance2"), spotLight2.maxDistance);
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightDirection2"), 1, glm::value_ptr(spotLight2.direction));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightCutOff2"), spotLight2.cutOff);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightPosition3"), 1, glm::value_ptr(spotLight3.position));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightColor3"), 1, glm::value_ptr(spotLight3.color));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightConstant3"), spotLight3.constant);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightLinear3"), spotLight3.linear);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightQuadratic3"), spotLight3.quadratic);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightIntensity3"), spotLight3.intensity);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightMaxDistance3"), spotLight3.maxDistance);
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightDirection3"), 1, glm::value_ptr(spotLight3.direction));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightCutOff3"), spotLight3.cutOff);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightPosition"), 1, glm::value_ptr(flashlight.position));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightColor"), 1, glm::value_ptr(flashlight.color));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightConstant"), flashlight.constant);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightLinear"), flashlight.linear);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightQuadratic"), flashlight.quadratic);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightIntensity"), flashlight.intensity);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightMaxDistance"), flashlight.maxDistance);
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightDirection"), 1, glm::value_ptr(flashlight.direction));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightCutOff"), flashlight.cutOff);

	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "dayTime"), daytime);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogDensity"), 0.03f);


	for (int i = 0; i < 23; ++i) {
		std::string pointLightStr = "pointLight";

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "Position" + std::to_string(i+1)).c_str()), 1, glm::value_ptr(pointLights[i].position));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "Color" + std::to_string(i+1)).c_str()), 1, glm::value_ptr(pointLights[i].color));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "Constant" + std::to_string(i+1)).c_str()), pointLights[i].constant);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "Linear" + std::to_string(i+1)).c_str()), pointLights[i].linear);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "Quadratic" + std::to_string(i+1)).c_str()), pointLights[i].quadratic);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "Intensity" + std::to_string(i+1)).c_str()), pointLights[i].intensity);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "MaxDistance" + std::to_string(i+1)).c_str()), pointLights[i].maxDistance);
}

	

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 0.5f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	lightColor = glm::vec3(0.45, 0.45f, 0.45f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	emissiveShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc2 = glGetUniformLocation(emissiveShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));

	//view = myCamera.getViewMatrix();
	viewLoc2 = glGetUniformLocation(emissiveShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));

	//normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc2 = glGetUniformLocation(emissiveShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc2, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc2 = glGetUniformLocation(emissiveShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));

/*	glm::vec3 cameraPosition = myCamera.getCameraPosition();
	glUniform3fv(glGetUniformLocation(emissiveShader.shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPosition));

	float shininessValue = 0.55;
	glUniform1f(glGetUniformLocation(emissiveShader.shaderProgram, "shininess"), shininessValue);

	glm::vec3 emissiveColor = glm::vec3(1.0, 1.0, 1.0);
	float emissiveStrength = 1.0;
	glUniform3fv(glGetUniformLocation(emissiveShader.shaderProgram, "emissiveColor"), 1, glm::value_ptr(emissiveColor));
	glUniform1f(glGetUniformLocation(emissiveShader.shaderProgram, "emissiveStrength"), emissiveStrength);*/


	//glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "numPointLights"), numPointLights);

	//for (int i = 0; i < numPointLights; ++i) {
	//	std::string pointLightStr = "pointLights[" + std::to_string(i) + "].";

	//	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "position").c_str()), 1, glm::value_ptr(pointLights[i].position));
	//	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "color").c_str()), 1, glm::value_ptr(pointLights[i].color));
	//	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "constant").c_str()), pointLights[i].constant);
	//	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "linear").c_str()), pointLights[i].linear);
	//	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, (pointLightStr + "quadratic").c_str()), pointLights[i].quadratic);
	//}

}

void initFBO() {
	// Create the FBO and the depth texture
	glGenFramebuffers(1, &shadowMapFBO);
	glGenTextures(1, &depthMapTexture);

	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Create the HDR FBO and the color buffer
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	glGenTextures(2, colorBuffers);

	for (unsigned int i = 0; i < 2; i++)
	{

		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, glWindowWidth, glWindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// Check for completeness and report any errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, glWindowWidth, glWindowHeight, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
}


glm::mat4 computeLightSpaceTrMatrix() {

	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightView = glm::lookAt(glm::vec3(lightRotation * glm::vec4(lightDir, 1.0f)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	const GLfloat near_plane = -22.0f, far_plane = 20.0f;
	glm::mat4 lightProjection = glm::ortho(-38.0f, 38.0f, -38.0f, 38.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}

void changeDayTime()
{
	if (daytime== 1)
	{
		daytime = 0;
	}
	else
	{
		daytime = 1;
	}
	std::vector<const GLchar*> faces;
	myCustomShader.useShaderProgram();

	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "dayTime"), daytime);

	if (daytime)
	{
		faces.push_back("skybox/right.tga");
		faces.push_back("skybox/left.tga");
		faces.push_back("skybox/top.tga");
		faces.push_back("skybox/bottom.tga");
		faces.push_back("skybox/back.tga");
		faces.push_back("skybox/front.tga");
		mySkyBox.Load(faces);

		lightColor = glm::vec3(0.45, 0.45f, 0.45f);
		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogDensity"), 0.03f);

	}
	else
	{
		faces.push_back("skybox/galaxy+X.tga");
		faces.push_back("skybox/galaxy-X.tga");
		faces.push_back("skybox/galaxy+Z.tga");
		faces.push_back("skybox/galaxy-Z.tga");
		faces.push_back("skybox/galaxy+Y.tga");
		faces.push_back("skybox/galaxy-Y.tga");
		mySkyBox.Load(faces);
	
		lightColor = glm::vec3(0.0f, 0.0f, 0.0f);
		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogDensity"), 0.01f);

	}
}

void initSkybox()
{
	std::vector<const GLchar*> faces;
	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");

	mySkyBox.Load(faces);
}

void drawObjectsEmissive(gps::Shader shader, bool depthPass)
{
	shader.useShaderProgram();
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc2, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	//nanosuit.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shader.useShaderProgram();
	//emissiveShader.setInt("hdrBuffer", 0); // Assuming texture unit 0 is used for HDR color buffer
	glUniform1i(glGetUniformLocation(emissiveShader.shaderProgram, "hdrBuffer"), 0);
	glUniform1f(glGetUniformLocation(emissiveShader.shaderProgram, "exposure"), 3.0f);
	lightPostGlassPart.Draw(shader);
	//glDisable(GL_DEPTH_TEST);
	//glBindTexture(GL_TEXTURE_2D, hdrFBO);
	//house.Draw(shader);
	//dwetFloorSign.Draw(shader);

	//rock.Draw(shader);


	if (!depthPass)
	{
		mySkyBox.Draw(skyboxShader, view, projection);
	}
}

void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	nanosuit.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	float time = glfwGetTime();

	float colorSpeed = 1.7f;
	float colorChange = (sin(time * colorSpeed) / 2) + 0.5f;
	pointLights[1].color = glm::mix(glm::vec3(0.968f, 0.831f, 0.270f), glm::vec3(0.839f, 0.513f, 0.117f), colorChange);

	float intensitySpeed = 7.0f;
	float intensityChange = (sin(time * intensitySpeed) / 2) + 0.5f;
	pointLights[1].intensity = glm::mix(3.0f, 5.0f, intensityChange);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor2"), 1, glm::value_ptr(pointLights[1].color));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightIntensity2"), pointLights[1].intensity);


	time = glfwGetTime();

	colorSpeed = 1.4f;
	colorChange = (sin(time * colorSpeed) / 2) + 0.5f;
	pointLights[2].color = glm::mix(glm::vec3(0.901f, 0.176f, 0.054f), glm::vec3(0.780f, 0.321f, 0.125f), colorChange);

	intensitySpeed = 4.0f;
	intensityChange = (sin(time * intensitySpeed) / 2) + 0.5f;
	pointLights[2].intensity = glm::mix(32.0f, 38.0f, intensityChange);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor3"), 1, glm::value_ptr(pointLights[2].color));
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightIntensity3"), pointLights[2].intensity);



	ground.Draw(shader);
	postLamps.Draw(shader);
	pathway.Draw(shader);
	fireplace.Draw(shader);
	lightPostMetalPart.Draw(shader);
	house.Draw(shader);
	TV.Draw(shader);
	woodenTable.Draw(shader);
	securityCamera.Draw(shader);
	houseLightBulbs.Draw(shader);
	wetFloorSign.Draw(shader);

	float moveIncrement = 0.03f;

	duckPos.z += moveIncrement;

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-27.2089f, -1.0f, duckDirection == 1 ? 17.4724f: 2.0f));

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, duckDirection == 1 ? -duckPos.z : duckPos.z));

	if (!duckDirection)
	{

		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (abs(duckPos.z) > 15.4724)
	{
		duckPos.z = 0;

		duckDirection = !duckDirection;
	}

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	rubberDuck.Draw(shader);

	flashlight.direction = glm::vec3(0.0f, 0.0f, 1.0f);
	shader.useShaderProgram();
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "flashlightDirection"), 1, glm::value_ptr(flashlight.direction));

	if (!depthPass)
	{
		mySkyBox.Draw(skyboxShader, view, projection);
	}
}

void renderScene() {

	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map

	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects(depthMapShader, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render depth map on screen - toggled with the M key

	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	}
	else {

		// final scene rendering pass (with shadows)
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use myCustom shader
		myCustomShader.useShaderProgram();

		//send uniforms for myCustomShader
		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));


		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		//draw with my custom shader
		drawObjects(myCustomShader, false);

		emissiveShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(emissiveShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(emissiveShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//drawObjectsEmissive(frameBuffer, false);
		drawObjectsEmissive(emissiveShader, false);
		bool horizontal = true, first_iteration = true;
		int amount = 10;
		blurShader.useShaderProgram();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glUniform1i(glGetUniformLocation(blurShader.shaderProgram, "horizontal"), horizontal);

			if(first_iteration)
				glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
			else
				glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);

			drawObjectsEmissive(blurShader, false);

			horizontal = !horizontal;
			//lightPostGlassPart.Draw(blurShader);
			
		}
		frameBuffer.useShaderProgram();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
		drawObjectsEmissive(frameBuffer, false);
		//glfwSwapBuffers(window);

		//lightPostGlassPart.Draw(emissiveShader);

		

		//draw a white cube around the light
		if (daytime)

		{
			lightShader.useShaderProgram();

			glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

			model = lightRotation;
			model = glm::translate(model, 1.0f * lightDir);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
			glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

			lightCube.Draw(lightShader);
		}

	}
}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initSkybox();
	initShaders();
	initUniforms();
	initFBO();

	glCheckError();

	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();
		updateDeltaTime();
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
