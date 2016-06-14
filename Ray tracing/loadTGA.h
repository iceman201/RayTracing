//=====================================================================
// LoadTGA.h
// Image loader for files in TGA format.
// Assumption:  Uncompressed data.
//
// Author:
// R. Mukundan, Department of Computer Science and Software Engineering
// University of Canterbury, Christchurch, New Zealand.
//=====================================================================
#if !defined(H_TGA)
#define H_TGA

#include <iostream>
#include <fstream>
using namespace std;

void loadTGA(string filename)
{
    char id, cmap, imgtype, bpp, c_garb;
    char* imageData, temp;
    short int s_garb, wid, hgt;
    int nbytes, size, indx;
    ifstream file( filename.c_str(), ios::in | ios::binary);
	if(!file)
	{
		cout << "*** Error opening image file: " << filename.c_str() << endl;
		exit(1);
	}
	file.read (&id, 1);
	file.read (&cmap, 1);
	file.read (&imgtype, 1);	
	if(imgtype != 2 && imgtype != 3 )   //2= colour (uncompressed),  3 = greyscale (uncompressed)
	{
		cout << "*** Incompatible image type: " << (int)imgtype << endl;
		exit(1);
	}
	//Color map specification
	file.read ((char*)&s_garb, 2);
	file.read ((char*)&s_garb, 2);
	file.read (&c_garb, 1);
	//Image specification
	file.read ((char*)&s_garb, 2);  //x origin
	file.read ((char*)&s_garb, 2);  //y origin
	file.read ((char*)&wid, 2);     //image width								    
	file.read ((char*)&hgt, 2);     //image height
	file.read (&bpp, 1);     //bits per pixel
	file.read (&c_garb, 1);  //img descriptor
	nbytes = bpp / 8;           //No. of bytes per pixels
	size = wid * hgt * nbytes;  //Total number of bytes to be read
	imageData = new char[size];
	file.read(imageData, size);
	//cout << ">>>" << nbytes << " " << wid << " " << hgt << endl;
	if(nbytes > 2)   //swap R and B
	{
	    for(int i = 0; i < wid*hgt;  i++)
	    {
	        indx = i*nbytes;
	        temp = imageData[indx];
	        imageData[indx] = imageData[indx+2];
	        imageData[indx+2] = temp;
        }
    }

	switch (nbytes)
	{
	     case 1:
	         glTexImage2D(GL_TEXTURE_2D, 0, 1, wid, hgt, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
	         break;
	     case 3:
	         glTexImage2D(GL_TEXTURE_2D, 0, 3, wid, hgt, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	         break;
	     case 4:
	         glTexImage2D(GL_TEXTURE_2D, 0, 4, wid, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	         break;
     }
     delete imageData;	         	         
}

#endif

