// ========================================================================
// COSC 363  Computer Graphics
// Ray Tracing of Assignment 2
// Name : Liguo Jiao
// Student ID: 91734390
// ========================================================================

#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include "Plane.h"
#include <vector>
#include "Vector.h"
#include "Object.h"
#include <GL/glut.h>
#include "loadTGA.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//------------//
// Components //
//------------//
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Color.h"
#include "Tetrahedron.h"

using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int PPU = 30;     //Total 600x600 pixels
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
bool RaygetInside = false;
vector<Object*> sceneObjects;
Vector light = Vector(-10.0, 15, -5.0);
Vector light2 = Vector(16.0, 16, -50.0);
Color backgroundCol = Color::GRAY;

struct PointBundle   
{
	Vector point;
	int index;
	float dist;
};
/*
* This function compares the given ray with all objects in the scene
* and computes the closest point  of intersection.
*/
PointBundle closestPt(Vector pos, Vector dir)
{
    Vector  point(0, 0, 0);
	float min = 10000.0;

	PointBundle out = {point, -1, 0.0};

    for(int i = 0;  i < sceneObjects.size();  i++)
	{
        float t = sceneObjects[i]->intersect(pos, dir);
		if(t > 0)        //Intersects the object
		{
			point = pos + dir*t;
			if(t < min)
			{
				out.point = point;
				out.index = i;
				out.dist = t;
				min = t;
			}
		}
	}return out;
}
/*
* Computes the colour value obtained by tracing a ray.
* If reflections and refractions are to be included, then secondary rays will 
* have to be traced from the point, by converting this method to a recursive
* procedure.
*/
Color trace(Vector pos, Vector dir, int step)
{
    PointBundle q = closestPt(pos, dir);
    Color colorSum;
    if(q.index == -1) return backgroundCol;        //no intersection
    Vector n = sceneObjects[q.index]->normal(q.point);
    Vector l = light - q.point;
    Vector l2 = light2 - q.point;
    l.normalise(); l2.normalise(); //Normalise the two lights
    float lDotn = l.dot(n);
    float l2Dotn = l2.dot(n);
    float non_color = 1.0;
    float Object = 1.01;
    Color col = sceneObjects[q.index]->getColor(); //Object's colour    
//-----------------------Texture----------------------------------------
    if((q.index == 5)||(q.index == 9)){
        if ((int(q.point.x) - int(q.point.z)) % 2 == 1) col = Color::BLACK;
        else col = Color::WHITE;
    }// Give the ground and top (Black & White) texture
    if(q.index == 6){
        if (((-int(q.point.x) - int(q.point.y)) % 2 == 1) or ((int(q.point.x) + int(q.point.y)) % 2 == 1)){
            col = Color::BLACK;
        }
        else col = Color::WHITE;
    }// Give the back ground a (Black & White) texture
    if((q.index == 8)||(q.index == 7)){
        if ((int(q.point.y) - int(q.point.z)) % 2 == 1) col = Color::RED;
        else col = Color::WHITE;
    }// Give the Left side wall and Right side wall a Red & White texture
    if(q.index == 1){
        if ((int(q.point.x) - int(q.point.y)) % 2 == 1) col = Color::GREEN;
        else col = Color::BLACK;
    }// Texture Sphere
//-----------------------Refraction-------------------------------------        
    if (q.index == 20 && step < MAX_STEPS) {
        if (!RaygetInside) {
            // Ray entering the Object
            float ratio = non_color/Object;
            float cos = sqrt(1-pow((ratio),2)*(1-pow(dir.dot(n),2)));
            Vector refractionVector = dir*ratio - n*( (ratio)*(dir.dot(n))+cos);
            refractionVector.normalise();
            RaygetInside = !RaygetInside;
            return trace(q.point, refractionVector, step+1);
        }
        else {
            // Ray exiting the Object
            float ratio = Object/non_color;
            float cos = sqrt(1-pow((ratio),2)*(1-pow(dir.dot(n),2)));
            n = n*(-1);
            Vector refractionVector = dir*ratio - n*( (ratio)*(dir.dot(n))+cos);
            refractionVector.normalise();
            RaygetInside = !RaygetInside;
            return trace(q.point, refractionVector, step+1);
        }
    }    
    if ((l.dot(n) <= 0) and (l2.dot(n) <= 0)){
        colorSum = col.phongLight(backgroundCol,0.0,0.0);
    }
    else{
        Vector r = ((n * 2) * lDotn) - l; // r = 2(L.n)n – L. ‘l’ = el
        Vector r2 = ((n * 2) * l2Dotn) - l2; // r = 2(L.n)n – L. ‘l’ = el 
        r.normalise(); 
        r2.normalise();
        Vector v(-dir.x, -dir.y, -dir.z); //View vector; 
        float rDotv = r.dot(v);
        float r2Dotv = r2.dot(v);
        float spec;
        if(rDotv < 0) spec = 0.0; 
        else spec = pow(rDotv, 10); //Phong exponent = 10

        if(r2Dotv < 0) spec = 0.0; 
        else spec = pow(r2Dotv, 10); //Phong exponent = 10
                
        PointBundle shadow = closestPt(q.point, l);
        PointBundle shadow2 = closestPt(q.point, l2);
        Vector reflectionVector = ((n*2)*(n.dot(v)))-v;
//-----------------------Shadow of Light 1------------------------------        
        if (shadow.index == -1){
            colorSum = col.phongLight(backgroundCol, lDotn, spec);
        }
        if (shadow.index != -1)
        {
            if (q.point.dist(shadow.point) < q.point.dist(light)){
                colorSum = col.phongLight(backgroundCol, lDotn/6, 0);
            }
        }
//-----------------------Shadow of Light 2------------------------------
        if (shadow2.index == -1){
            colorSum = col.phongLight(backgroundCol, l2Dotn, spec);
        }
        if (shadow2.index != -1)
        {
            if (q.point.dist(shadow2.point) < q.point.dist(light2)){
                colorSum = col.phongLight(backgroundCol, l2Dotn/6, 0);
            }
        }
//-----------------------Reflection of each component-------------------
        if (q.index == 0 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,1);
        }//Sphere
        if (q.index == 2 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.8);
        }//Sphere
        if (q.index == 3 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.1);
        }//Cylinder
        if (q.index == 4 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.5);
        }//Cone
        if ((q.index == 7 || q.index == 8)&& step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.1);
        }//Left side wall & Right side wall
        if ((q.index == 10 || q.index == 11 || q.index == 12 || q.index == 13 || q.index == 14 || q.index == 15) && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.5);
        }//Box
        if ((q.index == 16 || q.index == 17  || q.index == 18 || q.index == 19) && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.5);
        }//Tetrahedron
    }
    return colorSum;
}
//-----------------------Anti-Aliasing (Super Sampling)-----------------
Color Average_Pixel(Vector eye, Vector dir, float pixel, float x1, float y1)
{
    int size = 4; // Sampling size
    float R = 0, G = 0, B = 0;
    float half_pixelsize = pixel/2.0;
    float quarter_pixelsize = pixel/4.0;
    float xc = x1 + quarter_pixelsize;
    float yc = y1 + quarter_pixelsize;
	Color SetOfPixel[size];
    Vector current_dir;

    current_dir = Vector(xc, yc, -EDIST);
    current_dir.normalise();
    SetOfPixel[0] = trace(eye, current_dir, 1);
    current_dir = Vector(xc + half_pixelsize, yc, -EDIST);
    current_dir.normalise();
    SetOfPixel[1] = trace(eye, current_dir, 1);
    current_dir = Vector(xc, yc + half_pixelsize, -EDIST);
    current_dir.normalise();
    SetOfPixel[2] = trace(eye, current_dir, 1);
    current_dir = Vector(xc + half_pixelsize, yc + half_pixelsize, -EDIST);
    current_dir.normalise();
    SetOfPixel[3] = trace(eye, current_dir, 1);
 
    for (int i=0; i<size; i++){
        R += SetOfPixel[i].r;
        G += SetOfPixel[i].g;
        B += SetOfPixel[i].b;
    }
    //Split it into x4 and found the average color, then return the average.
    Color col(R/size,G/size,B/size);
    return col;
}
//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each pixel as quads.
//---------------------------------------------------------------------------------------
void display()
{
	int widthInPixels = (int)(WIDTH * PPU);
	int heightInPixels = (int)(HEIGHT * PPU);
	float pixelSize = 1.0/PPU;
	float halfPixelSize = pixelSize/2.0;
	float x1, y1, xc, yc;
    
	Vector eye(0., 0., 0.);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);  //Each pixel is a quad.

	for(int i = 0; i < widthInPixels; i++)	//Scan every "pixel"
	{
		x1 = XMIN + i*pixelSize;
		xc = x1 + halfPixelSize;
		for(int j = 0; j < heightInPixels; j++)
		{
			y1 = YMIN + j*pixelSize;
			yc = y1 + halfPixelSize;
		    Vector dir(xc, yc, -EDIST);	//direction of the primary ray
			Color col_ss = Average_Pixel(eye,dir,pixelSize,x1,y1); //Calling Super Sampling function
            dir.normalise();			//Normalise this direction
		    Color col = col_ss;//trace (eye, dir, 1);  //For the Super Sampling. If want turn off anti-aliasing
								// replace it to "trace (eye, dir, 1);" 
			glColor3f(col.r, col.g, col.b);
			glVertex2f(x1, y1);				//Draw each pixel with its color value
			glVertex2f(x1 + pixelSize, y1);
			glVertex2f(x1 + pixelSize, y1 + pixelSize);
			glVertex2f(x1, y1 + pixelSize);
        }
    }
    glEnd();
    glFlush();
}

void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0, 0, 0, 1);
//-----------------------Sphere, Cylinder & Cone------------------------
    Sphere *sphere0 = new Sphere(Vector(0, -10, -90), 2.0, Color::BLACK);
    sceneObjects.push_back(sphere0);
    Sphere *sphere1 = new Sphere(Vector(15, -15, -130), 5.0, Color::RED);
    sceneObjects.push_back(sphere1);
    Sphere *sphere2 = new Sphere(Vector(-15, -15, -90), 5.0, Color::GRAY);
    sceneObjects.push_back(sphere2);
    Cylinder *cylinder3 = new Cylinder(Vector(-10, 0, -120), 3, 10.0, Color(.205,.181,.205));
    sceneObjects.push_back(cylinder3);    
    Cone *cone4 = new Cone(Vector(3.5, -18, -80), 2.0, 6.0, Color(.100,.149,.237));
    sceneObjects.push_back(cone4);
//-----------------------Wall & Floor---------------------------------
    Plane *plane5 = new Plane(Vector(-20,-20,-30),Vector(20,-20,-30),Vector(20,-20,-150),Vector(-20,-20,-150), Color(1,1,1));
    sceneObjects.push_back(plane5);    
    Plane *plane_B_6 = new Plane(Vector(-20,-20,-150),Vector(20,-20,-150),Vector(20,20,-150),Vector(-20,20,-150), Color::GRAY);
    sceneObjects.push_back(plane_B_6);//background
    Plane *plane_L_7 = new Plane(Vector(-20,-20,-30),Vector(-20,-20,-150), Vector(-20, 20,-150),Vector(-20, 20,-30), Color(1,1,1));
    sceneObjects.push_back(plane_L_7);
    Plane *plane_R_8 = new Plane(Vector(20,20,-30),Vector(20,20,-150), Vector(20, -20,-150),Vector(20, -20,-30), Color(1,1,1));
    sceneObjects.push_back(plane_R_8);
    Plane *plane9 = new Plane(Vector(-20,20,-30),Vector(20,20,-30),Vector(20,20,-150),Vector(-20,20,-150), Color(1,1,1));
    sceneObjects.push_back(plane9);
//-----------------------BOX------------------------------------------
    Vector a = Vector(9,-14,-80);Vector b = Vector(15,-14,-80); Vector c = Vector(15,-14,-90); Vector d = Vector(9,-14,-90); 
    Vector e = Vector(9,-19,-80); Vector f = Vector(15,-19,-80); Vector g = Vector(15,-19,-90); Vector h = Vector(9,-19,-90);
    Plane *front_10 = new Plane(a,e,f,b,Color::BLUE);
    sceneObjects.push_back(front_10);
    Plane *back_11 = new Plane(d,c,g,h,Color::BLUE);
    sceneObjects.push_back(back_11);
    Plane *top_12 = new Plane(a,b,c,d,Color::RED);
    sceneObjects.push_back(top_12);
    Plane *bottom_13 = new Plane(e,f,g,h,Color::RED); 
    sceneObjects.push_back(bottom_13);
    Plane *left_14 = new Plane(h,e,a,d,Color::GREEN);
    sceneObjects.push_back(left_14);
    Plane *right_15 = new Plane(f,g,c,b,Color::GREEN);
    sceneObjects.push_back(right_15);
//-----------------------Tetrahedron------------------------------------
    Vector ta = Vector(-6,-14,-100); Vector tb = Vector(-3,-14,-110); Vector tc = Vector(-9,-14,-110); Vector top = Vector(-5.5,0,-112.5);  
    Tetrahedron *left_16 = new Tetrahedron(ta,top,tc,Color::BLUE);
    sceneObjects.push_back(left_16);
    Tetrahedron *right_17 = new Tetrahedron(ta,tb,top,Color::GREEN);
    sceneObjects.push_back(right_17);
    Tetrahedron *back_18 = new Tetrahedron(tc,tb,top,Color::BLACK);
    sceneObjects.push_back(back_18);
    Tetrahedron *bottom_19 = new Tetrahedron(ta,tb,tc,Color::BLACK);
    sceneObjects.push_back(bottom_19);
//-----------------------Air Bubble-------------------------------------
    Sphere *sphere20 = new Sphere(Vector(-5, -15, -80), 3.6, Color(.205,.181,.205));
    sceneObjects.push_back(sphere20);
}

int main(int argc, char *argv[]) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing By Liguo Jiao");
    glutInitContextVersion (4, 2);
    glutInitContextProfile (GLUT_CORE_PROFILE);
    
    glutDisplayFunc(display);
    initialize();
    glutMainLoop();
    return 0;
}
