#include <fstream>
#include <regex>
#include <unistd.h>
#include <math.h>
#include <iostream>
::std::ifstream touchEvent("/dev/input/by-path/platform-fe205000.i2c-event", ::std::ios::binary | std::ios::in);
::std::fstream brightness;
::std::ifstream trueBrightness;

class BinaryByte {
	public:
		bool digits[8];
		BinaryByte (int num) {
			for (int i = 7; i >= 0; i--) {
				int x = ::std::pow(2, i);
				digits[i] = (num >= x);
				num = num >= x ? num - x : num ;
			}
		}
		/*int toDec () {
			int x = 0;
			for (int i = 7; i >= 0; i--){
				x += ::std::pow(2, i) * digits[i];
			}
			return x;
		}
		int toBinaryInt () {
			int x = 0;
			for (int i = 7; i >= 0; i--){
				x += ::std::pow(10, i) * digits[i];
			}
			return x;
		}*/
		::std::string toStr () {
			::std::string x = "";
			for (int i = 7; i >= 0; i--){
				x.append(::std::to_string(digits[i]));
			}
			return x;
		}

};

void fade_in(int num) {
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
void fade_out(int num) {
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

void fade(int num) {
	trueBrightness.open("/sys/devices/platform/soc/fe205000.i2c/i2c-22/i2c-10/10-0045/backlight/10-0045/actual_brightness");
	brightness.open("/sys/devices/platform/soc/fe205000.i2c/i2c-22/i2c-10/10-0045/backlight/10-0045/brightness");
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
	trueBrightness.close();
	brightness.close();
}

int main () {
	fade(10);
	fade(255);
	fade(10);
	fade(255);
	fade(10);
	fade(255);
	fade(10);
	int i;
	bool waiting = false;
	double prevTimestamp = 0;
	::std::string bytes[24];
	while(true) {
		char x;
		touchEvent.get(x);
		BinaryByte num((int(x)));
		bytes[i] = num.toStr();
		i++;
		if(i >= 24) {
			long seconds = stol(bytes[7] + bytes[6] + bytes[5] + bytes[4] + bytes[3] + bytes[2] + bytes[1] + bytes[0], nullptr, 2);
			long microseconds = stol(bytes[15] + bytes[14] + bytes[13] + bytes[12] + bytes[11] + bytes[10] + bytes[9] + bytes[8], nullptr, 2);
			double timestamp = stod(::std::to_string(seconds) + "." + ::std::string(6 - ::std::to_string(microseconds).length(), '0') + ::std::to_string(microseconds));
			ushort type = stoul(bytes[17] + bytes[16], nullptr, 2);
			ushort code = stoul(bytes[19] + bytes[18], nullptr, 2);
			uint value = stoul(bytes[23] + bytes[22] + bytes[21] + bytes[20], nullptr, 2);

			switch (type) {
				case 1: //touchEvent
					switch (code) {
						case 330: //touchEvent
						switch (value) {
							case 1: //press
								int time = 30;
								if ((timestamp - prevTimestamp) > time) {
									fade(255);
									sleep(time);
									fade(10);
								}
								prevTimestamp = timestamp;
								break;
						}
						break;
					}
				break;
			}
			i = 0;
		}
	}
}