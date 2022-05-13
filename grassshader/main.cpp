#include <iostream>
#include "globals.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "model.h"

//screen settings
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

//timing
float deltaTime = 0.0f;
float lastTime = 0.0f;


//Positions
struct BugPosition{
	float x;
	float y;
	float z;
	float random;
}bugPosition;
BugPosition bugPositions[500];
void generateBugPositions() {
	for (int i = 0; i < 500; i++) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float range = 1 - (-1);
		float x = (random * range) + (-1);

		random = ((float)rand()) / (float)RAND_MAX;
		range = 1 - (0);
		float y = (random * range) + (0);

		random = ((float)rand()) / (float)RAND_MAX;
		range = 1 - (-1);
		float z = (random * range) + (-1);

		random = ((float)rand()) / (float)RAND_MAX;
		range = 1 - (-1);
		float rand = (random * range) + (-1);

		BugPosition tmp;
		tmp.x = x; tmp.y = y; tmp.z = z; tmp.random = rand;
		bugPositions[i] = tmp;
	}
}

void main() {
	std::srand(glfwGetTime());
	generateBugPositions();
	//intialize GLFW & GLAD
	INIT();

	//Camera Class
	Camera camera(window, glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, SCR_WIDTH / 2, SCR_HEIGHT / 2);
	gCamera = &camera;//update the global camera variable

	//#######################################################################################################################
	//load the model here
	Shader ourShader("Shaders/grass.vs", "Shaders/grass.fs");
	ourShader.activateGeometryShader("Shaders/grass.gs");
	Shader planeShader("Shaders/monkey.vs", "Shaders/monkey.fs");
	Model ourModel("resources/grassv3/grass.obj");

	Shader streetLightShader("Shaders/streetlight.vs", "Shaders/streetlight.fs");
	Model monkey("resources/monkeyv2/monkey.obj");
	//Model streetlight("resources/streetlight/streetlight.obj");
	Shader cartShader("Shaders/cart.vs", "Shaders/cart.fs");
	Model cart("resources/cart/cart.obj");
	Shader bugShader("Shaders/bug.vs", "Shaders/bug.fs");
	Model bug("resources/bug/bug.obj");
	
	//projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	//set projection for 3D MODEL
	ourShader.use();
	glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	planeShader.use();
	glUniformMatrix4fv(glGetUniformLocation(planeShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	streetLightShader.use();
	glUniformMatrix4fv(glGetUniformLocation(streetLightShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	cartShader.use();
	glUniformMatrix4fv(glGetUniformLocation(cartShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	bugShader.use();
	glUniformMatrix4fv(glGetUniformLocation(bugShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//CONSTANT UNIFORMS
	ourShader.use();
	//ATTENUATION
	glUniform1f(glGetUniformLocation(ourShader.ID, "constant"),1.0f);
	glUniform1f(glGetUniformLocation(ourShader.ID, "linear"), 0.09f);
	glUniform1f(glGetUniformLocation(ourShader.ID, "quadratic"), 0.032f);
	//SPOTLIGHT
	glUniform1f(glGetUniformLocation(ourShader.ID, "cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(ourShader.ID, "outerCutOff"), glm::cos(glm::radians(17.5f)));

	//render Loop
	glEnable(GL_DEPTH_TEST);//enable depth testing
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(window)) {
		//get time
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//process user input
		processInput(window,deltaTime);

		//all render stuff goes here
		glClearColor(0, 0.0f, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Update view Matrix
		glm::mat4 view = glm::lookAt(gCamera->position, gCamera->position + gCamera->front, gCamera->up);
		//set view for 3D MODEL
		ourShader.use();
		glUniform1f(glGetUniformLocation(ourShader.ID,"time"),glfwGetTime());
		glUniform3fv(glGetUniformLocation(ourShader.ID, "cameraPos"), 1, glm::value_ptr(gCamera->position));
		glUniform3fv(glGetUniformLocation(ourShader.ID, "spotlightDirection"), 1, glm::value_ptr(glm::vec3(0.0f,-1.0f,0.0f)));
		//glUniform3fv(glGetUniformLocation(ourShader.ID, "spotlightDirection"), 1, glm::value_ptr(gCamera->front));
		//glUniform3fv(glGetUniformLocation(ourShader.ID, "lightPos"), 1, glm::value_ptr(gCamera->position));
		glUniform3fv(glGetUniformLocation(ourShader.ID, "lightPos"), 1, glm::value_ptr(glm::vec3(0.0f,2.0f,0.0f)));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

		planeShader.use();
		glUniformMatrix4fv(glGetUniformLocation(planeShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		streetLightShader.use();
		glUniformMatrix4fv(glGetUniformLocation(streetLightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		cartShader.use();
		glUniformMatrix4fv(glGetUniformLocation(cartShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		bugShader.use();
		glUniformMatrix4fv(glGetUniformLocation(bugShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));


		//draw stuff here

		//update and render 3D MODEL
		ourShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(ourShader);
		//monkey.Draw(ourShader);

		planeShader.use();
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(planeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(planeShader);

		streetLightShader.use();
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(streetLightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//streetlight.Draw(planeShader);

		cartShader.use();
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(2.0f,0.0f,-1.0f));
		glUniformMatrix4fv(glGetUniformLocation(cartShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		cart.Draw(cartShader);

		for (int i = 0; i < 200; i++) {
			bugShader.use();
			glm::mat4 bmodel = glm::mat4(1.0f);
			bmodel = glm::translate(bmodel, glm::vec3(bugPositions[i].x, bugPositions[i].y, bugPositions[i].z));
			bmodel = glm::scale(bmodel, glm::vec3(0.05f, 0.05f, 0.05f));
			glUniform1f(glGetUniformLocation(bugShader.ID, "time"), glfwGetTime());
			glUniform1f(glGetUniformLocation(bugShader.ID, "random"), bugPositions[i].random);
			glUniformMatrix4fv(glGetUniformLocation(bugShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(bmodel));
			bug.Draw(bugShader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//exit
	glfwTerminate();
	return;
}
