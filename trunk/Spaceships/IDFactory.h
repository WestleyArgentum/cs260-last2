#pragma once

class IDFactory
{
public:
	IDFactory(void);
	~IDFactory(void);
	// get an ID
	static int GetNextID();
	// resets count to 0
	static void ResetID();
	// the next ID returned will be the value of seed
	static void ResetID(int seed);

	

private:
	static int currentID;
};

