#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
    #include <windows.h>
    #include "resource.h"
#endif

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "Game.h"
#include <iostream>


#define TARGET_FRAMERATE 60.0f

// Variables globales necesarias para el bucle de Emscripten (main_loop), antes estaban en el main
GLFWwindow* global_window;
double timePerFrame = 1.f / TARGET_FRAMERATE;
double timePreviousFrame;

/*
	-------------------------------------------------------------------------------------------
	Estas funciones son llamadas por GLFW cada vez que se produce un evento de teclado o ratón.
	Lo único que hacen es llamar a los métodos correspondientes de la clase Game para que esta pueda gestionar los eventos.
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		Game::instance().keyPressed(key);
	else if (action == GLFW_RELEASE)
		Game::instance().keyReleased(key);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game::instance().mouseMove(int(xpos), int(ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		Game::instance().mousePress(button);
	else if (action == GLFW_RELEASE)
		Game::instance().mouseRelease(button);
}
void window_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Game::instance().resize(width, height);
}
/* ------------------------------------------------------------------------------------------- */


void main_loop()
{
    double currentTime = glfwGetTime();
    if (currentTime - timePreviousFrame >= timePerFrame)    // Si ha pasado pasado el tiempo necesario para renderizar un nuevo frame...
    {
        /* Update & render steps of the game loop */
        if(!Game::instance().update(int(1000.0f * (currentTime - timePreviousFrame))))
            glfwSetWindowShouldClose(global_window, GLFW_TRUE);
        
        Game::instance().render();
        timePreviousFrame = currentTime;

        /* Swap front and back buffers */
        glfwSwapBuffers(global_window);
    }

    // Se analiza si se han producido eventos de teclado o ratón y se llaman a las funciones correspondientes de GLFW
    glfwPollEvents();
}

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Tamaño y nombre de la ventana */
	global_window  = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stranded in Alien Territory", NULL, NULL);
	if (!global_window)
	{
		glfwTerminate();
		return -1;
	}

	/* Posición inicial de la ventana dentro de la pantalla */
	glfwSetWindowPos(global_window , 100, 100);
	/* Marcamos el contexto de la ventana como el contexto actual para OpenGL */
	glfwMakeContextCurrent(global_window);

	// Set window icon (windows only)
	#ifdef _WIN32
		HWND hwnd = glfwGetWin32Window(global_window);
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    #endif
	/* Cada vez que se produzca un evento de teclado o ratón, se llamará a las funciones creadas arriba */
	glfwSetKeyCallback(global_window, key_callback);
	glfwSetCursorPosCallback(global_window, cursor_position_callback);
	glfwSetMouseButtonCallback(global_window, mouse_button_callback);
	glfwSetWindowSizeCallback(global_window, window_resize_callback);
	/* ------------------------------------------------------------------------------------------- */

	/* Init glew to have access to GL extensions */
	glewExperimental = GL_TRUE;
	glewInit();

	/* Obtenemos una instancia del juego (Clase singleton) y la inicializamos */
	Game::instance().init();
	timePreviousFrame = glfwGetTime();

	#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
    #else
        // A Windows, fem servir el bucle while tradicional
        while (!glfwWindowShouldClose(global_window))
        {
            main_loop();
        }

        glfwTerminate();
    #endif
	return 0;
}
