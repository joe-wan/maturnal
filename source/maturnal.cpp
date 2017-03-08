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
	map<string, string> parameters;
	try {
		parameters = jwutil::argvToMap(argc, argv);
	} catch (string message) {
		m->mothurOut(message);
		m->mothurOutEndLine();
		m->mothurOut("Use \"maturnal --help\" to see a list of options.");
		m->mothurOutEndLine();
		return 1;
	}

	// See if user has requested help, citation, or execution
	if (parameters.count("help") > 0) {
		// Print help
		m->mothurOut(ClassifySeqsCommand().getHelpString());
		m->mothurOutEndLine();
	} else if (parameters.count("citation") > 0) {
		// Print citation
		m->mothurOut(ClassifySeqsCommand().getCitation());
		m->mothurOutEndLine();
	} else {
		// Classify the sequences
		ClassifySeqsCommand command = ClassifySeqsCommand(parameters);
		command.execute();
	}

	return 0;
}

/**************************************************************************************************/
