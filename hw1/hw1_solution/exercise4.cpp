// CPP program to illustrate
// Scanline Polygon fill Algorithm

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define maxHt 600
#define maxWd 600
#define maxVer 10 //max edges for polygon
#define POINTS 1
#define FILLING 2
#define COLOR 2
#define LINE_RED_COLOR 3
#define LINE_GREEN_COLOR 4
#define LINE_BLUE_COLOR 5
#define CLEAR_SCREEN 6

int menu; // for menu creation
int selection; // for menu choices

int sxhma = 0;
int numCorners = 0;
float rgb[3]={0.0, 0.0, 1.0};
 typedef struct 
 { 
	 GLint x,y;
 }
 GLintPoint;

static GLintPoint corner[10];

void drawPoint( int ax, int ay)
{
	glBegin(GL_POINTS);
		glVertex2i(ax,ay);
	glEnd();
	glFlush();
}
void drawLine(int ax, int ay, int bx, int by)
{
	glBegin(GL_LINES);
		glVertex2i(ax,ay);
		glVertex2i(bx, by);
	glEnd();
	glFlush();
}
void drawPolygon( GLintPoint vertices[], int size)
{
	int count = 0;
	
	while(count < size - 1){
        drawPoint(vertices[count].x, vertices[count].y);
		drawLine(vertices[count].x, vertices[count].y, vertices[count + 1].x, vertices[count + 1].y);
		count++;
	}
    drawPoint(vertices[size- 1].x, vertices[size- 1].y);
	drawLine(vertices[size- 1].x, vertices[size - 1].y, vertices[0].x, vertices[0].y);
    printf("HELLO %d", size);
	/*glBegin(GL_POLYGON);
	for(int i = 0; i < size; i++){
		glVertex2i(vertices[i].x,vertices[i].y);
	}
	glEnd();
	glFlush();*/
}
// Start from lower left corner
typedef struct edgebucket
{
	int ymax; //max y-coordinate of edge
	float xcur; //x-coordinate of lowest edge point updated only in aet
	float slopeinverse; // 1/m = (x1-x0)/(y1-y0)
}EdgeBucket;

typedef struct edgetablel
{
	// the array will give the scanline number
	// The edge table (ET) with edges entries sorted
	// in increasing y and x of the lower end
	
	int countEdgeBucket; //no. of edgebuckets, height

	EdgeBucket buckets[maxVer];
}EdgeTableList;

EdgeTableList EdgeTable[maxHt], ActiveEdgeList;

void myDisplay(void)
{
	glClearColor(0.5, 0.5, 0.5, 0.0);    // select clearing (background) color
	glClear(GL_COLOR_BUFFER_BIT);		// clear the screen
	glFlush();							// send all output to display
}

// Scanline Function
void initEdgeTable()
{
	int i;
	for (i=0; i<maxHt; i++)
	{
		EdgeTable[i].countEdgeBucket = 0;
	}
	
	ActiveEdgeList.countEdgeBucket = 0;
}


void printList(EdgeTableList *list)
{
	int j;
	
	if (list->countEdgeBucket)
		printf("\nCount %d-----\n",list->countEdgeBucket);
		
		for (j=0; j<list->countEdgeBucket; j++)
		{
			printf("%d) %d+%.2f+%.2f", j,
			list->buckets[j].ymax, list->buckets[j].xcur,list->buckets[j].slopeinverse);
		}
}

void printTable()
{
	int i,j;
	
	for (i=0; i<maxHt; i++)
	{
		if (EdgeTable[i].countEdgeBucket)
			printf("\nScanline %d", i);
			
		printList(&EdgeTable[i]);
	}
}


/* Function to sort an array using insertion sort*/
void insertionSort(EdgeTableList *edgeTable)
{
	int i,j;
	EdgeBucket temp;
	//sort all list entries in the table
	for (i = 1; i < edgeTable->countEdgeBucket; i++){
		temp.ymax = edgeTable->buckets[i].ymax;
		temp.xcur = edgeTable->buckets[i].xcur;
		temp.slopeinverse = edgeTable->buckets[i].slopeinverse;
		j = i - 1;
		//sort with all other edges of the same list
		while ((temp.xcur < edgeTable->buckets[j].xcur) && (j >= 0))
		{
			edgeTable->buckets[j + 1].ymax = edgeTable->buckets[j].ymax;
			edgeTable->buckets[j + 1].xcur = edgeTable->buckets[j].xcur;
			edgeTable->buckets[j + 1].slopeinverse = edgeTable->buckets[j].slopeinverse;
			j = j - 1;
		}
		edgeTable->buckets[j + 1].ymax = temp.ymax;
		edgeTable->buckets[j + 1].xcur = temp.xcur;
		edgeTable->buckets[j + 1].slopeinverse = temp.slopeinverse;
	}
}


void storeEdgeInList (EdgeTableList *receiver,int ym,int xm,float slopInv)
{
	// both used for edgetable and active edge table..
	// The edge List sorted in increasing ymax and x of the lower end.
	(receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
	(receiver->buckets[(receiver)->countEdgeBucket]).xcur = (float)xm;
	(receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;
			
	// sort the buckets
	insertionSort(receiver);
		
	(receiver->countEdgeBucket)++;
	
	
}

void storeEdgeInTable (int x1,int y1, int x2, int y2)
{
	float m,minv;
	int ymaxTS,xwithyminTS, scanline; //ts stands for to store
	
	if (x2==x1)
	{
		minv=0.000000;
	}
	else
	{
	m = ((float)(y2-y1))/((float)(x2-x1));
	
	// horizontal lines are not stored in edge table
	if (y2==y1)
		return;
		
	minv = (float)1.0/m;//inverse slope
	printf("\nSlope string for %d %d & %d %d: %f",x1,y1,x2,y2,minv);
	}
	
	if (y1>y2)
	{
		scanline=y2;
		ymaxTS=y1;
		xwithyminTS=x2;
	}
	else
	{
		scanline=y1;
		ymaxTS=y2;
		xwithyminTS=x1;	
	}
	// the assignment part is done..now storage..
	storeEdgeInList(&EdgeTable[scanline],ymaxTS,xwithyminTS,minv);
	
	
}

void removeEdgeByYmax(EdgeTableList *activeList,int yy)
{
	int i,j;
	//for all active edges, check if ymax == y current
	for (i=0; i< activeList->countEdgeBucket; i++)
	{
		if (activeList->buckets[i].ymax == yy)	//if true, remove edge from active list
		{
			printf("\nRemoved at %d",yy);
			
			//shift list -1 position to the left
			for ( j = i ; j < activeList->countEdgeBucket -1 ; j++ )
				{
				activeList->buckets[j].ymax =activeList->buckets[j+1].ymax;
				activeList->buckets[j].xcur =activeList->buckets[j+1].xcur;
				activeList->buckets[j].slopeinverse = activeList->buckets[j+1].slopeinverse;
				}
				activeList->countEdgeBucket--;
			i--;
		}
	}
}	


void updatexbyslopeinv(EdgeTableList *activeList)//update x coord when scanline (y) changes, by the slope of each edge
{
	int i;
	
	for (i=0; i<activeList->countEdgeBucket; i++)
	{
		(activeList->buckets[i]).xcur =(activeList->buckets[i]).xcur + (activeList->buckets[i]).slopeinverse;
	}
}


void ScanlineFill()
{
	/* Follow the following rules:
	1. Horizontal edges: Do not include in edge table
	2. Horizontal edges: Drawn either on the bottom or on the top.
	3. Vertices: If local max or min, then count twice, else count
		once.
	4. Either vertices at local minima or at local maxima are drawn.*/


	int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;
	
	// we will start from scanline 0;
	// Repeat until last scanline:
	for (i=0; i<maxHt; i++)//4. Increment y by 1 (next scan line)
	{
		printList(&ActiveEdgeList);
		// 1. Add to the AEL all the edges from the current
		// AET position, so those edges whose ymin = y (entering edges)
		for (j=0; j<EdgeTable[i].countEdgeBucket; j++)
		{
			storeEdgeInList(&ActiveEdgeList,EdgeTable[i].buckets[j].
					ymax,EdgeTable[i].buckets[j].xcur,
					EdgeTable[i].buckets[j].slopeinverse);
		}
		//printList(&ActiveEdgeList);
		
		// 2. Remove from AET those edges for
		// which y=ymax (not involved in next scan line)
		removeEdgeByYmax(&ActiveEdgeList, i);
		
		//sort AET (remember: EL is presorted) according to x ascending
		insertionSort(&ActiveEdgeList);
		
		
		
		//3. Fill lines on scan line y by using pairs of x-coords from AET
		j = 0;
		FillFlag = 0;
		coordCount = 0; // arithmos shmeiwn tomhs, an perittos, tote to trexoshmeio einai mesa sto polygono-> color it, alliws an artios einai ektos
		x1 = 0;
		x2 = 0;
		ymax1 = 0;
		ymax2 = 0;
		while (j<ActiveEdgeList.countEdgeBucket)
		{
			if (coordCount%2==0) // ana 2, pairnei zeygh shmeiwn apo ta edges kai zwgrafizei anamesa tous, analoga me arithmo intersections
			{
				x1 = (int)(ActiveEdgeList.buckets[j].xcur);
				ymax1 = ActiveEdgeList.buckets[j].ymax;
				if (x1==x2)
				{
				/* three cases can arrive-
					1. lines are towards top of the intersection
					2. lines are towards bottom
					3. one line is towards top and other is towards bottom
				*/
					if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))//Case 3
					{
						x2 = x1;
						ymax2 = ymax1;
					}
					else
					{
						coordCount++;
					}
				}
				else
				{
					coordCount++;
				}
			}
			else
			{
				x2 = (int)ActiveEdgeList.buckets[j].xcur;
				ymax2 = ActiveEdgeList.buckets[j].ymax;
			
				FillFlag = 0;
				
				// checking for intersection...
				if (x1==x2)
				{
				/*three cases can arrive to check for intersection-
					1. both lines are towards top of the intersection, local minima, count as 2 intersections
					2. lines are towards bottom, local maxima, do not count
					3. one line is towards top and other is towards bottom, count as 1 intersecition
				*/
					if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))// Case 3, count 1 intersection
					{
						x1 = x2;	
						ymax1 = ymax2;
					}
					else
					{
						if((x1!=ymax1)&&(x2!=ymax2) && (ymax1!=ymax2))//Case 1, (ymax1!=ymax2) to check if not the sane point, condition from above
							FillFlag = 1;
						coordCount++;
						//FillFlag = 1;
					}
				}
				else//draw line inbetween the x1 and x2
				{
					coordCount++;
					FillFlag = 1;
				}
			
			
                if(FillFlag)
                {
                    //drawing actual lines...
                    glColor3f(rgb[0],rgb[1],rgb[2]);
                    
                    glBegin(GL_LINES);
                    glVertex2i(x1,i);
                    glVertex2i(x2,i);
                    glEnd();
                    glFlush();		
                    
                    // printf("\nLine drawn from %d,%d to %d,%d",x1,i,x2,i);
                }  
            }
                
            j++;
	    }
            
        // 5. For each nonvertical edge remaining in AET, update x for new y by its slope
        updatexbyslopeinv(&ActiveEdgeList);
    }

    printf("\nScanline filling complete");

}


void myInit(void)
{

	//glClearColor(0.5, 0.5, 0.5, 0.0);    // select clearing (background) color
	glClear(GL_COLOR_BUFFER_BIT);		// clear the screen
	//glColor3f(1.0, 1.0, 1.0);           // initialize viewing color values
	glColor3f(0.0,0.0,1.0);
	glPointSize(4);						// set point size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLfloat)maxWd, 0.0,(GLfloat)maxWd);// Creates a matrix for projecting two dimensional
																	// coordinates onto the screen and multiplies the 
																	// current projection matrix by it.
																	// It is used for projecting a two dimensional image 
																	// onto a two dimensional screen. 
																	// For two dimensional objects using two dimensional 
																	// vertex commands all z coordinates are zero.
	glFlush();
}

/*void drawPolyDino()
{

	glColor3f(1.0f,0.0f,0.0f);
	int count = 0,x1,y1,x2,y2;
	rewind(fp);
	while(!feof(fp) )
	{
		count++;
		if (count>2)
		{
			x1 = x2;
			y1 = y2;
			count=2;
		}
		if (count==1)
		{
			fscanf(fp, "%d,%d", &x1, &y1);
		}
		else
		{
			fscanf(fp, "%d,%d", &x2, &y2);
			printf("\n%d,%d", x2, y2);
			glBegin(GL_LINES);
				glVertex2i( x1, y1);
				glVertex2i( x2, y2);
			glEnd();
			storeEdgeInTable(x1, y1, x2, y2);//storage of edges in edge table.
			
			
			glFlush();
		}
	}
		
		
}

void drawDino(void)
{
	initEdgeTable();
	drawPolyDino();
	printf("\nTable");
	printTable();
	
	ScanlineFill();//actual calling of scanline filling..
}*/
// Menu choices
void myMenu(int selection)
{
	switch(selection) {

		case POINTS: // drawing  line
			sxhma=1;
			numCorners = 0;
			glutPostRedisplay(); // put comments to see what happens
			break;

		case FILLING: // drawing rectangle
			sxhma=2;
            //First create the polygon lines
            /*drawPolygon(corner, numCorners);
            if(numCorners > 2 ){
                //Then color it
                //initialise the edge table
                initEdgeTable();
                //assuming corners array is intact and has all vertices saved,
                //store edges in the edge table
                for(int j = 0; j < numCorners - 1; j++)
                    storeEdgeInTable(corner[j].x, corner[j].y, corner[j+1].x, corner[j+1].y);//storage of edges in edge table.
                storeEdgeInTable(corner[numCorners - 1].x, corner[numCorners - 1].y, corner[0].x, corner[0].y);//storage of edges in edge table.
               
                ScanlineFill();
                
                numCorners = 0;
            }
			printf("\n=====Table============");
	        printTable();
			glFlush();*/
			glutPostRedisplay();
			break;

		case CLEAR_SCREEN:
			glClear(GL_COLOR_BUFFER_BIT);  // clear the window
			numCorners = 0;
			glFlush();
			break;
		case LINE_RED_COLOR:
            rgb[0] = 1.0;
            rgb[1] = 0.0;
            rgb[2] = 0.0;
			glColor3f(1.0,0.0,0.0);
			break;

		case LINE_GREEN_COLOR:
            rgb[0] = 0.0;
            rgb[1] = 1.0;
            rgb[2] = 0.0;
			glColor3f(0.0,1.0,0.0);
			break;

		case LINE_BLUE_COLOR:
            rgb[0] = 0.0;
            rgb[1] = 0.0;
            rgb[2] = 1.0;
			glColor3f(0.0,0.0,1.0);
			break;
		default:
			printf("Invalid menu selection\n");
			break;
	}

}
//Specifying a polygon with the mouse
void myMouse (int button, int state, int x1, int y1)
{
	
	//static int numCorners = 0;
	if (button == GLUT_LEFT_BUTTON && state ==GLUT_DOWN)
	{		
		if(sxhma == 1){ //set points
			if(numCorners < 10 ){ //maximum points
				corner[numCorners].x =x1;
				corner[numCorners].y = maxHt - y1;
				drawPoint(corner[numCorners].x,corner[numCorners].y);
				numCorners++;
				
        	}
			else{
				printf("MAX POINTS, CLEAR SCREEN\n");
			}
		}
		
		
        if(sxhma == 2) //create and fill polygon
        {
            
            //First create the polygon lines
            drawPolygon(corner, numCorners);
			
            if(numCorners > 2 ){
                //Then color it
                //initialise the edge table
                initEdgeTable();
                //assuming corners array is intact and has all vertices saved,
                //store edges in the edge table
                for(int j = 0; j < numCorners - 1; j++)
                    storeEdgeInTable(corner[j].x, corner[j].y, corner[j+1].x, corner[j+1].y);//storage of edges in edge table.
                storeEdgeInTable(corner[numCorners - 1].x, corner[numCorners - 1].y, corner[0].x, corner[0].y);//storage of edges in edge table.
                printf("\nTable");
	            printTable();
                ScanlineFill();
                
                numCorners = 0;
            }
        }
    }
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		glClear(GL_COLOR_BUFFER_BIT);  // clear the window
	glFlush();
	
}

int main(int argc, char** argv){
	/*fp=fopen ("PolyDino.txt","r");
	if ( fp == NULL )
	{
		printf( "Could not open file" ) ;
		return;
	}*/
	int menu, polyColorSubmenu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(maxHt,maxWd);
	// set window initial position
	glutInitWindowPosition(1,1);
	glutCreateWindow("Scanline filled dinosaur");

	// Create a two-level pop-up menu
	polyColorSubmenu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Red", LINE_RED_COLOR);
	glutAddMenuEntry("Green", LINE_GREEN_COLOR);
	glutAddMenuEntry("Blue", LINE_BLUE_COLOR);

    glutCreateMenu(myMenu);
	glutAddMenuEntry("Mark polygon vertices (max 10)", POINTS);
	glutAddMenuEntry("Create & color polygon", FILLING);
	glutAddSubMenu("Choose color", polyColorSubmenu);
	glutAddMenuEntry("Clear Screen", CLEAR_SCREEN);

	// register menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMouseFunc(myMouse);
	myInit();
	glutDisplayFunc(myDisplay);
	
	glutMainLoop();

	glutDestroyMenu(polyColorSubmenu);
	glutDestroyMenu(menu);
	return 0;
}
