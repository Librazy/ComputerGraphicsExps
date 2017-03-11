#include<cstdio>
#include<GLTools.h>
#include<GLShaderManager.h>
#include<GL/freeglut.h>

int nWindow;
int nLoopMain = 0;
GLShaderManager shaderManager;
float cR = 1.0f;
float cG = 0.0f;
float cB = 0.0f;
float x = 0.0f;
float y = 0.0f;
void SampleSpecial(int c, int mx, int my) {
	switch (c)
	{
	case GLUT_KEY_UP:
		y += 0.01;
		break;
	case GLUT_KEY_DOWN:
		y -= 0.01;
		break;
	case GLUT_KEY_LEFT:
		x -= 0.01;
		break;
	case GLUT_KEY_RIGHT:
		x += 0.01;
		break;
	default: break;
	}
}
void SampleKeyboard(unsigned char cChar, int nMouseX, int nMouseY)
{
	switch (cChar)
	{
	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
	case 'C':
		if (cR != 0) {
			cR = 0;
			cG = 1.0f;
		}
		else if (cG != 0) {
			cG = 0;
			cB = 1.0f;
		}
		else {
			cB = 0;
			cR = 1.0f;
		}
		break;

	case 'r':
	case 'R':
		cR = 1.0f;
		cG = 0;
		cB = 0;
		break;

	case 'g':
	case 'G':
		cR = 0;
		cG = 1.0f;
		cB = 0;
		break;

	case 'b':
	case 'B':
		cR = 0;
		cG = 0;
		cB = 1.0f;
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
	glClearColor(cR, cG, cB, 0.0f);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glRasterPos2i(2, -glutBitmapHeight(GLUT_BITMAP_9_BY_15) + 3 + viewport[3]);

	GLBatch tri;
	tri.Begin(GL_TRIANGLES, 3);

	GLfloat vert[] = {
		x-0.5f, y     , 0.0f,
		x+0.5f, y     , 0.0f,
		x     , y+0.5f, 0.0f,
	};
	GLfloat color[] = {
		cG, cB, cR
	};
	GLfloat color2[] = {
		cB, cR, cG
	};
	tri.CopyVertexData3f(vert);
	tri.End();

	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, color);
	tri.Draw();
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, color2);
	glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<unsigned char*>("press f/h/c"));

	glutSwapBuffers();
	glutPostWindowRedisplay(win);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	nWindow = glutCreateWindow("homework 2");
	glutSetIconTitle("homework 1");

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW error : %s \n", reinterpret_cast<const char*>(glewGetErrorString(err)));
		return 1;
	}
	shaderManager.InitializeStockShaders();
	glutKeyboardFunc(SampleKeyboard);
	glutSpecialFunc(SampleSpecial);
	glutDisplayFunc(Redisplay);
	glutReshapeFunc(Reshape);

	glutMainLoop();

	return EXIT_SUCCESS;
}
