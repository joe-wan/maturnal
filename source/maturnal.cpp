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

#include "jwutil.hpp"
#include "classifyseqscommand.h"

/**************************************************************************************************/

MothurOut* MothurOut::_uniqueInstance = 0;

/***********************************************************************/
int main(int argc, char *argv[]){
	// Tell the program which version/release it is
	string releaseDate = RELEASE_DATE;
	string mothurVersion = VERSION;
	MothurOut* m = MothurOut::getInstance();
	m->setReleaseDate(releaseDate);
	m->setVersion(mothurVersion);

	// Parse arguments (expect '--') into a map<string, string>
	try {
		map<string, string> parameters = jwutil::argvToMap(argc, argv);
		ClassifySeqsCommand command = ClassifySeqsCommand(parameters);
		command.execute();
	} catch (string message) {
		m->mothurOut(message);
		m->mothurOutEndLine();
		return 1;
	}

	return 0;
}

/**************************************************************************************************/
