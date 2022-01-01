//Gia na mhn emfanizetai to console Window sthn arxh tou programmatos prepei na topoqeteitai to: 
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>

#define LINE 1
#define RECTANGLE 2
#define LINE_RED_COLOR 3
#define LINE_GREEN_COLOR 4
#define LINE_BLUE_COLOR 5
#define CLEAR_SCREEN 6

GLint screenWidth= 640;
GLint screenHeight =480;

int menu; // for menu creation
int selection; // for menu choices
int sxhma; // for drawing line or rectangle
int numCorners = 0;

 typedef struct 
 { 
	 GLint x,y;
 }
 GLintPoint;


void drawPoint( int ax, int ay)
{
	glBegin(GL_POINTS);
		glVertex2i(ax,ay);
	glEnd();
	glFlush();
}

// CHANGE
void drawLine(int x1, int y1, int x2, int y2)
{
	int dy, dx;
	int y = y1; // changes to y2 if y2 < y1
	int m = 0;
	int k = 1;
	dy = y2 - y1;
	dx = x2 - x1;
	
	int yi = 1; //value of increase
	printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
	
	if(dy < 0){// for negative slope,  (-inf<m<-1) or 0<m < -1
		printf("y2 < y1 \n");	
		yi = -1;
		int temp1 = y1;
			y1 = y2;
			y2 = temp1;
		if(abs(dy) > abs(dx)){ // (-inf<m<-1)
			
			//y = y2;

			temp1 = x1;
			x1 = x2;
			x2 = temp1;
		}
		
	}

	
	//for high slopes, inf>m>1 or -inf<m<-1, change x to y to run the for loop
	//according to appropriate axis
	if(abs(dy) > abs(dx)){
		printf("m>1 m<-1 \n");
		
		int temp;
		
		m = 2;
		
		temp = x1;
		x1 = y1;
		y1 = temp;
		temp = x2;
		x2 = y2;
		y2 = temp;
		y = y1;
	}
	
	dy = y2 - y1;
	dx = x2 - x1;
	if((m == 2) && (yi == -1) && (x1 - x2 < 0)){//-inf < m < -1
		printf("-inf \n ");
		dy = -dy;
	}
	printf("%d AFTER1 (%d, %d) -> (%d, %d)\n", m, x1, y1, x2, y2);	
	int pk = 2*dy-dx;

   for (int x = x1; x <= x2; x++)
   {	
      // Slope error reached limit, time to
      // increment y and update slope error.
      if (pk < 0)
      {
         pk += 2*dy;
      }
	  else{ 
		  pk  += 2*(dy-dx);
		  y += yi;
	  }
	  if( m > 1){
		//printf("m>1  \n");
		drawPoint(y , x + 1);
	  }
	  else{
		  //printf("m<1 \n");
		  drawPoint(x + 1, y);
	  }
   }
}


void myInit (void)
{
	//glClearColor(0.5, 0.5, 0.5, 0.0);    // select clearing (background) color
	glClear(GL_COLOR_BUFFER_BIT);		// clear the screen
	//glColor3f(1.0, 1.0, 1.0);           // initialize viewing color values
	glColor3f(0.0,0.0,1.0);
	glPointSize(4);						// set point size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLfloat)screenWidth, 0.0,(GLfloat)screenHeight);// Creates a matrix for projecting two dimensional
																	// coordinates onto the screen and multiplies the 
																	// current projection matrix by it.
																	// It is used for projecting a two dimensional image 
																	// onto a two dimensional screen. 
																	// For two dimensional objects using two dimensional 
																	// vertex commands all z coordinates are zero.
	glFlush();
}
										

void myDisplay(void)
{
	glClearColor(0.5, 0.5, 0.5, 0.0);    // select clearing (background) color
	glClear(GL_COLOR_BUFFER_BIT);		// clear the screen
	glFlush();							// send all output to display
}

 //Specifying a rectangle with the mouse
void myMouse (int button, int state, int x1, int y1)
{
	static GLintPoint corner[2];
	//static int numCorners = 0;
	if (button == GLUT_LEFT_BUTTON && state ==GLUT_DOWN)
	{		
		corner[numCorners].x =x1;
		corner[numCorners].y = screenHeight-y1;
		drawPoint(corner[numCorners].x,corner[numCorners].y);
		numCorners++;
		if (numCorners == 2)
		{
			//glColor3f(0.0,0.0,1.0);
		
			if(sxhma == 2)
			{
				
				//glColor3f(1.0,0.0,0.0);
				
				glRecti(corner[0].x, corner[0].y, corner[1].x, corner[1].y);
			}
			if (sxhma == 1)
			{
				//glColor3f(0.0,1.0, 0.0);

				//check if on right or left side of graph:
				if(corner[1].x - corner[0].x < 0){ 	//on left
					drawLine(corner[1].x, corner[1].y, corner[0].x, corner[0].y);
				}
				else{
					drawLine(corner[0].x, corner[0].y, corner[1].x, corner[1].y);
				}	
			}
			numCorners =0;
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		glClear(GL_COLOR_BUFFER_BIT);  // clear the window
		
	glFlush();
	
}
	// triggered when a Keyboard key is pressed
void keyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
		//Quit
		case 'Q':
		case 'q':
		exit(-1);
		break;
		default:
		break;
	}
}

// Menu choices
void myMenu(int selection)
{
	switch(selection) {

		case LINE: // drawing  line
			sxhma=1;
			numCorners = 0;
			glutPostRedisplay(); // put comments to see what happens
			break;

		case RECTANGLE: // drawing rectangle
			sxhma=2;
			numCorners = 0;
			glutPostRedisplay();
			break;

		case LINE_RED_COLOR:
			glColor3f(1.0,0.0,0.0);
			break;

		case LINE_GREEN_COLOR:
			glColor3f(0.0,1.0,0.0);
			break;

		case LINE_BLUE_COLOR:
			glColor3f(0.0,0.0,1.0);
			break;

		case CLEAR_SCREEN:
			glClear(GL_COLOR_BUFFER_BIT);  // clear the window
			glFlush();
			break;

		default:
			printf("Invalid menu selection\n");
			break;
	}

}



int main(int argc, char **argv)
{

	int menu, lineColorSubmenu;
	
	// initialize toolkit
	glutInit(&argc, argv);
	// set display mode
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// set window size
	glutInitWindowSize (screenWidth, screenHeight);
	// set window initial position
	glutInitWindowPosition(1,1);
	// creates a window
	glutCreateWindow ("Pop-up menu");
	// registers a redraw function, this function is called whenever 
	// the system determines that the window should be redrawn.
	// This first happens when the window is first opened.
	glutDisplayFunc(myDisplay);
	// registers a keyboard handler
	glutKeyboardFunc(keyEvent);
	glutMouseFunc(myMouse);
	
	// Create a two-level pop-up menu
	lineColorSubmenu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Red", LINE_RED_COLOR);
	glutAddMenuEntry("Green", LINE_GREEN_COLOR);
	glutAddMenuEntry("Blue", LINE_BLUE_COLOR);

	// menu creation
	menu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Line", LINE);
	glutAddMenuEntry("Rectangle", RECTANGLE);
	glutAddSubMenu("Color", lineColorSubmenu);
	glutAddMenuEntry("Clear Screen", CLEAR_SCREEN);

	// register menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Initializations
	myInit();

	// perpetual ever processing loop
	glutMainLoop();

	glutDestroyMenu(lineColorSubmenu);
	glutDestroyMenu(menu);

	return 0;
}

