#define _USE_MATH_DEFINES
#include<cmath>
#include<sstream>
#include<GLTools.h>
#include<GLShaderManager.h>
#include<GLMatrixStack.h>
#include<GLFrustum.h>
#include<GLGeometryTransform.h>
#include<GL/freeglut.h>
#include <string>
#include <chrono>

int nWindow;
int nLoopMain = 0;
int nFov = 35;

GLShaderManager shaderManager;
GLMatrixStack modelStack;
GLMatrixStack projectionStack;

GLFrame cameraFrame;
GLFrame objectFrame;
GLFrustum viewFrustum;

GLGeometryTransform transformPipeline;

GLTriangleBatch sunBatch;
GLTriangleBatch earthBatch;
GLTriangleBatch moonBatch;

boolean enable_GL_CULL_FACE = true;
boolean enable_GL_DEPTH_TEST;

enum class DisplayObject
{
	SPHERE,
	TORUS,
	CYLINDER,
	CONE,
	DISK,
};

GLenum fillMode = GL_FILL;
GLenum faceMode = GL_FRONT_AND_BACK;

void MouseWheel(int, int dir, int, int)
{
	if (dir > 0) {
		--nFov;
	} else {
		++nFov;
	}
}

void SampleSpecial(int key, int, int)
{
	switch (key) {
	case GLUT_KEY_F1:
		cameraFrame.MoveForward(-0.2f);
		break;
	case GLUT_KEY_F2:
		cameraFrame.MoveForward(0.2f);
		break;
	case GLUT_KEY_F3:
		cameraFrame.MoveRight(0.2f);
		break;
	case GLUT_KEY_F4:
		cameraFrame.MoveRight(-0.2f);
		break;
	case GLUT_KEY_F5:
		cameraFrame.MoveUp(-0.2f);
		break;
	case GLUT_KEY_F6:
		cameraFrame.MoveUp(0.2f);
		break;
	case GLUT_KEY_UP:
		cameraFrame.RotateLocalX(-0.01f);
		break;
	case GLUT_KEY_DOWN:
		cameraFrame.RotateLocalX(0.01f);
		break;
	case GLUT_KEY_LEFT:
		cameraFrame.RotateLocalY(0.01f);
		break;
	case GLUT_KEY_RIGHT:
		cameraFrame.RotateLocalY(-0.01f);
		break;
	default: ;
	}
}

std::string GetGLenumName(GLenum e)
{
	switch(e) {
	case GL_FILL: return "GL_FILL";
	case GL_LINE: return "GL_LINE";
	case GL_POINT: return "GL_POINT";
	case GL_FRONT: return "GL_FRONT";
	case GL_BACK: return "GL_BACK";
	case GL_FRONT_AND_BACK: return "GL_FRONT_AND_BACK";
	default: return "unknown";
	}
}

void SampleKeyboard(unsigned char cChar, int, int)
{
	switch (cChar) {
	case 27:
		glutLeaveMainLoop();
		break;
	case 'a':
	case 'A':
		switch (fillMode) {
			case GL_FILL:
				fillMode = GL_LINE;
				break;
			case GL_LINE:
				fillMode = GL_POINT;
				break;
			case GL_POINT:
			default:
				fillMode = GL_FILL;
				break;
		}
		break;
	case 's':
	case 'S':
		switch (faceMode) {
			case GL_FRONT:
				faceMode = GL_BACK;
				break;
			case GL_BACK:
				faceMode = GL_FRONT_AND_BACK;
				break;
			case GL_FRONT_AND_BACK:
			default:
				faceMode = GL_FRONT;
				break;
		}
		break;
	case 'f':
	case 'F':
		glutFullScreenToggle();
		break;
	case 'd':
	case 'D':
		enable_GL_DEPTH_TEST = !enable_GL_DEPTH_TEST;
		break;
	case 'c':
	case 'C':
		enable_GL_CULL_FACE = !enable_GL_CULL_FACE;
		break;
	default:
		break;
	}
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	viewFrustum.SetPerspective(35.0f, width / static_cast<float>(height), 1.0f, 500.0f);
	projectionStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelStack.LoadIdentity();

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, width, 0, height);
	glutSetWindow(nWindow);
}

static auto frameTime = std::chrono::high_resolution_clock::now();

void Redisplay()
{
	auto win = glutGetWindow();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	enable_GL_CULL_FACE? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	enable_GL_DEPTH_TEST? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0.0f);
	glClearDepth(1.0f);
	GLfloat colorD[]{0.0f, 0.0f, 0.0f, 1};
	GLfloat colorR[]{1.0f, 0.0f, 0.0f, 0.8};
	GLfloat colorG[]{0.0f, 1.0f, 0.0f, 0.5};
	GLfloat colorB[]{0.0f, 0.0f, 1.0f, 0.6};
	GLfloat colorText[]{0.2f, 0.5f, 1.0f, 1};
	glPolygonMode(faceMode, fillMode);

	modelStack.PushMatrix();
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelStack.MultMatrix(mCamera);
	M3DMatrix44f mObjectFrame;
	objectFrame.GetMatrix(mObjectFrame);
	modelStack.MultMatrix(mCamera);
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewFrustum.SetPerspective(nFov, viewport[2] / static_cast<float>(viewport[3]), 1.0f, 500.0f);
	projectionStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
	{
		static GLfloat sunScale = 0;
		modelStack.PushMatrix();
		modelStack.Scale(1 + 0.2 * sin(m3dDegToRad(sunScale)), 1 + 0.2 * cos(m3dDegToRad(sunScale)), 1 + 0.2 * sqrt(2.0f) * sin(m3dDegToRad(sunScale)) * cos(m3dDegToRad(sunScale)));
		shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), colorR);
		sunBatch.Draw();
		modelStack.PopMatrix();
	}
	{
		modelStack.PushMatrix();
		glPolygonOffset(-1.0f, -1.0f);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(faceMode, GL_LINE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(0.5f);
		static GLfloat earthRot = 0;
		earthRot += 3.0f;
		modelStack.Rotate(m3dDegToRad(earthRot), 0.0f, 1.0f, 0.0f);
		modelStack.Translate(20, 0, 0);
		shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), colorD);
		earthBatch.Draw(); 
		glLineWidth(1.0f);
		glPolygonMode(faceMode, fillMode);
		glPolygonOffset(-0.0f, -0.0f);
		glDisable(GL_POLYGON_OFFSET_LINE);
		shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), colorB);
		earthBatch.Draw(); 
		{

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			modelStack.PushMatrix();
			static GLfloat moonRot = 0;
			moonRot += 10.0f;
			modelStack.Rotate(m3dDegToRad(moonRot), 0.0f, 1.0f, 0.0f);
			modelStack.Translate(6, 0, 0);
			shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), colorG);
			moonBatch.Draw();
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
	M3DMatrix44f curCamera;
	cameraFrame.GetCameraMatrix(curCamera);
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, colorText);
	static int64_t lastFps = 0;
	std::stringstream camPos;
	camPos << "f: toggle full screen.\nesc: exit.\na: toggle fill.\ns: toggle face.\nc: toggle cull.\nd: toggle depth.\nF1~F6 & Arrows: camera control.\nMouse Whell: FOV.\nFPS : " << lastFps << "\n"
			<< "face: " << GetGLenumName(faceMode) << ", " << "fill: " << GetGLenumName(fillMode) << "\n"
			<< "GL_DEPTH_TEST: " << static_cast<int>(enable_GL_DEPTH_TEST) << ", " << "GL_CULL_FACE: " << static_cast<int>(enable_GL_CULL_FACE) << std::endl;
	glMatrixMode(GL_MODELVIEW); {
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION); {
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0, viewport[2], viewport[3], 0);

			glRasterPos2i(2, 20);
			glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(camPos.str().c_str()));

			glPopMatrix();
		}
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	glutSwapBuffers();
	glutPostWindowRedisplay(win);
	auto diff = std::chrono::high_resolution_clock::now() - frameTime;
	frameTime = std::chrono::high_resolution_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::microseconds>(diff);
	lastFps = (lastFps * 100.0 + 1000000.0 / dt.count()) / 101.0;
}

#ifdef _MSC_VER
#ifdef _DEBUG
int main(int argc, char* argv[])
#else
#define argc __argc
#define argv __argv
int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
#endif // !DEBUG

#else
int main(int argc, char* argv[])
#endif// _MSC_VER
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	nWindow = glutCreateWindow("homework 6");
	glutSetIconTitle("homework 6");

	auto err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW error : %s \n", reinterpret_cast<const char*>(glewGetErrorString(err)));
		return 1;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	shaderManager.InitializeStockShaders();

	transformPipeline.SetMatrixStacks(modelStack, projectionStack);
	cameraFrame.MoveForward(-50.0f);

	gltMakeSphere(sunBatch, 10, 50, 20);
	gltMakeSphere(earthBatch, 3, 50, 20);
	gltMakeSphere(moonBatch, 1, 50, 20);

	glutKeyboardFunc(SampleKeyboard);
	glutSpecialFunc(SampleSpecial);
	glutDisplayFunc(Redisplay);
	glutMouseWheelFunc(MouseWheel);
	glutReshapeFunc(Reshape);

	glutMainLoop();

	return EXIT_SUCCESS;
}
