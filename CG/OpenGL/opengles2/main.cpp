//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this
//    example is to demonstrate the basic concepts of
//    OpenGL ES 2.0 rendering.
#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"
#include "vmath.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;
   int mWidth;
   int mHeight;

} UserData;

UserData  userData;

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
    return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled )
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char* infoLog = (char*)malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

///
// Initialize the shader and program object
//
GLint uniformLoc;
int Init ( ESContext *esContext )
{
   //esContext->userData = malloc(sizeof(UserData));

   UserData *userData = (UserData*)esContext->userData;
   char vShaderStr[] =
      "attribute vec4 vPosition;    \n"
      "uniform mat4 uMVPMatrix;     \n "
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = uMVPMatrix * vPosition;  \n"
      "}                            \n";

   char fShaderStr[] =
      "precision mediump float;\n"\
      "void main()                                  \n"
      "{                                            \n"
      "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
      "}                                            \n";

   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   programObject = glCreateProgram ( );

   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // Bind vPosition to attribute 0
   glBindAttribLocation ( programObject, 0, "vPosition" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked )
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char* infoLog =(char*) malloc (sizeof(char) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return GL_FALSE;
   }

   uniformLoc = glGetUniformLocation(programObject, "uMVPMatrix");
   printf("uniformLoc:%d\n", uniformLoc);

   // Store the program object
   userData->programObject = programObject;

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   return GL_TRUE;
}

void Update(ESContext*esContext, float deltatime)
{
    vmath::mat4 mat = vmath::ortho(0, userData.mWidth, -userData.mHeight, 0, 0, 1.0f);
    vmath::mat4 scaleMat = vmath::scale(1.0f - deltatime, 1.0f, 0.8f);
    mat *= scaleMat;
    glUniformMatrix4fv(uniformLoc, 1, false, (GLfloat*)&mat);
    int ret = glGetError();
    if (ret != GL_NO_ERROR) {
        printf("ret: %d\n", ret);
    }
    //printf("deltatime:%f\n", deltatime);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData*)esContext->userData;
   GLfloat vVertices[] = {  0,  0,
       userData->mWidth, 0,
       0, -userData->mHeight,
       userData->mWidth, -userData->mHeight                    
   };
                          

   // Set the viewport
   //glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex data
   glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
}

int main ( int argc, char *argv[] )
{
   ESContext esContext;
   userData.mWidth = 640;
   userData.mHeight = 480;

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Hello Triangle", userData.mWidth, userData.mHeight, ES_WINDOW_RGB );

   if ( !Init ( &esContext ) )
      return 0;

   esRegisterUpdateFunc(&esContext, Update);
   esRegisterDrawFunc ( &esContext, Draw );

   esMainLoop ( &esContext );
}
