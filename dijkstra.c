/* 
Program To Simulate Dijkstra's Algorithm

Created By,

Srujan Kashyap D
Suraj B Poojary
*/

#include<GL/glut.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>

// defines the height and width of the window
GLsizei WIN_WIDTH = 1000, WIN_HEIGHT = 600;

// radius of the circle
const int RADIUS = 30;

// total number of nodes in the window
int NODE_COUNT = 0;

// maximum number of nodes
const int MAX_NODES = 26;

// adjacency matrix to store the graph
int adj_matrix[26][26];

// number of nodes selected to draw a line
int line_node_count = 0;

// source node created
int source_created = 0;

// destination node created
int destination_created = 0;

//begin Dijkstra
int begin_dijkstra = 0;

// instruction to display
int instruct = 1;

const int PI = 3.14285714286;

void dijkstra();
//structure defining a node
struct node
{
    int id;
    int x_coordinate;
    int y_coordinate;
} nodes[26], line_nodes[2], source_node, destination_node;

void init()
{
    glViewport(0,0,WIN_WIDTH, WIN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_WIDTH, 0, WIN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 1);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(5);
}


//write Distance between the nodes to screen
void write_distance(char *text, int x, int y)
{
    if(instruct)
		glColor3f(1.0, 1.0, 1.0);
	else
		glColor3f(1.0, 1.0, 0.0);
    glRasterPos2i(x,y);
    int len = strlen(text);
    for(int i = 0; i < len; i++)
    { 
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , text[i]);
    }
    glFlush();

}

//Function to display instructions
void display_hello()
{
	write_distance(" Welcome! To The Visualization of Dijkstra's Algorithn.", 200, 900);
	write_distance("#  Use Mouse Left Button to Create Nodes.", 250, 800);
	write_distance("#  Select any two Nodes to form a Path.", 250, 770);
	// write_distance("#  Use Mouse Right Button to select Source and Destrination.", 250, 740);
	// write_distance("#  Press ENTER key to begin Dijkstra's Algorithm.", 250, 710);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display_hello();

	glFlush();
}

void init_adj_matrix()
{
    for(int i = 0; i < MAX_NODES; i++)
    {
        for(int j = 0; j < MAX_NODES; j++)
        {
            adj_matrix[i][j] = 1e9;
        }
    }
}

int compute_distance(struct node a, struct node b)
{
	return (int)sqrt(pow(abs(a.x_coordinate - b.x_coordinate), 2) + pow(abs(a.y_coordinate - b.y_coordinate), 2));

}

// Checks if two nodes intersect i.e creates or selects the node
int create_or_select_node(int x, int y)
{
    // if there is no node then just return
    if(!NODE_COUNT)
    {
        return 1;
    }
    
    //create a temporary node
    struct node temp = {NODE_COUNT, x, y};
    
    for(int i = 0; i < NODE_COUNT; i++)
    {
        // if nodes intersect ie click on created node, then create line
        if(compute_distance(temp, nodes[i]) < 2 * RADIUS)
        {
            line_nodes[line_node_count] = nodes[i];
            return 0;
        }
    }
    //if no node is pressed then create the new node at x, y
    return 1;

}

void get_mid_point(int *mid_x, int *mid_y)
{
    *mid_x = (line_nodes[0].x_coordinate + line_nodes[1].x_coordinate)/2;
	*mid_y = (line_nodes[0].y_coordinate + line_nodes[1].y_coordinate)/2;
}

//get perpendicular coordinates to display the text
int get_perpendicular_coordinates(int *x, int *y)
{
    int x1, x2, y1, y2;

	x1 = line_nodes[0].x_coordinate;
	x2 = line_nodes[1].x_coordinate;
	y1 = line_nodes[0].y_coordinate;
	y2 = line_nodes[1].y_coordinate;

	float diff = (x2 - x1)? 0.0001 : x2 - x1;

	float slope = ((float)(y2 - y1)) / (diff);

	float perpendicular_slope = -1*(1 / slope);


	*x = *x + 25; 	
	*y = *y + perpendicular_slope * (*x) + 7;

}

//write Label to nodes to screen
void write_label(char text, int x, int y)
{	
	//change the color to yellow
	glColor3f(1.0, 1.0, 0.0);
    glRasterPos2i(x,y);

    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , text);
}


// funtion to draw a line
void draw_line(char color)
{
    //set color to white
	if(color == 'N' || color == 'R')
		glColor3f(0.3, 0.3, 0.3);

	if(color == 'D')
		glColor3f(0.8, 0.8, 0.8);

	if(color == 'M')
		glColor3f(0.5, 0.0, 0.0);

	//draw lines between selected nodes (stored in line_nodes)
	glBegin(GL_LINES);
		glVertex2f(line_nodes[0].x_coordinate, line_nodes[0].y_coordinate);
		glVertex2f(line_nodes[1].x_coordinate, line_nodes[1].y_coordinate);
	glEnd();
	glFlush();

	//Mid Point of the line segment to display distance
	int mid_x, mid_y;
	char distance_string[10]; 
	//get the mid coordinates 
	get_mid_point(&mid_x, &mid_y);


	//get nodes id, keys for the adjMatrix 
	int id1 = line_nodes[0].id;
	int id2 = line_nodes[1].id;

	//compute distance between the nodes.
	int distance = compute_distance(line_nodes[0], line_nodes[1]);
    
	//store it in adjMatrix
	adj_matrix[id1][id2] = distance;
	adj_matrix[id2][id1] = distance;

	//Convert distance to string
	sprintf(distance_string, "%d",distance);

	//Display to the window
	get_perpendicular_coordinates(&mid_x, &mid_y);
	write_distance(distance_string, mid_x, mid_y);

	glFlush();

	//write both circles' labels again
	write_label('A' + line_nodes[0].id, line_nodes[0].x_coordinate - 5, line_nodes[0].y_coordinate - 14);
	write_label('A' + line_nodes[1].id, line_nodes[1].x_coordinate - 5, line_nodes[1].y_coordinate - 14);

	glFlush();

	//Display the updated matrix to the console
	if(color == 'N')
	{
		printf("\n");
		for(int i = 0; i < NODE_COUNT; i++)
		{
			for(int j = 0; j < NODE_COUNT; j++)
			{
				printf("%d \t", adj_matrix[i][j]);
			}
			printf("\n");
		}
	}
}

//draw a circle
void draw_circle(float cx, float cy, float r, float num_segments, char color_character)
{
    //theta- angle in radians of the point 
	//x- x coordinate of point
	//y- y coordinate of point
	float theta,x,y;

	//Give the center of the circle a label [A-Z]
	if(color_character == 'N')
		write_label((char)65 + NODE_COUNT ,cx - 5, cy - 14);

	//change the color to green if color character = 'N'
	if(color_character == 'N')
		glColor3f(0.0, 1.0, 0.0);

	if(color_character == 'B')
		glColor3f(0.0, 0.0, 1.0);

	if(color_character == 'R')
		glColor3f(1.0, 0.0, 0.0);

	//line loop- connect all the points below 
	glBegin(GL_LINE_LOOP);
 
	for(int i = 0; i < num_segments; i++){
		//compute new angle and get the x, y coordinate
		theta = 2.0f * PI * i / num_segments;
 		x = r * cosf(theta);
		y = r * sinf(theta);
 		
 		//draw the point
		glVertex2f(x + cx, y + cy);
	}
	glEnd();

	//flush to output screen
	glFlush();

}

void mouse_click(int btn, int state, int x, int y)
{
    //Clear Instructions first
	if(instruct)
	{
		instruct = 0;
		glClear(GL_COLOR_BUFFER_BIT);
	}

	//dont listen to mouse events if dijkstra operation is running
	if(begin_dijkstra) 
	{
		return;
	}

	//fix coordinate system
	y = WIN_HEIGHT-y;
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(create_or_select_node(x, y))
        {
            // if node count exceeds the max nodes possible just return
            if(NODE_COUNT >= MAX_NODES)
            {
                return;

            }
            
            draw_circle(x, y, RADIUS, 200, 'N');
            //number of nodes selected to draw a line is again 0
			line_node_count = 0;
			//set the values of the new node (can use a constructor an improvement just a point to remember later)
			nodes[NODE_COUNT].x_coordinate = x;
			nodes[NODE_COUNT].y_coordinate = y;
			nodes[NODE_COUNT].id = NODE_COUNT;
            
            // increment node count
            NODE_COUNT++;

        }
        else // draw a line
        {
            if(line_node_count == 1)
			{
				draw_line('N');
				line_node_count = 0;
			}
			else
			{
				line_node_count++;
			}
        }
    }
	//right click: select source and destination nodes
	if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN && (!source_created || !destination_created))
	{
		//if nodes are created
		if(NODE_COUNT)
		{	
			//if a node is clicked, lineNodes[0] will have the node clicked
			if(!create_or_select_node(x, y))
			{
				//if source node is not assigned
				if(!source_created)
				{
					source_node = line_nodes[0];
					//source node created
					source_created = 1;
					//change color of the source node to (R)ed
					draw_circle(source_node.x_coordinate, source_node.y_coordinate, RADIUS, 200, 'R');
				}
				else 
				{
					//first, check if source is not selected again
					if(((source_node.x_coordinate - line_nodes[0].x_coordinate) != 0) || ((source_node.y_coordinate - line_nodes[0].y_coordinate) != 0 ))
					{
						destination_node = line_nodes[0];
						//destination node created
						destination_created = 1;
						//change color of the destination node to (B)lue
						draw_circle(destination_node.x_coordinate, destination_node.y_coordinate, RADIUS, 200, 'B');
					} 
				}

			}
		}
	}

}

void myReshape(GLsizei w, GLsizei h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	WIN_WIDTH = w;
	WIN_HEIGHT = h;
}

void beginDijkstra(unsigned char key, int x, int y)
{
	printf("%d\n",key);

	//if source or destination nodes are not selected dont start
	if(!source_created || !destination_created)
	{	
		printf("Source node / destination node not selected\n");
		return;
	}
	//begin dijkstra if enter key is pressed
	if((int)key == 13)
	{	
		begin_dijkstra = 1;
		dijkstra();
	}
}

int main(int argc, char** argv)
{
    init_adj_matrix();
    glutInit(&argc, argv);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Dijkstra's Algorithm");
    init();
    glutMouseFunc(mouse_click);
	glutKeyboardFunc(beginDijkstra);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}


int getMinimumDistanceNode(int *distanceFromSource, int *selected)
{
	int minimumNode = -1;
	int minimumDistance = 99999;

	//loop through the nodes
	for(int i = 0; i < NODE_COUNT; i++)
	{
		//if the node is not selected and has low distance choose it for now.
		if(selected[i] != 1 && distanceFromSource[i] < minimumDistance)
		{
			minimumNode = i;
			minimumDistance = distanceFromSource[i];
		}
	}

	//return the node with minimum distance 
	return minimumNode;
}

//route source to destination
void routeNodes(int n1, int n2)
{
	line_nodes[0] = nodes[n1];
	line_nodes[1] = nodes[n2];

	draw_line('M');
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Stroing start time
    clock_t start_time = clock();
 
    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds);
}


void dijkstra()
{

	//distance of all nodes from the source node
	int distanceFromSource[26];

	//node connecting to the current node
	int prev[26];

	//if node is visited
	int selected[26];

	//the current node
	int current;

	//initilize nodes distance with 99999(ie not visited yet), selected to -1(not visited yet), prev to -1(no connection yet)
	for(int i = 0; i < NODE_COUNT; i++)
	{
		distanceFromSource[i] = 99999;
		prev[i] = -1;
		selected[i] = -1;
	}

	//distance of source node to 0 to select it first
	distanceFromSource[source_node.id] = 0;

	for(int i = 0; i < NODE_COUNT; i++)
	{
		//get node with minimum distance not selected
		current = getMinimumDistanceNode(distanceFromSource, selected);
		//mark it selected

		if(current == -1)
			break;

		selected[current] = 1;
		printf("\n%c\n", current + 65);

		//loop through all the nodes finding its neighbours
		for(int j = 0; j < NODE_COUNT; j++)
		{
			//if it's already visited dont bother
			if(selected[j] != 1)
			{	
				//if the distance from the current node is less, select it as it's previous node
				if((distanceFromSource[current] + adj_matrix[current][j]) < distanceFromSource[j])
				{	
					distanceFromSource[j] = distanceFromSource[current] + adj_matrix[current][j];

					if(prev[j] != -1)
					{	
						line_nodes[0] = nodes[prev[j]];
						line_nodes[1] = nodes[j];
						delay(700);
						draw_line('R');

						delay(700);
						line_nodes[0] = nodes[current];
						draw_line('D');
					}
					else
					{	
						delay(700);
						line_nodes[1] = nodes[j];
						line_nodes[0] = nodes[current];
						draw_line('D');
					}

					prev[j] = current;
				}
			}
		}

	}

	while(1)
	{
		if(prev[destination_node.id] == -1)
			break;
		delay(500);
		routeNodes(destination_node.id, prev[destination_node.id]);
		destination_node = nodes[prev[destination_node.id]];
	}

	//display distance from array and previous array to console
	printf("node(i)\t\tdistance\tprevious\tselected\n");
	for(int i = 0; i < NODE_COUNT; i++)
	{
		printf("%c\t\t%d\t\t%c\t\t%d\n",65+i, distanceFromSource[i], ((prev[i] != -1)? 65+prev[i] : ' ') , selected[i]);
	}

}