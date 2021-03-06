//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"
// Geometry primitives
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
//Camara include
#include "Headers/FirstPersonCamera.h"

Sphere sphere(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Box box;

Shader shader;

//Se crea un objeto de tipo camara
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX, offsetx;
int lastMousePosY, offsety;

float rot1=0.0f, rot2=0.0f, rot3=0.0f, rot4=0.0f, rot5=0.0f, rot6=0.0f, rot7=0.0f;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	shader.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");

	sphere.init();
	sphere.setShader(&shader);
	sphere.setColor(glm::vec3(0.3, 0.3, 1.0));

	cylinder.init();
	cylinder.setShader(&shader);
	cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec3(0.2, 0.7, 0.3));

	box.init();
	box.setShader(&shader);
	box.setColor(glm::vec3(0.2, 0.8, 0.4));

	camera->setSensitivity(2.0);

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shader.destroy();
	sphere.destroy();
	cylinder.destroy();
	cylinder2.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetx = xpos - lastMousePosX;
	offsety = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetx, offsety, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rot2 -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rot3 -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rot4 -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rot5 -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rot6 -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rot7 -= 0.05;

	offsetx = 0;
	offsety = 0;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Matrix de proyeccion en perspectiva
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		// matrix de vista
		glm::mat4 view = camera->getViewMatrix();
	

		// Matrix con diagonal unitaria
		// Matriz del Cylindro del torso
		glm::mat4 matrix0 = glm::mat4(1.0f);
		// Se coloca el torso en la coordenada (0, 0, -1.0)
		matrix0 = glm::translate(matrix0, glm::vec3(0.0f, 0.0f, -1.0f));
		// Matrix de la esfera 1, se coloca -0.5 unidades en el eje y debajo del torso
		glm::mat4 matrixs1 = glm::translate(matrix0, glm::vec3(0.0f, -0.5f, 0.0f));
		// Se escala el cylidro del torso

		glm::mat4 matrixs5 = glm::translate(matrix0, glm::vec3(0.0f, 0.5f, 0.0f));
		glm::mat4 matrixs6 = glm::translate(matrixs5, glm::vec3(0.3f, 0.0f, 0.0f));
		glm::mat4 matrixs10 = glm::translate(matrixs5, glm::vec3(-0.3f, 0.0f, 0.0f));

		//Genera el movimiento del hombro
		matrixs6 = glm::rotate(matrixs6, rot1, glm::vec3(-1.0, 0.0, 0.0));
		matrixs6 = glm::rotate(matrixs6, rot2, glm::vec3(0.0, 0.0, -1.0));
		matrixs6 = glm::rotate(matrixs6, rot3, glm::vec3(0.0, -1.0, 0.0));
		matrixs10 = glm::rotate(matrixs10, rot1, glm::vec3(-1.0, 0.0, 0.0));
		matrixs10 = glm::rotate(matrixs10, rot2, glm::vec3(0.0, 0.0, -1.0));
		matrixs10 = glm::rotate(matrixs10, rot3, glm::vec3(0.0, -1.0, 0.0));

		//Matrices para el brazo derecho
		glm::mat4 matrix7 = glm::translate(matrixs6, glm::vec3(0.25f, 0.0f, 0.0f));//Matriz para la parte inferior del brazo derecho
		glm::mat4 matrixs7 = glm::translate(matrix7, glm::vec3(0.3f, 0.0f, 0.0f));//Matriz para el codo del brazo derecho
		
		//Matrices para el brazo izquierdo
		glm::mat4 matrix12 = glm::translate(matrixs10, glm::vec3(-0.25f, 0.0f, 0.0f));//Matriz para la parte inferior del brazo izquierdo
		glm::mat4 matrixs11 = glm::translate(matrix12, glm::vec3(-0.3f, 0.0f, 0.0f));//Matriz para el codo del brazo izquierdo

		//Genera el moimiento del codo 
		matrixs7 = glm::rotate(matrixs7, rot4, glm::vec3(-1.0, 0.0, 0.0));
		matrixs7 = glm::rotate(matrixs7, rot5, glm::vec3(0.0, -1.0, 0.0));
		matrixs11 = glm::rotate(matrixs11, rot4, glm::vec3(-1.0, 0.0, 0.0));
		matrixs11 = glm::rotate(matrixs11, rot5, glm::vec3(0.0, -1.0, 0.0));
		
		//Matrices para la parte inferior de los brazos
		glm::mat4 matrix8 = glm::translate(matrixs7, glm::vec3(0.25f, 0.0f, 0.0f));
		glm::mat4 matrixs8 = glm::translate(matrix8, glm::vec3(0.25f, 0.0f, 0.0f));
		glm::mat4 matrix13 = glm::translate(matrixs11, glm::vec3(-0.25f, 0.0f, 0.0f));
		glm::mat4 matrixs12 = glm::translate(matrix13, glm::vec3(-0.25f, 0.0f, 0.0f));
		
		//Genera el movimiento de la muneca
		matrixs8 = glm::rotate(matrixs8, rot6, glm::vec3(-1.0, 0.0, 0.0));
		matrixs8 = glm::rotate(matrixs8, rot7, glm::vec3(0.0, -1.0, 0.0));
		matrixs12 = glm::rotate(matrixs12, rot6, glm::vec3(-1.0, 0.0, 0.0));
		matrixs12 = glm::rotate(matrixs12, rot7, glm::vec3(0.0, -1.0, 0.0));


		glm::mat4 matrix9 = glm::translate(matrixs8, glm::vec3(0.08f, 0.0f, 0.0f));
		glm::mat4 matrix14 = glm::translate(matrixs12, glm::vec3(-0.08f, 0.0f, 0.0f));

		//Esfera que genera la muneca del brazo derecho
		matrixs8 = glm::scale(matrixs8, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs8);

		//Cubo que genera la mano derecha
		matrix9 = glm::rotate(matrix9, 1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix9 = glm::scale(matrix9, glm::vec3(0.1, 0.1, 0.15f));
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setColor(glm::vec3(0.9, 0.2, 0.5));
		box.render(matrix9);
		

		//Esfera que genera el codo del brazo derecho
		matrixs7 = glm::scale(matrixs7, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs7);
		
		//Parte inferior del brazo derecho
		matrix8 = glm::rotate(matrix8, 1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix8 = glm::scale(matrix8, glm::vec3(0.12, 0.5, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder.render(matrix8);
	
		//Parte superior del brazo derecho
		matrix7 = glm::rotate(matrix7, 1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix7 = glm::scale(matrix7, glm::vec3(0.15, 0.5, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder.render(matrix7);

		//Esfera superior de lado derecho
		matrixs6 = glm::scale(matrixs6, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs6);

		//Esfera superior de lado izquierdo
		matrixs10 = glm::scale(matrixs10, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs10);
	
		//Parte superior del brazo izquierdo
		matrix12 = glm::rotate(matrix12, -1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix12 = glm::scale(matrix12, glm::vec3(0.15, -0.5, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder.render(matrix12);

		//Esfera que genera el codo izquierdo
		matrixs11 = glm::scale(matrixs11, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs11);

		//Cilindro inferior del brazo izquierdo
		matrix13 = glm::rotate(matrix13, -1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix13 = glm::scale(matrix13, glm::vec3(0.12, -0.5, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder.render(matrix13);

		//Esfera que genera la muneca
		matrixs12 = glm::scale(matrixs12, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs12);

		//Cubo que genera la mano izquierda
		matrix14 = glm::rotate(matrix14, -1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix14 = glm::scale(matrix14, glm::vec3(0.1, 0.1, 0.15f));
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setColor(glm::vec3(0.9, 0.2, 0.5));
		box.render(matrix14);

		//Esfera que conforma al cuello
		matrixs5 = glm::scale(matrixs5, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		sphere.render(matrixs5);

		//Cilindro que genera el cuello
		glm::mat4 matrix15 = glm::translate(matrixs5, glm::vec3(0.0f, 0.5f, 0.0f));
		matrix15 = glm::scale(matrix15, glm::vec3(2.0, 1.5, 1.5f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder.render(matrix15);

		matrix0 = glm::scale(matrix0, glm::vec3(0.6f, 1.0f, 0.6f));
		// Se dibuja el cylindro
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder.render(matrix0);

		glm::mat4 matrixs2 = glm::translate(matrixs1, glm::vec3(-0.225f, 0.0f, 0.0f));
		glm::mat4 matrixs3 = glm::translate(matrixs1, glm::vec3(0.225f, 0.0f, 0.0f));

		//Esfera de la entrepierna
		matrixs1 = glm::scale(matrixs1, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs1);

		glm::mat4 matrix1 = glm::rotate(matrixs2, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix1 = glm::translate(matrix1, glm::vec3(0.0, -0.4, 0.0));

		glm::mat4 matrixs4 = glm::translate(matrix1, glm::vec3(0.0f, -0.4f, 0.0f));

		//Cilindro que forma la parte inferior de la pierna izquierda
		glm::mat4 matrix2 = glm::rotate(matrixs4, 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix2 = glm::translate(matrix2, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix2 = glm::scale(matrix2, glm::vec3(0.1, 0.6, 0.1));
		cylinder2.setProjectionMatrix(projection);
		cylinder2.setViewMatrix(view);
		cylinder2.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder2.render(matrix2);

		//Esfera que forma la rodilla de la pierna izquierda
		matrixs4 = glm::scale(matrixs4, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs4);
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		
		//Cilindro que forma la parte superior de la pierna izquierda
		matrix1 = glm::scale(matrix1, glm::vec3(0.15f, 0.8f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.1, 0.2, 0.4));
		cylinder.render(matrix1);

		//Esfera superior de la pierna izquierda
		matrixs2 = glm::scale(matrixs2, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs2);
	
		//Cilindro que forma la parte superior de la pierna derecha
		glm::mat4 matrix10 = glm::rotate(matrixs3, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixs9 = glm::translate(matrix10, glm::vec3(0.0f, -0.8f, 0.0f));
		matrix10 = glm::translate(matrix10, glm::vec3(0.0, -0.4, 0.0));
		matrix10 = glm::scale(matrix10, glm::vec3(0.15f, 0.8f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.setColor(glm::vec3(0.1, 0.2, 0.4));
		cylinder.render(matrix10);

		//Esfera superior de la pierna derecha
		matrixs3 = glm::scale(matrixs3, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs3);

		//Cilindro que forma la parte inferior de la pierna derecha
		glm::mat4 matrix11 = glm::rotate(matrixs9, -0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix11 = glm::translate(matrix11, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix11 = glm::scale(matrix11, glm::vec3(0.1, 0.6, 0.1));
		cylinder2.setProjectionMatrix(projection);
		cylinder2.setViewMatrix(view);
		cylinder2.enableWireMode();
		cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));
		cylinder2.render(matrix11);

		//Esfera que forma la rodilla de la pierna derecha
		matrixs9 = glm::scale(matrixs9, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs9);
		

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}