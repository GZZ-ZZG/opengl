
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
//窗口的大小
GLint width = 640, height = 480;
//model矩阵的平移向量
glm::vec3 cubePositions[] = {
	glm::vec3(-1.8f, -0.2f, -8.5f),       //紫红色的物体，即第一个画的物体
	glm::vec3( 0.4f,  0.0f, -5.5f)        //黄色的物体，即第二个画的物体
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
  //第二参数指定了源码中有多少个字符串
  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  //检测调用glCompileShader后是否编译成功了
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
  
  //删除着色器对象
  glDeleteShader(vs);
  glDeleteShader(fs);
  return 1;
}

//用于把投影矩阵和视图矩阵传给ray casting
glm::mat4 get_projection (glm::mat4 projection){
	return projection;
}
glm::mat4 get_view (glm::mat4 view){
	return view;
}


void onDisplay()
{

  //构建一个立方体需要36个点，每个正方形有6个面，每个面由2个三角形组成，
  //3个连续的点构成一个三角形
  //第一个纯色立方体和每个顶点的颜色值
  //点（-1，-1，-1）的颜色为（0.5，0，0）；点（-1，-1，1）的颜色为（0.5，0.5，0）；点（-1，1，1）的颜色为（0.5，0，0.5）
  //点（1，1，-1）的颜色为（0.5，0.5，0.5）；点（-1，1，-1）的颜色为（0，0.5，0）；点（1，1，1）的颜色为（0，0，0.5）
  //点（1，-1，1）的颜色为（0，0.5，0.5）；点（1，-1，-1）的颜色为（0.2，0.2，0.2）

	/***************************************************************
	 索引方式画立方体
     **************************************************************/

	//第一个立方体的顶点数据
	//透明度
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

	//第二个立方体的顶点数据
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
  //把顶点数组复制到缓冲中提供给OpenGL使用
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
  //解释顶点数据，设置顶点属性指针
  //第一个参数指定把数据传到哪个属性中
  //第二个参数指定顶点属性的大小，3即表明该属性是由3个数值组成
  //第三个参数指定数据的类型
  //第四个参数定义是否希望参数被标准化，即所有数据会被映射到0（对于有符号型signed数据是-1和1之间
  //第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性之间间隔有多少
  //第六个参数表示我们的位置数据在缓冲中起始位置的偏移量
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)0);
  //开启顶点属性，把顶点属性位置值作为它的参数
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  //解绑VAO
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

   //创建视图矩阵（与摄像机有关），投影矩阵以及模型矩阵
   glm::mat4 view;  
   glm::mat4 projection;
   //摄像机的相关参数
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
   //找到着色器中uniform的索引/地址。当我们得到uniform的索引/地址后，我们就可以更新它的值了
   GLint modelLoc = glGetUniformLocation(program, "model");
   GLint viewLoc = glGetUniformLocation(program, "view");
   GLint projLoc = glGetUniformLocation(program, "projection");
   //激活这个程序对象，激活了的着色器程序的着色器，在调用渲染函数时才可用
   //更新一个uniform之前必须使用着色器程序，因为它是在当前激活的着色器程序中设置uniform的
   glUseProgram(program);
   //改变相应矩阵的值，将矩阵传给shader
   glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
  
   //绑定第一个vao
   glm::mat4 model;
   model = glm::translate(model, cubePositions[0]);
   model = glm::rotate(model,glm::radians((GLfloat)glfwGetTime() * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   glUseProgram(program);

   glBindVertexArray(vao1);
   //用线框模式绘制三角形
   //第一个参数指应用到所有的三角形的前面和背面，第二个参数指用线来绘制
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   //glDrawArrays(GL_TRIANGLES, 0, 36);
   //第一个参数是图元的类型，第二个参数是索引数组中索引的数量，
   //第三个参数是索引数组的类型，最后一个参数是索引数组的地址(当使用EBO对象时指定EBO中的偏移量)
   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,index);
   glBindVertexArray(0);

   glm::mat4 model2;
   model2 = glm::translate(model2, cubePositions[1]);
   model2 = glm::rotate(model2, - (glm::radians((GLfloat)glfwGetTime()* 5.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
   glBindVertexArray(vao2);
   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,index);
   glBindVertexArray(0);
   //glDrawArrays(GL_TRIANGLES, 0, 3*12);  //第三个参数表示顶点的数目

   projection1 = get_projection(projection);
   view1 = get_view(view);


}


//---------------------------------------------------------------------------
//	 该函数用于获取ray向量，即两个端点的坐标v1, v2
//   v1表示近平面的坐标，v2表示远平面的坐标
//   viewport表示视口区域
//   利用glm::unProject函数来将窗口的二维坐标转换为世界坐标系中的三维坐标
//---------------------------------------------------------------------------

void Get3DRayUnderMouse(glm::vec3* v1, glm::vec3* v2, double xpos, double ypos)
{	
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f,width, height);
	*v1 = glm::unProject(glm::vec3(float(xpos), height-float(ypos), 0.0f), view1, projection1, viewport);
	*v2 = glm::unProject(glm::vec3(float(xpos), height-float(ypos), 1.0f), view1, projection1, viewport);
}

//----------------------------------------------------------------------------
//	该函数用于检测ray是否穿过包围cube的球体
//  vA表示ray的起点坐标，vB表示ray的终点坐标
//  先找到在ray向量上离球体最近的点，然后比较该点到球体的中心距离与半径的大小
//  来确定ray是否穿过了该球体
//----------------------------------------------------------------------------
bool RaySphereCollision(glm::vec3 vSphereCenter, float fSphereRadius, glm::vec3 vA, glm::vec3 vB)
{
	// 创建一个向量从点vA到球的中心
	glm::vec3 vDirToSphere = vSphereCenter - vA;
	// 求从点vA到vB的单位向量
	glm::vec3 vLineDir = glm::normalize(vB-vA);
	// 求ray向量的长度
	float fLineLength = glm::distance(vA, vB);
	// 利用点乘将vDirToSphere向量投影到ray的单位向量上
	float t = glm::dot(vDirToSphere, vLineDir);
	glm::vec3 vClosestPoint;
	// 如果投影的距离的小于或等于0，即ray向量与vDirToSphere向量的夹角大于90度，此时最近的点是vA
	if (t <= 0.0f)
		vClosestPoint = vA;
	// 如果投影的距离大于ray向量的长度，则最近的点是vB
	else if (t >= fLineLength)
		vClosestPoint = vB;
	// 否则最近的点在ray向量上，求出该点的坐标
	else
		vClosestPoint = vA+vLineDir*t;
	// 返回ray向量上最近的点与球中心的距离是否小于或等于半径
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

//键盘的回调函数
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{ 
  //当按下escape键时关闭glfw窗口
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
 
}

//上一帧鼠标左键是否按下
//该变量主要是使每点击一次鼠标只输出一次，避免点击一次鼠标输出很多次
GLboolean wasleftbuttonpresslastframe = false;

//鼠标的回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos){

//----------------------------------------------------------------------------------
//   用raycasting的方法来picking cube
//   该方法是ray是否穿过一个包围cube的球体来检测鼠标是否点击了该cube
//   包围球体的直径的长度是cube的斜对角线
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
			 // 在eye space中z越大则物体越靠前
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
 //   用color buffer的方法来picking cube
 //-----------------------------------------------------------------------------------
/*
  if ( GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
   {   
	    
	if (wasleftbuttonpresslastframe == false)
	{
		glFlush();
		glFinish ();
		//窗口的坐标原点在左上角，由左向右x增加，由上至下y增加
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
  //初始化glfw
  if (!glfwInit())
    exit(EXIT_FAILURE);
  //配置GLFW
  //前两个函数的第一个参数是设置OpenGL的版本号
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //创建一个窗口，前两个参数表示窗口的宽和高，第三个是窗口的名字，后面两个参数可以忽略
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
    {
    glfwTerminate();
    exit(EXIT_FAILURE);
    }
  //make the context of our window the main context on the current thread
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
  //在使用任何的OpenGL函数之前先初始化glew
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
  //glEnable(GL_BLEND); 
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  //glfwSetMouseButtonCallback(window, mouse_callback1);

  GLint maxV;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxV);
  fprintf(stderr, "maxv: %d\n", maxV);
  
  init_resources();

  //game loop让该窗口不那么快关闭，而是让用户决定是否关闭窗口
  //渲染的命令应该放在这个循环里面
  while (!glfwWindowShouldClose(window))
  {
	
	//清空屏幕
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    onDisplay();	
	//交换两个color buffer，里面存放了glfw窗口的每个像素的颜色值
    glfwSwapBuffers(window);
    glfwPollEvents();

  }
  free_resources();
  
  glfwDestroyWindow(window);
  //释放被分配内存的所有资源
  glfwTerminate();
  exit(EXIT_SUCCESS);
}




