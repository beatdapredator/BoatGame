//Base File was taken from circle.cpp end edited to make it in to my boat game.

#  include <cmath>
#  include <string>
#  include <iostream>
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#  include <glm.hpp>
#  include "getbmp.h"
#include <gtx/rotate_vector.hpp>
#pragma comment(lib, "glew32.lib")

using namespace glm;
using namespace std;



//camera control
static float camx = 0;
static float camy = 40.0f;
//effect the boat rotation and acceleration
vec3 boatPosition (0,0,0);
float anglex = 0.0, angley = 0.0;
//sets the boat orriantation
vec3 header(0, 0, -1);
//thw variables to work out the accaleration
int lasttime = 0;
int currenttime = 0;
float deltatime = 0;
float acceleration = 0.0f;
vec3 velocity(0, 0, 0);
void equation(void);
//this is where I declare the ammount of textures
static unsigned int texture[5];
//This is the variable I used to set up the speedometer
float speedometer = 0;


//Functions used from opengl experimentersource chapter 3
//This function is used so that the font can become a string for displaying
void writeBitmapString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

//this function was used from opengl experimentersource chapter 12 and changed to fit my code
void loadExternalTextures()
{
	//This stores the 5 images that im using for my textures
	BitMapFile *image[5];

	// Load the image.
	image[0] = getbmp("water.bmp");//http://www.textures.com/preview/waterplain0008/50936

	// Create texture object texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image[1] = getbmp("sky.bmp");//https://www.textures.com/download/skies0352/72109?q=cloud?secure=login

	// Create texture object texture[1]. 
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image[2] = getbmp("boat.bmp");//http://www.textures.com/preview/woodplanksoverlapping0106/122353?q=wood

	// Create texture object texture[2]. 
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image[3] = getbmp("buoy.bmp");//http://www.textures.com/download/bronzecopper0089/112531?q=metal

	// Create texture object texture[3]. 
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////Equations/////////////////////////////////////////////////////////////
//these are all of 5the equations that my game will need to do to work out the volocity of my boat
//volocity = previous velocity + acceleration x deltatime
void equation(void)
{

	cout << "header: " << header.x << ", " << header.y << ", " << header.z << endl;
	cout << "acc: " << acceleration << endl;
	cout << "deltaTime:" << deltatime << endl;
	velocity = velocity + (header*acceleration) * deltatime;
	boatPosition = boatPosition + (velocity * deltatime) + (0.5f * (header*acceleration)*(deltatime*deltatime));
}

/////////////////////////////////////////////////////////////////////this is the start of my draw sceen and will be where I draw all of my graphics////////////////////////////
// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0, 0, 1);// Water
	
	currenttime = glutGet(GLUT_ELAPSED_TIME);
	deltatime = (currenttime - lasttime) / 1000.0f;  // }this is how i worked out delta time for the gane.
	lasttime = currenttime;

	//equation();

	// This is the starting location of my boat. This is where it will staryt when the game initializes
	gluLookAt(boatPosition.x-(header.x*15), 10, boatPosition.z-(header.z*15) , // the position of the camara
		      boatPosition.x,boatPosition.y,boatPosition.z,// where the camara is looking
	          0.0,1.0,0.0);//the oriantation of the camara is the way you want it 

	/////////////////////////////////////////////////////////////////////////lighting////////////////////////////////////////////////////////////////////////////////////////

	//the material properties of the boat
	float boatAmb[] = { 0.98,1.0,0.0, 1.0 };
	float boatDif[] = { 0.98,0.98,0.0, 1.0 };
	float boatSpec[] = { 1, 1, 1, 1.0 };
	float boatShine[] = { 50 };
	float boatEmission[] = { 0.0, 0.0, 0, 1.0 };
	//the material properties of the boyes
	float boeysAmb[] = { 1.0, 1.0, 1.0, 1.0 };
	float boeysDif[] = { 1.0, 1.0, 1.0, 1.0 };
	float boeysSpec[] = { 1, 1, 1, 1.0 };
	float boeysShine[] = { 50 };
	float boeysEmission[] = { 0.0, 0.0, 0, 1.0 };
	//the material properties for water
	float waterAmb[] = { 0.0,0.0, 1.0, 1.0 };
	float waterDif[] = { 0.0, 0.0, 1.0, 1.0 };
	float waterSpec[] = { 1, 1, 1, 1.0 };
	float waterShine[] = { 50 };
	float waterEmission[] = { 0.0, 0.0, 0, 1.0 };

	////////////////////////////////////////////////////////////////////////Acceleration-deceleration////////////////////////////////////////////////////////////////
	if (acceleration <= 0.0f)
	{
		acceleration = 0.0f;
		velocity *= 0.8f;
	}
	else if (acceleration > 0.0f)
	{
		//cout << "lowering" << endl;

		acceleration -= 0.02f;
	}
	
	
	equation();

	//////////////////////////////////////////////////////////////////////////////////Speedometer/////////////////////////////////////////////////////////////////////////////////
	//This is where I found the way to store the sring as a char* to show the acceleration http://en.cppreference.com/w/cpp/string/byte/memcpy 
	speedometer = acceleration;
	string temp = to_string(speedometer);//convert spedometer to a string
	string speedo = "Speed = " + temp;//make a new sting which is equal to "speed = " the speedometer
	
	char *p = new char[speedo.size() + 1];//generate a char pointer = a char the size of speedo +1
	memcpy(p, speedo.c_str(), speedo.size() + 1);//from the start of speedo its copies the values to the memory location that the pointer is pointing at and does this for the length of speedo

	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glRasterPos3f(boatPosition.x, 12.0, boatPosition.z);//this sets the location of the time to be the same x and z position of the boat and have the y set to a hight I have determined to be appropriate
	writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, p);//this calls upon the P char variable to display the value of speedo and print them as the acceleration and shows the acceleration as you move the boat forward.
	glPopMatrix();
	//////////////////////////////////////////////////////////////////////////the creation of my boat/////////////////////////////////////////////////////////////////////////////
	////Material properties of sphere.
	glMaterialfv(GL_FRONT, GL_AMBIENT, boatAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, boatDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, boatSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, boatShine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, boatEmission);
	
	
	glPushMatrix();
	glTranslatef(boatPosition.x, 0.0, boatPosition.z);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(angley, 0.0,1 , 0.0);
	glColor3f(0.4, 0.6, 0);// This will then set the colour of the boat
	//This is the start of the creation of my boat
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 1, 0); glVertex3f(0, 7, 0);//0
	glVertex3f(0, 0.1, 0);//1
	glVertex3f(3, 7, 3);//2
	glVertex3f(3, 0.1, 3);//3
	glVertex3f(6, 7, 3);//4
	glVertex3f(6, 0.1, 3);//5
	glVertex3f(9, 7,3);//6
	glVertex3f(9, 0.1, 3);//7               //}   This section of 16 lines is the front section of my boat
	glVertex3f(12, 7, 3);//8
	glVertex3f(12, 0.1, 3);//9
	glVertex3f(15, 7,3);//10
	glVertex3f(15, 0.1,3);//11
	glVertex3f(18, 7,3);//12
	glVertex3f(18, 0.1,3);//13
	glVertex3f(21, 7,0);//14
	glVertex3f(21, 0.1, 0);//15
	glVertex3f(18, 7, -3);//16
	glVertex3f(18, 0.1,-3);//17
	glVertex3f(15, 7, -3);//18
	glVertex3f(15, 0.1, -3);//19
	glVertex3f(12, 7, -3);//20
	glVertex3f(12, 0.1, -3);//21
	glVertex3f(9, 7, -3);//22           //} This is the start of the back of my BOAT
	glVertex3f(9, 0.1, -3);//23
	glVertex3f(6, 7, -3);//24
	glVertex3f(6, 0.1, -3);//25
	glVertex3f(3, 7, -3);//26
	glVertex3f(3, 0.1, -3);//27
	glVertex3f(0, 7, 0);//28
	glVertex3f(0, 0.1, 0);//29
	//glPopMatrix();

	glEnd();
	//glPushMatrix();
	//glTranslatef(xBoat, 0.0, zBoat);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0, 1.0, 0);// This will then set the colour of the boat
	glVertex3f(0, 0.1, 0);//1
	glVertex3f(3, 0.1, -3);//27
	glVertex3f(3, 0.1, 3);//3
	glVertex3f(6,0.1,-3);//25
	glVertex3f(6, 0.1, 3);//5
	glVertex3f(9,0.1,-3);//23
	glVertex3f(9,0.1, 3);//7
	glVertex3f(12, 0.1, -3);//23
	glVertex3f(12, 0.1, 3);//9
	glVertex3f(15, 0.1, -3);//19
	glVertex3f(15, 0.1, 3);//11
	glVertex3f(18, 0.1, -3);//17
	glVertex3f(18, 0.1, 3);//13
	glVertex3f(21, 0.1, 0);//15
	glEnd();

	glPopMatrix();

	///////////////////////////////////////////////////////////////////the creation of my start and finish line//////////////////////////////////////////////////////

	//left bar
	//front
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-20, -1, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(-18, -1, -2);
	glTexCoord2f(1.0, 1.0); glVertex3f(-18, 12, -2);
	glTexCoord2f(0.0, 1.0); glVertex3f(-20, 12, -2);
	glEnd();
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-20, -1, -3);
	glTexCoord2f(1.0, 0.0); glVertex3f(-18, -1, -3);
	glTexCoord2f(1.0, 1.0); glVertex3f(-18, 12, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(-20, 12, -3);
	glEnd();
	glPopMatrix();
	
	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-20, -1, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(-20, -1, -3);
	glTexCoord2f(1.0, 1.0); glVertex3f(-20, 12, -2);
	glTexCoord2f(0.0, 1.0); glVertex3f(-20, 12, -3);
	glEnd();
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-18, -1, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(-18, -1, -3);
	glTexCoord2f(1.0, 1.0); glVertex3f(-18, 12, -2);
	glTexCoord2f(0.0, 1.0); glVertex3f(-18, 12, -3);
	glEnd();
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-20, 12, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(-18, 12, -2);
	glTexCoord2f(1.0, 1.0); glVertex3f(-18, 12, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(-20, 12, -3);
	glEnd();
	glPopMatrix();

	//bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-20, -1, -2);
	glTexCoord2f(1.0, 0.0);	glVertex3f(-18, -1, -2);
	glTexCoord2f(1.0, 1.0); glVertex3f(-18, -1, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(-20, -1, -3);
	glEnd();
	glPopMatrix();

	//top bar
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-20, 12, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(20, 12, -2);
	glTexCoord2f(1.0, 1.0); glVertex3f(20, 14, -2);
	glTexCoord2f(0.0, 1.0); glVertex3f(-20, 14, -2);
	glEnd();
	glPopMatrix();

	//right bar
	glPushMatrix();
	glBegin(GL_POLYGON);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glTexCoord2f(0.0, 0.0); glVertex3f(20, 12, -2);
	glTexCoord2f(3.0, 0.0); glVertex3f(18, 12, -2);
	glTexCoord2f(3.0, 1.0); glVertex3f(18, -1, -2);
	glTexCoord2f(0.0, 1.0); glVertex3f(20, -1, -2);
	glEnd();
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(20, -1, -3);
	glTexCoord2f(1.0, 0.0); glVertex3f(18, -1, -3);
	glTexCoord2f(1.0, 1.0); glVertex3f(18, 12, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(20, 12, -3);
	glEnd();
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(20, -1, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(20, -1, -3);
	glTexCoord2f(1.0, 1.0); glVertex3f(20, 12, -2);
	glTexCoord2f(0.0, 1.0); glVertex3f(20, 12, -3);
	glEnd();
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(18, -1, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(18, -1, -3);
	glTexCoord2f(1.0, 1.0); glVertex3f(18, 12, -2);
	glTexCoord2f(0.0, 1.0); glVertex3f(18, 12, -3);
	glEnd();
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(20, 12, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(18, 12, -2);
	glTexCoord2f(1.0, 1.0); glVertex3f(18, 12, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(20, 12, -3);
	glEnd();
	glPopMatrix();

	//bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(20, -1, -2);
	glTexCoord2f(1.0, 0.0); glVertex3f(18, -1, -2);
	glTexCoord2f(1.0, 1.0); glVertex3f(18, -1, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(20, -1, -3);
	glEnd();
	glPopMatrix();

	/////////////////////////////////////////////////////////////////////////////////the creation of my square bouys//////////////////////////////////////////////////////////////
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, boeysAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, boeysDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, boeysSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, boeysShine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, boeysEmission);

	// this is the left side of the track that I have made
	//front of box

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-15, 0, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(-10, 0, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(-10, 5, 0);
	glTexCoord2f(0.0, 1.0);glVertex3f(-15, 5, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-15, 0, -5);
	glTexCoord2f(1.0, 0.0);glVertex3f(-10, 0, -5);
	glTexCoord2f(1.0, 1.0);glVertex3f(-10, 5, -5);
	glTexCoord2f(0.0, 1.0);glVertex3f(-15, 5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-15, 5, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(-10, 5, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(-10, 5, -5);
	glTexCoord2f(0.0, 1.0);glVertex3f(-15, 5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right


	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-10, 0, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(-10, 5, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(-10, 5, -5);
	glTexCoord2f(0.0, 1.0);glVertex3f(-10, 0, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-15, 0, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(-15, 0, -5);
	glTexCoord2f(1.0, 1.0);glVertex3f(-15, 5, 0);
	glTexCoord2f(0.0, 1.0);glVertex3f(-15, 5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-15, 0, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(-10, 0, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(-10, 5, -5);
	glTexCoord2f(0.0, 1.0);glVertex3f(-15, 5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-13, 5, -1);
	glTexCoord2f(1.0, 0.0);glVertex3f(-12, 5, -1);
	glTexCoord2f(1.0, 3.0);glVertex3f(-12, 10, -1);
	glTexCoord2f(0.0, 3.0);glVertex3f(-13,10, -1);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-13, 5, -3);
	glTexCoord2f(1.0, 0.0);glVertex3f(-12, 5, -3);
	glTexCoord2f(1.0, 3.0);glVertex3f(-12, 10, -3);
	glTexCoord2f(0.0, 3.0);glVertex3f(-13, 10, -3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-12, 5, -1);
	glTexCoord2f(1.0, 0.0);glVertex3f(-12, 10,-1);
	glTexCoord2f(1.0, 1.0);glVertex3f(-12,10,-3);
	glTexCoord2f(0.0, 1.0);glVertex3f(-12,5,-3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-13, 5, -1);
	glTexCoord2f(1.0, 0.0);glVertex3f(-13,10,-1);
	glTexCoord2f(1.0, 1.0);glVertex3f(-13, 10, -3);
	glTexCoord2f(0.0, 1.0);glVertex3f(-13, 5, -3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-12, 10, -1);
	glTexCoord2f(0.5, 0.0);glVertex3f(-12, 10, -3);
	glTexCoord2f(0.5, 0.5);glVertex3f(-13, 10, -3);
	glTexCoord2f(0.0, 0.5);glVertex3f(-13, 10, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-12, 5, -1);
	glTexCoord2f(0.5, 0.0);glVertex3f(-12, 5, -3);
	glTexCoord2f(0.5, 0.5);glVertex3f(-13, 5, -3);
	glTexCoord2f(0.0, 0.5);glVertex3f(-13, 5, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

    ////////////////////////////////////////////////////////////////////////////////////////bouy 2///////////////////////////////////////////////////////////////////////
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 0, -40);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -40);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -45);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 0, -45);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -45);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 5, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 5, -40);
	glTexCoord2f(1.0, 1.0);glVertex3f(-10, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -45);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10, 5, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 5, -45);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 0, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(-10, 0, -40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(-15, 0, -45);
	glTexCoord2f(1.0, 1.0);	glVertex3f(-15, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -40);
	glTexCoord2f(1.0, 0.0);	glVertex3f(-10, 0, -40);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -45);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -42);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 5, -42);
	glTexCoord2f(1.0, 3.0); glVertex3f(-12, 10, -42);
	glTexCoord2f(0.0, 3.0); glVertex3f(-13, 10, -42);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -43);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 5, -43);
	glTexCoord2f(1.0, 3.0); glVertex3f(-12, 10, -43);
	glTexCoord2f(0.0, 3.0); glVertex3f(-13, 10, -43);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 5, -42);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 10, -42);
	glTexCoord2f(1.0, 1.0); glVertex3f(-12, 10, -43);
	glTexCoord2f(0.0, 1.0); glVertex3f(-12, 5, -43);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -42);
	glTexCoord2f(1.0, 0.0); glVertex3f(-13, 10, -42);
	glTexCoord2f(1.0, 1.0); glVertex3f(-13, 10, -43);
	glTexCoord2f(0.0, 1.0); glVertex3f(-13, 5, -43);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 10, -42);
	glTexCoord2f(0.5, 0.0); glVertex3f(-12, 10, -43);
	glTexCoord2f(0.5, 0.5); glVertex3f(-13, 10, -43);
	glTexCoord2f(0.0, 0.5); glVertex3f(-13, 10, -42);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 5, -42);
	glTexCoord2f(0.5, 0.0); glVertex3f(-12, 5, -43);
	glTexCoord2f(0.5, 0.5); glVertex3f(-13, 5, -43);
	glTexCoord2f(0.0, 0.5); glVertex3f(-13, 5, -42);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//////////////////////////////////////////////////////////////////////////////////////bouy 3////////////////////////////////////////////////////////////////
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 0, -80);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -80);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -80);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -85);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 0, -85);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -85);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 5, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 5, -80);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -85);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10, 5, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 5, -85);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 0, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(-10, 0, -80);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(-15, 0, -85);
	glTexCoord2f(1.0, 1.0); glVertex3f(-15, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -80);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-15, 0, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 0, -80);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -85);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -82);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 5, -82);
	glTexCoord2f(1.0, 3.0); glVertex3f(-12, 10, -82);
	glTexCoord2f(0.0, 3.0); glVertex3f(-13, 10, -82);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -83);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 5, -83);
	glTexCoord2f(1.0, 3.0); glVertex3f(-12, 10, -83);
	glTexCoord2f(0.0, 3.0); glVertex3f(-13, 10, -83);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	//right side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 5, -82);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 10, -82);
	glTexCoord2f(1.0, 1.0); glVertex3f(-12, 10, -83);
	glTexCoord2f(0.0, 1.0); glVertex3f(-12, 5, -83);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	//left side
	glPushMatrix();
	glBegin(GL_POLYGON);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -82);
	glTexCoord2f(1.0, 0.0); glVertex3f(-13, 10, -82);
	glTexCoord2f(1.0, 1.0); glVertex3f(-13, 10, -83);
	glTexCoord2f(0.0, 1.0); glVertex3f(-13, 5, -83);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 10, -82);
	glTexCoord2f(0.5, 0.0); glVertex3f(-12, 10, -83);
	glTexCoord2f(0.5, 0.5); glVertex3f(-13, 10, -83);
	glTexCoord2f(0.0, 0.5); glVertex3f(-13, 10, -82);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 5, -82);
	glTexCoord2f(0.5, 0.0); glVertex3f(-12, 5, -83);
	glTexCoord2f(0.5, 0.5); glVertex3f(-13, 5, -83);
	glTexCoord2f(0.0, 0.5); glVertex3f(-13, 5, -82);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//////////////////////////////////////////////////////////////////////////////////////bouy 4///////////////////////////////////////////////////////
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-15, 0, -120);
	glTexCoord2f(1.0, 0.0);	glVertex3f(-10, 0, -120);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -120);
	glTexCoord2f(0.0, 1.0);	glVertex3f(-15, 5, -120);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -125);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 0, -125);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -125);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 5, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 5, -120);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -125);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10, 5, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 5, -125);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 0, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(-10, 0, -120);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(-15, 0, -125);
	glTexCoord2f(1.0, 1.0); glVertex3f(-15, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -120);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	// bottom
	glPushMatrix();
	glBegin(GL_POLYGON);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15, 0, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(-10, 0, -120);
	glTexCoord2f(1.0, 1.0); glVertex3f(-10, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, 5, -125);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -122);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 5, -122);
	glTexCoord2f(1.0, 3.0); glVertex3f(-12, 10, -122);
	glTexCoord2f(0.0, 3.0); glVertex3f(-13, 10, -122);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -123);
	glTexCoord2f(1.0, 0.0); glVertex3f(-12, 5, -123);
	glTexCoord2f(1.0, 3.0); glVertex3f(-12, 10, -123);
	glTexCoord2f(0.0, 3.0); glVertex3f(-13, 10, -123);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 5, -122);
	glTexCoord2f(1.0, 0.0); (-12, 10, -122);
	glTexCoord2f(1.0, 1.0); glVertex3f(-12, 10, -123);
	glTexCoord2f(0.0, 1.0); glVertex3f(-12, 5, -123);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-13, 5, -122);
	glTexCoord2f(1.0, 0.0); glVertex3f(-13, 10, -122);
	glTexCoord2f(1.0, 0.1); glVertex3f(-13, 10, -123);
	glTexCoord2f(0.0, 0.1); glVertex3f(-13, 5, -123);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-12, 10, -122);
	glTexCoord2f(0.5, 0.0); glVertex3f(-12, 10, -123);
	glTexCoord2f(0.5, 0.5); glVertex3f(-13, 10, -123);
	glTexCoord2f(0.0, 0.5); glVertex3f(-13, 10, -122);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-12, 5, -122);
	glTexCoord2f(0.5, 0.0); glVertex3f(-12, 5, -123);
	glTexCoord2f(0.5, 0.5); glVertex3f(-13, 5, -123);
	glTexCoord2f(0.0, 0.5); glVertex3f(-13, 5, -122);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	/////////////////////////////////////////////////////////////////////////the creation of my square bouys right////////////////////////////////////////////////////////
	//This is the creation of the right side of my boys
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -5);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -5);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -5);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 5, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -5);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(10, 0, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, 0);
	glTexCoord2f(1.0, 1.0);	glVertex3f(10, 5, -5);
	glTexCoord2f(0.0, 1.0); glVertex3f(10, 0, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(15, 0, -5);
	glTexCoord2f(1.0, 1.0); glVertex3f(15, 5, -5);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -5);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -1);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 5, -1);
	glTexCoord2f(1.0, 3.0); glVertex3f(12, 10, -1);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -3);
	glTexCoord2f(1.0, 0.0);	glVertex3f(12, 5, -3);
	glTexCoord2f(1.0, 3.0);	glVertex3f(12, 10, -3);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 5, -1);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 10, -1);
	glTexCoord2f(1.0, 1.0); glVertex3f(12, 10, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(12, 5, -3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -1);
	glTexCoord2f(1.0, 0.0); glVertex3f(13, 10, -1);
	glTexCoord2f(1.0, 1.0); glVertex3f(13, 10, -3);
	glTexCoord2f(0.0, 1.0); glVertex3f(13, 5, -3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 10, -1);
	glTexCoord2f(0.5, 0.0); glVertex3f(12, 10, -3);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 10, -3);
	glTexCoord2f(0.0, 0.5); glVertex3f(13, 10, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 5, -1);
	glTexCoord2f(0.5, 0.0); glVertex3f(12, 5, -3);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 5, -3);
	glTexCoord2f(0.0, 0.5); glVertex3f(13, 5, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	///////////////////////////////////////////////////////////////////////////////////bouy 2///////////////////////////////////////////////////////////
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -40);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -40);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -45);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -45);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -45);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 5, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, -40);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -45);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(10, 5, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, -45);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 0, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(10, 0, -40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(15, 0, -45);
	glTexCoord2f(1.0, 1.0); glVertex3f(15, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -40);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -40);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -40);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -45);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -45);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -42);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 5, -42);
	glTexCoord2f(1.0, 3.0); glVertex3f(12, 10, -42);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -42);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -43);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 5, -43);
	glTexCoord2f(1.0, 3.0); glVertex3f(12, 10, -43);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -43);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 5, -42);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 10, -42);
	glTexCoord2f(1.0, 1.0); glVertex3f(12, 10, -43);
	glTexCoord2f(0.0, 1.0); glVertex3f(12, 5, -43);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(13, 5, -42);
	glTexCoord2f(1.0, 0.0); glVertex3f(13, 10, -42);
	glTexCoord2f(1.0, 1.0); glVertex3f(13, 10, -43);
	glTexCoord2f(0.0, 1.0); glVertex3f(13, 5, -43);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 10, -42);
	glTexCoord2f(0.5, 0.0); glVertex3f(12, 10, -43);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 10, -43);
	glTexCoord2f(0.0, 0.5); glVertex3f(13, 10, -42);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 5, -42);
	glTexCoord2f(0.5, 0.0); glVertex3f(12, 5, -43);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 5, -43);
	glTexCoord2f(0.0, 0.5); glVertex3f(13, 5, -42);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	/////////////////////////////////////////////////////////////////////////////////bouy 3/////////////////////////////////////////////////////////////////////
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -80);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -80);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -80);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -85);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -85);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -85);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 5, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, -80);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -85);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(10, 5, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, -85);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 0, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(10, 0, -80);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(15, 0, -85);
	glTexCoord2f(1.0, 1.0); glVertex3f(15, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -80);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -80);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -80);
	glTexCoord2f(1.0, 1.0); (10, 5, -85);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -85);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -82);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 5, -82);
	glTexCoord2f(1.0, 3.0); glVertex3f(12, 10, -82);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -82);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(13, 5, -83);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 5, -83);
	glTexCoord2f(1.0, 3.0); glVertex3f(12, 10, -83);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -83);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 5, -82);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 10, -82);
	glTexCoord2f(1.0, 1.0); glVertex3f(12, 10, -83);
	glTexCoord2f(0.0, 1.0); glVertex3f(12, 5, -83);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -82);
	glTexCoord2f(1.0, 0.0); glVertex3f(13, 10, -82);
	glTexCoord2f(1.0, 1.0); glVertex3f(13, 10, -83);
	glTexCoord2f(0.0, 1.0); glVertex3f(13, 5, -83);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 10, -82);
	glTexCoord2f(0.5, 0.0); glVertex3f(12, 10, -83);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 10, -83);
	glTexCoord2f(0.0, 0.5); glVertex3f(13, 10, -82);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 5, -82);
	glTexCoord2f(0.5, 0.0); glVertex3f(12, 5, -83);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 5, -83);
	glTexCoord2f(0.0, 0.5); glVertex3f(13, 5, -82);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	///////////////////////////////////////////////////////////////////////////////////////////bouy 4////////////////////////////////////////////////////////////////////////////
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -120);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -120);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -120);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -125);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -125);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -125);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 5, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, -120);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -125);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(10, 5, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 5, -125);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 0, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(10, 0, -120);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(15, 0, -125);
	glTexCoord2f(1.0, 1.0); glVertex3f(15, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -120);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(15, 0, -120);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 0, -120);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, 5, -125);
	glTexCoord2f(0.0, 1.0); glVertex3f(15, 5, -125);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -122);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 5, -122);
	glTexCoord2f(1.0, 3.0); glVertex3f(12, 10, -122);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -122);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -123);
	glTexCoord2f(1.0, 0.0);	glVertex3f(12, 5, -123);
	glTexCoord2f(1.0, 3.0); glVertex3f(12, 10, -123);
	glTexCoord2f(0.0, 3.0); glVertex3f(13, 10, -123);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();

	//leftside
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(12, 5, -122);
	glTexCoord2f(1.0, 0.0); glVertex3f(12, 10, -122);
	glTexCoord2f(1.0, 1.0);	glVertex3f(12, 10, -123);
	glTexCoord2f(0.0, 1.0);	glVertex3f(12, 5, -123);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(13, 5, -122);
	glTexCoord2f(1.0, 0.0); glVertex3f(13, 10, -122);
	glTexCoord2f(1.0, 1.0); glVertex3f(13, 10, -123);
	glTexCoord2f(0.0, 1.0); glVertex3f(13, 5, -123);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(12, 10, -122);
	glTexCoord2f(0.5, 0.0);	glVertex3f(12, 10, -123);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 10, -123);
	glTexCoord2f(0.0, 0.5);	glVertex3f(13, 10, -122);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(12, 5, -122);
	glTexCoord2f(0.5, 0.0);	glVertex3f(12, 5, -123);
	glTexCoord2f(0.5, 0.5); glVertex3f(13, 5, -123);
	glTexCoord2f(0.0, 0.5); glVertex3f(13, 5, -122);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	///////////////////////////////////////////////////////////////////////////////////Central Bouy to rotate around /////////////////////////////////////////////////////////////
	//front of box
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 0, -160);
	glTexCoord2f(1.0, 0.0); glVertex3f(3, 0, -160);
	glTexCoord2f(1.0, 1.0); glVertex3f(3, 5, -160);
	glTexCoord2f(0.0, 1.0); glVertex3f(-3, 5, -160);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//back
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 0, -165);
	glTexCoord2f(1.0, 0.0);	glVertex3f(3, 0, -165);
	glTexCoord2f(1.0, 1.0); glVertex3f(3, 5, -165);
	glTexCoord2f(0.0, 1.0); glVertex3f(-3, 5, -165);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 5, -160);
	glTexCoord2f(1.0, 0.0);	glVertex3f(3, 5, -160);
	glTexCoord2f(1.0, 1.0);	glVertex3f(3, 5, -165);
	glTexCoord2f(0.0, 1.0);	glVertex3f(-3, 5, -165);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//left
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(3, 5, -160);
	glTexCoord2f(1.0, 0.0); glVertex3f(3, 5, -165);
	glTexCoord2f(1.0, 1.0); glVertex3f(3, 0, -165);
	glTexCoord2f(0.0, 1.0); glVertex3f(3, 0, -160);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 0, -160);
	glTexCoord2f(1.0, 0.0); glVertex3f(-3, 0, -165);
	glTexCoord2f(1.0, 1.0);	glVertex3f(-3, 5, -165);
	glTexCoord2f(0.0, 1.0); glVertex3f(-3, 5, -160);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// bottom
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 0, -160);
	glTexCoord2f(1.0, 0.0); glVertex3f(3, 0, -160);
	glTexCoord2f(1.0, 1.0); glVertex3f(3, 5, -165);
	glTexCoord2f(0.0, 1.0); glVertex3f(-3, 5, -165);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//front of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 5, -162);
	glTexCoord2f(1.0, 0.0); glVertex3f(1, 5, -162);
	glTexCoord2f(1.0, 3.0); glVertex3f(1, 10, -162);
	glTexCoord2f(0.0, 3.0); glVertex3f(0, 10, -162);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);	glVertex3f(0, 5, -163);
	glTexCoord2f(1.0, 0.0); glVertex3f(1, 5, -163);
	glTexCoord2f(1.0, 3.0); glVertex3f(1, 10, -163);
	glTexCoord2f(0.0, 3.0); glVertex3f(0, 10, -163);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//leftside
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(1, 5, -162);
	glTexCoord2f(1.0, 0.0); glVertex3f(1, 10, -162);
	glTexCoord2f(1.0, 1.0); glVertex3f(1, 10, -163);
	glTexCoord2f(0.0, 1.0); glVertex3f(1, 5, -163);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//right side
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 5, -162);
	glTexCoord2f(1.0, 0.0); glVertex3f(0, 10, -162);
	glTexCoord2f(1.0, 1.0); glVertex3f(0, 10, -163);
	glTexCoord2f(0.0, 1.0); glVertex3f(0, 5, -163);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//top of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(1, 10, -162);
	glTexCoord2f(0.5, 0.0); glVertex3f(1, 10, -163);
	glTexCoord2f(0.5, 0.5); glVertex3f(0, 10, -163);
	glTexCoord2f(0.0, 0.5); glVertex3f(0, 10, -162);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottom of mast
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(1, 5, -162);
	glTexCoord2f(0.5, 0.0); glVertex3f(1, 5, -163);
	glTexCoord2f(0.5, 0.5); glVertex3f(0, 5, -163);
	glTexCoord2f(0.0, 0.5); glVertex3f(0, 5, -162);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

///////////////////////////////////////////////////////////////////////SKY BOX///////////////////////////////////////////////////////////////
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);	
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	//front
	glColor3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0);glVertex3f(-600,-1.5,600);
	glTexCoord2f(1.0, 1.0);glVertex3f(600,-1.5,600);
	glTexCoord2f(1.0, 0.0);glVertex3f(600,598.5,600);
	glTexCoord2f(0.0, 0.0);glVertex3f(-600, 598.5, 600);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	//bottom
	glColor3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0);glVertex3f(600, -1.5, -600);
	glTexCoord2f(10.0, 0.0);glVertex3f(600, -1.5, 600);
	glTexCoord2f(10.0, 10.0);glVertex3f(-600, -1.5, 600);
	glTexCoord2f(0.0, 10.0);glVertex3f(-600, -1.5, -600);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_POLYGON);
	//left
	glColor3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0);glVertex3f(-600,-1.5,600);
	glTexCoord2f(1.0, 1.0);glVertex3f(-600, -1.5, -600);
	glTexCoord2f(1.0, 0.0);glVertex3f(-600,598.5,-600);
	glTexCoord2f(0.0, 0.0);glVertex3f(-600, 598.5, 600);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_POLYGON);
	//top
	glColor3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0);glVertex3f(-600,598.5,600);
	glTexCoord2f(1.0, 0.0);glVertex3f(600, 598.5, 600);
	glTexCoord2f(1.0, 1.0);glVertex3f(600, 598.5, -600);
	glTexCoord2f(0.0, 1.0);glVertex3f(-600, 598.5, -600);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_POLYGON);
	//right
	glColor3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0);glVertex3f(600, -1.5, 600);
	glTexCoord2f(1.0, 1.0);glVertex3f(600, -1.5, -600);
	glTexCoord2f(1.0, 0.0);glVertex3f(600, 598.5, -600);
	glTexCoord2f(0.0, 0.0);glVertex3f(600, 598.5, 600);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_POLYGON);
	//back
	glColor3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0);glVertex3f(-600, -1.5, -600);
	glTexCoord2f(1.0, 1.0);glVertex3f(600, -1.5, -600);
	glTexCoord2f(1.0, 0.0);glVertex3f(600, 598.5, -600);
	glTexCoord2f(0.0, 0.0);glVertex3f(-600, 598.5, -600);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();
//////////////////////////////////////////////////////////////////////////////////////water////////////////////////////////////////////////////////////////////////////////
	glMaterialfv(GL_FRONT, GL_AMBIENT, waterAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, waterDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, waterSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, waterShine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, waterEmission);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);//I have made my water as a Plane as this way it will fit the view box better without worrying about the boat going through the bottom.
	glNormal3f(0, 1, 0); glTexCoord2f(0.0, 0.0); glVertex3f(500, -1, -500);
	glTexCoord2f(10.0, 0.0);glVertex3f(500, -1, 500);
	glTexCoord2f(10.0, 10.0);glVertex3f(-500, -1, 500);
	glTexCoord2f(0.0, 10.0);glVertex3f(-500, -1, -500);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	
	/*glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(boatPosition.x, 0.0, boatPosition.z);
	glLineWidth(5.0f);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);//I have made my water as a Plane as this way it will fit the view box better without worrying about the boat going through the bottom.
	glVertex3f(0, 10, 0);
	glVertex3f(header.x * 5, 10 + (header.y * 5), (header.z*5));
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();*/
	
	glutSwapBuffers();
	glutPostRedisplay();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glGenTextures(4, texture);

	loadExternalTextures();
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);



	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec0[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };


	// Light1 properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	double aspect = (double)w / (double)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, aspect, 0.01, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
		if (key == 'w')
		{
			acceleration += 0.375f;
			glutPostRedisplay();
		}
		if (key == 's')
		{
			acceleration -= 0.375f;
			glutPostRedisplay();
		}
		if (key == 'a')
		{
			angley += 0.7;
			header = normalize(rotateY(header, radians(0.7f)));
			cout << "header: " << header.x << ", " << header.y << ", " << header.z << endl;
			glutPostRedisplay();
		}
		if (key == 'd')
		{
			angley -= 0.7;
			header = normalize(rotateY(header, -radians(0.7f)));
			cout << "header: " << header.x << ", " << header.y << ", " << header.z << endl;
			glutPostRedisplay();
		}
}

void keySpecialInput(int key, int x, int y)
{
	switch (key)
	{
	
	case GLUT_KEY_UP:
		camy += 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		camy -= 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		camx += 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		camx -= 1.0f;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
}

// Main routine.
int main(int argc, char **argv)
{


	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("BoatGame.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	//glutKeyboardUpFunc
	glutSpecialFunc(keySpecialInput);
	glewExperimental = GL_TRUE;
	glewInit();
	cout << "Made By Matthew Dunphy" << endl;
	setup();

	glutMainLoop();
}
