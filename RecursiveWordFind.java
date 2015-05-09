/**
 * Created by Moishe on 12/15/2014.
 */
import java.util.Scanner;
import java.io.*;
public class RecursiveWordSpacing {
    public static WordSpacing Line = new WordSpacing ();
    public static Scanner in = new Scanner (System.in);
    public static void main (String[] args) {
        String Continue;
        if (Line.IsDictionary()) {
            if (!Line.IsCustomDictionary()) {
                System.out.println("Due to the configuration of this program, any words with a size");
                System.out.println(" of one or two letters must be included in the missing Dictionary");
                System.out.println("to be found. Therefore it is highly recommended to create this file.");
            }
            do {
                useWordSpacing();
                System.out.println("Would you like to continue?[y/n]");
                Continue = in.nextLine();
            } while (Continue.charAt(0) != 'n');
        } else {
            System.out.println("Add the Dictionary not found for this program to successfully find words");
        }
    }
    public static void useWordSpacing() {
        if (Line.IsDictionary()) {
            System.out.println("Enter Words without spaces between them:");
            Line.provideLine(in.nextLine());
            Line.spaceLine();
            String lineSpaced = Line.getLine();
            if (lineSpaced.equals("")) {
                System.out.println("At least one of the words used wasn't recognized as a word.");
                System.out.println("You can fix this issue in three easy steps:");
                System.out.println("1) Add any missing words to 'myDictionary.dic'.");
                System.out.println("2) Remember to adjust the number in the beginning to number of words now there.");
                System.out.println("3) Restart this application so that the updated Dictionary is loaded.");
            } else {
                System.out.println("All of the words you used were recognized.");
                System.out.println("Words Found:" + lineSpaced);
            }
        }
    }
    public static class WordContainer {
        public int my_Size;
        public String[] my_Words;
        public int searchTimes;
        WordContainer (){
            my_Size = 0;
            my_Words = null;
        }
        public String getWordAt (int position) {
            String value = "";
            if (position < my_Size && position >= 0) {
                value = my_Words[position];
            }
            return value;
        }
    }
    public static class WordSpacing {
        private String my_Line = "";
        private String[] my_LoadedWord;
        WordContainer[] wordsOfLetter = new WordContainer[54];
        private int LoadedWordCount = 0;
        private boolean isDictionary;
        private boolean isCustomDictionary;
        private int A = 1, B = 2, C = 3, D = 4, E = 5, F = 6, G = 7, H = 8, I = 9, J = 10, K = 11, L = 12, M = 13, N = 14, O = 15, P = 16, Q = 17, R = 18, S = 19, T = 20, U = 21, V = 22, W = 23, X = 24, Y = 25, Z = 26,
            sA = 27, sB = 28, sC = 29, sD = 30, sE = 31, sF = 32, sG = 33, sH = 34, sI = 35, sJ = 36, sK = 37, sL = 38, sM = 39, sN = 40, sO = 41, sP = 42, sQ = 43, sR = 44, sS = 45, sT = 46, sU = 47, sV = 48, sW = 49, sX = 50, sY = 51, sZ = 52, r = 53;
        private int[] letterAmount = new int[54];
        WordSpacing() {
            try {
                loadDictionary("Dictionary.dic", true);
            } catch (DictionaryNotFound error) {
                System.out.println(error.getMessage());
            }
        }

        public void provideLine(String Line) {
            my_Line = Line;
        }
        public String getLine() {
            return my_Line;
        }
        public void spaceLine() {
            if (isDictionary) {
                my_Line = separateWordsIn(my_Line);
            } else {
                System.out.println("Error-Cannot do desired activity without Dictionary!");
            }
        }
        private String separateWordsIn(String Line) {
            Line = Line.toLowerCase();
            String value = "";
            String tryAsWord;
            for (int amount = 0; amount < Line.length(); amount ++) {
                if (Line.length() >= 1) {
                    tryAsWord = Line.substring(amount);
                } else {
                    value = "";
                    break;
                }
                if (!isWordByLetter(tryAsWord, getLetter(tryAsWord.charAt(0))) && !isWordByLetter(tryAsWord, r)) {
                    value = "";
                } else {
                    String restOfWord = "";
                    for (int noTry = amount - 1; noTry >= 0; noTry--) {
                        restOfWord = Line.charAt(noTry) + restOfWord;
                    }
                    if (!restOfWord.equals("")) {
                        //System.out.println("Remainder:" + restOfWord);
                        value = separateWordsIn(restOfWord);
                    } else {
                        //System.out.println("Stopped at:" + tryAsWord);
                        value = tryAsWord;
                        break;
                    }
                }
                if (!value.equals("")) {
                    value += " " + tryAsWord;
                    break;
                }
            }
            return value;
        }
        private void loadDictionary(String fileName, boolean alphabetize) {
            try {
            FileReader inputFile = new FileReader(fileName);
            BufferedReader bufferReader = new BufferedReader(inputFile);
            String line;
            int wordAt = 0;
            LoadedWordCount = Integer.parseInt(bufferReader.readLine());
            my_LoadedWord = new String[++ LoadedWordCount];
            if (!alphabetize) {
                wordsOfLetter[r].my_Words = null;
                wordsOfLetter [r] = new WordContainer();
                wordsOfLetter[r].my_Words = new String[++ LoadedWordCount];
                wordsOfLetter [r].my_Size = LoadedWordCount;
            }
            for (int i = 0; i < LoadedWordCount - 1; i ++) {
                line = bufferReader.readLine();
                int theLetter;
                if (!line.equals("")) {
                    if (alphabetize) {
                        my_LoadedWord[wordAt] = null;
                        theLetter = getLetter(line.charAt(0));
                        my_LoadedWord[wordAt] = line;
                        letterAmount[theLetter]++;
                    } else {
                        line = line.toLowerCase();
                        wordsOfLetter[r].my_Words[wordAt] = line;
                    }
                    wordAt++;
                } else {
                    LoadedWordCount -= 1;
                }
            }
            bufferReader.close();
            if (alphabetize) {
                organizeWords();
                for (int i = 0; i < 54; i++) {
                    setSearchTime(i);
                }
                isDictionary = true;
            } else {
                orderAlphabetically();
                setSearchTime(r);
                isCustomDictionary = true;
            }
            System.out.println("'" + fileName + "' Loaded Successfully.");
            } catch (Exception err) {
                if (alphabetize) {
                    isDictionary = false;
                } else {
                    isCustomDictionary = false;
                }
                System.out.println(err.getMessage());
                throw new DictionaryNotFound (fileName);
            }
        }
        public boolean IsDictionary() {
            return isDictionary;
        }
        public boolean IsCustomDictionary() {
            return isCustomDictionary;
        }
        private void organizeWords () {
            for (int j = 0; j < 54; j ++) {
                wordsOfLetter [j] = null;
                wordsOfLetter [j] = new WordContainer();
                wordsOfLetter [j].my_Size = letterAmount[j];
                wordsOfLetter [j].my_Words = new String [letterAmount[j]];
                letterAmount [j] = 0;
            }
            for (int i = 0; i < LoadedWordCount - 1; i ++) {
                if (!my_LoadedWord [i].equals("")) {
                    int letterAt = getLetter(my_LoadedWord[i].charAt(0));
                    my_LoadedWord [i] = my_LoadedWord [i].toLowerCase();
                    wordsOfLetter [letterAt].my_Words [letterAmount [letterAt]] = my_LoadedWord [i];
                    letterAmount [letterAt] ++;
                }
            }
            try {
                loadDictionary("myDictionary.dic", false);
            } catch (DictionaryNotFound error) {
                System.out.println(error.getMessage());
            }
        }
        private boolean isWordByLetter(String Word, int letter) {
            boolean value = false;
            Word = Word.toLowerCase();
           int searchAt = 0;
            int amount = wordsOfLetter [letter].my_Size;
            for (int i = 0; i <= wordsOfLetter [letter].searchTimes + 5; i ++) {
                amount /= 2;
                if (amount == 0 ) {
                    amount = 1;
                }
                String wordHave = wordsOfLetter [letter].getWordAt(searchAt);
                if (wordHave != null && ((letter == r && wordHave.length() >= 1) || (letter != r && wordHave.length() > 2)) && wordHave.equals(Word)) {
                    value = true;
                    break;
                }
                if (getBefore(wordHave, Word).equals(wordHave)) {
                    searchAt += amount;
                } else {
                    searchAt -= amount;
                }
            }
            return value;
        }
        private int getLetter (char Word) {
            int value = r;
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
        private String getBefore (String word1, String word2) {
            String theAnswer = "";
            if (word1 != null && word2 != null) {
            word1 = word1.toLowerCase();
            word2 = word2.toLowerCase();
            reasonToCheck:
                do {
                    if (word1.equals(word2)) {
                        theAnswer = word2;
                        break;
                    }
                    int theSize = (word1.length() > word2.length()) ? word2.length() : word1.length();
                    for (int i = 0; i < theSize; i++) {
                        if (word1.charAt(i) > word2.charAt(i)) {
                            theAnswer = word2;
                            break reasonToCheck;
                        } else if (word2.charAt(i) > word1.charAt(i)) {
                            theAnswer = word1;
                            break reasonToCheck;
                        } else if (word1.charAt(i) == word2.charAt(i)) {
                            theAnswer = (word1.length() > word2.length()) ? word2 : word1;
                        }
                    }
                } while (false);
            }
            return theAnswer;
        }
        private void orderAlphabetically () {
            String my_temp[] = new String [wordsOfLetter[r].my_Size];
            String theTemp[] = new String [wordsOfLetter[r].my_Size];
            final String FILLER = "zzzzzzzzzzzzzzzzzzzz";
            for (int listAt = 0; listAt < wordsOfLetter[r].my_Size - 1; listAt ++) {
                wordsOfLetter[r].my_Words [listAt] = wordsOfLetter[r].my_Words [listAt].toLowerCase();
                theTemp [listAt] = wordsOfLetter[r].my_Words [listAt];
            }
            for (int wordFound = 0; wordFound < wordsOfLetter[r].my_Size - 1; wordFound ++) {
                String theWord = FILLER;
                int placeOfTheWord = 0;
                for (int placeAt = 0; placeAt < wordsOfLetter[r].my_Size - 1; placeAt ++) {
                    if (!theWord.equals("")) {
                        theWord = getBefore(theWord, theTemp [placeAt]);
                        if (theWord.equals(theTemp[placeAt])) {
                            placeOfTheWord = placeAt;
                        }
                    }
                }
                my_temp [wordFound] = theWord;
                theTemp [placeOfTheWord] = FILLER;
            }
            System.arraycopy(my_temp, 0, wordsOfLetter[r].my_Words, 0, my_temp.length);
        }
        private void setSearchTime (int letter) {
            int tempSize;
            int searchTimes = 0;
            tempSize = wordsOfLetter [letter].my_Size;
            while (tempSize >= 1) {
                tempSize /= 2;
                searchTimes ++;
            }
            wordsOfLetter [letter].searchTimes = searchTimes;
        }
    }
    public static class DictionaryNotFound extends RuntimeException {
        DictionaryNotFound (String fileName) {
            super("Dictionary " + "'" + fileName + "' Not Found");
        }
    }
}
