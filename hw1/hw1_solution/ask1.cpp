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
#define ELIPSE 2
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
float rgb[3]={0.0, 0.0, 1.0};

 typedef struct 
 { 
	 GLint x,y;
 }
 GLintPoint;

 void drawEllipsePoints(int x, int y, int xc, int yc){
    //point in quadrant 1
    glBegin(GL_POINTS);
		glVertex2i(xc + x, yc + y);
	glEnd();
    //point in quadrant 2
    glBegin(GL_POINTS);
		glVertex2i(xc - x, yc + y);
	glEnd();
    //point in quadrant 3
    glBegin(GL_POINTS);
		glVertex2i(xc - x, yc - y);
	glEnd();
    //point in quadrant 4
    glBegin(GL_POINTS);
		glVertex2i(xc + x, yc - y);
	glEnd();
	glFlush();
}
//bresenham ellipse
void plotEllipse(int a, int b, int xc, int yc){
    //set initial values 1st region
    int x = a, y = 0; //Starting point
    int a_sq_2 = 2*a*a; 
    int b_sq_2 = 2*b*b; 
    
    int xchange = (b*b)*(1 - 2*a);
    int ychange = a*a;

    int xstop = b_sq_2*a;
    int ystop = 0;
    //error rate
    int e0 = 0;
    //int D = 4*(b^2) + ((a^2)*(1 - 4*b)-2);

    while(xstop >= ystop){      //1st quadrant, 4 first points
        drawEllipsePoints(x, y, xc, yc);
        y = y + 1;
        ystop += a_sq_2;
        e0 += ychange;
        ychange += a_sq_2;
        if((2*e0 + xchange) > 0){
            x = x -1;
            xstop = xstop - b_sq_2;
            e0 = e0 + xchange;
            xchange = xchange + b_sq_2;
        }

    }

    //set initial values 2nd region
    x = 0, y = b; //Starting point

    xchange = b*b;
    ychange = (a*a)*(1 - 2*b);

    xstop = 0;
    ystop = a_sq_2*b;
    //error rate
    e0 = 0;
    //int D = 4*(b^2) + ((a^2)*(1 - 4*b)-2);

    while(xstop <= ystop){      //4 second points, dy<-1
        drawEllipsePoints(x, y, xc, yc);
        x = x + 1;
        xstop += b_sq_2;
        e0 += xchange;
        xchange += b_sq_2;
        if((2*e0 + ychange) > 0){
            y = y -1;
            ystop = ystop - a_sq_2;
            e0 = e0 + ychange;
            ychange = ychange + a_sq_2;
        }

    }

    
}
void bresenhamEllipse(int xc, int yc, int xa, int ya, int xb, int yb)
{
    //initial values of algorithm
    int a = abs(xc - xa);
    int b = abs(yc - yb);
    plotEllipse(a, b, xc, yc);

}

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
	
	float slope = dy/dx;
	float w = 1 - slope;
	float e = m*x1 - y1 +0.5;	// CHANGE

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
		 if(e < w){
			 e += m;
		 }
      }
	  else{ 
		  pk  += 2*(dy-dx);
		  if(e > w){
			 e -= w;
			}
		  y += yi;
	  }
	  if( m > 1){
		//printf("m>1  \n");
		glColor3f(rgb[0]*pk,rgb[1]*pk,rgb[2]*pk);
		drawPoint(y , x + 1);
	  }
	  else{
		  //printf("m<1 \n");
		  glColor3f(rgb[0]*pk,rgb[1]*pk,rgb[2]*pk);
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
	static GLintPoint corner[3];
	//static int numCorners = 0;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{		
		corner[numCorners].x =x1;
		corner[numCorners].y = screenHeight-y1;
		drawPoint(corner[numCorners].x,corner[numCorners].y);
		numCorners++;
		if (numCorners == 2)
		{
			//glColor3f(0.0,0.0,1.0);
		
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
				numCorners =0;
			}

			
		}
		if(numCorners == 3){
			if(sxhma == 2){
				bresenhamEllipse(corner[0].x, corner[0].y, corner[1].x, corner[1].y, corner[2].x, corner[2].y);

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

		case ELIPSE: // drawing elipse
			sxhma=2;
			numCorners = 0;
			glutPostRedisplay();
			break;

		case LINE_RED_COLOR:
            rgb[0] = 1.0;
            rgb[1]= 0.0;
            rgb[2]=0.0;
			glColor3f(1.0,0.0,0.0);
			break;

		case LINE_GREEN_COLOR:
            rgb[0] = 0.0;
            rgb[1]= 1.0;
            rgb[2]=0.0;
			glColor3f(0.0,1.0,0.0);
			break;

		case LINE_BLUE_COLOR:
            rgb[0] = 0.0;
            rgb[1]= 0.0;
            rgb[2]= 1.0;
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
	glutAddMenuEntry("Ellipse", ELIPSE);
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

