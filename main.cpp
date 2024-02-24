#include "zParser.h"

int main() {

	const char *ARR[] = {"[hello]", "[this]", "[is]", "[a]", "[config]", "[parser]"};
	const char *FILENAME = "config.txt";

	zParser T;

	std::cout << "\n[+] Reading Config File:\n" << std::endl;

	for (int i = 0; i < 6; i++) {
		std::cout << ARR[i] << " = " << T.ParseFile(FILENAME, ARR[i]) << std::endl;
	}

	std::cout << "\n[+] Completed\n" << std::endl;


	return 0;
}