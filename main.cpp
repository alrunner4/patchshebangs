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
		shebangLine.clear();
		std::getline(shebangLineStream, shebangLine);

		if (interpreter.empty()) {
			std::cerr << self << ": shebang line has no interpreter: " << argv[n] << std::endl;
			continue;
		}

		std::cerr << "DEBUG: file " << argv[n] << " has interpreter: \"" << interpreter
			  << "\" and remainder \"" << shebangLine << "\"\n";

		auto rewrite = rewrites.find(interpreter);
		if ( rewrite == rewrites.end() ) {
			// We don't have a rewrite for this interpreter. Ignore this file.
			continue;
		}

		char temporaryFileName[L_tmpnam];
		std::tmpnam(temporaryFileName);
		std::ofstream temporaryFile(temporaryFileName);
		temporaryFile << "#!" << rewrite->second << shebangLine << std::endl << file.rdbuf();
		temporaryFile.close();
		file.close();
		if ( std::remove(argv[n]) != 0) {
			std::cerr << self << ": couldn't delete file " << argv[n] << std::endl;
			std::perror(self.c_str());
			exit(4);
		}
		if ( std::system((std::string("mv ") + temporaryFileName + " " + argv[n]).c_str()) != 0 ) {
			std::cerr << self << ": couldn't replace original file " << argv[n]
				  << " with temporary file " << temporaryFileName << std::endl;
			std::perror(self.c_str());
			std::exit(4);
		}
	}

	return 0;
}
