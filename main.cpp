#include "Container.h"

#include <cassert>

int main(int argc, char *argv[]) {

	// first guard, correct arg count
	if (argc != 4) {
		std::cerr << "Invalid arguments." << std::endl;
		exit(1);
	}

	// constant vars from the argument
	const std::string json = argv[1];
	const std::string input = argv[2];
	const std::string output = argv[3];

	std::ifstream ifile(input);
	std::ofstream ofile(output);

	if (!ofile.is_open()) {
		std::cerr << "Error opening output file." << std::endl;
		return 1;
	}

	Container comments;
	comments.parseJSON(json);
	std::stringstream result;

	std::string line;
	while(getline(ifile, line))
		comments.executeCommand(result, line);

	ofile << result.str();

	ofile.close();
	ifile.close();
}