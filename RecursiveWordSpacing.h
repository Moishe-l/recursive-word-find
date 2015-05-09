#include <iostream> //We include this so that our program can input from and output to the user
#include <fstream> //We include this so that our program can read from the two Dictionaries
#include <string> //We include this so that our program can work with strings as we are dealing with them a lot here
using namespace std;
#ifndef RECURSIVEWORDSPACING //We set a preprocessor command so that this header file is only read once, ensuring we don't encounter any errors by the multiple references to here
#define RECURSIVEWORDSPACING //In the following line, we enumerate all the possible locations a word can be put in depending on which dictionary it was taken from and it's first letter
enum LETTER { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP, sQ, sR, sS, sT, sU, sV, sW, sX, sY, sZ, r };
class WordContainer { //This class is aggregated as an array of 53 instances in class WordSpacing and is managed using the enumeration LETTER and a method of Class WordSpacing getLetter
public: //Here are the public, directly accessible parts of class WordContainer
	WordContainer (); //The constructor
	~WordContainer (); //The destructor

	string getWordAt (int position); //This method returns the word at the selected position in the array of data that pointer my_Words points to in this instance of class WordContainer
	int my_Size; //This stores the size of the array of words in this instance of class WordContainer
	string * my_Words; //This is the pointer that points to the array of words in this instance of class WordContainer
	int searchTimes; //This stores the number of times necessary, in most cases, to find a word in this instance of class WordContainer, assuming the words stored are alphabetically sorted
};
class WordSpacing { //This class loads two dictionaries in its constructor and uses them to find a combination of words in an inputed string
public: //Here are the public, directly accessible parts of class WordSpacing
	WordSpacing (); //The constructor, which loads the main Dictionary and organizes it and then loads a second custom Dictionary
	~WordSpacing (); //The destructor

	void provideLine (string Line); //This method inputs a line and saves it in a private data member called my_Line
	string getLine (); //This method returns the line stored in private data member my_Line
	void SpaceLine (); //This method turns the string in private data member my_Line into words if possible
	bool IsDictionary (); //This method returns private data member isDictionary, which stores whether the main Dictionary exists or not
	bool IsCustomDictionary(); //This method returns private data member isCustomDictionary, which stores whether the custom Dictionary exists or not
private: //Here are the private, indirrectly accessible of class WordSpacing
	string my_Line; //This stores the string inputed by the user and is changed upon the call of method SpaceLine either to the words found or to empty
	string * my_LoadedWord; //This stores the main Dictionary in one big array
	WordContainer wordsOfLetter [53]; //This stores the main and custom Dictionaries in 53 instances of class WordContainer
	int LoadedWordCount; //This stores the total amount of words that private data pointer member my_LoadedWord points to
	bool isDictionary; //This  private data member stores whether or not the main Dictionary exists or not
	bool isCustomDictionary; //This  private data member stores whether or not the custom Dictionary exists or not
	string separateWordsIn (string Line); //This method is called by public method SpaceLine and recursively calls itself in attempt to find a combination of words with string my_Line

	bool isWordByLetter (string Word, LETTER letter); //This method is used by method seperateWordsIn and returns whether a string is a word or not based off the words stored in private data array member wordsOfLetter

	void loadDictionary (string fileName, bool alphabetize); //This method is used by the constructor and loads the main Dictionary, organizes it, and then loads the custom Dictionary

	void makeLowerCase (string &Line); //This method takes a string and makes all the characters lowercase in it
	void makeUpperCase(string &Line); //This method takes a string and makes all the characters uppercase in it

	void organizeWords (); //This method is used by method LoadDictionary and organizes the private data member my_LoadedWord into 1 of 53 spots in array of private data member wordsOfLetter. It then loads the custom Dictionary using method LoadDictionary
	LETTER getLetter (char Word); //This method returns the spot in array of private data member wordsOfLetter that the selected word should go in. It is used by inputing the first letter of the word

	string getBefore(string word1, string word2); //This method returns which word comes first alphabetically
	void orderAlphabetically(WordContainer &list); //This method is intended for a small array of words and organizes the array alphabetically using method getBefore. It is used to alphabetically organize the custom Dictionary, which is assumed to be small
	void setSearchTime(int letter); //This method computes and stores the number of times, in most cases, necessary to find a word in the selected position in array of private data member wordsOfLetter. It is stored in public data member of class WordContainer searchTimes
};
class DictionaryNotFound : public logic_error{ public:DictionaryNotFound(string dictionaryName); }; //This class inherits from class logic_error and is thrown when the selected Dictionary cannot be found
#endif //The end of the preprocessor command that says only read this file once
