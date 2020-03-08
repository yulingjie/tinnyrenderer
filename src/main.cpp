#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <model.h>
#include <tga/tgaimage.h>
#include <linmath.h>
#include <vector>
#include <algorithm>
#include "shaders.h"
static const char* vertex_shader_text =
"#version 430\n"
"uniform mat4 MVP;\n"
"attribute vec2 vPos;\n"
//"varying vec2 texcoord;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP* vec4(vPos, 10.00, 1.0);\n"
//"    texcoord = vPos;\n"
"}\n";
static const char* fragment_shader_text =
"#version 430\n"
//"uniform sampler2D texture;\n"
//"uniform vec3 color;\n"
//"varying vec2 texcoord;\n"
"out vec4 FragColor;"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0,1.0,1.0,1.0);"
//"    gl_FragColor = vec4(color * texture2D(texture, texcoord).rgb, 1.0);\n"
"}\n";

static const vec2 vertices[4] =
{
	{ -0.5f, -0.5f },
	{ 0.f, -0.5f },
	{ 0.5f, 0.5f },
	{ 0.5f, -0.5f }
};
static const int Indices[] = { 0,3,1,
1,3,2,
2,3,0,
0,1,2 };
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);


}
const int W = 600;
const int H = 600;
const int WW = W;
const int WH = H;
const int DEPTH = 255;

void Draw(TGAImage& frame)
{

	GLPipeline pipeline;
	pipeline.SetWidth(W);
	pipeline.SetHeight(H);
	pipeline.SetLightDir(Vec3f(1, 1, 0));
	pipeline.Viewport(W / 8, H / 8, W * 3 / 4., H * 3 / 4., DEPTH);
	pipeline.LookAt(Vec3f(0, 0, 1), Vec3f(0, 0, 0), Vec3f(0, 1, 0));


	
	Model model("obj/african_head.obj");
	
	pipeline.Run(model, frame);

}


void InitCameraTransform(const Vec3f& Target, const Vec3f& Up)
{
	Vec3f N = Target;
	N.normalize();
	Vec3f U = Up;
	U = cross(U, Target);
	U.normalize();
	Vec3f V = cross(N, U);

	mat4x4 m;
	m[0][0] = U.x; m[0][1] = U.y; m[0][2] = U.z; m[0][3] = 0.0f;
	m[1][0] = V.x; m[1][1] = V.y; m[1][2] = V.z; m[1][3] = 0.0f;
	m[2][0] = N.x; m[2][1] = N.y; m[2][2] = N.z; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void InitPerspectiveProj(mat4x4& m)
{
	
	const float ar = (float)W / (float)H;
	const float zNear = 0.3f;
	const float zFar = 1000.f;
	const float zRange = zNear - zFar;
	const float fov = 60.;
	const float tanHalfFOV = tan(ToRadian(fov / 2.0));

	m[0][0] = 1.0f / (tanHalfFOV * ar);
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = 1.f / tanHalfFOV;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = (-zNear - zFar) / zRange;
	m[2][3] = 2.0f * zFar * zNear / zRange;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 1.0f;
	m[3][3] = 0.0f;

}

//void Draw(TGAImage& frame);
//void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);
//void line(Vec2i v0, Vec2i v1, TGAImage& image, TGAColor color);
//void triangle(Vec3f t0, Vec3f t1, Vec3f t2,Vec2f uv0, Vec2f uv1, Vec2f uv2, float * zbuffer, TGAImage& image, TGAColor color,const TGAImage& texture);
//void triangle(Vec3f* pts,Vec2i* uvs,float* itys,float * zbuffer, TGAImage& image, TGAColor color,const Model& model);
//Vec3f cross(Vec3f lhs, Vec3f rhs);
//void lookat(Vec3f eye, Vec3f center, Vec3f up);
int main(void)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(WW, WH, "Line", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	gladLoadGL(glfwGetProcAddress);

	glfwSwapInterval(1);

	
	GLuint program;
	{
		GLuint vertex_shader, fragment_shader;
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
		glCompileShader(vertex_shader);
		GLint success = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			fprintf(stderr, "compile vertex shader error");
			exit(EXIT_FAILURE);
		}
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
		glCompileShader(fragment_shader);
		
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);


		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}
	

	GLuint mvp_location = glGetUniformLocation(program, "MVP");
	//GLuint color_location = glGetUniformLocation(program, "color");
	//GLuint texture_location = glGetUniformLocation(program, "texture");
	GLuint vpos_location = glGetAttribLocation(program, "vPos");

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glUseProgram(program);
	//glUniform1i(texture_location, 0);
	float scale = 3.f;
	mat4x4 mat;
	InitPerspectiveProj(mat);

	glUniformMatrix4fv(mvp_location, 1, GL_TRUE, &mat[0][0]);
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
		sizeof(vertices[0]), (void*)0);

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	while (!glfwWindowShouldClose(window))
	{

		const vec3 colors = { 1.f,1.f,1.f };

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0, 0.0, 1.);

	//	mat4x4 mvp;
	//	mat4x4_ortho(mvp, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
	//	glUniformMatrix4fv(mvp_location, 1, GL_FALSE,
	//		(const GLfloat*)mvp);
	//	glUniform3fv(color_location, 1, colors);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT,0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	glfwTerminate();
	exit(EXIT_SUCCESS);

}

