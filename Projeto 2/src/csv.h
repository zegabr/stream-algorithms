#ifndef CSV_READER
#define CSV_READER 1

#include <iostream>
#include <fstream>
#include "hasher.h"

class CSVReader{
    private:
        ifstream fileInput;
        int idColumn;
        int weightColumn;
        Hasher hasher;

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
        // Constructor for Count Min Sketch
        CSVReader(string &filename, int idCol, int weightCol){
            fileInput.open(filename);
            idColumn = idCol;
            weightColumn = weightCol;
            ignoreLine();
        }

        // Constructor for KMV Sketch
        CSVReader(string &filename, int idCol){
            fileInput.open(filename);
            idColumn = idCol;
            ignoreLine();
        }

        // Checks if csv has more lines to read
        bool hasNext(){
            return fileInput.peek() != EOF;
        }

        // Gets id and weight in the current csv line for Count Min Sketch
        vector<long long> getNextValueAndWeight(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            string idWord = getValueGivenPosition(line, idColumn);
            string weightWord = getValueGivenPosition(line, weightColumn);
            return {hasher.getUniqueHash(idWord), stoi(weightWord)};
        }

        // Gets id and weight in the current csv line for KMV Sketch
        long long getNextValue(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            string word = getValueGivenPosition(line, idColumn);
            return hasher.getUniqueHash(word);
        }

        // Ignores a line (only used once inside the constructor)
        void ignoreLine(){
            string firstLine;
            getline(fileInput, firstLine);
        }
};
#endif