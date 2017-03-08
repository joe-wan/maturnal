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
#include "args.hxx"

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

	// // Parse arguments (expect '--') into a map<string, string>
	// map<string, string> parameters;
	// try {
	// 	parameters = jwutil::argvToMap(argc, argv);
	// } catch (string message) {
	// 	m->mothurOut(message);
	// 	m->mothurOutEndLine();
	// 	m->mothurOut("Use \"maturnal --help\" to see a list of options.");
	// 	m->mothurOutEndLine();
	// 	return 1;
	// }
	//
	// // See if user has requested help, citation, or execution
	// if (parameters.count("help") > 0) {
	// 	// Print help
	// 	m->mothurOut(ClassifySeqsCommand().getHelpString());
	// 	m->mothurOutEndLine();
	// } else if (parameters.count("citation") > 0) {
	// 	// Print citation
	// 	m->mothurOut(ClassifySeqsCommand().getCitation());
	// 	m->mothurOutEndLine();
	// } else {
	// 	// Classify the sequences
	// 	ClassifySeqsCommand command = ClassifySeqsCommand(parameters);
	// 	command.execute();
	// }

	args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  parser.LongSeparator(" ");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

	args::Group inputGroup(parser, "Input:");
	args::ValueFlag<string> fastaF(inputGroup, "FILE",
		"Input FASTA file.", {"fasta", 'i'});
	args::ValueFlag<string> taxonomyF(inputGroup, "FILE",
		"Path to taxonomy file for training set.", {"taxonomy"});
	args::ValueFlag<string> referenceF(inputGroup, "FILE",
		"Path to reference (FASTA) file for training set.", {"reference"});

	args::Group outputGroup(parser, "Output:");
	args::ValueFlag<string> outputF(outputGroup, "FILE",
		"Output taxonomy", {'o', "output"});
	args::ValueFlag<string> outputFlippedF(outputGroup, "FILE",
		"", {"output_flipped"});
	args::ValueFlag<string> outputMatchdistF(outputGroup, "FILE",
		"", {"output_matchdist"});

	args::Group classifyGroup(parser, "Classification method:");
	args::ValueFlag<string> methodF(classifyGroup, "METHOD",
	"Classification method to use. Options are: wang, knn and zap. Default: wang",
	{"method"});
	args::ValueFlag<string> searchF(classifyGroup, "SEARCH",
		"The method to find most similar template. Options are: suffix, kmer, blast, align, distance. Default: kmer",
		{"search"});
	args::ValueFlag<int> ksizeF(classifyGroup, "KSIZE",
		"If --method is set to kmer, this option specifies kmer length. Default: 8",
		{"ksize"}, 8);

	args::Group classifyOptionsGroup(parser, "Classifier options:");
	args::ValueFlag<int> cutoffF(classifyOptionsGroup, "CUTOFF",
		"", {"cutoff"}, 80);
	args::ValueFlag<int> itersF(classifyOptionsGroup, "ITERS",
		"", {"iters"}, 100);
	args::ValueFlag<int> numwantedF(classifyOptionsGroup, "NUMWANTED",
		"If --method is knn, this option specifies the number (k) of database matches to consider.",
		{"numwanted"}, 10);
	args::ValueFlag<int> processorsF(classifyOptionsGroup, "PROCESSORS",
		"Number of processors. Default: 1", {"processors"}, 1);

	args::Group outFormatGroup(parser, "Output format:");
	args::ValueFlag<int> printlevelF(outFormatGroup, "PRINTLEVEL",
		"The number of taxonomic levels to include in the output taxonomy file.",
		{"printlevel"}, -1);
	args::Flag noProbsF(outFormatGroup, "",
		"If specified, omit bootsrapping probability from output file.",
		{"no_probs"});

	args::Group alignGroup(parser, "Alignment options \n(for --search blast):");
	args::ValueFlag<float> matchF(alignGroup, "MATCH",
		"Match score. Default: 1.0", {"match"}, 1.0);
	args::ValueFlag<float> mismatchF(alignGroup, "MISMATCH",
		"Mismatch score. Default: -1.0", {"mismatch"}, -1.0);
	args::ValueFlag<float> gapopenF(alignGroup, "GAPOPEN",
		"Gap open score. Default: -2.0", {"gapopen"}, -2.0);
	args::ValueFlag<float> gapextendF(alignGroup, "GAPEXTEND",
		"Gap extend score. Default: -1.0", {"gapextend"}, -1.0);

	try {
	    parser.ParseCLI(argc, argv);
	} catch (args::Help) {
	    std::cout << parser;
	    return 0;
	} catch (args::ParseError e) {
	    std::cerr << e.what() << std::endl;
	    std::cerr << parser;
	    return 1;
	} catch (args::ValidationError e) {
	    std::cerr << e.what() << std::endl;
	    std::cerr << parser;
	    return 1;
	}

	string fasta = args::get(fastaF);
	string taxonomy = args::get(taxonomyF);
	string reference = args::get(referenceF);
	string output = args::get(outputF);
	string outputFlipped = args::get(outputFlippedF);
	string outputMatchdist = args::get(outputMatchdistF);
	string method = args::get(methodF);
	string search = args::get(searchF);
	int ksize = args::get(ksizeF);
	int cutoff = args::get(cutoffF);
	int iters = args::get(itersF);
	int numwanted = args::get(numwantedF);
	int processors = args::get(processorsF);
	int printlevel = args::get(printlevelF);
	bool noProbs = args::get(noProbsF);
	float match = args::get(matchF);
	float mismatch = args::get(mismatchF);
	float gapopen = args::get(gapopenF);
	float gapextend = args::get(gapextendF);

	

	cout << "fasta\t" << fasta << endl;
	cout << "taxonomy\t" << taxonomy << endl;
	cout << "reference\t" << reference << endl;
	cout << "output\t" << output << endl;
	cout << "outputFlipped\t" << outputFlipped << endl;
	cout << "outputMatchdist\t" << outputMatchdist << endl;
	cout << "method\t" << method << endl;
	cout << "search\t" << search << endl;
	cout << "ksize\t" << ksize << endl;
	cout << "cutoff\t" << cutoff << endl;
	cout << "iters\t" << iters << endl;
	cout << "numwanted\t" << numwanted << endl;
	cout << "processors\t" << processors << endl;
	cout << "printlevel\t" << printlevel << endl;
	cout << "noProbs\t" << noProbs << endl;
	cout << "match\t" << match << endl;
	cout << "mismatch\t" << mismatch << endl;
	cout << "gapopen\t" << gapopen << endl;
	cout << "gapextend\t" << gapextend << endl;

	// if (integer) { std::cout << "i: " << args::get(integer) << std::endl; }
	// if (characters) { for (const auto ch: args::get(characters)) { std::cout << "c: " << ch << std::endl; } }
	// if (foo) { std::cout << "f: " << args::get(foo) << std::endl; }
	// if (numbers) { for (const auto nm: args::get(numbers)) { std::cout << "n: " << nm << std::endl; } }
	// return 0;

	return 0;
}

/**************************************************************************************************/
