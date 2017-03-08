/*
 *  interface.cpp
 *
 *
 *  Created by Pat Schloss on 8/14/08.
 *  Copyright 2008 Patrick D. Schloss. All rights reserved.
 *
 */

#include "maturnal.h"
#include "mothurout.h"

#include "classifyseqscommand.h"

/**************************************************************************************************/

MothurOut* MothurOut::_uniqueInstance = 0;

/***********************************************************************/
int main(int argc, char *argv[]){

	string releaseDate = RELEASE_DATE;
	string mothurVersion = VERSION;
	MothurOut* m = MothurOut::getInstance();
	m->setReleaseDate(releaseDate);
	m->setVersion(mothurVersion);

  if (argc == 7) {
    string taxonomyFileName = argv[1];
    string referenceFileName = argv[2];
    string search = "kmer";
    string kmerSize = argv[3];
    string cutoff = argv[4];
    string iters = argv[5];
    bool flip = 0;
    bool writeShortcuts = 1;
		string inputFile = argv[6];

    cout << taxonomyFileName << '\t' <<
				    referenceFileName << '\t' <<
				    search << '\t' <<
				    kmerSize << '\t' <<
				    cutoff << '\t' <<
				    iters << '\t' <<
				    inputFile << '\t' <<
				    endl;
		map<string, string> parameters = map<std::string, string>();
		parameters["method"] = "wang";
		parameters["taxonomy"] = taxonomyFileName;
		parameters["reference"] = referenceFileName;
		parameters["ksize"] = kmerSize;
		parameters["cutoff"] = cutoff;
		parameters["iters"] = iters;
		parameters["fasta"] = inputFile;
		ClassifySeqsCommand* command = new ClassifySeqsCommand(parameters);
		command->execute();
		delete command;
  }

	return 0;
}

/**************************************************************************************************/
