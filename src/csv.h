#ifndef CSV_READER
#define CSV_READER 1

#include <iostream>
#include <sstream>
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

        void updateRowVector(vector<string> &row, string &line){
            string word;
            replaceCommasWithSpaces(line);
            stringstream ss(line);
            while(ss >> word){
                row.push_back(word);
            }
        }

        string getValueGivenPosition(string &line, int pos){
            string word;
            replaceCommasWithSpaces(line);
            stringstream ss(line);
            ss >> word;
            while(pos -- > 0){
                ss >> word;
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
            updateRowVector(row, line);
            return {hasher.getUniqueHash(row[idColumn]), stoi(row[weightColumn])};
        }

        long long getNextValue(){
            string line;
            vector<string> row;

            getline(fileInput, line);
            getValueGivenPosition(line, idColumn);
            return hasher.getUniqueHash(row[idColumn]);
        }

        void ignoreFirstLine(){
            string firstLine;
            getline(fileInput, firstLine);
        }
};

#endif
