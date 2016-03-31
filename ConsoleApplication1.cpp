
#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
// Header file 
#include <glm/gtc/type_ptr.hpp>
#include <GLM/glm.hpp>
// Include GLM core features
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp>  
// Include GLM extensions  
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

GLuint program;
GLint attribute_coord2d;

  const char *vs_source =
  "#version 410\n"
  "layout (location = 0) in vec3 position;"
  "layout (location = 1) in vec4 color;"
  "out vec4 vertexColor;"
  "uniform mat4 model;"
  "uniform mat4 view;"

  "uniform mat4 projection;"

  "void main(void){"
  "vec4 v = vec4(position, 1.0f);"
  "gl_Position = projection * view * model * v;" 
  "vertexColor = color;"
  "}";


  const char *fs_cource =
  "#version 410\n"
  "in vec4 vertexColor;" 
  "out vec4 color;"
  "void main(void){"
  "color =vertexColor;"
  "}";

int init_resources()
{
  GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
  
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {

      GLint infoLogLength;
      glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(vs, infoLogLength, NULL, strInfoLog);

      fprintf(stderr, "Compile failure in shader:%s\n", strInfoLog);
      delete[] strInfoLog;
      fprintf(stderr, "error in vertex error\n");
      return 0;
    }
  
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fs, 1, &fs_cource, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {
      GLint infoLogLength;
      glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(fs, infoLogLength, NULL, strInfoLog);

      fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
      delete[] strInfoLog;

      fprintf(stderr, "error in fragment shader\n");
      return 0;
    }
  
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok)
    {
      fprintf(stderr, "glinkprogram\n");
      return 0;
    }

  return 1;
}

void onDisplay()
{

  //构建一个立方体需要36个点，每个正方形有6个面，每个面由2个三角形组成，
  //3个连续的点构成一个三角形
  //第一个纯色立方体和每个顶点的颜色值
  //点（-1，-1，-1）的颜色为（0.5，0，0）；点（-1，-1，1）的颜色为（0.5，0.5，0）；点（-1，1，1）的颜色为（0.5，0，0.5）
  //点（1，1，-1）的颜色为（0.5，0.5，0.5）；点（-1，1，-1）的颜色为（0，0.5，0）；点（1，1，1）的颜色为（0，0，0.5）
  //点（1，-1，1）的颜色为（0，0.5，0.5）；点（1，-1，-1）的颜色为（0.2，0.2，0.2）
  /*
	GLfloat alpha = 0.5f;
	GLfloat points1[] = {
     -1.0f,-1.0f,-1.0f,  0.5f, 0.0f, 0.0f, alpha,  //第一个三角形开始的点
     -1.0f,-1.0f, 1.0f,  0.5f, 0.5f, 0.0f, alpha,
     -1.0f, 1.0f, 1.0f,  0.5f, 0.0f, 0.5f, alpha, //第一个三角形结束的点  
      1.0f, 1.0f,-1.0f,  0.5f, 0.5f, 0.5f, alpha, //第二个三角形开始的点  
     -1.0f,-1.0f,-1.0f,  0.5f, 0.0f, 0.0f, alpha,
     -1.0f, 1.0f,-1.0f,  0.0f, 0.5f, 0.0f, alpha, // 第二个三角形结束的点
      1.0f,-1.0f, 1.0f,  0.0f, 0.5f, 0.5f, alpha,
	 -1.0f,-1.0f,-1.0f,  0.5f, 0.0f, 0.0f, alpha, 
	  1.0f,-1.0f,-1.0f,  0.2f, 0.2f, 0.2f, alpha, // 
      1.0f, 1.0f,-1.0f,  0.5f, 0.5f, 0.5f, alpha, 
	  1.0f,-1.0f,-1.0f,  0.2f, 0.2f, 0.2f, alpha,
	 -1.0f,-1.0f,-1.0f,  0.5f, 0.0f, 0.0f, alpha, //
	 -1.0f,-1.0f,-1.0f,  0.5f, 0.0f, 0.0f, alpha,
	 -1.0f, 1.0f, 1.0f,  0.5f, 0.0f, 0.5f, alpha,
	 -1.0f, 1.0f,-1.0f,  0.0f, 0.5f, 0.0f, alpha, //
	  1.0f,-1.0f, 1.0f,  0.0f, 0.5f, 0.5f, alpha,
	 -1.0f,-1.0f, 1.0f,  0.5f, 0.5f, 0.0f, alpha,
	 -1.0f,-1.0f,-1.0f,  0.5f, 0.0f, 0.0f, alpha, //
	 -1.0f, 1.0f, 1.0f,  0.5f, 0.0f, 0.5f, alpha,
	 -1.0f,-1.0f, 1.0f,  0.5f, 0.5f, 0.0f, alpha,
	  1.0f,-1.0f, 1.0f,  0.0f, 0.5f, 0.5f, alpha, //
	  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, alpha, 
	  1.0f,-1.0f,-1.0f,  0.2f, 0.2f, 0.2f, alpha, 
	  1.0f, 1.0f,-1.0f,  0.5f, 0.5f, 0.5f, alpha, //
	  1.0f,-1.0f,-1.0f,  0.2f, 0.2f, 0.2f, alpha, 
	  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, alpha, 
	  1.0f,-1.0f, 1.0f,  0.0f, 0.5f, 0.5f, alpha, //
	  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, alpha, 
	  1.0f, 1.0f,-1.0f,  0.5f, 0.5f, 0.5f, alpha,
	 -1.0f, 1.0f,-1.0f,  0.0f, 0.5f, 0.0f, alpha, //
	  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, alpha,
	 -1.0f, 1.0f,-1.0f,  0.0f, 0.5f, 0.0f, alpha,
	 -1.0f, 1.0f, 1.0f,  0.5f, 0.0f, 0.5f, alpha, //
      1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.5f, alpha,
	 -1.0f, 1.0f, 1.0f,  0.5f, 0.0f, 0.5f, alpha, 
	  1.0f,-1.0f, 1.0f,  0.0f, 0.5f, 0.5f, alpha
	  
  };
  */

	/***************************************************************
	 索引方式画立方体
     **************************************************************/

	//第一个立方体的顶点数据
	//透明度
	GLfloat alpha1 = 1.0f;
	GLfloat points1[] = {
		-1.0f, -1.0f, -1.0f,      0.5f, 0.0f, 0.0f,alpha1,    //0
		-1.0f, -1.0f,  1.0f,      0.5f, 0.5f, 0.0f,alpha1,    //1
		-1.0f,  1.0f,  1.0f,      0.5f, 0.0f, 0.5f,alpha1,    //2
		 1.0f,  1.0f, -1.0f,      0.5f, 0.5f, 0.5f,alpha1,	  //3
		-1.0f,  1.0f, -1.0f,      0.0f, 0.5f, 0.0f,alpha1,	  //4
		 1.0f,  1.0f,  1.0f,      0.0f, 0.0f, 0.5f,alpha1,	  //5
		 1.0f, -1.0f,  1.0f,      0.0f, 0.5f, 0.5f,alpha1,	  //6
		 1.0f, -1.0f, -1.0f,      1.0f, 1.0f, 1.0f,alpha1     //7
	};

	//第二个立方体的顶点数据
	GLfloat alpha2 = 0.3f;
	GLfloat points2[] = {
		-1.0f, -1.0f, -1.0f,      0.5f, 0.0f, 0.0f,alpha2,    //0
		-1.0f, -1.0f,  1.0f,      0.5f, 0.5f, 0.0f,alpha2,    //1
		-1.0f,  1.0f,  1.0f,      0.5f, 0.0f, 0.5f,alpha2,    //2
		 1.0f,  1.0f, -1.0f,      0.5f, 0.5f, 0.5f,alpha2,	  //3
		-1.0f,  1.0f, -1.0f,      0.0f, 0.5f, 0.0f,alpha2,	  //4
		 1.0f,  1.0f,  1.0f,      0.0f, 0.0f, 0.5f,alpha2,	  //5
		 1.0f, -1.0f,  1.0f,      0.0f, 0.5f, 0.5f,alpha2,	  //6
		 1.0f, -1.0f, -1.0f,      0.2f, 0.2f, 0.2f,alpha2     //7
	};

	GLuint index[] = {

	     0, 1, 2,  //左侧下
		 0, 2, 4,  //左侧上		
		 5, 7, 3,  //右面上
		 7, 5, 6,  //右面下
		 6, 1, 0,  //底面左
		 6, 0, 7,  //底面右	
		 3, 0, 4,  //正面上
		 3, 7, 0,  //正面下 
		 5, 2, 6,  //背面上
		 2, 1, 6,  //背面下
		 5, 3, 4,  //上面右
		 5, 4, 2   //上面左
	};

  //用于存储第一个立方体的顶点数据
  GLuint vbo1 = 0;
  glGenBuffers(1, &vbo1);
  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  //存储第二个立方体的顶点数据
  GLuint vbo2 = 0;
  glGenBuffers(1, &vbo2);
  glBindBuffer(GL_ARRAY_BUFFER, vbo2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  //绑定第一个vao
  GLuint vao1 = 0;
  glGenVertexArrays(1, &vao1);
  glBindVertexArray(vao1);
  glEnableVertexAttribArray(0);   
  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
  //绑定第二个vao
  GLuint vao2 = 0;
  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);
  glEnableVertexAttribArray(0);   
  glBindBuffer(GL_ARRAY_BUFFER, vbo2);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

 

   //创建平移
   glm::mat4 view;
   
   glm::mat4 projection;
   view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f));
   projection = glm::perspective(glm::radians(20.0f), (GLfloat)640 / (GLfloat)480, 0.1f, 100.0f);
   // Get their uniform location
   GLint modelLoc = glGetUniformLocation(program, "model");
   GLint viewLoc = glGetUniformLocation(program, "view");
   GLint projLoc = glGetUniformLocation(program, "projection");
   // 将矩阵传给shader
   glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
   // Note: currently we set the projection matrix each frame, but since the projection matrix 
   //rarely changes it's often best practice to set it outside the main loop only once.
   glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   
   
   //平移
   glm::vec3 cubePositions[] = {
		
	glm::vec3(-0.2f, -0.2f, -2.5f),     //靠前的
	glm::vec3( 0.4f,  0.0f,  4.0f)      //靠后的,改了z的值为正则该物体会在第一个前方,用于测试深度测试的参数

   };
   
   //绑定第一个vao
   //注：平移矩阵是叠加的，角度也是叠加的
   glm::mat4 model;
   model = glm::translate(model, cubePositions[0]);
   GLfloat angle;
   angle = 0.2f;
   model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   glBindVertexArray(vao1);
   // glDrawArrays(GL_TRIANGLES, 0, 36);
   //第一个参数是图元的类型，第二个参数是索引数组中索引的数量，
   //第三个参数是索引数组的类型，最后一个参数是索引数组的地址
   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,index);
   glUseProgram(program);
   //glBindVertexArray(0);

   model = glm::translate(model, cubePositions[1]);
   //angle = 0.0f;
   //model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   glBindVertexArray(vao2);
   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,index);
   
   glBindVertexArray(0);

  //glDrawArrays(GL_TRIANGLES, 0, 3*12);  //第三个参数表示顶点的数目
  
}

void free_resources()
{
  glDeleteProgram(program);
}

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{

  
  
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
    {
    glfwTerminate();
    exit(EXIT_FAILURE);
    }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK )
    {
      //      throw std::runtime_error("glew fails to start.");
      fprintf(stderr, "glew error\n");
    }

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);

  //深度测试
  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS);  // depth-testing interprets a smaller value as "closer"
  //glDepthFunc (GL_ALWAYS);  //The depth test always passes，即后画的会覆盖前面的


  //face culling
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_FRONT);  
  //glCullFace(GL_BACK);
  //glFrontFace(GL_CW);

  //blending必须先画不透明的再画透明的
  glEnable(GL_BLEND); 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  glfwSetKeyCallback(window, key_callback);
  
  GLint maxV;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxV);
  fprintf(stderr, "maxv: %d\n", maxV);
  
  init_resources();
  
  while (!glfwWindowShouldClose(window))
    {
    
    onDisplay();
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    }
  free_resources();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}




