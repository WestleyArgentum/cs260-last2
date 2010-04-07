#pragma once



const float PI = 3.14159f;
const int turnstepcount = 10;
const float PhysicsStep = 16.6f;

//lookup tables for values of sin and cos based on the number of allowed steps
//that can exist in a turn
//float COS[turnstepcount];
//float SIN[turnstepcount];

// useful formulats for physics that I forget:
// velocity = v0 + at;
// x = x0 + v0 = .5at^2

//= 2 * PI * turnstep (i.e. .1, .2, etc) = theta.
//sin theta = y'
//cos theta = x'

enum MovementCommand
{
	Forward,
	Reverse,
	Left,
	Right,
	Stop
};



class PhysicsCalculator{

public:
	static float CalcNewXValue(float theta, float curX, float velocity);
	static float CalcNewYValue(float theta, float curY, float velocity);

private:
	static float lastTimeStep;
	static float currentTimeStep;

};
