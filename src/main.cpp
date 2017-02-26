#include<cstdio>
#include <GL/freeglut.h>

int nWindow;
int nLoopMain = 0;
GLboolean bChildPosDone = GL_FALSE, bChildSizeDone = GL_FALSE;
float cR = 1.0f;
float cG = 0.0f;
float cB = 0.0f;

void SampleKeyboard( unsigned char cChar, int nMouseX, int nMouseY );
void Redisplay();
void Reshape(int width, int height);

void UnhideTimer(int window)
{
    glutSetWindow(window);
    glutShowWindow();
}

void ChangeTitleTimer(int unused)
{
    glutSetIconTitle("new icon title");
    glutSetWindowTitle("new test title");
}

void SampleKeyboard( unsigned char cChar, int nMouseX, int nMouseY )
{
    switch (cChar)
    {
    case 27:
        glutLeaveMainLoop();
        break;
        
    case 'c':
    case 'C':
        if(cR != 0){
            cR = 0;
            cG = 1.0f;
        }else if(cG != 0){
            cG = 0;
            cB = 1.0f;
        }else{
            cB = 0;
            cR = 1.0f;
        }
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

void Idle(void)
{
    glutPostRedisplay();
}

void Reshape(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glutSetWindow(nWindow);
}

void Redisplay()
{
    int win = glutGetWindow();
    int viewport[4];
    glClear(GL_COLOR_BUFFER_BIT); 
    glClearColor(cR, cG, cB, 0.f);
    glColor3f(cG,cB,cR);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glRasterPos2i(2, -glutBitmapHeight(GLUT_BITMAP_9_BY_15)+3+viewport[3]);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)"press f/h/c");
    
    glutSwapBuffers();
    glutPostWindowRedisplay(win);
}


int main(int argc, char* argv[])
{
    int border, caption;
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    
    border  = glutGet(GLUT_WINDOW_BORDER_WIDTH);
    caption = glutGet(GLUT_WINDOW_HEADER_HEIGHT);

    glutInitWindowPosition(150,250);
    glutInitWindowSize(200,200);

    nWindow = glutCreateWindow("homework 1");
    glutSetIconTitle("homework 1");

    glutKeyboardFunc( SampleKeyboard );
    glutDisplayFunc( Redisplay );
    glutReshapeFunc( Reshape );

    glutMainLoop();

    return EXIT_SUCCESS;
}
