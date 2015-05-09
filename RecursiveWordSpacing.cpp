#include "RecursiveWordSpacing.h" //This includes the class WordSpacing, WordContainer, and DictionaryNotFound so that this file can access and define them
void provideLine (string Line); //Here we declare all methods declared in "RecursiveWordSpacing.h"
string getLine ();
void SpaceLine ();
string separateWordsIn (string Line);
void loadDictionary (string fileName, bool alphabetize);
void makeLowerCase (string &Line);
void makeUpperCase (string &Line);
bool IsDictionary ();
bool IsCustomDictionary();
void organizeWords ();
bool isWordByLetter (string Word, LETTER letter);
LETTER getLetter (char Word);
string getBefore(string word1, string word2);
void orderAlphabetically(WordContainer &list);
void setSearchTime(int letter);
string getWordAt (int position);
int letterAmount[53]; //Here we store the number words there will be in here so that our pointer can point to an array with each position pointing an array of the necessary amount
WordContainer::WordContainer () :my_Size(0), my_Words(NULL) { //This constructor of class WordContainer intializes the data members of class WordContainer
}
WordContainer::~WordContainer () { //The destructor of class WordContainer
	//delete my_Words;
}
string WordContainer::getWordAt (int position) { //This method returns the word stored in selected position of array that pointer my_Words points to
	string value = "";
	if (position < my_Size && position >= 0) { //We only return a word that exists
		value = my_Words [position];
	}
	return value;
}
WordSpacing::WordSpacing () { //This constructor of class WordSpacing tries to load the main Dictionary, organize it, and then load the custom Dictionary
	try { //Put in a try block
		loadDictionary("Dictionary.dic", true);
	}
	catch (DictionaryNotFound error) //DictionaryNotFound error will be thrown if the dictionary isn't found
	{
		cout << error.what() << endl; //Here we print the error thrown that changes depending on which Dictionary isn't found
	}
}
void WordSpacing::provideLine (string Line) { //Here the user inputs a string that the class WordSpacing can try to find a combination of words from
	my_Line = Line;
}
string WordSpacing::getLine () { //Here the value stored in private data member my_Line is returned. It is contains the results of method SpaceLine after it is called
	return my_Line;
}
void WordSpacing::SpaceLine () { //This method calls private method separateWordsIn to try to find a combination of words from string stored in private data member my_Line
	if (isDictionary) { //We only attempt to find words from string when the main Dictionary exists
		my_Line = separateWordsIn (my_Line);
	} else { //Otherwise, we inform the user that the method SpaceLine cannot be used without the main Dictionary
		cout << "Error-Cannot do desired activity without Dictionary!" << endl;
	}
}
string WordSpacing::separateWordsIn (string Line) { //This method tries to separate the string inputted into it into words
	makeLowerCase (Line);
	string value = "";
	string tryAsWord = "";
	bool missingWords = false;
	for (int amount = 0; amount <= Line.size(); amount ++) { //This loop starts by selecting the whole string and then selects less and less till it finds a word
		tryAsWord = Line.substr(amount);
		if (!isWordByLetter(tryAsWord, getLetter(tryAsWord[0])) && !isWordByLetter(tryAsWord, r) && !isWordByLetter(tryAsWord, getLetter(toupper(tryAsWord[0])))) { //Not a word so move on
			value = "";
		} else { //Is a word so we try to find words with the rest of the string
			string restOfWord = "";
			for (int noTry = amount - 1; noTry >= 0; noTry --) { //We save the rest of the string into restOfWord to try to find words out of it
				restOfWord = Line [noTry] + restOfWord;
			}
			if (restOfWord != "") { //Here the string restOfWord isn't empty and such we try and find words out of it
				//cout << "Remainder:" << restOfWord << endl;
				value = separateWordsIn (restOfWord);
			} else { //Here we have found words with all the characters in the inputted string Line and such we are ready to return the last word found
				//cout << "Stopped at:" << tryAsWord << endl;
				value = tryAsWord;
				goto endOfSearch; //We skip to the end of the method to return the word found because it is a definite match
			}
		}
		if (value != "") { //Here we start adding the words we found together with a space in between only if a value has been returned from recursive call of method separateWordsIn isn't empty
			value += " " + tryAsWord;
			goto endOfSearch;  //We go to the end of this method and return the words we found together with a space
		}
	}
	endOfSearch:
	return value;

}
void WordSpacing::loadDictionary (string fileName, bool alphabetize) { //Here a Dictionary is loaded. It is organized into 52 parts of array wordsOfLetter or 53rd spot depending on bool value alphabetize
	LoadedWordCount = 0;
	my_LoadedWord = NULL;
	string line = "";
	string previousWord = "";
	ifstream in_stream;
	in_stream.open(fileName);
	if (in_stream.is_open()) { //Only proceed to load words into array if file actually exists and is open
		in_stream >> LoadedWordCount;
		my_LoadedWord = new string[++ LoadedWordCount];
		if (!alphabetize) { //If we are loading the custom Dictionary, we initialize the 53rd array of wordsOfLetter and give it the appropriate size
			wordsOfLetter [r].my_Words = NULL;
			wordsOfLetter [r].my_Words = new string [LoadedWordCount];
		}
		string prevline = "";
		for (int wordAt = 0; wordAt < LoadedWordCount; wordAt ++) { //This loop goes through the big array and stops when it finds no more words
			in_stream >> line;
			if (prevline == line)
			{
				goto stopLoading; //Stop loading words here
			}
			if (alphabetize) { //Here we find the number of the words in the main Dictionary in my_LoadedWord that will be stored in each of the 52 different spots in array wordsOfLetter
				my_LoadedWord [wordAt] = "";
				LETTER theLetter = getLetter (line[0]);
				my_LoadedWord [wordAt] = line;
				wordsOfLetter[theLetter].my_Size ++;
				letterAmount[theLetter] ++;
			} else { //Here we store the words in the custom Dictionary stored in my_LoadedWord into the 53rd spot in array wordsOfLetter
				makeLowerCase(line);
				wordsOfLetter [r].my_Words[letterAmount [r]] = line;
				//cout << wordsOfLetter [r].my_Words[letterAmount [r]] << endl;
				letterAmount [r] ++;
				wordsOfLetter[r].my_Size ++;
			}
			prevline = line; //We save the word just found to check it against the next word found to know to stop if the same word comes up again
		}
		stopLoading:
		in_stream.close();
		if (alphabetize) { //If we are loading the main Dictionary
			organizeWords(); //Here we store the words in the main Dictionary stored in my_LoadedWord into 1 of 52 different spots in array wordsOfLetter
			for (int i = 0; i < 53; i++) //We set the amount of times, in most cases, necessary to find a word in each of the 52 spots in array wordsOfLetter
			{
				setSearchTime(i);
			}
			isDictionary = true; //We store that the main Dictionary has loaded successfully
		} else {
			orderAlphabetically(wordsOfLetter[r]);
			setSearchTime(r); //We find and store the number of times, in most cases, necessary to find a word in the 53rd spot of array wordsOfLetter
			isCustomDictionary = true; //We store that the custom Dictionary has loaded successfully
		}
		cout << "'" + fileName + "' Loaded Successfully." << endl; //We let the user know that the selected Dictionary loaded successfully
	} else { //The Dictionary doesn't exist
		if (alphabetize) { //The main Dictionary doesn't exist
			isDictionary = false;
		} else { //The custom Dictionary doesn't exist
			isCustomDictionary = false;
		}
		throw DictionaryNotFound(fileName); //We throw ans error with the Dictionary name so that the user knows what the issue is
	}
}
bool WordSpacing::IsDictionary () { //This method returns whether the main Dictionary loaded correctly or not
	return isDictionary;
}
bool WordSpacing::IsCustomDictionary() { //This method returns whether the custom Dictionary loaded correctly or not
	return isCustomDictionary;
}
void WordSpacing::makeLowerCase (string &Line) { //This method takes a string and converts all the uppercase letters to lowerercase
	string lineTemp = "";
	for (int LetterFind = 0; LetterFind < Line.size(); LetterFind ++) { //Loop goes through each letter individually
		if (islower( Line [LetterFind])) { //If the letter is already lowercase, we don't need to change it
			lineTemp += Line [LetterFind];
		} else { //If the letter is uppercase, we make it lowercase
			lineTemp += tolower(Line [LetterFind]);
		}
	}
	Line = lineTemp; //The Line the user inputted is set to lowercase version
}
void WordSpacing::makeUpperCase (string &Line) { //This method takes a string and converts all the lowercase letters to uppercase
	string lineTemp = "";
	for (int LetterFind = 0; LetterFind < Line.size(); LetterFind ++) { //Loop goes through each letter individually
		if (isupper(Line [LetterFind])) { //If the letter is already uppercase, we don't need to change it
			lineTemp += Line [LetterFind];
		}
		else { //If the letter is lowercase, we make it uppercase
			lineTemp += toupper(Line [LetterFind]);
		}
	}
	Line = lineTemp; //The Line the user inputted is set to uppercase version
}
WordSpacing::~WordSpacing () { //The destructor of class WordSpacing
	//delete my_LoadedWord;
}
void WordSpacing::organizeWords () { //This method puts each word in my_LoadedWord in the correct spot in the array wordsOfLetter
	for (int j = 0; j <= 53; j ++) { //Each of the spots of the array wordsOfLetter are initialized and given the size that was determined in method LoadDictionary
		letterAmount [j] = 0;
		wordsOfLetter [j].my_Words = NULL;
		wordsOfLetter [j].my_Words = new string [wordsOfLetter [j].my_Size];
	}
	for (int i = 0; i <= LoadedWordCount; i ++) { //Each of the words are now sorted into the array that matches the first letter of the word
		if (my_LoadedWord [i] != "") {
			//cout << "Organizing at position:" << i << "Word:" << my_LoadedWord [i] << endl;
			LETTER letterAt = r;
			letterAt = getLetter(my_LoadedWord [i][0]);
			makeLowerCase(my_LoadedWord[i]);
			wordsOfLetter [letterAt].my_Words[letterAmount [letterAt]] = my_LoadedWord [i];
			letterAmount [letterAt] ++;
		}
	}
	//cout << "Organization Completed" << endl;
	try { //We try to load the custom Dictionary
		loadDictionary("myDictionary.dic", false);
	}
	catch (DictionaryNotFound error) //If the custom Dictionary doesn't exist, we print out the error thrown to us
	{
		cout << error.what() << endl;
	}
}
bool WordSpacing::isWordByLetter (string Word, LETTER letter) { //This method checks a word to see if it exists in the main and custom Dictionary
	bool value = false;
	makeLowerCase (Word);
	//if (Word.size() > 2) {
		/*for (int wordCount = 0; wordCount < wordsOfLetter[letter].my_Size; wordCount ++) {
			if ((wordsOfLetter[letter].getWordAt(wordCount)).size() > 2) {
				if (wordsOfLetter[letter].getWordAt(wordCount) == Word) {
					//cout << "Exists in Regular Dictionary:" << Word << endl;
					value = true;
					goto endOfSearch;
				}
			}
		} */
		int searchAt = 0;
		int amount = 0;
		amount = wordsOfLetter[letter].my_Size;
		for (int i = 0; i <= wordsOfLetter[letter].searchTimes + 5; i++) //We go the number of times necessary to find the word (We try a few more times for a margin of error)
		{
			amount /= 2; //each time we divide in half and choose which way to go, up or down, depending if are above or below the word
			if (amount == 0) //Here we make sure that the amount remains above zero, so that we are always moving at least one up or down until the end of the for loop
			{
				amount = 1;
			}
			string wordHave = "";
			wordHave = wordsOfLetter[letter].getWordAt(searchAt);
			if (((letter == r && Word.size() >= 1) || (letter != r && Word.size() > 2)) && wordHave == Word) //Word exist if it is in custom Dictionary or in the main Dictionary with at least three letters
			{
					value = true;
					goto endOfSearch;
			}
			if (getBefore(wordHave, Word) == wordHave) //If we are before the word we are looking for
			{
				searchAt += amount;
			}
			else //If we are after the word we are looking for
			{
				searchAt -= amount;
			}
		}
	//}
	/*if  (Word.size() >= 1){
		letter = r;
		for (int wordCount = 0; wordCount < wordsOfLetter[letter].my_Size; wordCount ++) {
			if (wordsOfLetter[letter].getWordAt(wordCount) == Word) {
				//cout << "Exists in Custom Dictionary:" << Word << endl;
				value = true;
				goto endOfSearch;
			}
		}
	}*/
	endOfSearch:
	return value;
}
LETTER WordSpacing::getLetter (char Word) { //This method returns the spot in the array of wordsOfLetter that the word, with the give first letter, belongs in
	//Word = toupper (Word);
	LETTER value = r;
	switch(Word)
	{
		case 'A':
			value = A;
		break;
		case 'B':
			value = B;
		break;
		case 'C':
			value = C;
		break;
		case 'D':
			value = D;
		break;
		case 'E':
			value = E;
		break;
		case 'F':
			value = F;
		break;
		case 'G':
			value = G;
		break;
		case 'H':
			value = H;
		break;
		case 'I':
			value = I;
		break;
		case 'J':
			value = J;
		break;
		case 'K':
			value = K;
		break;
		case 'L':
			value = L;
		break;
		case 'M':
			value = M;
		break;
		case 'N':
			value = N;
		break;
		case 'O':
			value = O;
		break;
		case 'P':
			value = P;
		break;
		case 'Q':
			value = Q;
		break;
		case 'R':
			value = R;
		break;
		case 'S':
			value = S;
		break;
		case 'T':
			value = T;
		break;
		case 'U':
			value = U;
		break;
		case 'V':
			value = V;
		break;
		case 'W':
			value = W;
		break;
		case 'X':
			value = X;
		break;
		case 'Y':
			value = Y;
		break;
		case 'Z':
			value = Z;
		break;
		case 'a':
			value = sA;
			break;
		case 'b':
			value = sB;
			break;
		case 'c':
			value = sC;
			break;
		case 'd':
			value = sD;
			break;
		case 'e':
			value = sE;
			break;
		case 'f':
			value = sF;
			break;
		case 'g':
			value = sG;
			break;
		case 'h':
			value = sH;
			break;
		case 'i':
			value = sI;
			break;
		case 'j':
			value = sJ;
			break;
		case 'k':
			value = sK;
			break;
		case 'l':
			value = sL;
			break;
		case 'm':
			value = sM;
			break;
		case 'n':
			value = sN;
			break;
		case 'o':
			value = sO;
			break;
		case 'p':
			value = sP;
			break;
		case 'q':
			value = sQ;
			break;
		case 'r':
			value = sR;
			break;
		case 's':
			value = sS;
			break;
		case 't':
			value = sT;
			break;
		case 'u':
			value = sU;
			break;
		case 'v':
			value = sV;
			break;
		case 'w':
			value = sW;
			break;
		case 'x':
			value = sX;
			break;
		case 'y':
			value = sY;
			break;
		case 'z':
			value = sZ;
			break;
	}
	return value;
}
string WordSpacing::getBefore(string word1, string word2) //This method gets the word out of two that comes first alphabetically
{
	makeLowerCase(word1);
	makeLowerCase(word2);
	string theanswer = "";
	if (word1 == word2) //If the two words are equal, we found our answer and go to the end of this method
	{
		theanswer = word2;
		goto stopChecking;
	}
	int thesize = (word1.size() > word2.size()) ? word1.size() : word2.size();
	for (int i = 0; i < thesize; i++) //This loop goes through each letter of both words and compares them to each other at each position
	{
		if (word1[i] > word2[i]) //If word1 is bigger at the current letter, then word2 comes before and is our answer
		{
			theanswer = word2;
			goto stopChecking;
		}
		else if (word2[i] > word1[i]) //If word2 is bigger at the current letter, then word1 comes before and is our answer
		{
			theanswer = word1;
			goto stopChecking;
		}
		else if (word1[i] == word2[i]) //Ifn the two words are equal at this current letter, then we go on to the remaing letters till a bigger letter is found
		{
		}
	}
stopChecking:
	//cout << theanswer << " comes before " << ((theanswer == word1) ? word2 : word1) << " alphabetically" << endl;
	return theanswer;
}
void WordSpacing::orderAlphabetically(WordContainer &list) //This method orders a small array of words alphabetically
{
	string * my_temp = NULL;
	my_temp = new string[list.my_Size];
	string * thetemp = NULL;
	thetemp = new string[list.my_Size];
	string FILLER = "zzzzzzzzzzzzzzzzzzzzzzz";
	for (int listAt = 0; listAt < list.my_Size; listAt++) //We make all the words lowercase, and save them to a separate array so that we can eliminate each word when it is found to be the one closest to a
	{
		makeLowerCase(list.my_Words[listAt]);
		thetemp[listAt] = list.my_Words[listAt];
	}
	for (int wordFound = 0; wordFound < list.my_Size; wordFound++) //We find a word one a time
	{
		string theWord = FILLER;
		int placeOfTheWord = 0;
		for (int placeAt = 0; placeAt < list.my_Size; placeAt++) //To find a word, we go through all words, finding the word that comes out on top, then saving it and its position
		{
			if (theWord != "") //A word can only come out on top if it actually has characters to it
			{
				theWord = getBefore(theWord, thetemp[placeAt]);
				if (theWord == thetemp[placeAt]) //If the word that comes out on top is a word that wasn't on top before, we save its position, making eliminating it possible if it is still on top when we reach the end of the array
				{
					placeOfTheWord = placeAt;
				}
			}
		}
		my_temp[wordFound] = theWord;
		thetemp[placeOfTheWord] = FILLER;
		//cout << my_temp[wordFound] << endl;
		//cout << "Deleted word at " << placeOfTheWord << endl;
	}
	for (int listAt = 0; listAt < list.my_Size; listAt++) //We synchronize the given array to the alphabetically ordered array we just created
	{
		//cout << list.getWordAt(listAt) << endl;
		list.my_Words[listAt] = my_temp[listAt];
	}
}
void WordSpacing::setSearchTime(int letter) //This method finds the number of times each spot in the array will take to reach one or less when dividing by two each time
{
	int tempSize = 0;
	int searchTimes = 0;
	tempSize = wordsOfLetter[letter].my_Size;
	while (tempSize >= 1) //As long as the number is greater than one, we divide it in half. We save the number of times it took for future word searching in the spot in the array
	{
		tempSize /= 2;
		searchTimes++;
	}
	wordsOfLetter[letter].searchTimes = searchTimes;
}
DictionaryNotFound::DictionaryNotFound(string dictionaryName) :logic_error("Error-'" + dictionaryName + "'" + " Not Found!") {} //This is the constructor for the error DictionaryNotFound that we throw when a Dictionary isn't found
