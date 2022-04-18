#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

int screenWidth = 640;
int screenHeight = 480;
int polygon_state = 0;//0 to create polygon, 1 to cut
int sxhma = 0;
int cuttingWindow = 0;//if 1, the cutting window is seen
int size =0 ;

class GLintPoint{
public:
	GLint x,y;
};

GLint numCorners=0, polCorners = 0;

void drawPoint(GLint x, GLint y)
{
	glColor3f(0.0, 0.0, 1.0);
		
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
	glFlush();
}


GLintPoint clipWindow[2];
GLintPoint polygon[20]; //MAX polygon vertices 10
GLintPoint vertices_buffer[20];//keeps the vertices of the previous checked window border
//essentially the output of the sutherland algorithm

void sutherland_hodgeman(){
    GLint xleft, xright, ytop, ybottom;
    GLintPoint temp_buffer[20];

    int yinter, xinter,j = 0,k = 0 , i = 0 ;

    //find left border
    if(clipWindow[0].x < clipWindow[1].x){
        xleft = clipWindow[0].x;
        xright = clipWindow[1].x;
    }
    else{
        xleft = clipWindow[1].x;
        xright = clipWindow[0].x;
    }
    //find top and bottom
    if(clipWindow[0].y < clipWindow[1].y){
        ybottom = clipWindow[0].y;
        ytop = clipWindow[1].y;
    }
    else{
        ybottom = clipWindow[1].y;
        ytop = clipWindow[0].y;
    }
    printf("L(%d), R(%d), B(%d), T(%d), TOTAL CORNERS %d\n", xleft, xright, ybottom, ytop, polCorners);
    //LEFT BORDER WINDOW
    //for each vertice check
    for(i = 0; i < polCorners-1; i++){
        //Check outside S
        if(polygon[i].x < xleft){
            //check outside-inside
            if(polygon[i+1].x > xleft){//put I and E in buffer
            printf("    O-I %d\n",i);
            //calculate the intersection point I, 
            //antikatestise gia x = xleft kai vres to y apo thn eksiswsh evtheias P0P1 twn trexwn edges tou polygwnou
            //yinter = y1 + m*(xmin – x1), m = dy/dx
            yinter = ((polygon[i].x*polygon[i+1].y - polygon[i].y*polygon[i+1].x) * (ybottom-ytop) -
              (polygon[i].y-polygon[i+1].y) * (xleft*ytop - ybottom*xleft))/((polygon[i].x-polygon[i+1].x) * (ybottom-ytop));
            //polygon[i].y + ((polygon[i+1].y - polygon[i].y)/(polygon[i+1].x - polygon[i].x))*(xleft - polygon[i].x);
            xinter = xleft;

            vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
            j++;
            vertices_buffer[j].x = polygon[i+1].x; vertices_buffer[j].y = polygon[i+1].y;
            j++;
            }
            else{
                printf("O-O\n");
            }
        }
        else{
            if(polygon[i+1].x > xleft){//inside-inside, output E
                printf("    I-I %d\n",i);
                vertices_buffer[j].x = polygon[i+1].x; vertices_buffer[j].y = polygon[i+1].y;
                j++;
            }
            else{//inside-outside, output I
                printf("    I-O %d\n",i);
                yinter = ((polygon[i].x*polygon[i+1].y - polygon[i].y*polygon[i+1].x) * (ybottom-ytop) -
              (polygon[i].y-polygon[i+1].y) * (xleft*ytop - ybottom*xleft))/((polygon[i].x-polygon[i+1].x) * (ybottom-ytop));
                //polygon[i].y + ((polygon[i+1].y - polygon[i].y)/(polygon[i+1].x - polygon[i].x))*(-(xleft - polygon[i].x));
                xinter = xleft;

                vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
                j++;
            }
        }
        
    }
    //check last with first vertex
        if(polygon[polCorners-1].x < xleft){
            //check outside-inside
            if(polygon[0].x > xleft){//put I and E in buffer
            printf("    O-I %d\n",i);
            //calculate the intersection point I, 
            //antikatestise gia x = xleft kai vres to y apo thn eksiswsh evtheias P0P1 twn trexwn edges tou polygwnou
            //yinter = y1 + m*(xmin – x1), m = dy/dx
            yinter = ((polygon[polCorners-1].x*polygon[0].y - polygon[polCorners-1].y*polygon[0].x) * (ybottom-ytop) -
              (polygon[polCorners-1].y-polygon[0].y) * (xleft*ytop - ybottom*xleft))/((polygon[polCorners-1].x-polygon[0].x) * (ybottom-ytop));
            xinter = xleft;

            vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
            j++;
            vertices_buffer[j].x = polygon[0].x; vertices_buffer[j].y = polygon[0].y;
            j++;
            }
        }
        else{
            if(polygon[0].x > xleft){//inside-inside, output E
                printf("    I-I %d\n",i);
                vertices_buffer[j].x = polygon[0].x; vertices_buffer[j].y = polygon[0].y;
                j++;
            }
            else{//inside-outside, output I
                printf("    I-O %d\n",i);
                yinter = ((polygon[polCorners-1].x*polygon[0].y - polygon[polCorners-1].y*polygon[0].x) * (ybottom-ytop) -
              (polygon[polCorners-1].y-polygon[0].y) * (xleft*ytop - ybottom*xleft))/((polygon[polCorners-1].x-polygon[0].x) * (ybottom-ytop));//polygon[polCorners-1].y + ((polygon[0].y - polygon[i].y)/(polygon[0].x - polygon[polCorners-1].x))*(-(xleft - polygon[polCorners-1].x));
                xinter = xleft;
                
                vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
                j++;
            }
        }
        printf("\n================LEFT DONE J =  %d\n", j);
    //save vertices to buffer
    for(k = 0; k <j; k++){
        temp_buffer[k] = vertices_buffer[k]; 
        printf("(%d,%d)->",vertices_buffer[k].x,vertices_buffer[k].y);
    }
    //RIGHT BORDER WINDOW
    for(k =0, i = 0; i < j-1; i++){
        //Check outside S
        if(temp_buffer[i].x > xright){
            //check outside-inside
            if(temp_buffer[i+1].x < xright){//put I and E in buffer
            yinter = ((temp_buffer[i].x*temp_buffer[i+1].y - temp_buffer[i].y*temp_buffer[i+1].x) * (ybottom-ytop) -
              (temp_buffer[i].y-temp_buffer[i+1].y) * (xright*ytop - ybottom*xright))/((temp_buffer[i].x-temp_buffer[i+1].x) * (ybottom-ytop));
            //temp_buffer[i].y + ((temp_buffer[i+1].y - temp_buffer[i].y)/(temp_buffer[i+1].x - temp_buffer[i].x))*(xright - temp_buffer[i].x);
            xinter = xright;
            printf("\n  O-I %d", i);
            vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
            k++;
            vertices_buffer[k].x = temp_buffer[i+1].x; vertices_buffer[k].y = temp_buffer[i+1].y;
            k++;
            }
        }
        else{
            if(temp_buffer[i+1].x < xright){//inside-inside, output E
                vertices_buffer[k].x = temp_buffer[i+1].x; vertices_buffer[k].y = temp_buffer[i+1].y;
                printf("\n  I-I %d", i);
                k++;
            }
            else{//inside-outside, output I
                yinter = ((temp_buffer[i].x*temp_buffer[i+1].y - temp_buffer[i].y*temp_buffer[i+1].x) * (ybottom-ytop) -
              (temp_buffer[i].y-temp_buffer[i+1].y) * (xright*ytop - ybottom*xright))/((temp_buffer[i].x-temp_buffer[i+1].x) * (ybottom-ytop));
                xinter = xright;
                printf("\n  I-O %d", i);
                vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
                k++;
            }
        }
    }
    //check last with first vertex
        if(temp_buffer[i].x > xright){
            //check outside-inside
            if(temp_buffer[0].x < xright){//put I and E in buffer
            printf("    O-I %d\n",i);
            //calculate the intersection point I, 
            //antikatestise gia x = xleft kai vres to y apo thn eksiswsh evtheias P0P1 twn trexwn edges tou polygwnou
            //yinter = y1 + m*(xmin – x1), m = dy/dx
            yinter = ((temp_buffer[i].x*temp_buffer[0].y - temp_buffer[i].y*temp_buffer[0].x) * (ybottom-ytop) -
              (temp_buffer[i].y-temp_buffer[0].y) * (xright*ytop - ybottom*xright))/((temp_buffer[i].x-temp_buffer[0].x) * (ybottom-ytop));
            xinter = xright;

            vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
            k++;
            vertices_buffer[k].x = temp_buffer[0].x; vertices_buffer[k].y = temp_buffer[0].y;
            k++;
            }
        }
        else{
            if(temp_buffer[0].x < xright){//inside-inside, output E
                printf("    I-I %d\n",i);
                vertices_buffer[k].x = temp_buffer[0].x; vertices_buffer[k].y = temp_buffer[0].y;
                k++;
            }
            else{//inside-outside, output I
                printf("    I-O %d\n",i);
                yinter = ((temp_buffer[i].x*temp_buffer[0].y - temp_buffer[i].y*temp_buffer[0].x) * (ybottom-ytop) -
              (temp_buffer[i].y-temp_buffer[0].y) * (xright*ytop - ybottom*xright))/((temp_buffer[i].x-temp_buffer[0].x) * (ybottom-ytop));
                xinter = xright;

                vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
                k++;
            }
        }
    printf("\n================RIGHT DONE J =  %d\n", k);
    //save vertices to buffer
    for(j = 0; j < k; j++){
        temp_buffer[j] = vertices_buffer[j];
        printf("(%d,%d)->",vertices_buffer[j].x,vertices_buffer[j].y); 
    }
    //BOTTOM BORDER WINDOW
    for(i = 0, j =0; i < k-1; i++){
        //Check outside S
        if(temp_buffer[i].y < ybottom){
            //check outside-inside
            if(temp_buffer[i+1].y > ybottom){//put I and E in buffer
            
            xinter = ((temp_buffer[i].x*temp_buffer[i+1].y - temp_buffer[i].y*temp_buffer[i+1].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[i+1].x) * (xleft*ybottom - ybottom*xright))/(-(temp_buffer[i].y-temp_buffer[i+1].y) * (xleft-xright));
              //temp_buffer[i].x + ((temp_buffer[i+1].x - temp_buffer[i].x)/(temp_buffer[i+1].y - temp_buffer[i].y))*(ybottom - temp_buffer[i].y);
            yinter = ybottom;
            printf("\n  O-I %d", i);
            vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
            j++;
            vertices_buffer[j].x = temp_buffer[i+1].x; vertices_buffer[j].y = temp_buffer[i+1].y;
            j++;
            }
        }
        else{
            if(temp_buffer[i+1].y > ybottom){//inside-inside, output E
                vertices_buffer[j].x = temp_buffer[i+1].x; vertices_buffer[j].y = temp_buffer[i+1].y;
                printf("\n  I-I %d", i);
                j++;
            }
            else{//inside-outside, output I
                xinter = ((temp_buffer[i].x*temp_buffer[i+1].y - temp_buffer[i].y*temp_buffer[i+1].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[i+1].x) * (xleft*ybottom - ybottom*xright))/(-(temp_buffer[i].y-temp_buffer[i+1].y) * (xleft-xright));
                //temp_buffer[i].x + ((temp_buffer[i+1].x - temp_buffer[i].x)/(temp_buffer[i+1].y - temp_buffer[i].y))*(ybottom - temp_buffer[i].y);
                yinter = ybottom;
                printf("\n  I-O %d", i);
                vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
                j++;
            }
        }
    }

    //check last with first vertex
    //Check outside S
    if(temp_buffer[i].y < ybottom){
        //check outside-inside
        if(temp_buffer[0].y > ybottom){//put I and E in buffer
        
        xinter = ((temp_buffer[i].x*temp_buffer[0].y - temp_buffer[i].y*temp_buffer[0].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[0].x) * (xleft*ybottom - ybottom*xright))/(-(temp_buffer[i].y-temp_buffer[0].y) * (xleft-xright));
        //temp_buffer[i].x + ((temp_buffer[0].x - temp_buffer[i].x)/(temp_buffer[0].y - temp_buffer[i].y))*(ybottom - temp_buffer[i].y);
        yinter = ybottom;
        printf("\n  O-I %d", i);
        vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
        j++;
        vertices_buffer[j].x = temp_buffer[0].x; vertices_buffer[j].y = temp_buffer[0].y;
        j++;
        }
    }
    else{
        if(temp_buffer[0].y > ybottom){//inside-inside, output E
            vertices_buffer[j].x = temp_buffer[0].x; vertices_buffer[j].y = temp_buffer[0].y;
            printf("\n  I-I %d", i);
            j++;
        }
        else{//inside-outside, output I
            xinter = ((temp_buffer[i].x*temp_buffer[0].y - temp_buffer[i].y*temp_buffer[0].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[0].x) * (xleft*ybottom - ybottom*xright))/(-(temp_buffer[i].y-temp_buffer[0].y) * (xleft-xright));
            //temp_buffer[i].x + ((temp_buffer[0].x - temp_buffer[i].x)/(temp_buffer[0].y - temp_buffer[i].y))*(ybottom - temp_buffer[i].y);
            yinter = ybottom;
            printf("\n  I-O %d", i);
            vertices_buffer[j].x = xinter; vertices_buffer[j].y = yinter;
            j++;
        }
    }
    printf("\n================BOTTOM DONE J =  %d\n", j);
    //save vertices to buffer
    for(k = 0; k < j; k++){
        temp_buffer[k] = vertices_buffer[k]; 
        printf("(%d,%d)->",vertices_buffer[k].x,vertices_buffer[k].y);
    }
    //TOP BORDER WINDOW
    for(i = 0, k =0; i < j-1; i++){
        //Check outside S
        if(temp_buffer[i].y > ytop){
            //check outside-inside
            if(temp_buffer[i+1].y < ytop){//put I and E in buffer
            printf("\n    O-I %d",i);
            xinter = ((temp_buffer[i].x*temp_buffer[i+1].y - temp_buffer[i].y*temp_buffer[i+1].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[i+1].x) * (xleft*ytop - ytop*xright))/(-(temp_buffer[i].y-temp_buffer[i+1].y) * (xleft-xright));
            //temp_buffer[i].x + ((temp_buffer[i+1].x - temp_buffer[i].x)/(temp_buffer[i+1].y - temp_buffer[i].y))*(ytop - temp_buffer[i].y);
            yinter = ytop;

            vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
            k++;
            vertices_buffer[k].x = temp_buffer[i+1].x; vertices_buffer[k].y = temp_buffer[i+1].y;
            k++;
            }
        }
        else{
            if(temp_buffer[i+1].y < ytop){//inside-inside, output E
                printf("\n    I-I %d",i);
                vertices_buffer[k].x = temp_buffer[i+1].x; vertices_buffer[k].y = temp_buffer[i+1].y;
                k++;
            }
            else{//inside-outside, output I
                xinter = ((temp_buffer[i].x*temp_buffer[i+1].y - temp_buffer[i].y*temp_buffer[i+1].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[i+1].x) * (xleft*ytop - ytop*xright))/(-(temp_buffer[i].y-temp_buffer[i+1].y) * (xleft-xright));
                yinter = ytop;
                printf("\n    I-O %d",i);
                vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
                k++;
            }
        }
    }

    //check last vertice
    //Check outside S
    if(temp_buffer[i].y > ytop){
        //check outside-inside
        if(temp_buffer[0].y < ytop){//put I and E in buffer
        printf("\n    O-I %d",i);
        xinter = ((temp_buffer[i].x*temp_buffer[0].y - temp_buffer[i].y*temp_buffer[0].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[0].x) * (xleft*ytop - ytop*xright))/(-(temp_buffer[i].y-temp_buffer[0].y) * (xleft-xright));
        yinter = ytop;

        vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
        k++;
        vertices_buffer[k].x = temp_buffer[0].x; vertices_buffer[k].y = temp_buffer[0].y;
        k++;
        }
    }
    else{
        if(temp_buffer[0].y < ytop){//inside-inside, output E
            printf("\n    I-I %d",i);
            vertices_buffer[k].x = temp_buffer[0].x; vertices_buffer[k].y = temp_buffer[0].y;
            k++;
        }
        else{//inside-outside, output I
            xinter = ((temp_buffer[i].x*temp_buffer[0].y - temp_buffer[i].y*temp_buffer[0].x) * (xleft-xright) -
              (temp_buffer[i].x-temp_buffer[0].x) * (xleft*ytop - ytop*xright))/(-(temp_buffer[i].y-temp_buffer[0].y) * (xleft-xright));
            yinter = ytop;
            printf("\n    I-O %d %d",i, xinter);
            vertices_buffer[k].x = xinter; vertices_buffer[k].y = yinter;
            k++;
        }
    }
    printf("\n================TOP DONE J =  %d\n", k);
    for(i = 0;i < k; i++){
        polygon[i].x = vertices_buffer[i].x;
        polygon[i].y = vertices_buffer[i].y;
        printf("(%d, %d) ",vertices_buffer[i].x,vertices_buffer[i].y); 
    }
    size = k;
    polCorners = size;
};

void init (void)
{
	glClear(GL_COLOR_BUFFER_BIT);//The buffers currently enabled for color writing 
	glClearColor (0.0, 0.0, 0.0, 1.0);	// select clearing (background) color
   	glColor3f(0.0, 0.0, 1.0); 			
	glPointSize(6);

	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity(); 
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);	//define a 2-D orthographic projection matrix 
}


void display(void){
	
	//glClear(GL_COLOR_BUFFER_BIT);	//The buffers currently enabled for color writing
	//glColor3f(1.0, 0.0, 0.0); 
    if(sxhma == 1){
        //printf("WINDOW\n");
        if (numCorners ==2){
            glClear(GL_COLOR_BUFFER_BIT);//everytime clear the scene, otherwise bug
            glColor3f(1.0, 0.0, 0.0);
            glRecti(clipWindow[0].x, clipWindow[0].y, clipWindow[1].x, clipWindow[1].y);
            cuttingWindow = 1;

            //create the polygon
            glBegin(GL_POLYGON);
            glColor3f(0.0, 0.0, 1.0);
            for(int i = 0; i < polCorners; i++){
                glVertex2i(polygon[i].x,polygon[i].y);
            }
            glEnd();
            glFlush();
        }

    }
	if(sxhma == 2) //create and fill polygon
    {
        //create the polygon
        glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 1.0);
        for(int i = 0; i < polCorners; i++){
            glVertex2i(polygon[i].x,polygon[i].y);
        }
        glEnd();
        glFlush();
    }
    if(sxhma == 3){//cut polygon
        //create the cut polygon
        printf("Sutherland %d\n", size);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.0, 0.0);
        glRecti(clipWindow[0].x, clipWindow[0].y, clipWindow[1].x, clipWindow[1].y);
        glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 1.0);
        for(int i = 0; i < size; i++){
            glVertex2i(vertices_buffer[i].x,vertices_buffer[i].y);
        }
        glEnd();
        glFlush();
    }
	
	glFlush();
}
void keyCut(int key,int x,int y){	// x,y mouse location when key pressed
  
    switch (key) {
      // Polygon clip-> call Sutherland – Hodgeman routine
    //F1, go to polygon cutting state
    case GLUT_KEY_F1:
    default:
        polygon_state = 1;
        break;
    }
}
// triggered when a keyboard key is pressed
void keyEvent(unsigned char key,int x,int y){	// x,y mouse location when key pressed
  
    switch (key) {
      // Polygon clip-> call Sutherland – Hodgeman routine
    case 'C':
    case 'c':
    sutherland_hodgeman();
    sxhma = 3;
    glutPostRedisplay();
    break;
    //pressing P draws the polygon
    case 'P':
    case 'p':
        sxhma = 2;
        glutPostRedisplay();
        break;
    //space: show/hide cutting polygon
    case 32:
        glClear(GL_COLOR_BUFFER_BIT);  // clear the window
        //check if polygon cut window active
        //if yes, hide it, just redraw the polygon
        //if no, draw it and then redraw polygon
        if(cuttingWindow == 0){
            glColor3f(1.0, 0.0, 0.0);
            glRecti(clipWindow[0].x, clipWindow[0].y, clipWindow[1].x, clipWindow[1].y);
            glutPostRedisplay();
            cuttingWindow = 1;
        }
        else    
            cuttingWindow = 0;
        //and redraw polygon
        sxhma = 2;
        glutPostRedisplay();
        break;
    case 'Q':
	exit(0);
	break;	      
    default:	
	break;
    
    }
}

void myMouse(int button, int state, GLint xm, GLint ym)
{
    //polygon cutting
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && polygon_state == 1)
	{
		glColor3f(0.0, 0.0, 1.0);
				
		clipWindow[0].x= xm;
		clipWindow[0].y =screenHeight - ym;
		numCorners =1;
		glutPostRedisplay();
	}
    //polygon creating
    else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && polygon_state == 0)
	{
		glColor3f(1.0, 1.0, 1.0);
		printf("POINT %d\n\n", polCorners);
		polygon[polCorners].x= xm;
		polygon[polCorners].y =screenHeight - ym;
        drawPoint(polygon[polCorners].x, polygon[polCorners].y);
        printf("(%d, %d)\n", polygon[polCorners].x, polygon[polCorners].y);
		polCorners++;
        
        glFlush();
		glutPostRedisplay();

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        printf("Right click\n");
		glClear(GL_COLOR_BUFFER_BIT);
        polCorners = 0;
        numCorners = 0;
        cuttingWindow = 0;
        polygon_state = 0;
        sxhma = 0;
    }
    if (button==GLUT_LEFT_BUTTON && state==GLUT_UP && polygon_state == 1){
        printf("RELEASED\n");
    }
		
	glFlush();
}

void moveMouse(int xnew, int ynew)
 {
     if(polygon_state == 1){
     clipWindow[1].x= xnew;
	 clipWindow[1].y =screenHeight - ynew;
	 numCorners=2;
     sxhma = 1;
	 glutPostRedisplay();
     }

 }

int main(int argc, char** argv) 
{
	//initialize toolkit
	glutInit(&argc, argv);
	//set display mode 
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
	//set window size
	glutInitWindowSize (640, 480);
	//set window initial position
	glutInitWindowPosition (1, 1);
	//creates a window 
	glutCreateWindow ("glutMouseFunc2");
			
	glutDisplayFunc(display);
	
	//registers a keyboard handler
	glutKeyboardFunc(keyEvent);
    glutSpecialFunc(keyCut);
	glutMouseFunc(myMouse);	
	glutMotionFunc(moveMouse);
	//additional initializatons
	init ();
	
	//perpetual ever processing loop
	glutMainLoop(); 
	return 0;	
}      