#ifndef PROGRESS_H
#define PROGRESS_H

#include "maturnal.h"
#include "mothurout.h"

class Progress {
	
public:
	Progress();
	Progress(string, int);
	void update(int);
	void newLine(string, int);
	void finish();
	
private:
	int nTicks;
	int finalPos;
	MothurOut* m;	
};

#endif
