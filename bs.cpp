#include <fstream>
#include <regex>
#include <unistd.h>
::std::fstream brightness("/sys/class/backlight/10-0045/brightness");
void bs(::std::string num = "255"){
	if(getuid()) {
		return;
	}
	int trueNum = ::std::regex_match(num, ::std::regex("[(-|+)|][0-9]+")) ? stoi(num) : 255;
	if (trueNum > 255) trueNum = 255;
	if (trueNum < 11) trueNum = 11;
	brightness << trueNum;
}
int main(int argc, char* argv[]) {
	bs(argc > 1 ? argv[1] : "");
	return 0;
}