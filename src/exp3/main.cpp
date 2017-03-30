#define _USE_MATH_DEFINES
#include<cmath>
#include<cstdio>
#include<GLTools.h>
#include<GLShaderManager.h>
#include<GL/freeglut.h>

int nWindow;
int nLoopMain = 0;
GLShaderManager shaderManager;

GLBatch pointBatch;
GLBatch lineBatch;
GLBatch lineStripBatch;
GLBatch lineLoopBatch;
GLBatch triangleBatch;
GLBatch triangleStripBatch;
GLBatch triangleFanBatch;

GLfloat vEllipse[24][3]{};
GLfloat vRing[50][3]{};
GLfloat vCycle[50][3]{};

constexpr double Tau = 2.0 * M_PI;
constexpr double Pi = M_PI;

enum class DisplayMode
{
	POINTBATCH,
	LINEBATCH,
	LINESTRIPBATCH,
	LINELOOPBATCH,
	TRIANGLEBATCH,
	TRIANGLESTRIPBATCH,
	TRIANGLEFANBATCH,
};

DisplayMode curMode = DisplayMode::POINTBATCH;
GLenum fillMode = GL_LINE;

void SampleKeyboard(unsigned char cChar, int nMouseX, int nMouseY)
{
	switch (cChar) {
	case 27:
		glutLeaveMainLoop();
		break;

	case 'q':
	case 'Q':
		curMode = DisplayMode::POINTBATCH;
		break;
	case 'w':
	case 'W':
		curMode = DisplayMode::LINEBATCH;
		break;
	case 'e':
	case 'E':
		curMode = DisplayMode::LINESTRIPBATCH;
		break;
	case 'r':
	case 'R':
		curMode = DisplayMode::LINELOOPBATCH;
		break;
	case 't':
	case 'T':
		curMode = DisplayMode::TRIANGLEBATCH;
		break;
	case 'y':
	case 'Y':
		curMode = DisplayMode::TRIANGLESTRIPBATCH;
		break;
	case 'u':
	case 'U':
		curMode = DisplayMode::TRIANGLEFANBATCH;
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
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glutSetWindow(nWindow);
}

void Redisplay()
{
	int win = glutGetWindow();
	int viewport[4];
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 0.0f);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glRasterPos2i(2, -glutBitmapHeight(GLUT_BITMAP_9_BY_15) + 3 + viewport[3]);
	GLfloat color[] = {1.0f, 0.5f, 0.0f};
	GLfloat color2[] = {0.0f, 0.5f, 1.0f};
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, color);
	glPointSize(3.0f);
	glLineWidth(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, fillMode);
	switch (curMode) {
	case DisplayMode::POINTBATCH:
		pointBatch.Draw();
		break;
	case DisplayMode::LINEBATCH:
		lineBatch.Draw();
		break;
	case DisplayMode::LINESTRIPBATCH:
		lineStripBatch.Draw();
		break;
	case DisplayMode::LINELOOPBATCH:
		lineLoopBatch.Draw();
		break;
	case DisplayMode::TRIANGLEBATCH:
		triangleBatch.Draw();
		break;
	case DisplayMode::TRIANGLESTRIPBATCH:
		triangleStripBatch.Draw();
		break;
	case DisplayMode::TRIANGLEFANBATCH:
		triangleFanBatch.Draw();
		break;
	default: break;
	}
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, color2);
	glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>("f: toggle full screen.\nh: minimize the window.\nesc: exit.\nq~u: choose graph.\na: toggle fill."));

	glutSwapBuffers();
	glutPostWindowRedisplay(win);
}


constexpr void vertex()
{
	for (auto i = 0; i < 24; ++i) {
		vEllipse[i][0] = 0.3 * 2 * cos(i * Tau / 24.0);
		vEllipse[i][1] = 0.3 * sin(i * Tau / 24.0);
		vEllipse[i][2] = 0;
	}
	for (auto i = 0; i < 50; i += 2) {
		vRing[i][0] = 0.6 * cos(i * Tau / 48.0);
		vRing[i][1] = 0.6 * sin(i * Tau / 48.0);
		vRing[i][2] = 0;
	}
	for (auto i = 1; i < 50; i += 2) {
		vRing[i][0] = 0.4 * cos(i * Tau / 48.0);
		vRing[i][1] = 0.4 * sin(i * Tau / 48.0);
		vRing[i][2] = 0;
	}
	vCycle[0][0] = 0;
	vCycle[0][1] = 0;
	vCycle[0][2] = 0;
	for (auto i = 1; i < 50; ++i) {
		vCycle[i][0] = 0.4 * cos(i * Tau / 48.0);
		vCycle[i][1] = 0.4 * sin(i * Tau / 48.0);
		vCycle[i][2] = 0;
	}
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
	vertex();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	nWindow = glutCreateWindow("homework 3");
	glutSetIconTitle("homework 3");

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW error : %s \n", reinterpret_cast<const char*>(glewGetErrorString(err)));
		return 1;
	}
	shaderManager.InitializeStockShaders();
	pointBatch.Begin(GL_POINTS, 24);
	pointBatch.CopyVertexData3f(vEllipse);
	pointBatch.End();
	lineBatch.Begin(GL_LINES, 24);
	lineBatch.CopyVertexData3f(vEllipse);
	lineBatch.End();
	lineStripBatch.Begin(GL_LINE_STRIP, 24);
	lineStripBatch.CopyVertexData3f(vEllipse);
	lineStripBatch.End();
	lineLoopBatch.Begin(GL_LINE_LOOP, 24);
	lineLoopBatch.CopyVertexData3f(vEllipse);
	lineLoopBatch.End();
	triangleBatch.Begin(GL_TRIANGLES, 49);
	triangleBatch.CopyVertexData3f(vRing);
	triangleBatch.End();
	triangleStripBatch.Begin(GL_TRIANGLE_STRIP, 50);
	triangleStripBatch.CopyVertexData3f(vRing);
	triangleStripBatch.End();
	triangleFanBatch.Begin(GL_TRIANGLE_FAN, 50);
	triangleFanBatch.CopyVertexData3f(vCycle);
	triangleFanBatch.End();
	glutKeyboardFunc(SampleKeyboard);
	glutDisplayFunc(Redisplay);
	glutReshapeFunc(Reshape);

	glutMainLoop();

	return EXIT_SUCCESS;
}
