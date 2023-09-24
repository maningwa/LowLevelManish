#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

SDL_Window * gWindow ;
SDL_GLContext context ;
SDL_Event evt;
bool running = true;
GLfloat vertices[] = {
		 0.5f,	 0.5f,	 0.0f,
		-0.5f,	 0.5f,	 0.0f,
		-0.5f,	-0.5f,	 0.0f,
		-0.5f,	 0.5f,	 0.0f,
		 0.5f,	 0.5f,	-0.5f,
		-0.5f,	 0.5f,	-0.5f,
		-0.5f,	-0.5f,	-0.5f,
		-0.5f,	 0.5f,	-0.5f
		};
GLuint indices [] ={
		 1,	2,	4,
		 4,	2,	4,
		 5,	1,	8,
		 8,	1,	4,
		 6,	5,	7,
		 7,	5,	8,
		 2,	1,	6,
		 6,	1,	5,
		 4,	3,	8,
		 8,	1,	4
		 };

std::string getSource(const char * filename){
	std::ifstream file(filename,std::ios::binary);
	if(file){
		std::string contents;
		file.seekg(0,std::ios::end);
		contents.resize(file.tellg());
		file.seekg(0,std::ios::beg);
		file.read(&contents[0],contents.size());
		file.close();
		return contents;
	}throw(errno);
};

int main(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){printf("Failed to initialise SDL\n");return 1;};
	
	gWindow =SDL_CreateWindow("Comprehension run",0,0,600,600,SDL_WINDOW_OPENGL);
	if(gWindow==nullptr){printf("SDL failed to create Window Context\n"); return 1;};
	
	context = SDL_GL_CreateContext(gWindow);
	if(context<0){printf("Failed to initialise OpenGL context\n");return 1;};
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);	
	gladLoadGL();
	
	std::string verttext = getSource("vert.c");
	const char * vertsrc = verttext.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertsrc,NULL);
	glCompileShader(vertexShader);

	GLint vertSuccess;
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&vertSuccess);

	if(!vertSuccess){
		GLchar infoLog[1024];
		glGetShaderInfoLog(vertexShader,1024,NULL,infoLog);
		printf("VertexShader Compilation error:\n%s\n",infoLog);
	}
	std::string fragtext = getSource("frag.c");
	const char * fragsrc = fragtext.c_str();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragsrc,NULL);
	glCompileShader(fragmentShader);

	GLint fragSuccess;
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&vertSuccess);

	if(!fragSuccess){
		GLchar infoLog[1024];
		glGetShaderInfoLog(fragmentShader,1024,NULL,infoLog);
		printf("FragmentShader Compilation error:\n%s\n",infoLog);
	};

	GLuint ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram,vertexShader);
	glAttachShader(ShaderProgram,fragmentShader);
	glLinkProgram(ShaderProgram);

	glUseProgram(ShaderProgram);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint VAO,VBO,EBO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices)/sizeof(vertices[0]),vertices,GL_STATIC_DRAW);

	glGenBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices)/sizeof(indices[0]),indices,GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
		
	SDL_Delay(5000);


	while(SDL_PollEvent(&evt)){
		if(evt.type == SDL_QUIT){
			running = false;
		};

		glClearColor(0.02,0.03,0.07,1.00);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawElements(GL_LINES,0,sizeof(indices)/sizeof(indices[0]),0);
		SDL_GL_SwapWindow(gWindow);
	}

	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1,&EBO);
	
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}
