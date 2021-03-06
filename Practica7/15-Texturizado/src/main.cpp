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
#include "Headers/FirstPersonCamera.h"
//Texture includes
//Descomentar
#include "Headers/Texture.h"

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere(20, 20);
Sphere sphere2(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Box box;

Shader shader;
//Descomentar
Shader shaderTexture;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8;

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX, offsetX;
int lastMousePosY, offsetY;

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
	//Descomentar
	shaderTexture.initialize("../../Shaders/texturizado.vs", "../../Shaders/texturizado.fs");

	sphere.init();
	sphere.setShader(&shader);
	sphere.setColor(glm::vec3(0.3, 0.3, 1.0));

	sphere2.init();
	//Cambiar el objetos shader
	sphere2.setShader(&shaderTexture);
	sphere2.setColor(glm::vec3(0.3, 0.3, 1.0));
	sphere2.scaleUVS(glm::vec2(3.0f, 1.0f));//Numero de veces que se repite la imagen en los ejes (Valor eje x, Valor eje y)

	cylinder.init();
	cylinder.setShader(&shaderTexture);
	cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec3(0.2, 0.7, 0.3));

	box.init();
	box.setShader(&shaderTexture);
	box.setColor(glm::vec3(0.2, 0.8, 0.4));

	camera->setPosition(glm::vec3(0.0f, 0.0f, 0.4f));

	// Descomentar
	//Primera textura para la esfera
	int imageWidth, imageHeight;
	Texture texture1("../../Textures/texturaLadrillos.jpg");//Se instancia el objeto textura y se le pasa el archivo
	FIBITMAP* bitmap = texture1.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data = texture1.convertToData(bitmap, imageWidth, imageHeight);//Se obtiene los bytes de la imagen
	glGenTextures(1, &textureID1);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID1);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//eje x// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); //eje y
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data){
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth, //Ancgho de la inagem
			imageHeight, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture1.freeImage(bitmap);
	
	//Segunda textura para el cubo
	int imageWidth2, imageHeight2;
	Texture texture2("../../Textures/fantasma.png");//Se instancia el objeto textura y se le pasa el archivo de la imagen 
	FIBITMAP* bitmap2 = texture2.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data2 = texture2.convertToData(bitmap2, imageWidth2, imageHeight2);//Se obtiene los bytes de la imagen
	glGenTextures(2, &textureID2);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID2);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth2, //Ancgho de la inagem
			imageHeight2, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data2);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture2.freeImage(bitmap2);

	//int imageWidth2, imageHeight2;
	Texture texture6("../../Textures/pacman.png");//Se instancia el objeto textura y se le pasa el archivo de la imagen 
	FIBITMAP* bitmap6 = texture6.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data6 = texture6.convertToData(bitmap6, imageWidth2, imageHeight2);//Se obtiene los bytes de la imagen
	glGenTextures(1, &textureID6);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID6);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data6) {
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth2, //Ancgho de la inagem
			imageHeight2, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data6);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture6.freeImage(bitmap6);

	
	Texture texture7("../../Textures/fantasma2.png");//Se instancia el objeto textura y se le pasa el archivo de la imagen 
	FIBITMAP* bitmap7 = texture7.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data7 = texture7.convertToData(bitmap7, imageWidth2, imageHeight2);//Se obtiene los bytes de la imagen
	glGenTextures(1, &textureID7);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID7);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data7) {
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth2, //Ancgho de la inagem
			imageHeight2, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data7);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture7.freeImage(bitmap7);

	//Textura para el cubo
	Texture texture8("../../Textures/espacio.jpg");//Se instancia el objeto textura y se le pasa el archivo de la imagen 
	FIBITMAP* bitmap8 = texture8.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data8 = texture8.convertToData(bitmap8, imageWidth2, imageHeight2);//Se obtiene los bytes de la imagen
	glGenTextures(1, &textureID8);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID8);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data8) {
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth2, //Ancgho de la inagem
			imageHeight2, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data8);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture8.freeImage(bitmap8);

	//Tercera textura cuerpo de la lata
	int imageWidth3, imageHeight3;
	Texture texture3("../../Textures/mirinda.jpg");//Se instancia el objeto textura y se le pasa el archivo de la imagen 
	FIBITMAP* bitmap3 = texture3.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data3 = texture3.convertToData(bitmap3, imageWidth3, imageHeight3);//Se obtiene los bytes de la imagen
	glGenTextures(1, &textureID3);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID3);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data3) {
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth3, //Ancgho de la inagem
			imageHeight3, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data3);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture3.freeImage(bitmap3);

	//Cuarta textura tapa superior de la lata
	int imageWidth4, imageHeight4;
	Texture texture4("../../Textures/tapalata.jpg");//Se instancia el objeto textura y se le pasa el archivo de la imagen 
	FIBITMAP* bitmap4 = texture4.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data4 = texture4.convertToData(bitmap4, imageWidth4, imageHeight4);//Se obtiene los bytes de la imagen
	glGenTextures(1, &textureID4);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID4);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data4) {
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth4, //Ancgho de la inagem
			imageHeight4, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data4);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture4.freeImage(bitmap4);

	//Quinta textura tapa inferior de la lata
	int imageWidth5, imageHeight5;
	Texture texture5("../../Textures/tapainferiorlata.jpg");//Se instancia el objeto textura y se le pasa el archivo de la imagen 
	FIBITMAP* bitmap5 = texture5.loadImage(false);//Se carga la imagen que en un arreglo bidimencional
	unsigned char * data5 = texture5.convertToData(bitmap5, imageWidth5, imageHeight5);//Se obtiene los bytes de la imagen
	glGenTextures(1, &textureID5);// Se indica el numero de texturas y el indicador de la textura
	glBindTexture(GL_TEXTURE_2D, textureID5);//Se enlaza el tipo de textura al ID textureID1 (texture 2D)
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data5) {
		glTexImage2D(GL_TEXTURE_2D,//Tipo de textura
			0, //Niveles de MIPMAPS
			GL_RGBA,//Formato interno que utilizara el buffer
			imageWidth5, //Ancgho de la inagem
			imageHeight5, //Largo de la imagen
			0, //Border
			GL_BGRA,//Formato de la libreria 
			GL_UNSIGNED_BYTE//Tipo de dato
			, data5);//Datos de la imagen (en donde esta almacenada la imagen) 
		glGenerateMipmap(GL_TEXTURE_2D);//Le indica a OpenGL que se encarge de generar los mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture5.freeImage(bitmap5);

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
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
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
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
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

		//Descomentar
		//Esfera 
		glBindTexture(GL_TEXTURE_2D, textureID1);//Enlazamos la textura que se desea utilizar 
		sphere2.setProjectionMatrix(projection);
		sphere2.setViewMatrix(view);
		//sphere2.enableWireMode();
		sphere2.setPosition(glm::vec3(0.0, 0.0, -4.0));
		sphere2.render();

		//Cubo
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0f, 0.0f, -3.0f));
		box.render(0, 6);

		glBindTexture(GL_TEXTURE_2D, textureID6);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0f, 0.0f, -3.0f));
		box.render(6, 6);

		glBindTexture(GL_TEXTURE_2D, textureID7);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0f, 0.0f, -3.0f));
		box.render(12, 6);

		glBindTexture(GL_TEXTURE_2D, textureID8);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0f, 0.0f, -3.0f));
		box.render(18, 6);

		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0f, 0.0f, -3.0f));
		box.render(24, 6);

		glBindTexture(GL_TEXTURE_2D, textureID6);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(2.0f, 0.0f, -3.0f));
		box.render(30, 6);


		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.setPosition(glm::vec3(-2.0f, 0.0f, -3.0f));
		cylinder.render(0, cylinder.getSlices() * cylinder.getStacks() * 2 * 3);//Contorno del cilindro
		glBindTexture(GL_TEXTURE_2D, textureID4);
		cylinder.render(cylinder.getSlices() * cylinder.getStacks() * 2 * 3, cylinder.getSlices() * 3);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		cylinder.render(cylinder.getSlices() * cylinder.getStacks() * 2 * 3 + cylinder.getSlices() * 3, cylinder.getSlices() * 3);

		//Descomentar
		glBindTexture(GL_TEXTURE_2D, 0);//No utilizamos ninguna textura 

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}