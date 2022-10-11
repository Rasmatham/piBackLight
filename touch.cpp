#include <fstream>
#include <regex>
#include <unistd.h>
#include <iostream>
#include <math.h>
::std::ifstream touchEvent("/dev/input/event8", ::std::ios::binary | std::ios::in);

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
			::std::cout << "========================================" << ::std::endl;
			i = 0;
		}
	}
}
/*
0	
1	
2	
3	
4	
5	
6	
7	
8	
9	
10	
11	
12	
13	
14	
15	
16	type
17	
18	code
19	start_value
20	
21	
22	
23	end_value


01100101
10000001
01000101
01100011

01100101100000010100010101100011
*/