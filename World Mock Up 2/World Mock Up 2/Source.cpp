/*
COPYRIGHT BENJAMIN ISHERWOOD 25/02/2014
THIS SOFTWARE IS INTENDED FOR OPEN SOURCE USE, REDISTRIBUTION
IS ENCOURAGE

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
*/

/*
CURRENT VERSION ALPHA 0.01
VERSION NOTES
-Basic game operation (i.e. movement, basic collision detection)
-Basic file operation (i.e. fully readable files, no organization structure)
-Texturing
-Walk animations

-No level editor
-No win condition
-No lose condition
-No story elements

UPDATES REQUIRED

-Separation of the test globals from the
keyboard function (This is an NPC issue)

-Separation of the NPC animation and
update procedures

-Locking the mouse to the movement
	-An updated method will be need

-Lose condition needs to be added

-DTD, XML reader and conf reader needs to be implemented

*/

#pragma once
#ifndef _PURE_KLEPTOMANIA
#define _PURE_KLEPTOMANIA	"PURE KLEPTOMANIA"

#include "COLLECTIONS.h"
//#include "RENDERER.h"

using namespace std;

//Current values for maintaining the
//The size of the screen
int WIDTH = 600;
int HEIGHT = 600;

//This is the initial centering of the view port
//Set to 0,0 since the starting position needs to be
//decided by the current map
int viewPortCenter[2] = {0,0};

//These are just test constants and are pretty self
//explanatory
unsigned int initSize[2] = {1,1};
world DAN(initSize, "world.txt");
player PLAYER_ONE(&DAN);
bool pause = false;
//bool suspicious = false;

/*
Added by Ryan and needs to be built
into its own class
*/

//NPC variables
/*int detectionRange = 64 * 100;
int frameCounter = 0;
int randomNumNPC;
int frameStop = 1000; //NPCs will update their direction in less frames if they hit a wall*/
//NPC variables

renderer scene;

#include "KEYBOARD.h"
using namespace kyb;

/*
contains:
player character
	character designates the player that the view port is following

Update viewport creates an invisible box around the player which gives
the area that the camera can see
*/
void updateViewPort(player* character)
{
	//The viewport encompasses 75% of the the center of the scene
	//Therefore when the chracter position reaches 25%> and <75%
	//The view port moves at the same speed as the character
	if((character -> getPositionX() - viewPortCenter[0]) > 0.75*WIDTH)
		viewPortCenter[0] += character -> getSpeed(); 
	else if((character -> getPositionX() - viewPortCenter[0]) < 0.25*WIDTH)
		viewPortCenter[0]-= character -> getSpeed(); 
	if((character -> getPositionY() - viewPortCenter[1]) > 0.75*HEIGHT)
		viewPortCenter[1]+= character -> getSpeed(); 
	else if((character -> getPositionY() - viewPortCenter[1]) < 0.25*HEIGHT)
		viewPortCenter[1]-= character -> getSpeed(); 
}

/*
Passive Mouse function that modifies the current hardcoded player information
and then passes it to the scene to render it
*/
void passiveMouse(int x, int y)
{
	int pos[] = {x, y};
	int dimensions[] = {WIDTH, HEIGHT};
	PLAYER_ONE.changeDirection(pos, dimensions);
	scene.animatePlayer(PLAYER_ONE, false);
}

/*
This is the obligatory display function, THE COMMENTS INSIDE
THE FUNCTION ARE IMPORTANT

Required updates:

- Removal of NPC logic and rendering
*/
void display(void)
{
	//Obligatory Set up functions
	glClearColor(GLclampf(0),GLclampf(0),GLclampf(.2),GLclampf(0));
	glutInitDisplayMode(GL_DEPTH | GL_DOUBLE | GL_RGBA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//The Depth buffer is necessary for the transparencies to work
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);

	//LoadIdentiry is used to clear all current transformations (only useful on start up
	//since all movement is done by modifiying the draw and not by using transisitions)
	glLoadIdentity();
	
	gluOrtho2D(viewPortCenter[0], WIDTH + viewPortCenter[0], viewPortCenter[1], HEIGHT + viewPortCenter[1]);
	glViewport(0,0,WIDTH, HEIGHT);

	//Update the view port to maintain the camera on the player
	updateViewPort(&PLAYER_ONE);

	//Never Forget the teture initialization
	scene.render();

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void reshape(int x, int y)
{
	WIDTH = x, HEIGHT = y;
	updateViewPort(&PLAYER_ONE);
}

void idle(void)
{
	if(!pause)
	{
		PLAYER_ONE.setSpeed(2);
		menuStates(PLAYER_ONE, &DAN, &scene);
		 //Added by Ryan
		DAN.updateNPCSet(&PLAYER_ONE, &scene); // This is the NPC idler (it works :D)
		//scene.UpdateActorArrays(&DAN);
	}
}

/*
Populate world reads in unformatted files and populates the current
game world

Required Updates:
-Dynamic file name
-Text file size should be determined by the
file and not the function
-The file reader should be able to handle structured
file types, specifically in 
	-XML(This will be used for data transmission of hard coded entities)
	-DTD(This is the key file that explains to the reader
	which information belongs to which class and how to handle it)
	-CONF(This will be used for the dynamic information such as 
	gamefiles, world files, object files etc.)
	-PNG(This will be used to store images)

-If time permits there should be an update added so that 
files are stored as Byte information and not as plaintext
as to allow a 4 bit shift to each character to make the file
unreadable outside of the game program

*/

/*
Obligatory main function

This requires a lot of clean up

argc == argument count
argv == argument vector
*/
void main(int argc, char* argv[])
{
		resetKeys();

		/*(+) NPC stuff 
		*This will initialize all the actors and push them into DAN.actorSet
		*/
		for (int i = 0; i < 50; i++){
			
			actor newActor(5*64,5*64, 4, "test_subject_2.png", &DAN);
			DAN.addActor(newActor);
		}
		DAN.actorSet = DAN.getActorSet();
		//(-) NPC stuff //

		//scene.setUpCharacters(7);
        tile block;
        object newBlock;

        unsigned int size[] = {5,5};

        //size of text file rows,columns
		unsigned int txtFileSize[] = {26,19};

        DAN.changePlayerStart(size);
        player greg(&DAN);
        PLAYER_ONE = greg;

        block.changeDescription("HOORAY");

        DAN.changeDimension(txtFileSize);

		DAN.addTile(block);
		block.changePassThrough(true);
		block.changeDescription("NOT HOORAY");
		DAN.addTile(block);
		block.changePassThrough(false);
		DAN.addTile(block);
		DAN.addTile(block);
		DAN.addTile(block);
		DAN.addTile(block);
		DAN.addTile(block);
		DAN.addTile(block);
		DAN.addTile(block);
		DAN.addTile(block);
        //DAN.printLog();

        DAN.populateWorld();

		scene.worldToArray(&DAN);
		scene.setUpPlayer("Charactersforreal.png", PLAYER_ONE, &DAN);
		
		scene.setupActorArrays(&DAN);
		unsigned int testPos[2];
		testPos[0]= 3, testPos[1] = 5;

        glutInit(&argc, argv);
        glutInitWindowSize(600,600);
		glutCreateWindow(_PURE_KLEPTOMANIA);
		//Glew has to be initialized on a window for window basiss
		glewInit();
        
		glutDisplayFunc(display);
        glutIdleFunc(idle);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboardInput);
		glutPassiveMotionFunc(passiveMouse);
        glutKeyboardUpFunc(keyRelease);
        glutMainLoop();
}

#endif