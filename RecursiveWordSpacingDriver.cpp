/*This program was created by Moishe Lipsker in December of 2014. Copyright. All rights reserved.
The Dictionary used was found online at http://wordlist.aspell.net/, created by Kevin Atkinson.
This program uses Recursion to find words in a string that contains a number of words that do not have spaces in them
*/
#include "RecursiveWordSpacing.h" //This includes the class WordSpacing so that our program can create an instance of it
void useWordSpacing (); // and then feed it the string the user desires to break into words 
WordSpacing Line; //We create an instance of the class WordSpace and attempt to load the two Dictionaries
void main () { //This is the main function that gives the user the opportunity to repeatedly choose a string for the program to break into words
	string Continue = "n";
	if (Line.IsDictionary()) //We only continue if the main Dictionary was found
	{
		if (!Line.IsCustomDictionary()) //If the custom Dictionary wasn't found, we suggest to the user that they create one
		{
			cout << "Due to the configuration of this program, any words with a size" << endl;
			cout << " of one or two letters must be included in the missing Dictionary" << endl;
			cout << "to be found. Therefore it is highly recommended to create this file." << endl;
		}
		do { //Here we loop and keept on accepting strings from the user until the user requests to stop 
			useWordSpacing();
			cout << "Would you like to continue?[y/n]" << endl;
			cin >> Continue;
		} while (Continue[0] != 'n');
	}
	else //We inform the user that the main Dictionary must exist for this program to break a string the user provides into words
	{
		cout << "Add the Dictionary not found for this program to successfully find words" << endl;
	}
}
void useWordSpacing () { //This function is called by the main function to handle all the input that the user inputs and the uotput from the class WordSpacing
	if (Line.IsDictionary () ) { //We only proceed if the main Dictionary was found
		cout << "Enter Words without spaces between them:" << endl;
		string input = "";
		cin >> input; //This is the string that the user inputs that we will attempt to break into words
		Line.provideLine (input); //We input the string that the user provides to the instance of WordSpacing to save it and modify it
		Line.SpaceLine (); //The instance of WordSpacing works to find a combination of words that works for the entire string and returns the first match it finds 
		string lineSpaced = "";
		lineSpaced = Line.getLine (); //We get the result from the instance of WordSpacing and check if the string was able to be broken into words
		if (lineSpaced == "") { //The user used a word unrecognized by the the instance of Class WordSpacing. To allow the word to recognized, we inform the user what steps to take
			cout << "At least one of the words used wasn't recognized as a word." << endl;
			cout << "You can fix this issue in three easy steps:" << endl;
			cout << "1) Add any missing words to 'myDictionary.dic'." << endl;
			cout << "2) Remember to adjust the number in the beginning to number of words now there." << endl;
			cout << "3) Restart this application so that the updated Dictionary is loaded." << endl;
		} else {
			cout << "All of the words you used were recognized." << endl; //We have a match and the instance of Class WordSpacing found a possible combination of words
			cout << "Words Found:" << lineSpaced << endl; // from the provided string
		}
	}
}
