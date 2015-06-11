#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <typeinfo>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shaderloader.hpp>
#include <mesh.h>
#include "objloader.cpp"
#include "texture.h"

using namespace glm;

GLFWwindow* window;

double Time;


double lastTime = glfwGetTime();
int nbFrames = 0;

void RefreshConsoleFPS(){
    Time = glfwGetTime();
    nbFrames++;
    if ( Time - lastTime >= 1.0 ){
        printf("%f ms/frame (= %i FPS)\n", 1000.0/double(nbFrames), nbFrames);
        nbFrames = 0;
        lastTime += 1.0;
    }
}

int main( void )
{



/********************* Formalities & Window Initialization ************************/
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1600, 900, "I'm an OpenGL Noob", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try on mashines with NV or ATi cards.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if ( glewInit() ) {                                                                 //GLEW_OK = 0
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }


    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	Mesh Chair_Legs("./Chair_Legs.obj");
	Mesh Chair_Body("./Chair_Body.obj");
	Texture Leather("./brown_fabric.jpg");
	Texture Macassar("./Macassar.jpg");





	GLuint ShaderPrograms = LoadShaders( "./vShader.vert", "./fShader.frag" );
    // Get a handle for MVP.
    GLuint handle_MVP = glGetUniformLocation(ShaderPrograms, "m4_MVP");
    GLuint handle_M = glGetUniformLocation(ShaderPrograms, "m4_M");
    GLuint handle_V = glGetUniformLocation(ShaderPrograms, "m4_V");
    GLuint handle_Light = glGetUniformLocation(ShaderPrograms, "v3_light_world");

    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint uvbuffer;

    GLuint CurrentTexture  = glGetUniformLocation(ShaderPrograms, "Texture_Chair");


/**************************** End: Window Initialization ******************************/



    do{

        RefreshConsoleFPS();

/******************************* MVP *******************************/

        // Projection matrix : 30 Degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(30.0f, 16.f / 9.f, 0.1f, 100.0f);
        //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

        glm::mat4 View         = glm::lookAt(
                    glm::vec3(20,40,30), // Camera Pos
                    glm::vec3(0,15,0), // Where to look at
                    glm::vec3(0,-1,0)  // "UP"
                    );

        glm::mat4 Model      = glm::mat4(
                    glm::vec4(cos(Time),0,sin(Time),0),
                    glm::vec4(0,1,0,0),
                    glm::vec4(sin(Time),0,-cos(Time),0),
                    glm::vec4(0,0,0,1)
                    );
//        glm::mat4 Model = glm::mat4(1);
        glm::mat4 MVP        = Projection * View * Model; // Reminder: Matrix multiplication is the other way around

        glm::vec3 Light = glm::vec3(0,100,30);

/**************************** End: MVP ********************************/


/*************************** Frame "Loop" *****************************/
        glClear( GL_COLOR_BUFFER_BIT );
        glUseProgram(ShaderPrograms);


        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);


        // Send the transformation to the currently bound shader in the "MVP" uniform
        glUniformMatrix4fv(handle_MVP, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(handle_M, 1, GL_FALSE, &Model[0][0]);
        glUniformMatrix4fv(handle_V, 1, GL_FALSE, &View[0][0]);
        glUniform3fv(handle_Light, 1, &Light[0]);


        Macassar.Bind();
        Chair_Legs.Draw();
        Leather.Bind();
        Chair_Body.Draw();


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
/********************** End: Frame Rendering ***********************/

/********************* Window Closing Condition *********************/

    while( glfwGetKey(window, GLFW_KEY_ENTER ) != GLFW_PRESS &&
           !glfwWindowShouldClose(window) );

/**************************  Clean-Ups  **************************/
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(ShaderPrograms);
    glDeleteTextures(1, &CurrentTexture);
    glDeleteVertexArrays(1, &VertexArrayID);

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}

