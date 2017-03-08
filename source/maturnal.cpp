/*
 *  interface.cpp
 *
 *
 *  Created by Pat Schloss on 8/14/08.
 *  Copyright 2008 Patrick D. Schloss. All rights reserved.
 *
 */

#include "maturnal.h"
#include "engine.hpp"
#include "mothurout.h"

#include "classifyseqscommand.h"

/**************************************************************************************************/

CommandFactory* CommandFactory::_uniqueInstance = 0;
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
    string templateFileName = argv[2];
    string search = "kmer";
    int kmerSize = stoi(argv[3]);
    int cutoff = stoi(argv[4]);
    int iters = stoi(argv[5]);
    bool flip = 0;
    bool writeShortcuts = 1;
		string inputFile = argv[6];

    cout << taxonomyFileName << '\t' <<
				    templateFileName << '\t' <<
				    search << '\t' <<
				    kmerSize << '\t' <<
				    cutoff << '\t' <<
				    iters << '\t' <<
				    inputFile << '\t' <<
				    endl;

		std::stringstream options;
		options << "method=wang, taxonomy=" << taxonomyFileName <<
			", template=" << templateFileName <<
			", ksize=" << kmerSize <<
			", cutoff=" << cutoff <<
			", iters=" << iters <<
			", fasta=" << inputFile;
		ClassifySeqsCommand* command = new ClassifySeqsCommand(options.str());
		command->execute();
  }

	return 0;
}

/**************************************************************************************************/
