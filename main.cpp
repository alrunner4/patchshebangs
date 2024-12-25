#include <cstdio>   // remove, rename
#include <cstdlib>  // exit
#include <fstream>  // ifstream
#include <iostream> // cin, cout
#include <map>      // map
#include <sstream>  // stringstream
#include <string>   // getline, string

int main(int argc, const char **argv) {
	const std::string self(argv[0]);
	std::map<std::string, std::string> rewrites;
	std::string line, from, to;
	while ( std::getline(std::cin, line), !line.empty() ) {
		std::stringstream lineStream(line);
		from.clear();
		to.clear();
		lineStream >> from >> to;
		if ( lineStream.bad() || !lineStream.eof() || from.empty() || to.empty() ) {
			std::cerr << self << ": bad rewrite input: " << line << std::endl;
			std::exit(1);
		}
		if ( rewrites.find(from) != rewrites.end() ) {
			std::cerr << self << ": duplicate rewrite for " << from << std::endl;
			std::exit(2);
		}
		rewrites.insert(std::make_pair(from, to));
	}

	for ( int n = 1; n < argc; ++n ) {
		std::ifstream file(argv[n]);
		{
			char she, bang;
			file >> she >> bang;
			if ( she != '#' || bang != '!' ) continue;
		}

		std::string shebangLine;
		std::getline(file, shebangLine);
		std::stringstream shebangLineStream(shebangLine);
		std::string interpreter;
		shebangLineStream >> interpreter;

		if (interpreter.empty()) {
			std::cerr << "shebang line has no interpreter: " << argv[n] << std::endl;
			continue;
		}

		auto rewrite = rewrites.find(interpreter);
		if ( rewrite == rewrites.end() ) {
			// We don't have a rewrite for this interpreter. Ignore this file.
			continue;
		}

		char temporaryFileName[L_tmpnam];
		std::tmpnam(temporaryFileName);
		std::ofstream temporaryFile(temporaryFileName);
		temporaryFile << "#!" << rewrite->second << file.rdbuf();

		temporaryFile.close();
		file.close();
		std::remove(argv[n]);
		std::rename(temporaryFileName, argv[n]);
	}

	// DEBUG
	for ( auto i : rewrites ) {
		std::cout << i.first << " -> " << i.second << std::endl;
	}
	return 0;
}
