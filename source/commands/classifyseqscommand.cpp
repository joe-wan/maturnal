/*
 *  classifyseqscommand.cpp
 *  Mothur
 *
 *  Created by westcott on 11/2/09.
 *  Copyright 2009 Schloss Lab. All rights reserved.
 *
 */

#include "classifyseqscommand.h"

ClassifySeqsCommand::ClassifySeqsCommand(string fastaFileName_, string taxonomyFileName_, string referenceFileName_,
		string outTax_, string outAccnos_, string outMatchdist_, string method_,
		string search_, int kmerSize_, int cutoff_, int iters_, int numwanted_,
		int processors_, int printlevel_, bool probs_, float match_, float misMatch_,
		float gapOpen_, float gapExtend_, bool flip_) : ClassifySeqsCommand() {
	abort = false; calledHelp = false;


	fastaFileName = fastaFileName_;
	taxonomyFileName = taxonomyFileName_;
	templateFileName = referenceFileName_;
	outTax = outTax_;
	outAccnos = outAccnos_;
	outMatchdist = outMatchdist_;
	method = method_;
	search = search_;
	kmerSize = kmerSize_;
	cutoff = cutoff_;
	iters = iters_;
	numWanted = numwanted_;
	processors = processors_;
	printlevel = printlevel_;
	probs = probs_;
	match = match_;
	misMatch = misMatch_;
	gapOpen = gapOpen_;
	gapExtend = gapExtend_;
	flip = flip_;

	writeShortcuts = true;

	m->setProcessors(std::to_string(processors));

	if (taxonomyFileName == "") {
			m->mothurOut("[ERROR]: The reference parameter is a required for the classify.seqs command.\n");
			abort = true;
	}

	if ((method == "wang") && (search != "kmer"))  {
		m->mothurOut("The wang method requires the kmer search. " + search + " will be disregarded, and kmer will be used." ); m->mothurOutEndLine();
		search = "kmer";
	}

	if ((method == "zap") && ((search != "kmer") && (search != "align")))  {
		m->mothurOut("The zap method requires the kmer or align search. " + search + " will be disregarded, and kmer will be used." ); m->mothurOutEndLine();
		search = "kmer";
	}

}

ClassifySeqsCommand::ClassifySeqsCommand(){
	m = MothurOut::getInstance();
       try {
               abort = true; calledHelp = true;
       }
       catch(exception& e) {
               m->errorOut(e, "ClassifySeqsCommand", "ClassifySeqsCommand");
               exit(1);
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
