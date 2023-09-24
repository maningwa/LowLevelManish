#include <stdio.h>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>


std::string  getSource(const char * path){
	std::ifstream file(path,std::ios::binary);
	if(file){
		std::string contents;
		file.seekg(0,std::ios::end);
		contents.resize(file.tellg());
		file.seekg(0,std::ios::beg);
		file.read(&contents[0],contents.size());
		file.close();
		return contents;	
	}throw(errno);
}
int main(){
	//initialise the SDL video
	if(!SDL_Init(SDL_INIT_VIDEO)<0){
		printf("Failed to Initialise SDL_Video!!!");
		return 1;
	}

	//create Window COntext	
	SDL_Window * window = SDL_CreateWindow("Window",0,0,600,600,SDL_WINDOW_OPENGL);
	if(window <0){
		printf("Failed to Initialise Window!!!!");
		return 1;
	}

	//create the Opengl Context
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if(context < 0){
		printf("Failed to initialise OpenGL context");
		return 1;
	}
	

	//Set the relevant opengl Attributes for SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
		

	
	bool running = true;
	SDL_Event evt;

	//load opengl functions and attribs
	gladLoadGL();
	glViewport(0,0,600,600);

	//generate vertex shader
	//glShaderSource params 1=the shader id, 2= number of source (default is 1)\
	3=refference to the source code as a c_str(), 4,just place NULL for now
	std::string verttext = getSource("vert.c");
	const char * vertsrc = verttext.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertsrc,NULL);
	glCompileShader(vertexShader);
	

	//Error checking for shader compilation
	GLint vSuccess;
	//glGetShaderiv Params 1= Shader ID, 2= GL_COMPILE_STATUS, 3=referrence to success flag
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&vSuccess);
	
	if(!vSuccess){
		GLchar InfoLog[1024];
		glGetShaderInfoLog(vertexShader,1024,NULL,InfoLog);
		printf("Error Compiling Vertext Shader:\n %s\n",InfoLog);
	}
	
	std::string fragtext = getSource("frag.c");
	const char * fragsrc = fragtext.c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragsrc,NULL);
	glCompileShader(fragmentShader);

	GLint fSuccess; 
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&fSuccess);

	if(!fSuccess){
		GLchar InfoLog[1024];
		glGetShaderInfoLog(fragmentShader,1024,NULL,InfoLog);
		printf("Error Compiling Fragment Shader:\n %s\n",InfoLog);
	}

	GLuint ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram,vertexShader);
	glAttachShader(ShaderProgram,fragmentShader);

	glLinkProgram(ShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat  vertices[] ={
		-0.5 ,	-0.5 ,	0.0 ,	0.0 ,	1.0 ,	1.0 ,
		 0.5 ,	-0.5 ,	0.0 ,	0.0 ,	1.0 ,	1.0 ,
		 0.0 ,	 0.5 ,	0.0 ,	0.0 ,	1.0 ,	1.0 };


	GLuint VAO,VBO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices)*sizeof(GLfloat),vertices,GL_STATIC_DRAW);




	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(void*)0);



	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));


	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);



	glClearColor(0.01,0.02,0.07,1.00);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);



	while(running){
		while(SDL_PollEvent(&evt)){
			if(evt.type == SDL_QUIT)
				running = false;
		}
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,3);
		SDL_GL_SwapWindow(window);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1,&VAO);
	glDeleteProgram(ShaderProgram);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
};

