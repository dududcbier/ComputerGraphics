#include "camera.h" 
		
Triple Camera::getEye() {
	return eye;
}

Triple Camera::getCenter() {
	return center;
}

Triple Camera::getUpVector() {
	return upVector;
}

int Camera::getHeight() {
	return height;
}

int Camera::getWidth() {
	return width;
}
		
void Camera::setEye(Triple t) {
	eye = t;
}

void Camera::setCenter(Triple c) {
	center = c;
}

void Camera::setUpVector(Triple u) {
	upVector = u;
}

void Camera::setHeight(int y) {
	height = y;
}
	
void Camera::setWidth(int x) {
	width = x;
}
