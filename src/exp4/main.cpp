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

int nWindow;
int nLoopMain = 0;

GLShaderManager shaderManager;
GLMatrixStack modelStack;
GLMatrixStack projectionStack;

GLFrame cameraFrame;
GLFrame objectFrame;
GLFrustum viewFrustum;

GLGeometryTransform transformPipeline;

GLTriangleBatch sphereBatch;
GLTriangleBatch torusBatch;
GLTriangleBatch cylinderBatch;
GLTriangleBatch coneBatch;
GLTriangleBatch diskBatch;

enum class DisplayObject
{
	SPHERE,
	TORUS,
	CYLINDER,
	CONE,
	DISK,
};

DisplayObject curObj = DisplayObject::SPHERE;
GLT_STOCK_SHADER curShader = GLT_SHADER_POINT_LIGHT_DIFF;

GLenum fillMode = GL_FILL;

void SampleSpecial(int key, int, int)
{
	switch(key)
	{
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
	default:;
	}
}

void SampleKeyboard(unsigned char cChar, int, int)
{
	switch (cChar) {
	case 27:
		glutLeaveMainLoop();
		break;

	case 'q':
	case 'Q':
		curObj = DisplayObject::SPHERE;
		break;
	case 'w':
	case 'W':
		curObj = DisplayObject::TORUS;
		break;
	case 'e':
	case 'E':
		curObj = DisplayObject::CYLINDER;
		break;
	case 'r':
	case 'R':
		curObj = DisplayObject::CONE;
		break;
	case 't':
	case 'T':
		curObj = DisplayObject::DISK;
		break;

	case 'i':
	case 'I':
		curShader = GLT_SHADER_FLAT;
		break;
	case 'o':
	case 'O':
		curShader = GLT_SHADER_DEFAULT_LIGHT;
		break;
	case 'p':
	case 'P':
		curShader = GLT_SHADER_POINT_LIGHT_DIFF;
		break;

	case 'a':
	case 'A':
		fillMode = fillMode == GL_FILL ? GL_LINE : GL_FILL;
		break;
	case 'f':
	case 'F':
		glutFullScreenToggle();
		break;

	case 'h':
	case 'H':
		glutSetWindow(nWindow);
		glutIconifyWindow();
		break;
	default:
		break;
	}
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, width, 0, height);

	viewFrustum.SetPerspective(35.0f, width / static_cast<float>(height), 1.0f, 500.0f);
	projectionStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelStack.LoadIdentity();
	
	glutSetWindow(nWindow);
}

void Redisplay()
{
	auto win = glutGetWindow();
	int viewport[4];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 0.0f);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glRasterPos2i(2, -glutBitmapHeight(GLUT_BITMAP_9_BY_15) + 3 + viewport[3]);
	GLfloat color1[]{1.0f, 0.5f, 0.0f};
	GLfloat color2[]{0.0f, 0.5f, 1.0f};
	GLfloat color3[]{1.0f, 0.0f, 1.0f};
	glPolygonMode(GL_FRONT_AND_BACK, fillMode);

	modelStack.PushMatrix();
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelStack.MultMatrix(mCamera);
	M3DMatrix44f mObjectFrame;
	objectFrame.GetMatrix(mObjectFrame);
	modelStack.MultMatrix(mCamera);

	switch (curShader) {
	case GLT_SHADER_FLAT: 
		shaderManager.UseStockShader(curShader, transformPipeline.GetModelViewProjectionMatrix(), color3);
		break;
	case GLT_SHADER_DEFAULT_LIGHT: 
		shaderManager.UseStockShader(curShader, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), color1);
		break;
	default: 
		M3DMatrix44f curCamera;
		cameraFrame.GetCameraMatrix(curCamera);
		M3DVector4f vLightPos{ 5.0f, 4.0f, 0.0f, 5.0f };
		M3DVector4f vLightEyePos;
		m3dTransformVector4(vLightEyePos, vLightPos, curCamera);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, color1);
	}

	switch (curObj) {
	case DisplayObject::SPHERE:
		sphereBatch.Draw();
		break;
	case DisplayObject::TORUS:
		torusBatch.Draw();
		break;
	case DisplayObject::CYLINDER:
		cylinderBatch.Draw();
		break;
	case DisplayObject::CONE:
		coneBatch.Draw();
		break;
	case DisplayObject::DISK:
		diskBatch.Draw();
		break;
	default: break;
	}
	M3DMatrix44f curCamera;
	cameraFrame.GetCameraMatrix(curCamera);
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, color2);
	std::stringstream camPos;
	camPos << "f: toggle full screen.\nh: minimize the window.\nesc: exit.\nq~t: choose graph.\na: toggle fill.\ni~p: choose shader.\nF1~F6 & Arrows: camera control.\n"
		<< curCamera[0] << ", " << curCamera[4] << ", " << curCamera[8] << ", " << curCamera[12] << ";\n"
		<< curCamera[1] << ", " << curCamera[5] << ", " << curCamera[9] << ", " << curCamera[13] << ";\n"
		<< curCamera[2] << ", " << curCamera[6] << ", " << curCamera[10] << ", " << curCamera[14] << ";\n"
		<< curCamera[3] << ", " << curCamera[7] << ", " << curCamera[11] << ", " << curCamera[15] << ";" << std::endl;
	glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(camPos.str().c_str()));
	modelStack.PopMatrix();

	glutSwapBuffers();
	glutPostWindowRedisplay(win);
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	nWindow = glutCreateWindow("homework 4");
	glutSetIconTitle("homework 4");

	auto err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW error : %s \n", reinterpret_cast<const char*>(glewGetErrorString(err)));
		return 1;
	}
	shaderManager.InitializeStockShaders();

	transformPipeline.SetMatrixStacks(modelStack, projectionStack);
	cameraFrame.MoveForward(-15.0f);

	gltMakeSphere(sphereBatch, 3, 50, 20);
	gltMakeTorus(torusBatch, 3.0f, 0.75f, 55, 55);
	gltMakeCylinder(cylinderBatch, 2.0f, 2.0f, 3.0f, 50, 20);
	gltMakeCylinder(coneBatch, 2.0f, .0f, 3.0f, 50, 20);
	gltMakeDisk(diskBatch, 1.5f, 3.0f, 50, 30);

	glutKeyboardFunc(SampleKeyboard);
	glutSpecialFunc(SampleSpecial);
	glutDisplayFunc(Redisplay);
	glutReshapeFunc(Reshape);

	glutMainLoop();

	return EXIT_SUCCESS;
}
