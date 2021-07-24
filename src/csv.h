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

        void replaceCommasWithSpaces(string &line){
            for(char &c : line){
                if(c == ',') c = ' ';
            }
        }

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
        CSVReader(string &filename, int idCol, int weightCol){
            fileInput.open(filename);
            idColumn = idCol;
            weightColumn = weightCol;
            ignoreFirstLine();
        }

        CSVReader(string &filename, int idCol){
            fileInput.open(filename);
            idColumn = idCol;
            ignoreFirstLine();
        }

        bool hasNext(){
            return fileInput.peek() != EOF;
        }

        vector<long long> getNextValueAndWeight(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            string idWord = getValueGivenPosition(line, idColumn);
            string weightWord = getValueGivenPosition(line, weightColumn);
            return {hasher.getUniqueHash(idWord), stoi(weightWord)};
        }

        long long getNextValue(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            string word = getValueGivenPosition(line, idColumn);
            return hasher.getUniqueHash(word);
        }

        void ignoreFirstLine(){
            string firstLine;
            getline(fileInput, firstLine);
        }
};

#endif
