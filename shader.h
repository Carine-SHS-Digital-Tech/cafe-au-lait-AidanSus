#pragma once

namespace shader
{

	std::string read_shader_data(const std::string& file_path, const std::string& shader)
	{

		//Open shader file
		std::ifstream shader_file(file_path);
		//Create char buffer
		char current_line_buffer[256];
		std::string current_line;
		bool found{ false };

		std::stringstream shader_source;

		while (shader_file.getline(current_line_buffer, 256))
		{

			current_line = current_line_buffer;

			if (current_line.find("#shader") != std::string::npos)
			{

				if (current_line.find(shader) != std::string::npos)
					found = true;
				else if (found == true) break;

			}
			else if (found == true)
				shader_source << current_line << '\n';

		}

		return shader_source.str();

	}

	unsigned int compile_shader(unsigned int shader_type, const std::string shader_source)
	{

		unsigned int shader_id = glCreateShader(shader_type);
		const char* source = shader_source.c_str();
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);

		int result{ 0 };
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{

			//Get error length and add buffer
			int length{ 0 };
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
			char* error_message = reinterpret_cast<char*>(malloc(sizeof(char) * length));
			glGetShaderInfoLog(shader_id, length, &length, error_message);

			printf_s("Failed to compile %s\n", shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment");
			printf_s("%s\n", error_message);

			glDeleteShader(shader_id);
			return 0;

		}

		return shader_id;

	}

	unsigned int create_shader()
	{

		std::string vertex_shader_src = read_shader_data("Shaders/basic.shader", "vertex");
		std::string fragment_shader_src = read_shader_data("Shaders/basic.shader", "fragment");

		unsigned int shader_program = glCreateProgram();
		unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
		unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);

		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		glValidateProgram(shader_program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return shader_program;

	}

}