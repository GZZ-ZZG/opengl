
#include "stdafx.h"
#include <string>
#include <algorithm>
using namespace std;
//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <GLM/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp>   
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

GLuint program, programfb;
GLint attribute_coord2d;
GLfloat alpha1, alpha2, alpha = 0;
GLfloat pixel[3] = {0};
glm::mat4 view1, projection1;
//���ڵĴ�С
GLint width = 640, height = 480;
//model�����ƽ������
glm::vec3 cubePositions[] = {
	glm::vec3(-1.8f, -0.2f, -8.5f),       //�Ϻ�ɫ�����壬����һ����������
	glm::vec3( 0.4f,  0.0f, -5.5f)        //��ɫ�����壬���ڶ�����������
};

   
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
  "vertexColor = color/255;"
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
  //�ڶ�����ָ����Դ�����ж��ٸ��ַ���
  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  //������glCompileShader���Ƿ����ɹ���
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
  
  //ɾ����ɫ������
  glDeleteShader(vs);
  glDeleteShader(fs);
  return 1;
}

//���ڰ�ͶӰ�������ͼ���󴫸�ray casting
glm::mat4 get_projection (glm::mat4 projection){
	return projection;
}
glm::mat4 get_view (glm::mat4 view){
	return view;
}


void onDisplay()
{

  //����һ����������Ҫ36���㣬ÿ����������6���棬ÿ������2����������ɣ�
  //3�������ĵ㹹��һ��������
  //��һ����ɫ�������ÿ���������ɫֵ
  //�㣨-1��-1��-1������ɫΪ��0.5��0��0�����㣨-1��-1��1������ɫΪ��0.5��0.5��0�����㣨-1��1��1������ɫΪ��0.5��0��0.5��
  //�㣨1��1��-1������ɫΪ��0.5��0.5��0.5�����㣨-1��1��-1������ɫΪ��0��0.5��0�����㣨1��1��1������ɫΪ��0��0��0.5��
  //�㣨1��-1��1������ɫΪ��0��0.5��0.5�����㣨1��-1��-1������ɫΪ��0.2��0.2��0.2��

	/***************************************************************
	 ������ʽ��������
     **************************************************************/

	//��һ��������Ķ�������
	//͸����
	alpha1 = 255.0f;
	GLfloat points1[] = {
		-1.0f, -1.0f, -1.0f,      255, 0, 255,alpha1,     //0
		-1.0f, -1.0f,  1.0f,      255, 0, 255,alpha1,     //1
		-1.0f,  1.0f,  1.0f,      255, 0, 255,alpha1,     //2
		 1.0f,  1.0f, -1.0f,      255, 0, 255,alpha1, 	  //3
		-1.0f,  1.0f, -1.0f,      255, 0, 255,alpha1, 	  //4
		 1.0f,  1.0f,  1.0f,      255, 0, 255,alpha1, 	  //5
		 1.0f, -1.0f,  1.0f,      255, 0, 255,alpha1, 	  //6
		 1.0f, -1.0f, -1.0f,      255, 0, 255,alpha1,     //7
	};

	//�ڶ���������Ķ�������
	alpha2 = 0.3f*255;
	GLfloat points2[] = {
		-1.0f, -1.0f, -1.0f,      255, 255, 0, alpha2,    //0
		-1.0f, -1.0f,  1.0f,      255, 255, 0, alpha2,    //1
		-1.0f,  1.0f,  1.0f,      255, 255, 0, alpha2,    //2
		 1.0f,  1.0f, -1.0f,      255, 255, 0, alpha2,	  //3
		-1.0f,  1.0f, -1.0f,      255, 255, 0, alpha2,	  //4
		 1.0f,  1.0f,  1.0f,      255, 255, 0, alpha2,	  //5
		 1.0f, -1.0f,  1.0f,      255, 255, 0, alpha2,	  //6
		 1.0f, -1.0f, -1.0f,      255, 255, 0, alpha2,    //7
	};

	GLuint index[] = {

	     0, 1, 2,  //�����
		 0, 2, 4,  //�����		
		 5, 7, 3,  //������
		 7, 5, 6,  //������
		 6, 1, 0,  //������
		 6, 0, 7,  //������	
		 3, 0, 4,  //������
		 3, 7, 0,  //������ 
		 5, 2, 6,  //������
		 2, 1, 6,  //������
		 5, 3, 4,  //������
		 5, 4, 2   //������
	};

  //���ڴ洢��һ��������Ķ�������
  GLuint vbo1 = 0;
  glGenBuffers(1, &vbo1);
  //�Ѷ������鸴�Ƶ��������ṩ��OpenGLʹ��
  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  //�洢�ڶ���������Ķ�������
  GLuint vbo2 = 0;
  glGenBuffers(1, &vbo2);
  glBindBuffer(GL_ARRAY_BUFFER, vbo2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  //�󶨵�һ��vao
  GLuint vao1 = 0;
  glGenVertexArrays(1, &vao1);
  glBindVertexArray(vao1);
  glEnableVertexAttribArray(0);   
  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  //���Ͷ������ݣ����ö�������ָ��
  //��һ������ָ�������ݴ����ĸ�������
  //�ڶ�������ָ���������ԵĴ�С��3����������������3����ֵ���
  //����������ָ�����ݵ�����
  //���ĸ����������Ƿ�ϣ����������׼�������������ݻᱻӳ�䵽0�������з�����signed������-1��1֮��
  //�����������������(Stride)�������������������Ķ�������֮�����ж���
  //������������ʾ���ǵ�λ�������ڻ�������ʼλ�õ�ƫ����
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)0);
  //�����������ԣ��Ѷ�������λ��ֵ��Ϊ���Ĳ���
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  //���VAO
  glBindVertexArray(0);
  //�󶨵ڶ���vao
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

   //������ͼ������������йأ���ͶӰ�����Լ�ģ�;���
   glm::mat4 view;  
   glm::mat4 projection;
   //���������ز���
   glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
   glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
   glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
   glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
   glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
   view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
           glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3(0.0f, 1.0f, 0.0f));

   projection = glm::perspective(glm::radians(45.0f), (GLfloat)640 / (GLfloat)480, 0.1f, 100.0f);
   //�ҵ���ɫ����uniform������/��ַ�������ǵõ�uniform������/��ַ�����ǾͿ��Ը�������ֵ��
   GLint modelLoc = glGetUniformLocation(program, "model");
   GLint viewLoc = glGetUniformLocation(program, "view");
   GLint projLoc = glGetUniformLocation(program, "projection");
   //�������������󣬼����˵���ɫ���������ɫ�����ڵ�����Ⱦ����ʱ�ſ���
   //����һ��uniform֮ǰ����ʹ����ɫ��������Ϊ�����ڵ�ǰ�������ɫ������������uniform��
   glUseProgram(program);
   //�ı���Ӧ�����ֵ�������󴫸�shader
   glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
  
   //�󶨵�һ��vao
   glm::mat4 model;
   model = glm::translate(model, cubePositions[0]);
   model = glm::rotate(model,glm::radians((GLfloat)glfwGetTime() * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   glUseProgram(program);

   glBindVertexArray(vao1);
   //���߿�ģʽ����������
   //��һ������ָӦ�õ����е������ε�ǰ��ͱ��棬�ڶ�������ָ����������
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   //glDrawArrays(GL_TRIANGLES, 0, 36);
   //��һ��������ͼԪ�����ͣ��ڶ�������������������������������
   //������������������������ͣ����һ����������������ĵ�ַ(��ʹ��EBO����ʱָ��EBO�е�ƫ����)
   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,index);
   glBindVertexArray(0);

   glm::mat4 model2;
   model2 = glm::translate(model2, cubePositions[1]);
   model2 = glm::rotate(model2, - (glm::radians((GLfloat)glfwGetTime()* 5.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
   glBindVertexArray(vao2);
   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,index);
   glBindVertexArray(0);
   //glDrawArrays(GL_TRIANGLES, 0, 3*12);  //������������ʾ�������Ŀ

   projection1 = get_projection(projection);
   view1 = get_view(view);


}


//---------------------------------------------------------------------------
//	 �ú������ڻ�ȡray�������������˵������v1, v2
//   v1��ʾ��ƽ������꣬v2��ʾԶƽ�������
//   viewport��ʾ�ӿ�����
//   ����glm::unProject�����������ڵĶ�ά����ת��Ϊ��������ϵ�е���ά����
//---------------------------------------------------------------------------

void Get3DRayUnderMouse(glm::vec3* v1, glm::vec3* v2, double xpos, double ypos)
{	
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f,width, height);
	*v1 = glm::unProject(glm::vec3(float(xpos), height-float(ypos), 0.0f), view1, projection1, viewport);
	*v2 = glm::unProject(glm::vec3(float(xpos), height-float(ypos), 1.0f), view1, projection1, viewport);
}

//----------------------------------------------------------------------------
//	�ú������ڼ��ray�Ƿ񴩹���Χcube������
//  vA��ʾray��������꣬vB��ʾray���յ�����
//  ���ҵ���ray����������������ĵ㣬Ȼ��Ƚϸõ㵽��������ľ�����뾶�Ĵ�С
//  ��ȷ��ray�Ƿ񴩹��˸�����
//----------------------------------------------------------------------------
bool RaySphereCollision(glm::vec3 vSphereCenter, float fSphereRadius, glm::vec3 vA, glm::vec3 vB)
{
	// ����һ�������ӵ�vA���������
	glm::vec3 vDirToSphere = vSphereCenter - vA;
	// ��ӵ�vA��vB�ĵ�λ����
	glm::vec3 vLineDir = glm::normalize(vB-vA);
	// ��ray�����ĳ���
	float fLineLength = glm::distance(vA, vB);
	// ���õ�˽�vDirToSphere����ͶӰ��ray�ĵ�λ������
	float t = glm::dot(vDirToSphere, vLineDir);
	glm::vec3 vClosestPoint;
	// ���ͶӰ�ľ����С�ڻ����0����ray������vDirToSphere�����ļнǴ���90�ȣ���ʱ����ĵ���vA
	if (t <= 0.0f)
		vClosestPoint = vA;
	// ���ͶӰ�ľ������ray�����ĳ��ȣ�������ĵ���vB
	else if (t >= fLineLength)
		vClosestPoint = vB;
	// ��������ĵ���ray�����ϣ�����õ������
	else
		vClosestPoint = vA+vLineDir*t;
	// ����ray����������ĵ��������ĵľ����Ƿ�С�ڻ���ڰ뾶
	return glm::distance(vSphereCenter, vClosestPoint) <= fSphereRadius;
}



void free_resources()
{
  glDeleteProgram(program);
}

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

//���̵Ļص�����
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{ 
  //������escape��ʱ�ر�glfw����
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
 
}

//��һ֡�������Ƿ���
//�ñ�����Ҫ��ʹÿ���һ�����ֻ���һ�Σ�������һ���������ܶ��
GLboolean wasleftbuttonpresslastframe = false;

//���Ļص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos){

//----------------------------------------------------------------------------------
//   ��raycasting�ķ�����picking cube
//   �÷�����ray�Ƿ񴩹�һ����Χcube���������������Ƿ����˸�cube
//   ��Χ�����ֱ���ĳ�����cube��б�Խ���
//----------------------------------------------------------------------------------
if ( GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
	if (wasleftbuttonpresslastframe == false)
	{   
		bool isIntersection[2];
		GLfloat radius = 1.414;
		glm::vec3 v1;
		glm::vec3 v2;
		Get3DRayUnderMouse(&v1, &v2, xpos, ypos);
		for (int i = 0; i < 2; i++)
			isIntersection[i] = RaySphereCollision(cubePositions[i], radius, v1, v2);
		
		if (isIntersection[0] == true && isIntersection[1] == false)
			printf("cube1\n");
		else if (isIntersection[1] == true && isIntersection[0] == false)
			printf("cube2\n");
		else if (isIntersection[0] == true && isIntersection[1] == true){
			 // ��eye space��zԽ��������Խ��ǰ
			 if (cubePositions[0].z > cubePositions[1].z)
				 printf ("cube1\n");
			 else printf ("cube2\n");
		}
		else printf("no cube \n");
	
	wasleftbuttonpresslastframe = true;
  }
 }

   else if (GLFW_RELEASE == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
			wasleftbuttonpresslastframe = false;


 //-----------------------------------------------------------------------------------
 //   ��color buffer�ķ�����picking cube
 //-----------------------------------------------------------------------------------
/*
  if ( GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
   {   
	    
	if (wasleftbuttonpresslastframe == false)
	{
		glFlush();
		glFinish ();
		//���ڵ�����ԭ�������Ͻǣ���������x���ӣ���������y����
        //glfwGetCursorPos(window, &xpos, &ypos);
		glReadPixels((GLint)xpos,480-(GLint) ypos,1, 1, GL_RGB, GL_FLOAT, pixel);
		//printf("alpha: %f  %f  %f\n",pixel[0], pixel[1], pixel[2]);
		if (pixel[0] == 1.0f && pixel[1] == 0.0f && pixel[2] == 1.0f)
			printf ("the selection is : cube1\n");
		else if (pixel[0] == 1.0f && pixel[1] == 1.0f && pixel[2] == 0.0f)
			printf ("the selection is : cube2\n");
		else if (pixel[0] == 0.0f && pixel[1] == 0.0f && pixel[2] == 0.0f)
			printf ("the selection is : no cube\n");
		wasleftbuttonpresslastframe = true;
		}		
	}
  else if (GLFW_RELEASE == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
			wasleftbuttonpresslastframe = false;
*/
}

int main(void)
{
 
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  //��ʼ��glfw
  if (!glfwInit())
    exit(EXIT_FAILURE);
  //����GLFW
  //ǰ���������ĵ�һ������������OpenGL�İ汾��
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //����һ�����ڣ�ǰ����������ʾ���ڵĿ�͸ߣ��������Ǵ��ڵ����֣����������������Ժ���
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
    {
    glfwTerminate();
    exit(EXIT_FAILURE);
    }
  //make the context of our window the main context on the current thread
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
  //��ʹ���κε�OpenGL����֮ǰ�ȳ�ʼ��glew
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK )
    {
      //throw std::runtime_error("glew fails to start.");
      fprintf(stderr, "glew error\n");
    }

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);

  //��Ȳ���
  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS);  // depth-testing interprets a smaller value as "closer"
  //glDepthFunc (GL_ALWAYS);  //The depth test always passes�����󻭵ĻḲ��ǰ���


  //face culling
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_FRONT);  
  //glCullFace(GL_BACK);
  //glFrontFace(GL_CW);

  //blending�����Ȼ���͸�����ٻ�͸����
  //glEnable(GL_BLEND); 
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  //glfwSetMouseButtonCallback(window, mouse_callback1);

  GLint maxV;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxV);
  fprintf(stderr, "maxv: %d\n", maxV);
  
  init_resources();

  //game loop�øô��ڲ���ô��رգ��������û������Ƿ�رմ���
  //��Ⱦ������Ӧ�÷������ѭ������
  while (!glfwWindowShouldClose(window))
  {
	
	//�����Ļ
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    onDisplay();	
	//��������color buffer����������glfw���ڵ�ÿ�����ص���ɫֵ
    glfwSwapBuffers(window);
    glfwPollEvents();

  }
  free_resources();
  
  glfwDestroyWindow(window);
  //�ͷű������ڴ��������Դ
  glfwTerminate();
  exit(EXIT_SUCCESS);
}




