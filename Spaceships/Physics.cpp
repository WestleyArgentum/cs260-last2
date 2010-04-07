#include "Physics.h"

float PhysicsCalculator::CalcNewYValue(float theta, float curY, float velocity){
	float temp = 0;
	temp = 2*PI*theta;
	temp = cos(temp);
	temp = temp * velocity;
	return curY - temp;
}

float PhysicsCalculator::CalcNewXValue(float theta, float curX, float velocity){
	float temp = 0;
	temp = 2*PI*theta;
	temp = sin(temp);
	temp = temp * velocity;
	return temp + curX;
}