#include <fstream>
#include <unistd.h>
::std::fstream blPower("/sys/devices/platform/soc/fe205000.i2c/i2c-22/i2c-10/10-0045/backlight/10-0045/bl_power");
void swapState() {
	::std::string currentState;
	while (getline (blPower, currentState)) {
		blPower << abs(stoi(currentState)-1);
	}
};
void blt(::std::string state = ""){
	if(getuid()) {
		return;
	}
	if(state.length()){
		if(state == "on"){
			blPower << 0;
		} else if (state == "off") {
			blPower << 1;
		} else {
			swapState();
		}
	} else {
		swapState();
	}
	return;
}
int main(int argc, char* argv[]){
	blt(argc > 1 ? argv[1] : "");
	return 0;
}