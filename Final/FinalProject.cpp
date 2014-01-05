/*Who Loves Ray Casting? by Jason Paolasini
 Student Number 1162915
 3GC3 Assignment 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
//#include <GLUT/glut.h>
//#include <OpenGL/OpenGL.h>
#include "3DMathLib.h"
#include <math.h>
#include <iostream>
#include <vector>
#include "Object.h"
#include <ctime>
#include <time.h>
#include <string.h>
#include <cmath>




//Global Variables


float camPos[] = {0, 10, 30};
point3D origin(0,0,0);
vec3D normalPlane(0,1,0);
vec3D vel0(0,0,0);
char itemToMake[]={'c','s','t','o','y'};
char currentItem = 'c';
int item = 1;
bool carSelect = true;
point3D start(5,0,30);
bool moving = true;
bool gameComplete = false;
bool winner = false;
int count = 0;
int trafficTimer = 0;
int traffic = 0;
int trafficCounter;

//used to add slow down effect on grass
int friction=0;

float xrot = 10;
float yrot = 0;
float angle=0.0;
float lastx=0.0;
float lasty=0.0;
float carRotation = 0;
float ypos = 0;
float cRadius = 10.0f; // our radius distance from our character



//Lighting Values

float light_pos[] = {0.0, 2.0, -150.0, 0.01};
float light_pos1[] = {-40.0,-20.0,130.0,0.90};
float light_pos2[] = {5.0,20.0,-130.0,0.90};

float amb0[4]  = {1, 1, 1, 1};
float diff0[4] = {1, 1, 1, 1};
float spec0[4] = {1, 1, 1, 1};

// Mouse Ray

ray mouseRay;

#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];



//Imports in the final texture for the victory lane

GLuint texture;
static GLuint texName;

void makeCheckImage(void)
{
      int i, j, c;

      for (i = 0; i < checkImageHeight; i++) {
             for (j = 0; j < checkImageWidth; j++) {
                     c = ((((i&0x8)==0)^((j&0x8))==0))*255;//   c = ((((i&0x8)==0)^((j&0x8))==0))*255;
                     checkImage[i][j][0] = (GLubyte) c;
                     checkImage[i][j][1] = (GLubyte) c;
                    checkImage[i][j][2] = (GLubyte) c;
                     checkImage[i][j][3] = (GLubyte) 255;
                 }
           }
    }






/*Important*** Particle lifespan is not time based,In order to keep a fluent animation
 the lifespan of a particle is based on its Y value or Bounce count. Bomb fragments are 
 based on bounce count.
 */


//TextureLoader from wikibooks.org

//GLuint raw_texture_load(const char *filename, int width, int height)
//{
//    GLuint texture;
//    unsigned char *data;
//    FILE *file;
//    
//    // open texture data
//    file = fopen(filename, "rb");
//    if (file == NULL) return 0;
//    
//    // allocate buffer
//    data = (unsigned char*) malloc(width * height * 4);
//    
//    // read texture data
//    fread(data, width * height * 4, 1, file);
//    fclose(file);
//    
//    // allocate a texture name
//    glGenTextures(1, &texture);
//    
//    // select our current texture
//    glBindTexture(GL_TEXTURE_2D, texture);
//    
//    // select modulate to mix texture with color for shading
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
//    
//    // when texture area is small, bilinear filter the closest mipmap
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//    // when texture area is large, bilinear filter the first mipmap
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    
//    // texture should tile
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    
//    // build our texture mipmaps
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    
//    // free buffer
//    free(data);
//    
//    return texture;
//}




/*Using Lighting, it was important to store the materials that we would be using in a type of datastore
 The Struct was used to store all material properties*/



char currentMaterial = 'g';

typedef struct materialStruct {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
} materialStruct;

materialStruct redPlastic = {
    {0.3, 0.0, 0.0, 1.0},
    {0.6, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    {0.0, 0.0, 0.0, 0.0},
    32.0};
materialStruct greenGrass = {
    {0.0, 0.3, 0.0, 1.0},
    {0.0, 0.6, 0.0, 1.0},
    {0.8, 0.6, 0.0, 1.0},
    {0.0, 0.8, 0.0, 0.0},
    0.0};

materialStruct water = {
    {0.0,0.0,0.3},
    {0.0,0.0,0.6},
    {0.6,0.6,0.8},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct blackPlastic = {
    {0.03,0.03,0.03},
    {0.4,0.4,0.4},
    {0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct whitePlastic = {
    {0.3,.3,0.3},
    {0.9,0.9,0.9},
    {0.1,0.7,0.7},
    {0.0,0.0,0.0,0.0},
    100.0};
materialStruct yellowPlastic = {
    {1,1,0},
    {0.9,0.9,0.9},
    {0.1,0.7,0.7},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct gold = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.0, 0.0, 0.0, 0.0},
    {0.99, 0.91, 0.81, 1.0},
    100.8};


GLfloat redLight_emissive[] = {1.0f, 0.0f, 0.0f, 0.0f};
GLfloat orangeLight_emissive[] = {1.0f, 0.4f, 0.0f, 0.0f};
GLfloat greenLight_emissive[] = {0.0f, 1.0f, 0.0f, 0.0f};





/*Generate the vector that stores all the particles. A vector was used because it is a dynamic datastructure that
can be modified as we add particles*/

std::vector<object> cars;



boundingBox standardBox(){
    
    //initialize the norms for all of the planes
    
    vec3D rightN(-1,0,0);
    vec3D leftN(1,0,0);
    vec3D topN(0,1,0);
    vec3D bottomN(0,-1,0);
    vec3D frontN(0,0,1);
    vec3D backN(0,0,-1);
    
    //initialize origins
    
    point3D rightO(0.5,0,0);
    point3D leftO(-0.5,0,0);
    point3D topO(0,0.5,0);
    point3D bottomO(0,-0.5,0);
    point3D frontO(0,0,0.5);
    point3D backO(0,0,-0.5);
    
    

    plane3D rightP(rightN,rightO);
    
    plane3D leftP(leftN,leftO);
    
    plane3D topP(topN,topO);
    
    plane3D bottomP(bottomN,bottomO);
    
    plane3D frontP(frontN,frontO);
    
    plane3D backP(backN,backO);
    
    boundingBox box(frontP, backP, leftP, rightP, topP, bottomP);

    
    return box;
}



//Creates the drivablevehicle that is used in the game


object myCar(start, 0.0,vel0, 'b',standardBox(), true);


object createObject(){
    
    object newObject(origin, 0.0,vel0, 'b',standardBox(), true);

    
    return newObject;
}


//Traffic light creation to let the drivers know when it is a good time to go
//We felt that this added to the feel of the game and the use of emmission from the lights
//was well worth it


void drawTrafficLight(float size, GLfloat topEmissive[], GLfloat middleEmissive[], GLfloat bottomEmissive[]) {
	glMaterialfv(GL_FRONT, GL_EMISSION, blackPlastic.emission);
	glBegin(GL_QUADS);
		glVertex3f(-size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f,  size/2);
		glVertex3f(-size/2, -size*1.5f,  size/2);

		glVertex3f(-size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f, -size/2);
		glVertex3f( size/2,  size*1.5f, -size/2);
		glVertex3f(-size/2,  size*1.5f, -size/2);

		glVertex3f(-size/2, -size*1.5f, -size/2);
		glVertex3f(-size/2, -size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f, -size/2);

		glVertex3f(-size/2, -size*1.5f,  size/2);
		glVertex3f( size/2, -size*1.5f,  size/2);
		glVertex3f( size/2,  size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f,  size/2);

		glVertex3f( size/2, -size*1.5f, -size/2);
		glVertex3f( size/2, -size*1.5f,  size/2);
		glVertex3f( size/2,  size*1.5f,  size/2);
		glVertex3f( size/2,  size*1.5f, -size/2);
		
		glVertex3f(-size/2,  size*1.5f, -size/2);
		glVertex3f( size/2,  size*1.5f, -size/2);
		glVertex3f( size/2,  size*1.5f,  size/2);
		glVertex3f(-size/2,  size*1.5f,  size/2);
	glEnd();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, topEmissive);
		glTranslatef(0, size, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, middleEmissive);
		glTranslatef(0, 0, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, bottomEmissive);
		glTranslatef(0, -size, size/2);
		glutSolidSphere(size/3, 16, 16);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, blackPlastic.emission);
}





void notesToTheTA(void){
    
    printf("--------------------------- Car Selection -\nClick the mouse to select a car\nHit ENTER to confirm\nLeft/right = Camera Pan X\nUp/down = Camera Pan Y\nF1/F2 = Camera Pan Z\nEsc = Quit\n----------------------- Race Instructions -\nMouse = Direct Car\nW = Drive\nS = Reverse\nA = Left\nD = Right\nR = Reset\nEsc = Quit\n)");}




void deselectAll(){
    
    for(int i = 0; i<cars.size();i++){
        cars[i].selected = false;
    }
}


//Timer

void timer(int id){
    glutPostRedisplay();
}





object translateFunction(object vehicle,float x, float y, float z){
    
   
     
            
            vec3D trans(x,y,z);
            
            vehicle.location = vehicle.location.shift(trans);
            
            
            //shift all bounding boxes
            vehicle.box.left.origin =  vehicle.box.left.origin.shift(trans);
            vehicle.box.right.origin = vehicle.box.right.origin.shift(trans);
            vehicle.box.top.origin =  vehicle.box.top.origin.shift(trans);
            vehicle.box.bottom.origin = vehicle.box.bottom.origin.shift(trans);
            vehicle.box.back.origin = vehicle.box.back.origin.shift(trans);
            vehicle.box.front.origin = vehicle.box.front.origin.shift(trans);
            
    return vehicle;
    
    
    
    
}


//Not used for cars because they dont scale...


object scaleFunction(object car,float x, float y, float z){
   
            
            
            
            //shift all bounding boxes
            //this is done by calculating the distance between the center of the object and the far edges
            //This value is then multiplied by half the scaling factor to increase the bounding box size
            
            
            car.box.left.origin = car.location.shift(createVector(car.location, car.box.left.origin).scalarMultiply(1+x));
            
            car.box.right.origin = car.location.shift(createVector(car.location, car.box.right.origin).scalarMultiply(1+x));
            
            car.box.bottom.origin = car.location.shift(createVector(car.location, car.box.bottom.origin).scalarMultiply(1+y));
            
            car.box.top.origin = car.location.shift(createVector(car.location, car.box.top.origin).scalarMultiply(1+y));
            
            car.box.front.origin = car.location.shift(createVector(car.location, car.box.front.origin).scalarMultiply(1+z));
    
            car.box.back.origin = car.location.shift(createVector(car.location, car.box.back.origin).scalarMultiply(1+z));
            
    
    return car;
    
}


void rotationRunction(float x,float y, float z,float angle){
    
    
    
}




void clearcars(object object1,int i){
    
    
    cars.erase(cars.begin()+i);
    
    
}






void reset(){
    
    for(int i = 0 ;i<cars.size();i++){
        clearcars(cars[i], i);
    }
        
        carSelect = true;
        moving = true;
        gameComplete = false;
        winner = false;
        count = 0;
        
        //used to add slow down effect on grass
        friction=0;
        traffic = 0;
    trafficCounter=0;
        xrot = 10;
        yrot = 0;
        angle=0.0;
        lastx=0.0;
        lasty=0.0;
        carRotation = 0;
        ypos = 0;
        cRadius = 10.0f; // our radius distance from our character
    
        
        
        //Lighting Values
        
        float light_pos[] = {0.0, 2.0, -150.0, 0.01};
        float light_pos1[] = {-40.0,-20.0,130.0,0.90};
        float light_pos2[] = {5.0,20.0,-130.0,0.90};
        
        float amb0[4]  = {1, 1, 1, 1};
        float diff0[4] = {1, 1, 1, 1};
        float spec0[4] = {1, 1, 1, 1};
    
    
}


//Bitmap character strings user for informing users of the game




void drawBitmapText(char *string, float x, float y, float z)
{
    char *c;
    glRasterPos3f(x, y, z);

    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}




void drawStrokeText(char*string, int x, int y, int z)
{
    char *c;
    glPushMatrix();
    glTranslatef(x, y+8, z);

    for (c=string; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}





void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
    
}





void updateForward(){
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    myCar.location.x += 3.5*float(sin(yrotrad))-3.5*friction;
    myCar.location.z -= 3.5*float(cos(yrotrad))-3.5*friction;
    ypos -= 6.5*float(sin(xrotrad));
    moving = true;
}




void updateBackward(){
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    myCar.location.x -= float(sin(yrotrad));
    myCar.location.z += float(cos(yrotrad));
    ypos += float(sin(xrotrad));
}





//All keyboard controls worked with a tutorial from http://www.swiftless.com/


void keyboard(unsigned char key, int x, int y)
{
    
    int mod  = glutGetModifiers();

	if ( key==27)
        {
			exit (0);
            
        }
    if(key == 13){
        carSelect = false;
        //carSelect=false;
        
    }

        if (key=='q')
        {
            xrot += 1;
            if (xrot >360) xrot -= 360;
        }
        
        if (key=='e')
        {
            xrot -= 1;
            if (xrot < -360) xrot += 360;
        }
        
        if (key=='w')
        {
            updateForward();
        }
    
    
        if (key=='s')
        {
            updateBackward();
        }
        
        if (key=='d')
        {
            float yrotrad;
            yrotrad = (yrot / 180 * 3.141592654f);
            myCar.location.x += float(cos(yrotrad)) * 0.2;
            myCar.location.z += float(sin(yrotrad)) * 0.2;
        }
        
        if (key=='a')
        {
            float yrotrad;
            yrotrad = (yrot / 180 * 3.141592654f);
            myCar.location.x -= float(cos(yrotrad)) * 0.2;
            myCar.location.z -= float(sin(yrotrad)) * 0.2;
        }

     if(key == 'r'){
         while(cars.size()>0){
        
         reset();
        }
     }
    
    
    //Used to change Lighting Positions
    
     else if(key == '6'){
        light_pos1[0]=light_pos1[0]+5;
         light_pos[0]=light_pos[0]+5;
         
     }
     else if(key == '7'){
         light_pos1[0]=light_pos1[0]-5;
         light_pos[0]=light_pos[0]-5;
         
     }
     else if(key == '8'){
         
         light_pos1[1]=light_pos1[1]+5;
         light_pos[1]=light_pos[1]+5;
     }
     else if(key == '9'){
         light_pos1[1]=light_pos1[1]-5;
         light_pos[1]=light_pos[1]-5;
         
     }
     else if(key == '0'){
         
         light_pos1[2]=light_pos1[2]+5;
         light_pos[2]=light_pos[2]+5;
     }
     else if(key == '-'){
         
         light_pos1[2]=light_pos1[2]-5;
         light_pos[2]=light_pos[2]-5;
     }
  
    

}





// mouse movement for game

void mouseMovement(int x, int y) {
    int diffx=x-lastx;
    int diffy=y-lasty; //check the difference between the
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position

    xrot += (float) diffy; //set the xrot to xrot with the addition
   
    
    if (xrot<360) {
        xrot = 360;
    }
    else if(xrot>450){
        xrot = 450;
    }
    yrot += (float) diffx;    //set the xrot to yrot with the addition
}






void init(void)
{
	glClearColor(0.25, 0.25, 0.25, 0); //sets backgorund to grey
	//---------------------------------------------
    glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImage);
 
    
    
//---------------------------------------------------------------
//load texture
    
// texture = raw_texture_load("road.raw", 256, 256);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 500);

    
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec0);
    
    glLightfv(GL_LIGHT2, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spec0);
   
    
}





// In-order to quickly call material changes, The glMaterial calls are quickly accessed by key value

void setMaterial(char key){
    
    switch (key) {
        case 'r':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,redPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlastic.shininess);
            
            break;
            
        case 'b':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,water.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, water.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, water.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, water.shininess);
            
            break;
            
        case 'B':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,blackPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackPlastic.shininess);
            
            break;
            
        case 'G':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,greenGrass.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,greenGrass.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greenGrass.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, greenGrass.shininess);
            
            break;
            
        case 'w':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,whitePlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whitePlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whitePlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, whitePlastic.shininess);
            
            break;
        
        case 'y':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,yellowPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,yellowPlastic.shininess);
            
            break;
            
        case 'g':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,gold.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,gold.shininess);
            break;
        
        default:
            break;
    }
    
    
    
    
}


void makeCar(char col){
    glPushMatrix();
    glPushMatrix();
    setMaterial('B');
    glPushMatrix();
    glTranslatef(1.5,0,1);
    glRotatef(90, 0,1,0);
    glRotatef(180, 1,0,0);
    glutSolidCone(1,1,100,1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.5,0,1);
    glRotatef(-90, 0,1,0);
    glRotatef(-180, 1,0,0);
    glutSolidCone(1,1,100,1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.5,0,-1.5);
    glRotatef(90, 0,1,0);
    glRotatef(180, 1,0,0);
    glutSolidCone(1,1,100,1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.5,0,-1.5);
    glRotatef(-90, 0,1,0);
    glRotatef(-180, 1,0,0);
    glutSolidCone(1,1,100,1);
    glPopMatrix();
    
    glPushMatrix();
    //glTranslatef(XCube,YCube,ZCube);
    glutSolidCone(0.5,1,3,3);
    glPopMatrix();
    
    setMaterial(col);
    
    glTranslatef(0,1,0);
    glutSolidCube(2);
    glPopMatrix();
    //setMaterial('b');
    
    glScalef(1, 1, 2);
    glutSolidCube(2);
    
    glPopMatrix();
}

//Used an online tutorial to build this method to creat the ray vector
//http://nehe.gamedev.net/article/using_gluunproject/16013/





vec3D GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return vec3D(posX, posY, posZ);
    

}







void moveCar(){
    if(moving){
        cars[2].location.z = cars[2].location.z-(cars[2].location.z-(myCar.location.z))-30+((cars[2].location.z-(myCar.location.z))/(1+rand()%300))-((cars[2].location.z-(myCar.location.z))/(1+rand()%300));
    
      ;
    }
    
    translateFunction(cars[1], cars[1].speed.x, cars[1].speed.y, cars[1].speed.z);
      moving=false;

}




//Camera controls in main menu


void special(int key, int x, int y)
{
	//arrow key presses move the camera
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=2.5;
			break;
            
		case GLUT_KEY_RIGHT:
			camPos[0]+=2.5;
			break;
            
		case GLUT_KEY_UP:
			camPos[2] -= 2.5;
            
			break;
            
		case GLUT_KEY_DOWN:
			camPos[2] += 2.5;
			break;
            
		case GLUT_KEY_F1:
			camPos[1] += 2.5;
			break;
            
		case GLUT_KEY_F2:
			camPos[1] -= 2.5;
			break;
        
            
	}
	
}



//Continually generates particles at the users selected rate set by ballCount

void idle(){

    

    
}



point3D intersectionPoint(plane3D aPlane, ray aRay){
    
    point3D origin(0,0,0);
    point3D pointInt(NULL,NULL,NULL);
    
    


    
    if (aRay.p1.dot(aPlane.norm)!=0){
 point3D pointInt;
        
        point3D p1(aRay.p1.x,aRay.p1.y,aRay.p1.z);
    
    
        pointInt = aRay.p0.shift(createVector(aRay.p0, p1).scalarMultiply(((aPlane.norm.dot(createVector(aRay.p0, aPlane.origin)))/(aPlane.norm.dot(createVector(aRay.p0,p1))))));
       
        return pointInt;
        
    }
    
    else{
       
    
         return pointInt;
    }
    
    
}



//In order to keep track of all intersection planes they were subdivided, rather than overwriting them
//This was done to debug, and work with the plane intersection test

bool intersectObject(object object, ray aRay){
    
    
    
    bool intersection = false;
    point3D pointOfIntersection1;
    point3D pointOfIntersection2;
    point3D pointOfIntersection3;
    point3D pointOfIntersection4;
    point3D pointOfIntersection5;
    point3D pointOfIntersection6;
    
  
   
    pointOfIntersection1 = intersectionPoint(object.box.front, aRay);
    pointOfIntersection2 = intersectionPoint(object.box.back, aRay);
    pointOfIntersection3 = intersectionPoint(object.box.bottom, aRay);
    pointOfIntersection4 = intersectionPoint(object.box.top, aRay);
    pointOfIntersection5 = intersectionPoint(object.box.right, aRay);
    pointOfIntersection6 = intersectionPoint(object.box.left, aRay);

    
    

        //checks front face against x and y bounds
    if((pointOfIntersection1.x<=object.box.right.origin.x) && (pointOfIntersection1.x>=object.box.left.origin.x) && (pointOfIntersection1.y>=object.box.bottom.origin.y) && (pointOfIntersection1.y<=object.box.top.origin.y))
    
    {
        
      
        intersection = true;
    }
    
    //checks back face

    
    

    else if((pointOfIntersection2.x<object.box.right.origin.x) && (pointOfIntersection2.x>object.box.left.origin.x) && (pointOfIntersection2.y>object.box.bottom.origin.y) && (pointOfIntersection2.y<object.box.top.origin.y))
    {
                intersection = true;
        
    }
    
    //checks bottom face
   

    else if((pointOfIntersection3.x<object.box.right.origin.x) && (pointOfIntersection3.x>object.box.left.origin.x) && (pointOfIntersection3.z<object.box.front.origin.z) && (pointOfIntersection3.z>object.box.back.origin.z))
    {
        intersection = true;
         
    }
    
   // checks top face

    
   else if((pointOfIntersection4.x<object.box.right.origin.x) && (pointOfIntersection4.x>object.box.left.origin.x) && (pointOfIntersection4.z<object.box.front.origin.z) && (pointOfIntersection4.z>object.box.back.origin.z))
    {
        intersection = true;
       
        
    }
    
    
    
    // checks right face
    
    
      else if((pointOfIntersection5.y<object.box.top.origin.y) && (pointOfIntersection5.y>object.box.bottom.origin.y) && (pointOfIntersection5.z<object.box.front.origin.z) && (pointOfIntersection5.z>object.box.back.origin.z))
    {
        intersection = true;
        
    }
    
    
    
    else if((pointOfIntersection6.y<object.box.top.origin.y) && (pointOfIntersection6.y>object.box.bottom.origin.y) && (pointOfIntersection6.z<object.box.front.origin.z) && (pointOfIntersection6.z>object.box.back.origin.z))
    {
        intersection = true;
        
    }
    

    
    else{
        intersection = false;
        
    }
  
    return intersection;

}




void mouse(int btn, int state, int x, int y){
    
        vec3D rayVec = GetOGLPos(x, y);
        point3D mouseScreen(camPos[0],camPos[1],camPos[2]);
        ray ray1(mouseScreen, rayVec);
    
    
    if(btn==GLUT_LEFT_BUTTON)
    {
       
        
        //Checks to see if there are any intersections with the resulting planes of the cars
        deselectAll();
        
        for(int i = 0;i<cars.size();i++){
            
            
            if(intersectObject(cars[i], ray1)){
                
                cars[i].selected =true;
                
                }
            
            else{
                    cars[i].selected =false;
                
           }
       }
    }
    
    
    else if(btn == GLUT_RIGHT_BUTTON && cars.size()>=1){
        
        
        
     
            
        
        

    }
    
    else if(state==!GLUT_DOWN){
        
    }
    else{
        
    }
    
}


void isSelected(){
    
    for(int i =0;i<cars.size();i++){
 
    
    
    }
}

void createCars(){
    
    object newObj(origin, 0.0, vel0, 'r',standardBox(), false);
    cars.push_back(newObj);
    cars[0] = translateFunction(cars[0], -10, 0,0);
    cars[0]=scaleFunction(cars[0],2, 2, 2);
    
    object newObj2(origin, 0.0, vel0, 'y',standardBox(), false);
    cars.push_back(newObj2);
   cars[1] = translateFunction(cars[1], 10, 0,0);
        cars[1]=scaleFunction(cars[1],2, 2, 2);
    
    object newObj3(origin, 0.0, vel0, 'b',standardBox(), true);
    cars.push_back(newObj3);
        cars[2]=scaleFunction(cars[2],2, 2, 2);
  
}


//The timerFunc is set to redisplay every 5ms
void drawEnvironment(){
    
    
    glBegin(GL_QUADS);
    
    glNormal3f(0, 1, 0); //set the normal for stage lighting
    setMaterial('G');
    glVertex3f(500, -1, -500 );
    glVertex3f(500, -1, 500 );
    glVertex3f(-500,-1, 500 );
    glVertex3f(-500, -1, -500 );
    
    setMaterial('B');
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15,-.8, -500); //8+72=80,
	glTexCoord2f(0.0, 1.0); glVertex3f(15,-.8, -500);
	glTexCoord2f(1.0, 1.0); glVertex3f(15,-.8, -485 );//-8+72
	glTexCoord2f(1.0, 0.0); glVertex3f(-15,-.8,-485);
    
	/*glTexCoord2f(0.0, 0.0); glVertex4f(1.0, -1.0, 0.0);
     glTexCoord2f(0.0, 1.0); glVertex4f(1.0, 1.0, 0.0);
     glTexCoord2f(1.0, 1.0); glVertex4f(2.41421, 1.0, -1.41421);
     glTexCoord2f(1.0, 0.0); glVertex4f(2.41421, -1.0, -1.41421);*/
	glEnd();
    
    
    
	//glBindTexture(GL_TEXTURE_2D, texture name goes here);
	glBegin(GL_QUADS);
	//glTexCoord2f(0.0, 0.0);
	glVertex3f(-15,-.9, -485); //8+72=80,
	//glTexCoord2f(0.0, 1.0);
	glVertex3f(15,-.9, -485);
	//glTexCoord2f(1.0, 1.0);
	glVertex3f(15,-.9, 500 );//-8+72
	//glTexCoord2f(1.0, 0.0);
	glVertex3f(-15,-.9 ,500);
	glEnd();
    
	glFlush();
	glDisable(GL_TEXTURE_2D);
    
    
}




void opponentCar(){
    
    glPushMatrix();
    
    glTranslatef(-6,0,30);
    glTranslatef(cars[2].location.x, cars[2].location.y, cars[2].location.z);
    makeCar('r');
    
    glPopMatrix();
}





void gameCheck(){
    //THis is the finish line
    
    if ((myCar.location.z<-482.3) ||(cars[2].location.z<(-482.3-30)) ){
        gameComplete = true;

		trafficTimer = 0;
        
        if (myCar.location.z<cars[2].location.z+30) {
            winner = true;
            
        }
        else{
            
            winner = false;
         

            
        }
        
    }
    
    
}
//For Future Use

//void collision(){
//    if((abs(myCar.location.x-cars[2].location.x)<=1 )&&(myCar.location.z-cars[2].location.z-30)<=2){
//        
//        myCar.location.x = myCar.location.x+0.05;
//    }
//    if(abs(myCar.location.x)>14.5){
//        
//        friction = .7;
//    }
//    else{
//        friction= 0;
//    }
//        
//        }

//




void trafficLogic(){
    
    
if (traffic == 0) {
    drawTrafficLight(2, blackPlastic.emission, blackPlastic.emission, blackPlastic.emission);
}
else if (traffic == 1) {
    drawTrafficLight(2, redLight_emissive, blackPlastic.emission, blackPlastic.emission);
}
else if (traffic == 2) {
    drawTrafficLight(2, redLight_emissive, orangeLight_emissive, blackPlastic.emission);
}
else {
    drawTrafficLight(2, redLight_emissive, orangeLight_emissive, greenLight_emissive);
}

}




void display(void){
    
if(carSelect){

        count=0;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);
        //---------------------------------------------
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, texName);
        glDisable(GL_TEXTURE_2D);
        //-----------------------------------------------------------

        carRotation+=0.5;

    
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);

    
        
        
        glutKeyboardFunc(keyboard);
        
        if (cars.size()==0){
            createCars();
        }
        
        for(int i = 0; i<cars.size();i++){
            glPushMatrix();
            
            glTranslatef(cars[i].location.x,cars[i].location.y , cars[i].location.z);
            glRotatef(carRotation, 0, 1, 0);
            
            makeCar(cars[i].material);
            
            if(cars[i].selected){
                glutWireCube(5);
                
            }
            else{
                
                
            }
            glPopMatrix();
            
        }
        
        
        //Used to switch the current targets shape a switch command was used to be able to easily change
        //Shapes
		glPushMatrix();

		setMaterial('w');
        drawBitmapText("Select your car and hit enter ",-4,10,0);
      drawBitmapText("Use UP, DOWN and Side Arrows to inspect your ride ",-10,3,0);
    
		glPopMatrix();

        glutSwapBuffers();
        glutTimerFunc(5,timer,0);

    }
    
    
    
    
    
    else if(gameComplete){


		//resets the traffic light back to original state


		trafficCounter=0;
		traffic = 0;
        carSelect = false;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);
        //---------------------------------------------
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, texName);
        glDisable(GL_TEXTURE_2D);
        //-----------------------------------------------------------
        
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
        
        
        
        
        glutKeyboardFunc(keyboard);
        
        
        glPushMatrix();
        
        setMaterial('w');
        
        if (winner) {
                 drawBitmapText("YOU HAVE WON!!! ",-4,10,0);
        }
        else
            {
                drawBitmapText("NICE TRY, BUT NO CIGAR ",-4,10,0);
            }
        
       
        glPopMatrix();
        count++;
        if (count>600) {
            
            gameComplete = false;
            carSelect = true;
        }
        
        glutSwapBuffers();
        glutTimerFunc(5,timer,0);
        
        
    }
    
    
    
    
    
    else{
        
        trafficCounter++;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(135/255., 206/255., 250/255., 1);
	//---------------------------------------------
	
	
        
        


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cars[1].location.x, cars[1].location.y+10, cars[1].location.z+30,cars[1].location.x, cars[1].location.y, cars[1].location.z, 0,1,0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightfv(GL_LIGHT1,GL_POSITION,light_pos1);
        glLightfv(GL_LIGHT2,GL_POSITION,light_pos2);
        
        
      //  collision(); //currently not in use

    glutKeyboardFunc(keyboard);
   
    for(int i = 0; i<cars.size();i++){

        
       //Used to switch the current targets shape a switch command was used to be able to easily change
    //Shapes
        

        setMaterial('w');
        glLoadIdentity();
     
        glTranslatef(0.0f, 0.0f, -cRadius);
        glRotatef(xrot,1.0,0.0,0.0);
        glColor3f(1.0f, 0.0f, 0.0f);
        
        for (int i =0; i<cars.size(); i++) {
            if (cars[i].selected) {
                
                myCar = cars[i];
                myCar.location.z = 30;
                myCar.location.x = 6;
                yrot = 0;
                xrot = 370;
                cars[i].selected = false;
                cars[2].location.z = 30;
                cars[2].location.x = -6;
            }
            
            
        }
       makeCar(myCar.material);
        
        
        glRotatef(yrot,0.0,1.0,0.0);
        glTranslated(-myCar.location.x,0.0f,-myCar.location.z);
        glRotatef(cars[1].direction, 0, 1, 0);
        glMatrixMode(GL_MODELVIEW);
        moveCar();
        opponentCar();
        
        drawEnvironment();
    
        
        glPushMatrix();
		glTranslatef(0.0, 5.0, -2.5);
        trafficLogic();
        
        glPopMatrix();
    }
    
        gameCheck();
        
        
        
        
        
        if (trafficCounter< 100) {
            traffic = 0;
        }
        else if (100 <= trafficCounter && trafficCounter< 200) {
            traffic = 1;
        }
        else if (200 <= trafficCounter && trafficCounter < 300) {
            traffic = 2;
        }
        else {
            traffic = 3;
        }
        
       
    //The timerFunc is set to redisplay every 5ms
    

	glutSwapBuffers();
    glutTimerFunc(5,timer,0);

    }
    
    

}





int main(int argc, char** argv)
{
    
    glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
    
   
    
    
	glutCreateWindow("Hot Ridez");	//creates the window
    glutMouseFunc(mouse);
    glutSpecialFunc(special);

 
    init();

    
    
    glutDisplayFunc(display);	//registers "display" as the display callback function
    
    glutIdleFunc (display);
    glutReshapeFunc (reshape);
    
    glutPassiveMotionFunc(mouseMovement);
	
	

    glutIdleFunc(display);
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BACK);
    
   
  

    notesToTheTA();
    
	glutMainLoop();				//starts the event loop
    
    
	return(0);					//return may not be necessary on all compilers
}