#ifndef CSV_READER
#define CSV_READER 1

#include <iostream>
#include <fstream>

class CSVReader{
    private:
        ifstream fileInput;
        int valColumn;

        // Gets the string of the csv line, given its position
        string getValueGivenPosition(string &line, int pos){
            string word;
            int currPos = 0;
            for(char c : line){
                if(c == ','){
                    currPos++;
                    if(currPos > pos) return word;
                }else if(pos == currPos){
                    word.push_back(c);
                }
            }
            return word;
        }

    public:
        // Constructor for Sketches
        CSVReader(string &filename, int valCol){
            fileInput.open(filename);
            valColumn = valCol;
            ignoreLine();
        }

        // Checks if csv has more lines to read
        bool hasNext(){
            return fileInput.peek() != EOF;
        }

        // Gets val in the current csv line
        int getNextValue(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            string word = getValueGivenPosition(line, valColumn);
            return stoi(word);
        }

        // Ignores a line (only used once in the constructor)
        void ignoreLine(){
            string firstLine;
            getline(fileInput, firstLine);
        }
};
#endif