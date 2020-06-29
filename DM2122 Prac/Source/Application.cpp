#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Application.h"
#include "Scene5.h"

Camera2 camera(Vector3(0, 0, 80), Vector3(0, 0, 0), Vector3(0, 1, 0));
GLFWwindow *m_window;
const unsigned char FPS = 60;
const unsigned int frameTime = 1000 / FPS; //Time for each frame

bool firstMouse = 1;
double switchBounceTime = 0.0;
extern double elapsedTime;
extern float FOV = 45.0f;
GLfloat xLast = 0, yLast = 0;

static void error_callback(int error, const char *description){ //Define an error callback
	fputs(description, stderr);
	_fgetchar();
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){ //Define the key input callback
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos){ //For mouse movement
	if(firstMouse){
		xLast = GLfloat(xpos);
		yLast = GLfloat(ypos);
		firstMouse = 0;
	}
	float SENS = 0.03f, xOffset = (GLfloat(xpos) - xLast) * SENS, yOffset = (GLfloat(ypos) - yLast) * SENS; 
	xLast = GLfloat(xpos);
	yLast = GLfloat(ypos);
	camera.UpdateCamVectors(xOffset, yOffset);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){ //For mouse buttons
	if(action == GLFW_PRESS){
		(button == GLFW_MOUSE_BUTTON_LEFT ? camera.leftMouse : camera.rightMouse) = 1;
	} else{
		camera.leftMouse = camera.rightMouse = 0;
	}
}

static void scroll_callback(GLFWwindow *window, double xOffset, double yOffset){ //For mouse scroll wheel
	if((yOffset > 0 && FOV > 5.0f) || (yOffset < 0 && FOV < 120.0f)){
		FOV -= float(yOffset);
	}
}

bool Application::IsKeyPressed(unsigned short key){
	return (GetAsyncKeyState(key) & 0x8001) != 0;
}

void resize_callback(GLFWwindow *window, int w, int h){
	glViewport(0, 0, w, h); //Update OpenGL with the new window size
}

Application::Application(){
	glfwSetErrorCallback(error_callback); //Set the error callback
	if(!glfwInit()){ //Init GLFW
		exit(EXIT_FAILURE);
	}

	//Set GLFW window creation hints (optional)
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x anti-aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
												   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Makes MacOS happy, should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	m_window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL); //Create a window and create its OpenGL context
	glfwSetWindowSizeCallback(m_window, resize_callback);
	if(!m_window){ //If the window can't be created...
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window); //Makes the context of the specified window current on the calling thread
									  //glfwSetKeyCallback(m_window, key_callback); //Sets the key callback
	glewExperimental = 1; //For core profile
	GLenum err = glewInit(); //Init GLEW
	if(err != GLEW_OK){ //If GLEW is not initialised...
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	hideCursor = 1;
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide window cursor
	glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	if(glfwRawMouseMotionSupported()){
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); //Enable raw (unscaled and unaccelerated) mouse motion
	}
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
}

Application::~Application(){}

void Application::Run(){
	Scene *scene = new Scene5;
	m_timer.startTimer(); //Start timer to calculate how long it takes to render this frame
	while(!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE)){ //Main Loop
		double dt = m_timer.getElapsedTime();
		elapsedTime += dt;
		if(IsKeyPressed('C') && switchBounceTime <= elapsedTime){
			hideCursor = !hideCursor;
			if(hideCursor){
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide window cursor
			} else{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //Show window cursor
			}
			switchBounceTime = elapsedTime + 0.5;
		}
		camera.Update(dt);
		scene->Update(dt, FOV);
		scene->Render(camera);
		glfwSwapBuffers(m_window);
		glfwPollEvents(); //Get and organize events like kb and mouse input, window resizing, etc.
		m_timer.waitUntil(frameTime); //Limits each frame to a specified time in ms
	}
	delete scene;
}

void Application::Exit(){
	glfwDestroyWindow(m_window); //Close OpenGL window and terminate GLFW
	glfwTerminate(); //Finalise and clean up GLFW
}