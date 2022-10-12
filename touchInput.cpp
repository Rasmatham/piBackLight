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
	int cursorx;
	int cursory;
	while(true) {
		char x;
		touchEvent.get(x);
		BinaryByte num((int(x)));
		bytes[i] = num.toStr();
		i++;
		if(i >= 24) {
			long seconds = stol(bytes[7] + bytes[6] + bytes[5] + bytes[4] + bytes[3] + bytes[2] + bytes[1] + bytes[0], nullptr, 2);
			long microseconds = stol(bytes[15] + bytes[14] + bytes[13] + bytes[12] + bytes[11] + bytes[10] + bytes[9] + bytes[8], nullptr, 2);
			ushort type = stoul(bytes[17] + bytes[16], nullptr, 2);
			ushort code = stoul(bytes[19] + bytes[18], nullptr, 2);
			uint value = stoul(bytes[23] + bytes[22] + bytes[21] + bytes[20], nullptr, 2);


			//::std::cout << "unix timestamp: " << seconds << ::std::endl;
			//::std::cout << "microseconds: " << microseconds << ::std::endl;
			
			switch (type)
			{
			case 0:
				::std::cout << "type: " << "aaa		" << code << "		" << value << ::std::endl;
				break;
			case 1:
				::std::cout << "type: " << "bbb		" << code << "		" << value << ::std::endl;
				break;
			case 3:
				::std::cout << "type: " << "ccc		" << code << "		" << value << ::std::endl;
				break;
			
			default:
				::std::cout << "type: " << type << code << ::std::endl;
			}

			//::std::cout << "code: " << code << ::std::endl;
			//::std::cout << "value: " << value << ::std::endl;
			//::std::cout << "====================" << ::std::endl;
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

/*
ras@pi:~/piBackLight $ touch
type: ccc				57				165			|	start read (maybe counter)
type: ccc				53				74			|	xpos_in
type: ccc				54				48			|	ypos_in
type: bbb				330				1			|	press
type: ccc				0				74			|	xpos_in
type: ccc				1				48			|	xpos_out
type: aaa				0				0			|	release
type: ccc				57				4294967295	|	end read
type: bbb				330				0			|	end read
type: aaa				0				0			|	end read
^C
ras@pi:~/piBackLight $ touch
type: ccc				57				166			|	start read (maybe counter)
type: ccc				53				734			|	xpos_in
type: ccc				54				344			|	xpos_out
type: bbb				330				1			|	press
type: ccc				0				734			|	xpos_out
type: ccc				1				344			|	ypos_out
type: aaa				0				0			|	release
type: ccc				57				4294967295	|	end read
type: bbb				330				0			|	end read
type: aaa				0				0			|	end read
^C
ras@pi:~/piBackLight $ 
*/