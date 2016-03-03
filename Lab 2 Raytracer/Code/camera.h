// Camera object for the scenes

#ifndef CAMERA_H
#define CAMERA_H

#include "triple.h"

class Camera {
	private:
		Triple eye;
		Triple center;
		Triple upVector;
		int height;
		int width;
		
	public:
		Triple getEye();
		Triple getCenter();
		Triple getUpVector();
		int getHeight();
		int getWidth();
		
		void setEye(Triple t);
		void setCenter(Triple c);
		void setUpVector(Triple u);
		void setHeight(int y);
		void setWidth(int x);
};

#endif
