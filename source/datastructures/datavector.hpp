#ifndef datavector_h
#define datavector_h


#include "maturnal.h"
#include "mothurout.h"
#include "groupmap.h"

/* This class is parent to listvector, ordervector, rabundvector, sabundvector, sharedordervector, sharedrabundvector, sharedsabundvector.
	The child classes all contain OTU information in different forms. */



class DataVector {

public:
	DataVector(){ m = MothurOut::getInstance(); }// : maxRank(0), numBins(0), numSeqs(0){};
	DataVector(string l) : label(l) { m = MothurOut::getInstance();};
	DataVector(const DataVector& dv) : label(dv.label){ m = MothurOut::getInstance();};//, maxRank(dv.maxRank), numBins(dv.numBins), numSeqs(dv.numSeqs) {};
	DataVector(ifstream&) {m = MothurOut::getInstance();}
	DataVector(ifstream&, GroupMap*){m = MothurOut::getInstance();}
	virtual ~DataVector(){};

//	virtual int getNumBins()	{	return numBins;		}
//	virtual int getNumSeqs()	{	return numSeqs;		}
//	virtual int getMaxRank()	{	return maxRank;		}

	virtual void resize(int) = 0;
	virtual int size()	= 0;
	virtual void print(ostream&) = 0;
    virtual void print(ostream&, map<string, int>&) {}
    virtual void print(ostream&, bool) { m->mothurOut("[ERROR]: no print function\n"); }
	virtual void printHeaders(ostream&) {};
	virtual void clear() = 0;

	void setLabel(string l)		{	label = l;			}
	string getLabel()			{	return label;		}


protected:
	string label;
	MothurOut* m;
//	int maxRank;
//	int numBins;
//	int numSeqs;
};

/***********************************************************************/

#endif
