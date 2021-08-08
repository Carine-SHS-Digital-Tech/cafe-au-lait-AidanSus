#include "common.h"


int main()
{

	//Init glfw
	if (!glfwInit())
	{

		printf("Failed to initialize GLFW\n");

		return -1;

	}
		
	//Hint to GLFW what version we're using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create our window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Cafe Au Lait", nullptr, nullptr);

	if (window == nullptr)
	{

		printf_s("Failed to create window! Terminating...\n");

		glfwTerminate();
		return -1;

	}
	//Associate context with the new window
	glfwMakeContextCurrent(window);

	//Dynamically link OpenGL with glad
	gladLoadGL();
	//Create the viewport
	glViewport(0, 0, 1280, 720);


	unsigned int shader_program = shader::create_shader();
	glUseProgram(shader_program);

	gui::init(window);

	//Update by swapping front and back buffers.
	glfwSwapBuffers(window);

	//Main window loop
	while (!glfwWindowShouldClose(window))
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		gui::render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		//Process all pending window events
		glfwPollEvents();

	}

	//Destroy ImGui and it's contexts
	gui::destroy();

	glDeleteProgram(shader_program);

	//Exit and destroy
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}