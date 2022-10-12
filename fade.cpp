#include <fstream>
#include <regex>
#include <unistd.h>
::std::fstream blPower("/sys/devices/platform/soc/fe205000.i2c/i2c-22/i2c-10/10-0045/backlight/10-0045/bl_power");
::std::fstream brightness("/sys/devices/platform/soc/fe205000.i2c/i2c-22/i2c-10/10-0045/backlight/10-0045/brightness");
::std::ifstream trueBrightness("/sys/devices/platform/soc/fe205000.i2c/i2c-22/i2c-10/10-0045/backlight/10-0045/actual_brightness");
void fade_in(int num = 255) {
	::std::string currentState;
	trueBrightness.seekg(0,::std::ios::beg);
	trueBrightness >> currentState;
	int currentStateNum = stoi(currentState);
	for(int i = currentStateNum; i <= num; i++) {
		brightness << i;
		brightness.flush();
		usleep(500);
	}
}
void fade_out(int num = 10) {
	::std::string currentState;
	trueBrightness.seekg(0,::std::ios::beg);
	trueBrightness >> currentState;
	int currentStateNum = stoi(currentState);
	for(int i = currentStateNum; i >= num; i--) {
		brightness << i;
		brightness.flush();
		usleep(500);
	}
}
void fade() {
	if(getuid()) {
		return;
	}
	::std::string test;
	trueBrightness >> test;
	if (stoi(test) > 10){
		fade_out();
	} else {
		fade_in();
	}
}
void fade(::std::string state) {
	if(getuid()) {
		return;
	}
	if (state == "in") {
		fade_in();
	} else if (state == "out") {
		fade_out();
	}
}
void fade(int num) {
	if(getuid()) {
		return;
	}
	int trueBrightnessNum;
	trueBrightness >> trueBrightnessNum;
	if (num > 255) num = 255;
	if (num < 10) num = 10;
	if (trueBrightnessNum < num) {
		fade_in(num);
	} else if (trueBrightnessNum > num) {
		fade_out(num);
	}
}
int main(int argc, char* argv[]){
	if(argc > 1) {
		if (argv[1] == ::std::string("in") || argv[1] == ::std::string("out")) {
			fade(argv[1]);
		} else {
			int trueNum = ::std::regex_match(argv[1], ::std::regex("[-|+]?[0-9]+")) ? stoi(::std::string(argv[1])) : 255;
			fade(trueNum);
		}
	} else {
		fade();
	}
	trueBrightness.close();
	brightness.close();
	blPower.close();
	return 0;
}