#include <fstream>
#include <regex>
#include <unistd.h>
#include <iostream>
#include <math.h>
::std::ifstream touchEvent("/dev/input/event1", ::std::ios::binary | std::ios::in);

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
		int toDec () {
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
		}
		::std::string toStr () {
			::std::string x = "";
			for (int i = 7; i >= 0; i--){
				x.append(::std::to_string(digits[i]));
			}
			return x;
		}

};

int main () {
	int i;
	::std::string bytes[24];
	while(true) {
		char x;
		touchEvent.get(x);
		BinaryByte num((int(x)));
		bytes[i] = num.toStr();
		i++;
		if(i >= 24) {
			for (auto byte : bytes) ::std::cout << byte << ::std::endl;
			long seconds = stol(bytes[7] + bytes[6] + bytes[5] + bytes[4] + bytes[3] + bytes[2] + bytes[1] + bytes[0], nullptr, 2);
			long microseconds;
			ushort type;
			ushort code;
			uint value;
			::std::cout << bytes[3] + bytes[2] + bytes[1] + bytes[0] << ::std::endl;
			::std::cout << "unix timestamp: " << seconds << ::std::endl;
			::std::cout << "====================" << ::std::endl;
			i = 0;
		}
	}
}
/*
0	long timestamp_start
1	|
2	|
3	long timestamp_end
4	? (likely also timestamp)
5	? (likely also timestamp)
6	? (likely also timestamp)
7	? (likely also timestamp)
8	long uSecTimestamp_start
9	|
10	|
11	long uSecTimestamp_end
12	? (likely also timestamp)
13	? (likely also timestamp)
14	? (likely also timestamp)
15	? (likely also timestamp)
16	type_end
17	type_tart
18	code_end
19	code_start
20	value_end
21	|
22	|
23	value_start


01100101
10000001
01000101
01100011

01100101100000010100010101100011
*/