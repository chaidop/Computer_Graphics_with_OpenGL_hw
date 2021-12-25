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
#define ELLIPSE 2
#define CLEAR_SCREEN 6

GLint screenWidth= 640;
GLint screenHeight =480;

int menu; // for menu creation
int selection; // for menu choices
int sxhma = 0; // for drawing line or ellipse
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

void plotLineLow(int x0, int y0, int x1, int y1){
    //set initial values
    int r = 0.0; int g = 0.0; int b = 1.0;
    int y = y0;

    int dx = x1 - x0;
    int dy = y1 - y0;
    
    int yi = 1;

    int size = dx;
    GLintPoint points[size]={};

    //compare variables for antialising
    float w = 1 - (dy/dx);
    float e = 0.6;
    //////////////

    if (dy < 0){    //line on negative y axis
        yi = -1;
        dy = -dy;
    }
    //decision parameter
    int D = (2 * dy) - dx;
    glColor3f(e*r,e*g,e*b);
    glBegin(GL_POINTS);
            glVertex2i(x0,y);
    glEnd();
    int yold = 0;
    //start the algorithm until the end point
    for(int x = x0; x < x1; x++){
        yold = y;
        if(D > 0){      //distance1 < distance2, so choose the upper pixel next (xk+1, yk + 1)
            y = y + yi; //could be y + 1 or y - 1 , depending on wether we have positive or negative slope
            D = D + (2 * (dy - dx));
        }
        else{           //(xk+1, yk)
            D = D + 2*dy;
        }
        if(e > w){    
            e = e - w;
            /*if(yold == y){
                y = y + yi;
            }*/
        }
        else{           
            e = e + (dy/dx);
        }
        glBegin(GL_POINTS);
            //estimate percentage of antialising color
            glColor3f(e*r,e*g,e*b);
            glVertex2i(x,y);
        glEnd();
    }

    glFlush();
}

void plotLineHigh(int x0, int y0, int x1, int y1){
    //set initial values
    int r = 0.0; int g = 0.0; int b = 1.0;
    int x = x0;

    int dx = x1 - x0;
    int dy = y1 - y0;

    int xi = 1;

    int size = dy;
    GLintPoint points[size]={};

    //compare variables for antialising
    float w = 1 - (dy/dx);
    float e = 0.5;

    if(dx < 0){ //negative slope
        xi = -1;
        dx = -dx;
    }
    //decision parameter
    int D = (2 * dx) - dy;
    //error for antialising 
    glColor3f(e*r,e*g,e*b);
        glBegin(GL_POINTS);
            glVertex2i(x,y0);
        glEnd();
    int xold = 0;
    for(int y = y0; y < y1; y++){
        xold = x;
        if (D > 0){  
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else{
            D = D + 2*dx;
        }

        if(e > w){    
            e = e - w;

        }
        else{           
            e = e + (dy/dx);
        }
        glBegin(GL_POINTS);
            //estimate percentage of antialising color
            glColor3f(e*r,e*g,e*b);
            glVertex2i(x,y);
        glEnd();
    }
    glFlush();
}

void bresenhamLine(int x0, int y0, int x1, int y1)
{
    //initial values of algorithm

    //for small slopes, stepping over x axis
    if(abs(y1 - y0) < abs(x1 - x0)){     //for 0<m<1, -1<m<0 and the other 2 symmetrical quaternions 
        if (x0 > x1){       //invert coordinates
            plotLineLow(x1, y1, x0, y0);
        }
        else{
            plotLineLow(x0, y0, x1, y1);
        }
    }
    //for large slopes, stepping over y axis
    else{                               //for inf>m>1, -inf<m<-1 and the other 2 symmetrical quaternions
        if (y0 > y1){    //invert coordinates
            plotLineHigh(x1, y1, x0, y0);
        }
        else{
            plotLineHigh(x0, y0, x1, y1);
        }
    }

    //after getting all the pixels, draw the line from 
    //the vector containing all the pixel coordinates
    
    /*glBegin(GL_LINES);
		glVertex2i(ax,ay);
		glVertex2i(bx, by);
	glEnd();
	glFlush();*/
}

void bresenhamEllipse(int xc, int yc, int xa, int ya, int xb, int yb)
{
    //initial values of algorithm
    int a = abs(xc - xa);
    int b = abs(yc - yb);
    plotEllipse(a, b, xc, yc);

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
	if (button == GLUT_LEFT_BUTTON && state ==GLUT_DOWN)
	{		
		corner[numCorners].x =x1;
		corner[numCorners].y = screenHeight-y1;
        glColor3f(0.0, 0.0, 1.0);
		drawPoint(corner[numCorners].x,corner[numCorners].y);
		numCorners++;
		if (numCorners == 2)
		{
			if (sxhma == 1)//create line with Bresenham
			{
				glColor3f(0.0,0.0, 1.0);
				bresenhamLine(corner[0].x, corner[0].y, corner[1].x, corner[1].y);
			}
            if(sxhma == 0 || sxhma == 1)
			    numCorners =0;
		}
        if(numCorners == 3){
            //glColor3f(0.0,0.0,1.0);
			if(sxhma == 2)//create an ellipse with Bresenham
			{
				//glColor3f(1.0,0.0,0.0);
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

		case ELLIPSE: // drawing rectangle
			sxhma=2;
			numCorners = 0;
			glutPostRedisplay();
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
	


	// menu creation
	menu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Line", LINE);
	glutAddMenuEntry("Ellipse", ELLIPSE);
	glutAddMenuEntry("Clear Screen", CLEAR_SCREEN);

	// register menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Initializations
	myInit();

	// perpetual ever processing loop
	glutMainLoop();

	glutDestroyMenu(menu);

	return 0;
}
