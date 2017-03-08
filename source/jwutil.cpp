#include "jwutil.hpp"

using namespace std;

namespace jwutil {
	const string ARG_NAME_PREFIX = "--";

	map<string, string> argvToMap(int argc, char *argv[]) {
		map<string, string> result = map<string, string>();
		// Evaluate argument pairs
		for (int i = 1; i < argc; i += 2) {
			string a = argv[i];
			if (i + 1 >= argc) {
				throw string("ERROR: you must include pairs of argument names and ") +
					"values (e.g. maturnal --ARGUMENT1 VALUE1 --ARGUMENT2 VALUE2)";
			}
			string b = argv[i+1];
			// Make sure a starts with the argument prefix
			if (a.compare(0, ARG_NAME_PREFIX.size(), ARG_NAME_PREFIX) != 0) {
				throw "ERROR: expected argument name with " + ARG_NAME_PREFIX +
					"; got " + a + " instead";
			}
			// Add argument-value pair to the result
			result[a.substr(ARG_NAME_PREFIX.size())] = b;
		}
		return result;
	}
}