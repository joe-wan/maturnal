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
#include "args.hxx"
#include "jwargs.hpp"

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

	// Set up the argument parser
	args::ArgumentParser parser(std::string() + "A command-line utility for " +
		"classifying marker gene amplicons, repackaged from the mothur source " +
		"code.\n\n" +
		"Basic usage (uses default naive Bayesian classifier):\n\n" +
		"    maturnal --input [INPUT FASTA] --reference [TRAINING FASTA] \\\n" +
		"        --taxonomy [TRAINING TAXONOMY] --output [CLASSIFICATION OUTPUT]\n\n" +
		"Please cite the creators of mothur and of the RDP Classifier. For the "
		"full citation, type:\n\n" +
		"    maturnal --cite");
  parser.LongSeparator(" ");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag cite(parser, "cite", "Display citation information.",
		{'c', "cite"});

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
	jwargs::ChoiceFlag methodF(classifyGroup, "METHOD",
		"Classification method to use. Options are: wang, knn and zap. Default: wang",
		{"method"}, {"wang", "knn", "zap"}, "wang");
	jwargs::ChoiceFlag searchF(classifyGroup, "SEARCH",
		"The method to find most similar template. Options are: suffix, kmer, blast, align, distance. Default: kmer",
		{"search"}, {"suffix", "kmer", "blast", "align", "distance"}, "kmer");
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
	args::Flag noFlipF(classifyOptionsGroup, "",
		"If specified, don't try reverse complement of sequences.",
		{"no_flip"});

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

	// Parse the arguments, printing to STDERR if there are any exceptions
	try {
	    parser.ParseCLI(argc, argv);
	} catch (args::Help) {
	    std::cout << parser;
	    return 0;
	} catch (args::ParseError e) {
			std::cerr << "ERROR: could not parse input." << std::endl;
	    std::cerr << e.what() << std::endl << std::endl;
	    std::cerr << parser;
	    return 1;
	} catch (args::ValidationError e) {
			std::cerr << "ERROR: invalid options." << std::endl;
	    std::cerr << e.what() << std::endl << std::endl;
	    std::cerr << parser;
	    return 1;
	}

	if (args::get(cite)) {
			std::string citeMessage = std::string() +
				"When using, please cite:\n\n"
				"Schloss, P.D., et al., Introducing mothur: Open-source, " +
				"platform-independent, community-supported software for describing " +
				"and comparing microbial communities. Appl Environ Microbiol, 2009. " +
				"75(23):7537-41.\n\n" +
				"For the naive Bayesian classifier (the default algorithm, " +
				"\"--method wang\"), cite:\n\n" +
				"Wang Q, Garrity GM, Tiedje JM, Cole JR (2007). Naive Bayesian " +
				"classifier for rapid assignment of rRNA sequences into the new " +
				"bacterial taxonomy. Appl Environ Microbiol 73: 5261-7.";
			for (const std::string line : args::Wrap(citeMessage, 80)) {
				std::cout << line << std::endl;
			}
			return 0;
	}

	// Store the arguments
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
	bool probs = !args::get(noProbsF); // This flag is "true" when chosen
	float match = args::get(matchF);
	float mismatch = args::get(mismatchF);
	float gapopen = args::get(gapopenF);
	float gapextend = args::get(gapextendF);
	bool flip = !args::get(noFlipF); // This flag is "true" when chosen

	// Validate arguments
	if (fasta.size() == 0) {
		std::cerr << "ERROR: you must specify an input FASTA file " <<
			"(--input [FILE])." << std::endl << std::endl;
		std::cerr << parser;
		return 1;
	} else if (reference.size() == 0) {
		std::cerr << "ERROR: you must specify a training set FASTA file " <<
			"(--reference [FILE])." << std::endl << std::endl;
		std::cerr << parser;
		return 1;
	} else if (taxonomy.size() == 0) {
		std::cerr << "ERROR: you must specify a training set taxonomy file " <<
			"(--taxonomy [FILE])." << std::endl << std::endl;
		std::cerr << parser;
		return 1;
	} else if (output.size() == 0) {
		std::cerr << "ERROR: you must specify an output file " <<
			"(--output [FILE])." << std::endl << std::endl;
		std::cerr << parser;
		return 1;
	}

	// Run the classifier
	ClassifySeqsCommand command = ClassifySeqsCommand(fasta, taxonomy, reference,
		output, outputFlipped, outputMatchdist, method,
		search, ksize, cutoff, iters, numwanted,
		processors, printlevel, probs, match, mismatch,
		gapopen, gapextend, flip);
	command.execute();

	return 0;
}

/**************************************************************************************************/
