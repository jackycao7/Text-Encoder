#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <sstream>
#include <locale>
#include <unordered_map>
#include <cctype>
#include <stdio.h>
#include <codecvt>

using namespace std;

unordered_map<char, string> utf8ToMorse {
	{'A', ".-"},{'B', "-..."},{'C', "-.-."},{'D', "-.."},{'E', "."},{'F', "..-."},{'G', "--."},{'H', "...."},{'I', ".."},
	{'J', ".---"},{'K', "-.-"},{'L', ".-.."},{'M', "--"},{'N', "-."},{'O', "---"},{'P', ".--."},{'Q', "--.-"},
	{'R', ".-."},{'S', "..."},{'T', "-"},{'U', "..--"},{'V', "...-"},{'W', ".--"},{'X', "-..-"},{'Y', "-.--"},{'Z', "--.."},
	{'0', "-----"},{'1', ".----"},{'2', "..---"},{'3', "...--"},{'4', "....-"},{'5', "....."},
	{'6', "-...."},{'7', "--..."},{'8', "---.."},{'9', "----."},{',', "--..--"},{'=', "-..-"},{'.',".-.-.-"},{'?',"..--.."}
};

unordered_map<string, char> morseToUTF8 {
	{".-", 'A'},{"-...", 'B'},{"-.-.", 'C'},{"-..", 'D'},{".", 'E'},{"..-.", 'F'},{"--.",'G'},{"....",'H'},
	{"..",'I'},{".---",'J'},{"-.-",'K'},{".-..",'L'},{"--",'M'},{"-.",'N'},{"---",'O'},{".--.",'P'},
	{"--.-",'Q'},{".-.",'R'},{"...",'S'},{"-",'T'},{"..--",'U'},{"...-",'V'},{".--",'W'},{"-..-",'X'},
	{"-.--",'Y'},{"--..",'Z'},{"-----",'0'},{".----",'1'},{"..---",'2'},{"...--",'3'},{"....-",'4'},{".....",'5'},
	{"-....",'6'},{"--...",'7'},{"---..",'8'},{"----.",'9'},{"--..--",','},{"-..-",'='},{".-.-.-", '.'},{"..--..", '?'}
};

unordered_map<string, string> utf8ToBaudot {
	{"A","00011"},{"B","11001"},{"C","01110"},{"D","01001"},{"E","00001"},{"F","01101"},{"G","11010"},{"H","10100"},
	{"I","00110"},{"J","01011"},{"K","01111"},{"L","10010"},{"M","11100"},{"N","01100"},{"O","11000"},{"P","10110"},
	{"Q","10111"},{"R","01010"},{"S","00101"},{"T","10000"},{"U","00111"},{"V","11110"},{"W","10011"},
	{"X","11101"},{"Y","10101"},{"Z","10001"},
};

unordered_map<string, string> baudotToUTF8 {
	{"00011","A"},{"11001","B"},{"01110","C"},{"01001","D"},{"00001","E"},{"01101","F"},{"11010","G"},{"10100","H"},
	{"00110","I"},{"01011","J"},{"01111","K"},{"10010","L"},{"11100","M"},{"01100","N"},{"11000","O"},{"10110","P"},
	{"10111","Q"},{"01010","R"},{"00101","S"},{"10000","T"},{"00111","U"},{"11110","V"},{"10011","W"},
	{"11101","X"},{"10101","Y"},{"10001","Z"},
};

unordered_map<string,string> utf8FiguresToBaudot {
	{"-","00011"},{"?","11001"},{":","01110"},{"3","00001"},{"%","01101"},{"@","11010"},{"£","10100"},
	{"8","00110"},{"J","01011"},{"(","01111"},{")","10010"},{".","11100"},{",","01100"},{"9","11000"},{"0","10110"},
	{"1","10111"},{"4","01010"},{"'","00101"},{"5","10000"},{"7","00111"},{"=","11110"},{"2","10011"},
	{"/","11101"},{"6","10101"},{"+","10001"},
};

unordered_map<string,string> baudotToUTF8Figures {
	{"00011","-"},{"11001","?"},{"01110",":"},{"00001","3"},{"01101","%"},{"11010","@"},{"10100","£"},
	{"00110","8"},{"01011","J"},{"01111","("},{"10010",")"},{"11100","."},{"01100",","},{"11000","9"},{"10110","0"},
	{"10111","1"},{"01010","4"},{"00101","'"},{"10000","5"},{"00111","7"},{"11110","="},{"10011","2"},
	{"11101","/"},{"10101","6"},{"10001","+"},
};

bool convertToUTF32(char* fileName, string newFileName) {
	string word;
	string line;
	ofstream mFile(newFileName);
	ifstream mInputFile(fileName);

	while(getline(mInputFile, line)) {
		istringstream lineStream(line);
		while(lineStream >> word) {
			bitset<8> one("10000000");
			bitset<8> two("11000000");
			bitset<8> three("11100000");
			bitset<8> four("11110000");
			for(int i = 0;i< word.size(); i++) {
				bitset<8> firstByte(word.c_str()[i]);
				bitset<8> w(word.c_str()[i]);
				if((w&=four) == four) {
					cout << "four\n";
					bitset<8> firstByte(word.c_str()[i]);
					bitset<8> secondByte(word.c_str()[i+1]);
					bitset<8> thirdByte(word.c_str()[i+2]);
					bitset<8> fourthByte(word.c_str()[i+3]);
					string cat = firstByte.to_string() + secondByte.to_string() + thirdByte.to_string() + fourthByte.to_string();
					bitset<32> x(cat);
					mFile << x;
					i = i + 3;
				}
				else if((w &=three) == three) {
					cout << "three\n";
					bitset<8> firstByte(word.c_str()[i]);
					bitset<8> secondByte(word.c_str()[i+1]);
					bitset<8> thirdByte(word.c_str()[i+2]);
					string cat = firstByte.to_string() + secondByte.to_string() + thirdByte.to_string();
					bitset<32> x(cat);
					mFile << x;
					i = i + 2;
				}
				else if ((w &=two) == two) {
					bitset<8> firstByte(word.c_str()[i]);
					bitset<8> secondByte(word.c_str()[i+1]);
					string cat = firstByte.to_string() + secondByte.to_string();
					bitset<32> x(cat);
					mFile << x;
					i++;
				}
				else {
					bitset<32> x(word.c_str()[i]);
					mFile << x;
				}
			}
			mFile << "00000000000000000000000000100000";
		}
		mFile << "00000000000000000000000000001010";
	}
	return true;
	
}

bool convertUTF32ToUTF8(char* fileName, string newFileName) {
	string word;
	string line;
	ofstream mFile(newFileName);
	ifstream mInputFile(fileName);
	getline(mInputFile,line);
	int stringOffset = 0;
	int fileLength = line.size();

	wstring_convert<codecvt_utf8<char32_t>, char32_t> cnv;

	while(stringOffset < line.size()) {
		string mCharacter = line.substr(stringOffset,32);
		bitset<32> currChar(mCharacter);
		string ans = "";
		int value;
		if(mCharacter.substr(0,5) == "11110") {
			ans+= mCharacter.substr(5,3) + mCharacter.substr(10,6) + mCharacter.substr(18,6) + mCharacter.substr(26,6);
			bitset<21> bitToHex(ans);
			stringstream str;
			str << hex << uppercase<< bitToHex.to_ulong() << endl;
			str >> hex >> value;
		}
		else if (mCharacter.substr(8,4) == "1110") {
			ans+= mCharacter.substr(12,4) + mCharacter.substr(18,6) + mCharacter.substr(26,6);
			bitset<16> bitToHex(ans);
			stringstream str;
			str << hex << uppercase<< bitToHex.to_ulong() << endl;
			str >> hex >> value;
		}
		else if (mCharacter.substr(16,3) == "110") {
			ans+= mCharacter.substr(19,5) + mCharacter.substr(26,6);
			bitset<11> bitToHex(ans);

			stringstream str;
			str << hex << uppercase<< bitToHex.to_ulong() << endl;
			str >> hex >> value;
		}
		else {
			ans += mCharacter.substr(25,7);
			ans+= mCharacter.substr(19,5) + mCharacter.substr(26,6);
			bitset<7> bitToHex(ans);

			stringstream str;
			str << hex << uppercase<< bitToHex.to_ulong() << endl;
			str >> hex >> value;
		}
		string u8 = cnv.to_bytes(value);
    	mFile << u8.c_str();
		stringOffset+= 32;
	}
	return true;

}

bool convertToBaudot(char* fileName, string newFileName) {
	string word;
	string line;
	ofstream mFile(newFileName);
	ifstream mInputFile(fileName);
	int state = 2;
	while(getline(mInputFile, line)) {
		istringstream lineStream(line);
		// 0 = letters, 1 = figures
		while(lineStream >> word) {
			char x;
			for(int i = 0;i<word.size(); i++) {
				x = word[i];
				char upperChar = toupper(x);
				string s(1,upperChar);
				if(utf8ToBaudot.find(s) != utf8ToBaudot.end()) {
					// letter
					if(state != 0) {
						state = 0;
						mFile << "00011111";
						//cout << "stage change\n";
					}
					//cout <<  upperChar << ": 000" << utf8ToBaudot[s] << endl;
					mFile << "000" << utf8ToBaudot[s]; 
				}
				else if(utf8FiguresToBaudot.find(s) != utf8FiguresToBaudot.end()) {
					// figure
					if(state != 1) {
						state = 1;
						mFile << "00011011";
						//cout << "stage change\n";
					}					
					//cout << upperChar << ": 000" << utf8FiguresToBaudot[s] << endl;
					mFile << "000" << utf8FiguresToBaudot[s]; 
				}
				else {
					cout << "Error: Cannot convert '" << upperChar << "' to baudot\n";
					remove(newFileName.c_str());
					return false;
				}

				if(i+1 >= word.size()) {
					mFile << "00000100";
					//cout << "new word\n";
				}
			}
		}
		mFile << "00001000";
		//cout << "new line\n";
	}
	return true;
}

bool convertBaudotToUTF8(char* fileName, string newFileName) {
	string word;
	string line;
	ofstream mFile(newFileName);
	ifstream mInputFile(fileName);
	int stringOffset = 3;
	int state = 0;
	getline(mInputFile, line);
	while(stringOffset < line.size()) {
		string mCharacter = line.substr(stringOffset,5);
		bitset<5> currChar(mCharacter);

		if(mCharacter == "11111") {
			state = 0;
		}
		else if(mCharacter == "11011") {
			state = 1;
		}
		else if(mCharacter == "00100") {
			mFile << " ";
			//cout << "\n";
		}
		else if (mCharacter == "01000") {
			mFile << endl;
			//cout << "new line\n";
		}
		else {
			switch(state) {
				case 0:
					if(baudotToUTF8.find(mCharacter) != baudotToUTF8.end()) {
						mFile << baudotToUTF8[mCharacter];
						//cout << mCharacter << "= " << baudotToUTF8[mCharacter] << endl;
					}
					break;
				case 1:
					if(baudotToUTF8Figures.find(mCharacter) != baudotToUTF8Figures.end()) {
						mFile <<baudotToUTF8Figures[mCharacter];
						//cout << mCharacter << "= " << baudotToUTF8Figures[mCharacter] << endl;
					}
				default:
					break;
			}
		}
		stringOffset+=8;
	}
	return true;

}

bool convertToMorse(char* fileName, string newFileName) {
	string word;
	string line;
	ofstream mFile(newFileName);
	ifstream mInputFile(fileName);
	while(getline(mInputFile, line)) {
		istringstream lineStream(line);
		// letters sperated by 1 spaces
		// words seperated by 2 spaces
		while(lineStream >> word) {
			char x;
			for(int i = 0;i<word.size(); i++) {
				x = word[i];
				char upperChar = toupper(x);
				if(utf8ToMorse.find(upperChar) != utf8ToMorse.end()) {
					//cout << utf8ToMorse[upperChar] << endl;
					mFile << utf8ToMorse[upperChar];
					if(i+1 < word.size()) {
						mFile << " ";
					}
					// last char in word is a period = sentence
					else if(x == '.' or x == '?') {
						mFile << "   ";
					}
				}
				else {
					cout << "Error: Cannot convert '" << x << "' to morse." << endl;
					remove(newFileName.c_str());
					return false;
				}
			}
			if(x != '.' or x!= '?') {
				mFile << "  ";
			}
		}
		mFile << endl;
	}
	return true;
}

bool convertMorseToUTF8(char* fileName, string newFileName) {
	string word;
	string line;
	string sentence;
	ofstream mFile(newFileName);
	ifstream mInputFile(fileName);
	// letters seperated by 1 space
	// words two spaces
	// sentences three
	while(getline(mInputFile, line)) {
		int lineSize = line.size();
		string currCharInMorse = "";
		int sizeOfSpaces = 0;
		for(int index = 0; index < lineSize; index++) {
			if(line[index] != ' ') {
				string mChar(1,line.at(index));
				//cout << "mychar: " << mChar << endl;
				switch(sizeOfSpaces) {
					// building morse string
					case 0:
						sizeOfSpaces = 0;
						currCharInMorse+= mChar;
						//cout << "curr: " << currCharInMorse << endl;
						break;
					case 1:
						sizeOfSpaces = 0;
						currCharInMorse+= mChar;
						//cout << "curr: " << currCharInMorse << endl;
						break;
					// current letter is for the next word.
					case 2:
						sizeOfSpaces = 0;
						//cout << "start of new word \n";
						mFile << " ";
						currCharInMorse+= mChar;
						//cout <<currCharInMorse << endl;
						break;
					case 3:
						sizeOfSpaces = 0;
						//cout << "start of new sentence\n";
						mFile << " ";
						currCharInMorse+= mChar;
						//cout <<currCharInMorse << endl;
						break;
					default:
						sizeOfSpaces = 0;
						//cout << "start of new sentence\n";
						mFile << " ";
						currCharInMorse+= mChar;
						//cout <<currCharInMorse << endl;
						break;
				}
			}
			else {
				if(sizeOfSpaces == 0) {
					if(morseToUTF8.find(currCharInMorse) != morseToUTF8.end()) {
						mFile << morseToUTF8[currCharInMorse];
						//cout << morseToUTF8[currCharInMorse] << endl;
						currCharInMorse = "";
					}
					else {
						cout << "Error: cannot convert " << currCharInMorse << " to UTF8\n";
						remove(newFileName.c_str());
						return false;
					}
				}
				sizeOfSpaces++;
				//cout << "end" << endl;
			}
		}
		mFile << endl;
	}
	return true;
}

bool convertToUTF8(int inputFormat, int outputFormat, char* fileName) {
	char* temp = (char*)"temp";
	// switch other formats to utf8 first
	if(outputFormat != 1) {
		switch(inputFormat) {
			case 2:
				convertUTF32ToUTF8(fileName, temp);
				break;
			case 3:
				convertBaudotToUTF8(fileName, temp);
				break;
			case 4:
				convertMorseToUTF8(fileName, temp);
				break;
			default:
				break;
		}
	}

	string newFileName;
	cout << "Enter name for the encoded file (not including .txt): ";
	getline(cin, newFileName);
	newFileName+=".txt";
	switch(outputFormat) {
		// utf8
		case 1:
			if(inputFormat == 2) {
				if(convertUTF32ToUTF8(fileName, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
				}
				break;
			}
			if(inputFormat == 3) {
				if(convertBaudotToUTF8(fileName, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
				}
				break;
			}
			else if(inputFormat == 4) {
				if(convertMorseToUTF8(fileName, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
				}
				break;
			}
		// utf32
		case 2:
			if(inputFormat == 1) {
				if(convertToUTF32(fileName, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
				}
			}
			else {
				if(convertToUTF32(temp, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
					remove(temp);
				}
			}
			break;
		// baudot
		case 3:
			if(inputFormat == 1) {
				if(convertToBaudot(fileName, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
				}
			}
			else {
				if(convertToBaudot(temp, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
					remove(temp);
				}
			}
			break;
		// morse
		case 4:
			if(inputFormat == 1) {
				if(convertToMorse(fileName, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
				}
			}
			else {
				if(convertToMorse(temp, newFileName)) {
					cout << "Encoded text recorded in " << newFileName << '\n';
					remove(temp);
				}
			}
			break;
		default:
			break;
	}
	return true;
}

int main(int argc, char* argv[]) {
	if(argc != 4) {
		cout << "Usage: ./translator <input format id> <output format id> <input file name>\n";
		cout << "Format options: UTF-8 (1), UTF-32 (2), Baudot (3), Morse (4) \n";
		return 0;
	}
	int mInput = atoi(argv[1]);
	int mOutput = atoi(argv[2]);
	char* mFileName = argv[3];
	cout << "Input format: " << mInput << " Output Format: " << mOutput << "\n";

	if(mInput == mOutput) {
		cout << "File is already in the correct format\n";
		return 0;
	}
	else if (mInput < 1 or mInput > 4 or mOutput < 1 or mOutput > 4) {
			cout << "Unrecognized file format\n";
			cout << "Format options: UTF-8 (1), UTF-32 (2), Baudot (3), Morse (4)\n";
			return 0;
	}

	switch(mInput) {
		case 1:
			cout << "Input file: UTF-8\n";
			convertToUTF8(mInput, mOutput, mFileName);
			break;
		case 2:
			cout << "Input file: UTF-32\n";
			convertToUTF8(mInput, mOutput, mFileName);
			break;
		case 3:
			cout << "Input file: Baudot\n";
			convertToUTF8(mInput, mOutput, mFileName);
			break;
		case 4:
			cout << "Input file: Morse\n";
			convertToUTF8(mInput, mOutput, mFileName);
			break;
		default:
			cout << "Unrecognized file format\n";
			cout << "Format options: UTF-8 (1), UTF-32 (2), Baudot (3), Morse (4)\n";
			break;
	}
}