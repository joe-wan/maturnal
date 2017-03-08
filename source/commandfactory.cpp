/*
 *  commandfactory.cpp
 *
 *
 *  Created by Pat Schloss on 10/25/08.
 *  Copyright 2008 Patrick D. Schloss. All rights reserved.
 *
 */

#include "command.hpp"
#include "quitcommand.h"
#include "commandfactory.hpp"
#include "nocommands.h"
#include "classifyseqscommand.h"

//needed for testing project
//CommandFactory* CommandFactory::_uniqueInstance;

/*******************************************************/

/******************************************************/
CommandFactory* CommandFactory::getInstance() {
	if( _uniqueInstance == 0) {
		_uniqueInstance = new CommandFactory();
	}
	return _uniqueInstance;
}
/***********************************************************/

/***********************************************************/
CommandFactory::CommandFactory(){
	string s = "";
	m = MothurOut::getInstance();

	command = new NoCommand(s);
	shellcommand = new NoCommand(s);
	pipecommand = new NoCommand(s);

	outputDir = ""; inputDir = "";
	logFileName = "";
	append = false;

	//initialize list of valid commands
	commands["classify.seqs"]		= "classify.seqs";
	commands["quit"]				= "quit";

}

/***********************************************************/
CommandFactory::~CommandFactory(){
	_uniqueInstance = 0;
	delete command;
	delete shellcommand;
	delete pipecommand;
}
/***********************************************************/

/***********************************************************/
int CommandFactory::checkForRedirects(string optionString) {
    try {

        int pos = optionString.find("outputdir");
        if (pos != string::npos) { //user has set outputdir in command option string
            string outputOption = "";
            bool foundEquals = false;
            for(int i=pos;i<optionString.length();i++){
                if(optionString[i] == ',')       { break;               }
                else if(optionString[i] == '=')  { foundEquals = true;	}
                if (foundEquals)       {   outputOption += optionString[i]; }
            }
            if (outputOption[0] == '=') { outputOption = outputOption.substr(1); }
            if(m->mkDir(outputOption)){
                setOutputDirectory(outputOption);
                m->mothurOut("Setting output directory to: " + outputOption); m->mothurOutEndLine();
            }
        }

        pos = optionString.find("inputdir");
        if (pos != string::npos) { //user has set inputdir in command option string
            string intputOption = "";
            bool foundEquals = false;
            for(int i=pos;i<optionString.length();i++){
                if(optionString[i] == ',')       { break;               }
                else if(optionString[i] == '=')  { foundEquals = true;	}
                if (foundEquals)       {   intputOption += optionString[i]; }
            }
            if (intputOption[0] == '=') { intputOption = intputOption.substr(1); }
            if(m->dirCheck(intputOption)){
                setInputDirectory(intputOption);
                m->mothurOut("Setting input directory to: " + intputOption); m->mothurOutEndLine();
            }
        }

        pos = optionString.find("seed=");
        if (pos != string::npos) { //user has set seed in command option string
            string intputOption = "";
            bool foundEquals = false;
            for(int i=pos;i<optionString.length();i++){
                if(optionString[i] == ',')       { break;               }
                else if(optionString[i] == '=')  { foundEquals = true;	}
                if (foundEquals)       {   intputOption += optionString[i]; }
            }
            if (intputOption[0] == '=') { intputOption = intputOption.substr(1); }
            bool seed = false; int random;
            if (intputOption == "clear") {
                random = time(NULL);
                seed = true;
            }else {
                if (m->isNumeric1(intputOption)) { m->mothurConvert(intputOption, random); seed=true; }
                else { m->mothurOut("[ERROR]: Seed must be an integer."); m->mothurOutEndLine(); seed = false;}
            }

            if (seed)  {
                srand(random);
                m->mothurOut("Setting random seed to " + toString(random) + ".\n\n");
            }
        }


        return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "getCommand");
		exit(1);
	}
}
/***********************************************************/

/***********************************************************/
//This function calls the appropriate command fucntions based on user input.
Command* CommandFactory::getCommand(string commandName, string optionString){
	try {

		delete command;   //delete the old command

        checkForRedirects(optionString);

		//user has opted to redirect output from dir where input files are located to some other place
		if (outputDir != "") {
			if (optionString != "") { optionString += ", outputdir=" + outputDir; }
			else { optionString += "outputdir=" + outputDir; }
		}

		//user has opted to redirect input from dir where mothur.exe is located to some other place
		if (inputDir != "") {
			if (optionString != "") { optionString += ", inputdir=" + inputDir; }
			else { optionString += "inputdir=" + inputDir; }
		}

		if(commandName == "classify.seqs")			{	command = new ClassifySeqsCommand(optionString);			}
		else											{	command = new NoCommand(optionString);						}

		return command;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "getCommand");
		exit(1);
	}
}
/***********************************************************/

/***********************************************************/
//This function calls the appropriate command fucntions based on user input.
Command* CommandFactory::getCommand(string commandName, string optionString, string mode){
	try {
		delete pipecommand;   //delete the old command

        checkForRedirects(optionString);

		//user has opted to redirect output from dir where input files are located to some other place
		if (outputDir != "") {
			if (optionString != "") { optionString += ", outputdir=" + outputDir; }
			else { optionString += "outputdir=" + outputDir; }
		}

		//user has opted to redirect input from dir where mothur.exe is located to some other place
		if (inputDir != "") {
			if (optionString != "") { optionString += ", inputdir=" + inputDir; }
			else { optionString += "inputdir=" + inputDir; }
		}

		if(commandName == "classify.seqs")			{	pipecommand = new ClassifySeqsCommand(optionString);			}
		else											{	pipecommand = new NoCommand(optionString);						}

		return pipecommand;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "getCommand");
		exit(1);
	}
}
/***********************************************************/

/***********************************************************/
//This function calls the appropriate command fucntions based on user input, this is used by the pipeline command to check a users piepline for errors before running
Command* CommandFactory::getCommand(string commandName){
	try {
		delete shellcommand;   //delete the old command

		if (commandName == "classify.seqs")			{	shellcommand = new ClassifySeqsCommand();			}
		else											{	shellcommand = new NoCommand();						}

		return shellcommand;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "getCommand");
		exit(1);
	}
}
/***********************************************************
//This function is used to interrupt a command
Command* CommandFactory::getCommand(){
	try {
		delete command;   //delete the old command

		string s = "";
	    command = new NoCommand(s);

		return command;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "getCommand");
		exit(1);
	}
}
***********************************************************************/
bool CommandFactory::isValidCommand(string command) {
	try {

		//is the command in the map
		if ((commands.find(command)) != (commands.end())) {
			return true;
		}else{
			m->mothurOut(command + " is not a valid command in Mothur.  Valid commands are ");
			for (it = commands.begin(); it != commands.end(); it++) {
				m->mothurOut(it->first + ", ");
			}
			m->mothurOutEndLine();
			return false;
		}

	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "isValidCommand");
		exit(1);
	}
}
/***********************************************************************/
bool CommandFactory::isValidCommand(string command, string noError) {
	try {

		//is the command in the map
		if ((commands.find(command)) != (commands.end())) {
			return true;
		}else{
			return false;
		}

	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "isValidCommand");
		exit(1);
	}
}
/***********************************************************************/
void CommandFactory::printCommands(ostream& out) {
	try {
        it = commands.begin();
		out << "Valid commands are: " << it->first << ", ";
        it++;
        out << it->first;
        it++;
		for (; it != commands.end(); it++) {
			out << ", " << it->first;
		}
		out <<  "." << endl;
	}
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "printCommands");
		exit(1);
	}
}
/***********************************************************************/
void CommandFactory::printCommandsCategories(ostream& out) {
    try {
        map<string, string> commands = getListCommands();
        map<string, string>::iterator it;

        map<string, string> categories;
        map<string, string>::iterator itCat;
        //loop through each command outputting info
        for (it = commands.begin(); it != commands.end(); it++) {

            Command* thisCommand = getCommand(it->first);

            //don't add hidden commands
            if (thisCommand->getCommandCategory() != "Hidden") {
                itCat = categories.find(thisCommand->getCommandCategory());
                if (itCat == categories.end()) {
                    categories[thisCommand->getCommandCategory()] = thisCommand->getCommandName();
                }else {
                    categories[thisCommand->getCommandCategory()] += ", " + thisCommand->getCommandName();
                }
            }
        }

        for (itCat = categories.begin(); itCat != categories.end(); itCat++) {
            out << itCat->first << " commmands include: " << itCat->second << endl;
        }

    }
	catch(exception& e) {
		m->errorOut(e, "CommandFactory", "printCommandsCategories");
		exit(1);
	}
}

/***********************************************************************/
