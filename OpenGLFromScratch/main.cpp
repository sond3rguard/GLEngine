#include <iostream>
#include <GL/glew.h>

/*
	TODO LIST:
	DECIDE ON MY PERSONAL NAMING CONVENTION! Full CamelCase ? Alternating camelCase ? under_scores ? 

	1) User input. 
		- Camera movement:
		   Learn how 'look at' and 'up' correlate. Seems that those two vector must be perpendicular at all times. Learn how to update this.
		- Object movement:
			Eventually I want a Player class to own a Drawable class, so that when a key input is detected, the player class can decide
			how to handle it, moving the player as necessary. (Player movement done simply by changing the transform, not velocity (???))

	3) Main game loop.
		I want to update this so that it runs on a timer, being independant of CPU/GPU speeds. 
		I want physics engine to run at 120 hz and framerate to be uncapped. (with max 120 i guess).

	4) General code cleanup / walkthrough.
		- Add tags for future improvements (@Robustness @Hardcoded @Cleanup @TODO)
		- Look for inefficiencies that can be improved (memory and timewise)
		- Make sure I understand everything, rewrite what is not mine, or use free libaries (obj_loader and stb_image).

    6) Find a way to initialise GLEW outside of Display.cpp so that if it fails we can close the program. (Do memory management too, deleting context etc).

    7) Where possible: overload functions that take a single glm::vec3 to also be able to take 3 floats. (for convineince)

    8) Find out how to draw UI elements. (Possible use a different shader).

	NOTES:
		To make the camera track an object, simply set its lookDirection to object.pos - cam.pos
*/

#include "main.h"
bool GLOBAL_shouldClose = false;

#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Drawable.h"

using std::cin;
using std::cout;
using std::endl;

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 800

int main(int argc, char *argv[]) {
	cout << "====== Starting Program... ======" << endl;

	//Craete the window and context.
	Display window(WINDOW_WIDTH, WINDOW_HEIGHT, "Main window.");

    //Create the main camera.
    Camera mainCamera(glm::vec3(0, 5, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), 70.0f, window.GetAspectRatio(), 0.01f, 1000.0f);

    //Set the camera for our drawable class as the main camera.
    Drawable::SetCamera(&mainCamera);

    //Create the basic shaders.
    Shader shader("./res/basicShader");

    //Load our tectures.
    Texture piranahs("./res/download.jpg");
    Texture bricks("./res/bricks.jpg");

    //Load the game object meshes.
	Mesh monkeyMesh("./res/monkey3.obj");

	Transform origin_transform;
    origin_transform.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

    //Create our drawable game objects.
    Drawable monkeyOneDrawable("Monkey Number One", &shader, &piranahs, &monkeyMesh);
    Drawable monkey_origin("Monkey Number Two", &shader, &bricks, &monkeyMesh, origin_transform);

    monkeyOneDrawable.GetTransform().SetPos(glm::vec3(-2, 0, 0));

	float counter = 0.0f;
	float sinCounter;
	float cosCounter;

	//The main loop!
	cout << "\nEntering main loop." << endl;
	while (!GLOBAL_shouldClose) {
        //Handle events. @Incomplete
        HandleSDLEvents();

		//A random counter to make things change!
		counter += 0.02f;
		sinCounter = sinf(counter);
		cosCounter = cosf(counter);

		//Clear the window.
		//window.Clear(cosCounter * sinCounter, cosCounter, sinCounter * sinCounter, cosCounter);
		window.Clear(0.5, 0.5, 0.5, 0.5);

        //Track monkeyOne position with camera.
        /*glm::vec3 monkeyOnePos = monkeyOneDrawable.GetTransform().GetPos();
        glm::vec3 camPos = mainCamera.GetPosition();
		mainCamera.SetLookDirection(monkeyOnePos - camPos);*/

		//Make the transform change based on the counter.
        monkeyOneDrawable.GetTransform().SetPos(glm::vec3(2, 0, sinCounter));

        //Draw our drawables.
        monkeyOneDrawable.Draw();
        monkey_origin.Draw();

		//Swap buffers.
		window.Update();
	}

	cout << "End of main loop." << endl;

	cout << "\nFreeing resources..." << endl;

	cout << "Deinitialising SDL..." << endl;
	SDL_Quit();

	cout << "====== Ending Program... ======" << endl;
	cin.get();
	return 0;
}

void FailAndExit(std::string message){
	cout << "\n======\nTHE PROGRAM HAS FAILED\n======\n" << message << endl;
	cin.get();
	exit(-1);
}