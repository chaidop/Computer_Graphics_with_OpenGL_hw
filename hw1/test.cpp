#include <GL/glut.h>

int screenWidth = 640;
int screenHeight = 480;

void init()
{
    //paints the screen with the given rgb values
	glClearColor (0.8, 0.8, 0.8, 1.0);

    //the color that all geometrical shapes we create later will have
	glColor3f(0.6, 0.4, 0.3); 
    glPointSize(4);				// set point size
	glMatrixMode (GL_PROJECTION);    
	glLoadIdentity ();    
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);	
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);	//The buffers currently enabled for color writing
    glBegin (GL_LINES);	
        glVertex2i(100,50);
        glVertex2i(100,150);
        glVertex2i(150,150);
        glVertex2i(250,250);
        glVertex2i(550,350);
        glVertex2i(650,450);
    glEnd();
    glFlush();			//force execution of GL commands

}

// triggered when a mouse key is pressed
void mouseEvent(int button, int state, int x, int y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2i(x, screenHeight-y);
		glEnd();
		
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");
    glutDisplayFunc(myDisplay);
    glutMouseFunc(mouseEvent);
    init();
    glutMainLoop();
    return 0;
}
