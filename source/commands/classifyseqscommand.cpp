/*
 *  classifyseqscommand.cpp
 *  Mothur
 *
 *  Created by westcott on 11/2/09.
 *  Copyright 2009 Schloss Lab. All rights reserved.
 *
 */

#include "classifyseqscommand.h"



//**********************************************************************************************************************
vector<string> ClassifySeqsCommand::setParameters(){
	try {
		CommandParameter ptaxonomy("taxonomy", "InputTypes", "", "", "none", "none", "none","",false,true,true); parameters.push_back(ptaxonomy);
		CommandParameter ptemplate("reference", "InputTypes", "", "", "none", "none", "none","",false,true,true); parameters.push_back(ptemplate);
		CommandParameter pfasta("fasta", "InputTypes", "", "", "none", "none", "none","taxonomy",false,true,true); parameters.push_back(pfasta);
		CommandParameter pname("name", "InputTypes", "", "", "NameCount", "none", "none","",false,false,true); parameters.push_back(pname);
		CommandParameter poutput("output", "Multiple", "simple-detail", "detail", "", "", "","",false,false, true); parameters.push_back(poutput);
		CommandParameter psearch("search", "Multiple", "kmer-blast-suffix-distance-align", "kmer", "", "", "","",false,false); parameters.push_back(psearch);
		CommandParameter pksize("ksize", "Number", "", "8", "", "", "","",false,false); parameters.push_back(pksize);
		CommandParameter pmethod("method", "Multiple", "wang-knn-zap", "wang", "", "", "","",false,false); parameters.push_back(pmethod);
		CommandParameter pprocessors("processors", "Number", "", "1", "", "", "","",false,false,true); parameters.push_back(pprocessors);
		CommandParameter pmatch("match", "Number", "", "1.0", "", "", "","",false,false); parameters.push_back(pmatch);
		CommandParameter pprintlevel("printlevel", "Number", "", "-1", "", "", "","",false,false); parameters.push_back(pprintlevel);
		CommandParameter pmismatch("mismatch", "Number", "", "-1.0", "", "", "","",false,false); parameters.push_back(pmismatch);
		CommandParameter pgapopen("gapopen", "Number", "", "-2.0", "", "", "","",false,false); parameters.push_back(pgapopen);
		CommandParameter pgapextend("gapextend", "Number", "", "-1.0", "", "", "","",false,false); parameters.push_back(pgapextend);
		CommandParameter pcutoff("cutoff", "Number", "", "80", "", "", "","",false,true); parameters.push_back(pcutoff);
		CommandParameter pprobs("probs", "Boolean", "", "T", "", "", "","",false,false); parameters.push_back(pprobs);
		CommandParameter piters("iters", "Number", "", "100", "", "", "","",false,true); parameters.push_back(piters);
		CommandParameter pnumwanted("numwanted", "Number", "", "10", "", "", "","",false,true); parameters.push_back(pnumwanted);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
		CommandParameter outtax("outtax", "String", "", "", "", "", "","",false,true,true); parameters.push_back(outtax);
		CommandParameter outaccnos("outaccnos", "String", "", "", "", "", "","",false,false); parameters.push_back(outaccnos);
		CommandParameter outmatchdist("outmatchdist", "String", "", "", "", "", "","",false,false); parameters.push_back(outmatchdist);
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "ClassifySeqsCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string ClassifySeqsCommand::getHelpString(){
	try {
		string helpString = "";
		helpString += "The classify.seqs command reads a fasta file containing sequences and creates a .taxonomy file and a .tax.summary file.\n";
		helpString += "The classify.seqs command parameters are reference, fasta, name, group, count, search, ksize, method, taxonomy, processors, match, mismatch, gapopen, gapextend, numwanted and probs.\n";
		helpString += "The reference, fasta and taxonomy parameters are required.\n";
		helpString += "The search parameter allows you to specify the method to find most similar template.  Your options are: suffix, kmer, blast, align and distance. The default is kmer.\n";
		helpString += "The method parameter allows you to specify classification method to use.  Your options are: wang, knn and zap. The default is wang.\n";
		helpString += "The ksize parameter allows you to specify the kmer size for finding most similar template to candidate.  The default is 8.\n";
		helpString += "The processors parameter allows you to specify the number of processors to use. The default is 1.\n";
		helpString += "The match parameter allows you to specify the bonus for having the same base. The default is 1.0.\n";
		helpString += "The mistmatch parameter allows you to specify the penalty for having different bases.  The default is -1.0.\n";
		helpString += "The gapopen parameter allows you to specify the penalty for opening a gap in an alignment. The default is -2.0.\n";
		helpString += "The gapextend parameter allows you to specify the penalty for extending a gap in an alignment.  The default is -1.0.\n";
		helpString += "The numwanted parameter allows you to specify the number of sequence matches you want with the knn method.  The default is 10.\n";
		helpString += "The cutoff parameter allows you to specify a bootstrap confidence threshold for your taxonomy.  The default is 80.\n";
		helpString += "The probs parameter shuts off the bootstrapping results for the wang and zap method. The default is true, meaning you want the bootstrapping to be shown.\n";
		helpString += "The iters parameter allows you to specify how many iterations to do when calculating the bootstrap confidence score for your taxonomy with the wang method.  The default is 100.\n";
		helpString += "The output parameter allows you to specify format of your summary file. Options are simple and detail. The default is detail.\n";
    helpString += "The printlevel parameter allows you to specify taxlevel of your summary file to print to. Options are 1 to the max level in the file.  The default is -1, meaning max level.  If you select a level greater than the level your sequences classify to, mothur will print to the level your max level. \n";
		helpString += "The classify.seqs command should be in the following format: \n";
		helpString += "classify.seqs(reference=yourTemplateFile, fasta=yourFastaFile, method=yourClassificationMethod, search=yourSearchmethod, ksize=yourKmerSize, taxonomy=yourTaxonomyFile, processors=yourProcessors) \n";
		helpString += "Example classify.seqs(fasta=amazon.fasta, reference=core.filtered, method=knn, search=gotoh, ksize=8, processors=2)\n";
		helpString += "The .taxonomy file consists of 2 columns: 1 = your sequence name, 2 = the taxonomy for your sequence. \n";
		helpString += "The .tax.summary is a summary of the different taxonomies represented in your fasta file. \n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "ClassifySeqsCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string ClassifySeqsCommand::getOutputPattern(string type) {
    try {
        string pattern = "";

        if (type == "taxonomy") {  pattern = "[filename],[tag],[tag2],taxonomy"; }
        else if (type == "accnos") {  pattern =  "[filename],[tag],[tag2],flip.accnos"; }
        else if (type == "matchdist") {  pattern =  "[filename],[tag],[tag2],match.dist"; }
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->control_pressed = true;  }

        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "ClassifySeqsCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
ClassifySeqsCommand::ClassifySeqsCommand(){
	try {
		abort = true; calledHelp = true;
		setParameters();
	}
	catch(exception& e) {
		m->errorOut(e, "ClassifySeqsCommand", "ClassifySeqsCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

ClassifySeqsCommand::ClassifySeqsCommand(map<string, string> parameters) {
	abort = false; calledHelp = false;

	vector<string> myArray = setParameters();

	ValidParameters validParameter("classify.seqs");
	map<string, string>::iterator it;

	//check to make sure all parameters are valid for command
	for (it = parameters.begin(); it != parameters.end(); it++) {
		if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
	}

	fastaFileName = validParameter.validFile(parameters, "fasta", false);

	//check for optional parameter and set defaults
	// ...at some point should added some additional type checking...
	string temp;
	temp = validParameter.validFile(parameters, "processors", false);	if (temp == "not found"){	temp = m->getProcessors();	}
	m->setProcessors(temp);
	m->mothurConvert(temp, processors);

	//this has to go after save so that if the user sets save=t and provides no reference we abort
	templateFileName = validParameter.validFile(parameters, "reference", true);
	if (templateFileName == "not found") {
			m->mothurOut("[ERROR]: The reference parameter is a required for the classify.seqs command.\n"); abort = true;
	}else if (templateFileName == "not open") { abort = true; }


	//this has to go after save so that if the user sets save=t and provides no reference we abort
	taxonomyFileName = validParameter.validFile(parameters, "taxonomy", true);
	if (taxonomyFileName == "not found") {  m->mothurOut("[ERROR]: The taxonomy parameter is a required for the classify.seqs command.\n"); abort = true;
	}else if (taxonomyFileName == "not open") { abort = true; }

	search = validParameter.validFile(parameters, "search", false);		if (search == "not found"){	search = "kmer";		}

	method = validParameter.validFile(parameters, "method", false);		if (method == "not found"){	method = "wang";	}

				temp = validParameter.validFile(parameters, "ksize", false);		if (temp == "not found"){
						temp = "8";
						if (method == "zap") { temp = "7"; }
				}
	m->mothurConvert(temp, kmerSize);

	temp = validParameter.validFile(parameters, "match", false);		if (temp == "not found"){	temp = "1.0";			}
	m->mothurConvert(temp, match);

				temp = validParameter.validFile(parameters, "printlevel", false);		if (temp == "not found"){	temp = "-1";		}
				m->mothurConvert(temp, printlevel);

	temp = validParameter.validFile(parameters, "mismatch", false);		if (temp == "not found"){	temp = "-1.0";			}
	m->mothurConvert(temp, misMatch);

	temp = validParameter.validFile(parameters, "gapopen", false);		if (temp == "not found"){	temp = "-2.0";			}
	m->mothurConvert(temp, gapOpen);

	temp = validParameter.validFile(parameters, "gapextend", false);	if (temp == "not found"){	temp = "-1.0";			}
	m->mothurConvert(temp, gapExtend);

	temp = validParameter.validFile(parameters, "numwanted", false);	if (temp == "not found"){	temp = "10";			}
	m->mothurConvert(temp, numWanted);

	temp = validParameter.validFile(parameters, "cutoff", false);		if (temp == "not found"){	temp = "80";				}
	m->mothurConvert(temp, cutoff);

	temp = validParameter.validFile(parameters, "probs", false);		if (temp == "not found"){	temp = "true";			}
	probs = m->isTrue(temp);

	temp = validParameter.validFile(parameters, "shortcuts", false);	if (temp == "not found"){	temp = "true";			}
	writeShortcuts = m->isTrue(temp);

	temp = validParameter.validFile(parameters, "outtax", false);	if (temp == "not found"){
			m->mothurOut("[ERROR]: The reference parameter is a required for the classify.seqs command.\n"); abort = true;}
	outTax = temp;
	temp = validParameter.validFile(parameters, "outaccnos", false);	if (temp == "not found"){	temp = "";			}
	outAccnos = temp;
	temp = validParameter.validFile(parameters, "outmatchdist", false);	if (temp == "not found"){	temp = "";			}
	outMatchdist = temp;

	//temp = validParameter.validFile(parameters, "flip", false);			if (temp == "not found"){	temp = "T";				}
	//flip = m->isTrue(temp);
	flip = true;

	temp = validParameter.validFile(parameters, "iters", false);		if (temp == "not found") { temp = "100";			}
	m->mothurConvert(temp, iters);

	if ((method == "wang") && (search != "kmer"))  {
		m->mothurOut("The wang method requires the kmer search. " + search + " will be disregarded, and kmer will be used." ); m->mothurOutEndLine();
		search = "kmer";
	}

				if ((method == "zap") && ((search != "kmer") && (search != "align")))  {
		m->mothurOut("The zap method requires the kmer or align search. " + search + " will be disregarded, and kmer will be used." ); m->mothurOutEndLine();
		search = "kmer";
	}

}

//**********************************************************************************************************************
ClassifySeqsCommand::~ClassifySeqsCommand(){
	if (abort == false) {
		for (int i = 0; i < lines.size(); i++) {  delete lines[i];  }  lines.clear();
	}
}
//**********************************************************************************************************************

int ClassifySeqsCommand::execute(){
    string outputMethodTag = method;
		if(method == "wang") {
			classify = new Bayesian(taxonomyFileName, templateFileName, search,
				kmerSize, cutoff, iters, m->getRandomNumber(), flip, writeShortcuts);
		} else if(method == "knn") {
			classify = new Knn(taxonomyFileName, templateFileName, search,
				kmerSize, gapOpen, gapExtend, match, misMatch, numWanted, m->getRandomNumber());
		} else if(method == "zap"){
			outputMethodTag = search + "_" + outputMethodTag;
    	if (search == "kmer") {
				classify = new KmerTree(templateFileName, taxonomyFileName, kmerSize, cutoff);
			} else {
				classify = new AlignTree(templateFileName, taxonomyFileName, cutoff);
			}
    } else {
			throw string("ERROR: ") + search + " is not a valid classifier method.";
		}

		// Process FASTA file
		m->mothurOut("Classifying sequences from " + fastaFileName + " ..." ); m->mothurOutEndLine();

		string baseTName = m->getSimpleName(taxonomyFileName);

    // Set output file names
		string RippedTaxName = "";
    bool foundDot = false;
    for (int i = baseTName.length()-1; i >= 0; i--) {
        if (foundDot && (baseTName[i] != '.')) {  RippedTaxName = baseTName[i] + RippedTaxName; }
        else if (foundDot && (baseTName[i] == '.')) {  break; }
        else if (!foundDot && (baseTName[i] == '.')) {  foundDot = true; }
    }
    map<string, string> variables;
    variables["[filename]"] = m->hasPath(fastaFileName) + m->getRootName(m->getSimpleName(fastaFileName));
    variables["[tag]"] = RippedTaxName;
    variables["[tag2]"] = outputMethodTag;

		string tempTaxonomyFile = outTax + ".temp";
		keepAccnos = true;
		if (outAccnos.size() == 0) {
			keepAccnos = false;
			outAccnos = outTax + ".accnos.temp";
		}

		if ((method == "knn") && (search == "distance")) {
			classify->setDistName(outMatchdist);
		}

		int start = time(NULL);
		int numFastaSeqs = 0;
		for (int i = 0; i < lines.size(); i++) { delete lines[i]; } lines.clear();

		vector<unsigned long long> positions;

		// Split job into parallel tasks
		#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
			positions = m->divideFile(fastaFileName, processors);
			for (int i = 0; i < (positions.size()-1); i++) {
				lines.push_back(new linePair(positions[i], positions[(i+1)]));
			}
		#else
			if (processors == 1) {
				lines.push_back(new linePair(0, 1000));
			} else {
				positions = m->setFilePosFasta(fastaFileName, numFastaSeqs);
                if (numFastaSeqs < processors) { processors = numFastaSeqs; }

				//figure out how many sequences you have to process
				int numSeqsPerProcessor = numFastaSeqs / processors;
				for (int i = 0; i < processors; i++) {
					int startIndex =  i * numSeqsPerProcessor;
					if(i == (processors - 1)){	numSeqsPerProcessor = numFastaSeqs - i * numSeqsPerProcessor; 	}
					lines.push_back(new linePair(positions[startIndex], numSeqsPerProcessor));
				}
			}
		#endif

		if (processors == 1) {
			numFastaSeqs = driver(lines[0], outTax, tempTaxonomyFile, outAccnos, fastaFileName);
		} else {
			numFastaSeqs = createProcesses(outTax, tempTaxonomyFile, outAccnos, fastaFileName);
		}

		// Check if any sequences were reversed.
		if (!m->isBlank(outAccnos)) {
			m->mothurOutEndLine();
			m->mothurOut(string("[WARNING]: mothur reversed some your sequences ") +
				"for a better classification.");
			if (keepAccnos) {
				m->mothurOut(string(" If you would like to take a closer ") +
				"look, please check " + outAccnos + " for the list of the " +
				"sequences.");
			}
			m->mothurOutEndLine();
    }

		if (!keepAccnos) {
			m->mothurRemove(outAccnos);
		}

		// Report time for classification
    m->mothurOutEndLine();
    m->mothurOut("It took " + toString(time(NULL) - start) +
			" secs to classify " + toString(numFastaSeqs) + " sequences.");
		m->mothurOutEndLine();
		m->mothurOutEndLine();

    start = time(NULL);

    //output taxonomy with the unclassified bins added
    ifstream inTax;
    m->openInputFile(outTax, inTax);

    ofstream outTax_handle;
    string unclass = outTax + ".unclass.temp";
    m->openOutputFile(unclass, outTax_handle);

    //get maxLevel from phylotree so you know how many 'unclassified's to add
    int maxLevel = classify->getMaxLevel();

    //read taxfile - this reading and rewriting is done to preserve the confidence scores.
    string name, taxon;
    string group = "";
    GroupMap* groupMap = NULL;
    CountTable* ct = NULL;
    PhyloSummary* taxaSum;

    while (!inTax.eof()) {
        inTax >> name; m->gobble(inTax);
        taxon = m->getline(inTax); m->gobble(inTax);

        string newTax = m->addUnclassifieds(taxon, maxLevel, probs);

        outTax_handle << name << '\t' << newTax << endl;
    }

    inTax.close();
    outTax_handle.close();

    m->mothurRemove(outTax);
    m->renameFile(unclass, outTax);

    if (ct != NULL) { delete ct; }
    if (groupMap != NULL) { delete groupMap; } delete taxaSum;
    m->mothurRemove(tempTaxonomyFile);

    m->mothurOutEndLine();
    m->mothurOut("It took " + toString(time(NULL) - start) + " secs to create the summary file for " + toString(numFastaSeqs) + " sequences."); m->mothurOutEndLine(); m->mothurOutEndLine();

    delete classify;

		return 0;
}
/**************************************************************************************************/

int ClassifySeqsCommand::createProcesses(string taxFileName, string tempTaxFile, string accnos, string filename) {
	try {

		int num = 0;
		processIDS.clear();
        bool recalc = false;

#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
		int process = 1;

		//loop through and create all the processes you want
		while (process != processors) {
			pid_t pid = fork();

			if (pid > 0) {
				processIDS.push_back(pid);  //create map from line number to pid so you can append files in correct order later
				process++;
			}else if (pid == 0){
				num = driver(lines[process], taxFileName + m->mothurGetpid(process) + ".temp", tempTaxFile + m->mothurGetpid(process) + ".temp", accnos + m->mothurGetpid(process) + ".temp", filename);

				//pass numSeqs to parent
				ofstream out;
				string tempFile = filename + m->mothurGetpid(process) + ".num.temp";
				m->openOutputFile(tempFile, out);
				out << num << endl;
				out.close();

				exit(0);
            }else {
                m->mothurOut("[ERROR]: unable to spawn the number of processes you requested, reducing number to " + toString(process) + "\n"); processors = process;
                for (int i = 0; i < processIDS.size(); i++) { kill (processIDS[i], SIGINT); }
                //wait to die
                for (int i=0;i<processIDS.size();i++) {
                    int temp = processIDS[i];
                    wait(&temp);
                }
                m->control_pressed = false;
                recalc = true;
                break;
            }
		}

        if (recalc) {
            //test line, also set recalc to true.
            //for (int i = 0; i < processIDS.size(); i++) { kill (processIDS[i], SIGINT); } for (int i=0;i<processIDS.size();i++) { int temp = processIDS[i]; wait(&temp); } m->control_pressed = false;  processors=3; m->mothurOut("[ERROR]: unable to spawn the number of processes you requested, reducing number to " + toString(processors) + "\n");
            for (int i = 0; i < lines.size(); i++) {  delete lines[i];  }  lines.clear();
            vector<unsigned long long> positions = m->divideFile(filename, processors);
            for (int i = 0; i < (positions.size()-1); i++) {	lines.push_back(new linePair(positions[i], positions[(i+1)]));	}

            num = 0;
            processIDS.resize(0);
            process = 1;

            while (process != processors) {
                pid_t pid = fork();

                if (pid > 0) {
                    processIDS.push_back(pid);  //create map from line number to pid so you can append files in correct order later
                    process++;
                }else if (pid == 0){
                    num = driver(lines[process], taxFileName + m->mothurGetpid(process) + ".temp", tempTaxFile + m->mothurGetpid(process) + ".temp", accnos + m->mothurGetpid(process) + ".temp", filename);

                    //pass numSeqs to parent
                    ofstream out;
                    string tempFile = filename + m->mothurGetpid(process) + ".num.temp";
                    m->openOutputFile(tempFile, out);
                    out << num << endl;
                    out.close();

                    exit(0);
                }else {
                    m->mothurOut("[ERROR]: unable to spawn the necessary processes."); m->mothurOutEndLine();
                    for (int i = 0; i < processIDS.size(); i++) { kill (processIDS[i], SIGINT); }
                    exit(0);
                }
            }
        }


		//parent does its part
		num = driver(lines[0], taxFileName, tempTaxFile, accnos, filename);

		//force parent to wait until all the processes are done
		for (int i=0;i<processIDS.size();i++) {
			int temp = processIDS[i];
			wait(&temp);
		}

		for (int i = 0; i < processIDS.size(); i++) {
			ifstream in;
			string tempFile =  filename + toString(processIDS[i]) + ".num.temp";
			m->openInputFile(tempFile, in);
			if (!in.eof()) { int tempNum = 0; in >> tempNum; num += tempNum; }
			in.close(); m->mothurRemove(m->getFullPathName(tempFile));
		}
#else
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//Windows version shared memory, so be careful when passing variables through the alignData struct.
		//Above fork() will clone, so memory is separate, but that's not the case with windows,
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		vector<classifyData*> pDataArray;
		DWORD   dwThreadIdArray[processors-1];
		HANDLE  hThreadArray[processors-1];

		//Create processor worker threads.
		for( int i=0; i<processors-1; i++ ){
			// Allocate memory for thread data.
			string extension = "";
			if (i != 0) { extension = toString(i) + ".temp"; processIDS.push_back(i); }

			classifyData* tempclass = new classifyData((accnos + extension), probs, method, templateFileName, taxonomyFileName, (taxFileName + extension), (tempTaxFile + extension), filename, search, kmerSize, iters, numWanted, m, lines[i]->start, lines[i]->end, match, misMatch, gapOpen, gapExtend, cutoff, i, flip, false);
			pDataArray.push_back(tempclass);

			//MySeqSumThreadFunction is in header. It must be global or static to work with the threads.
			//default security attributes, thread function name, argument to thread function, use default creation flags, returns the thread identifier
			hThreadArray[i] = CreateThread(NULL, 0, MyClassThreadFunction, pDataArray[i], 0, &dwThreadIdArray[i]);

		}

		//parent does its part
		num = driver(lines[processors-1], taxFileName + toString(processors-1) + ".temp", tempTaxFile + toString(processors-1) + ".temp", accnos + toString(processors-1) + ".temp", filename);
		processIDS.push_back((processors-1));

		//Wait until all threads have terminated.
		WaitForMultipleObjects(processors-1, hThreadArray, TRUE, INFINITE);

		//Close all thread handles and free memory allocations.
		for(int i=0; i < pDataArray.size(); i++){
			num += pDataArray[i]->count;
      if (pDataArray[i]->count != pDataArray[i]->end) {
        m->mothurOut("[ERROR]: process " + toString(i) +
					" only processed " + toString(pDataArray[i]->count) +
					" of " + toString(pDataArray[i]->end) +
					" sequences assigned to it, quitting. \n");
				m->control_pressed = true;
      }
			CloseHandle(hThreadArray[i]);
			delete pDataArray[i];
		}
	#endif
        vector<string> nonBlankAccnosFiles;
		if (!(m->isBlank(accnos))) { nonBlankAccnosFiles.push_back(accnos); }
		else { m->mothurRemove(accnos); } //remove so other files can be renamed to it

		for(int i=0;i<processIDS.size();i++){
			m->appendFiles((taxFileName + toString(processIDS[i]) + ".temp"), taxFileName);
			m->appendFiles((tempTaxFile + toString(processIDS[i]) + ".temp"), tempTaxFile);
            if (!(m->isBlank(accnos + toString(processIDS[i]) + ".temp"))) {
				nonBlankAccnosFiles.push_back(accnos + toString(processIDS[i]) + ".temp");
			}else { m->mothurRemove((accnos + toString(processIDS[i]) + ".temp"));  }

			m->mothurRemove((m->getFullPathName(taxFileName) + toString(processIDS[i]) + ".temp"));
			m->mothurRemove((m->getFullPathName(tempTaxFile) + toString(processIDS[i]) + ".temp"));
		}

        //append accnos files
		if (nonBlankAccnosFiles.size() != 0) {
			rename(nonBlankAccnosFiles[0].c_str(), accnos.c_str());

			for (int h=1; h < nonBlankAccnosFiles.size(); h++) {
				m->appendFiles(nonBlankAccnosFiles[h], accnos);
				m->mothurRemove(nonBlankAccnosFiles[h]);
			}
		}else { //recreate the accnosfile if needed
			ofstream out;
			m->openOutputFile(accnos, out);
			out.close();
		}

		return num;

	}
	catch(exception& e) {
		m->errorOut(e, "ClassifySeqsCommand", "createProcesses");
		exit(1);
	}
}
//**********************************************************************************************************************

int ClassifySeqsCommand::driver(linePair* filePos, string taxFName, string tempTFName, string accnos, string filename){
	try {
		ofstream outTax_handle;
		m->openOutputFile(taxFName, outTax_handle);

		ofstream outTaxSimple;
		m->openOutputFile(tempTFName, outTaxSimple);

		ofstream outAcc;
		m->openOutputFile(accnos, outAcc);

		ifstream inFASTA;
		m->openInputFile(filename, inFASTA);

		string taxonomy;

		inFASTA.seekg(filePos->start);

		bool done = false;
		int count = 0;

		while (!done) {
			if (m->control_pressed) {
				inFASTA.close();
				outTax_handle.close();
				outTaxSimple.close();
				outAcc.close(); return 0; }

			Sequence* candidateSeq = new Sequence(inFASTA); m->gobble(inFASTA);

			if (candidateSeq->getName() != "") {

				taxonomy = classify->getTaxonomy(candidateSeq);

				if (m->control_pressed) { delete candidateSeq; return 0; }

				if (taxonomy == "unknown;") { m->mothurOut("[WARNING]: " + candidateSeq->getName() + " could not be classified. You can use the remove.lineage command with taxon=unknown; to remove such sequences."); m->mothurOutEndLine(); }

				//output confidence scores or not
				if (probs) {
					outTax_handle << candidateSeq->getName() << '\t' << taxonomy << endl;
				}else{
					outTax_handle << candidateSeq->getName() << '\t' << classify->getSimpleTax() << endl;
				}

				if (classify->getFlipped()) { outAcc << candidateSeq->getName() << endl; }

				outTaxSimple << candidateSeq->getName() << '\t' << classify->getSimpleTax() << endl;

				count++;
			}
			delete candidateSeq;

			#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
				unsigned long long pos = inFASTA.tellg();
				if ((pos == -1) || (pos >= filePos->end)) { break; }
			#else
				if (inFASTA.eof()) { break; }
			#endif

			//report progress
			if((count) % 100 == 0){	m->mothurOutJustToScreen("Processing sequence: " + toString(count) +"\n"); 		}

		}
		//report progress
		if((count) % 100 != 0){	m->mothurOutJustToScreen("Processing sequence: " + toString(count)+"\n"); 		}

		inFASTA.close();
		outTax_handle.close();
		outTaxSimple.close();
		outAcc.close();

		return count;
	}
	catch(exception& e) {
		m->errorOut(e, "ClassifySeqsCommand", "driver");
		exit(1);
	}
}
/**************************************************************************************************/
