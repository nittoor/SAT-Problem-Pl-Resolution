#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "CNFGenerator.h"
#include "PLResolution.h"

using namespace std;

const int FILE_NAME_SIZE=256;


int main (int argc, char * argv [])
{
        char inputFileName[FILE_NAME_SIZE];
        char outputFileName[FILE_NAME_SIZE];
		bool firstLine=true;
        if (argc!=3)
        {
                cerr<<"Usage : executable_name <input file> <output file>";
                exit(1);
        }

        strcpy(inputFileName,argv[1]);
        strcpy(outputFileName,argv[2]);

        ifstream inputFile(inputFileName);
        if (!inputFile)
        {
                cerr<<"Could not open input file"<<inputFileName<<endl;
                exit(1);
        }

        vector<string> relationString;
        string inputLine;
		int tables, persons;
        if (getline(inputFile,inputLine))
        {
				if (firstLine)
				{
					sscanf(inputLine.c_str(),"%d %d",&tables,&persons);
					firstLine=false;
				}
				//cout<<"input line="<<inputLine<<endl;
                
        }

		CNFGenerator CNFGen(tables,persons);

		while (getline(inputFile,inputLine))
        {
				
			relationString.push_back(inputLine);
			//cout<<"input line="<<inputLine<<endl;    
        }
		CNFGen.loadRelationshipFromVector(relationString);
		CNFGen.GenerateCNF();
		//CNFGen.printCNF();
		bool result;
		PLResolution PlResol(CNFGen.getSetOfClauses());
		result=PlResol.ApplyResolution();

		ofstream outputFile(outputFileName);
        if (!outputFile)
        {
                cerr<<"Could not open input file"<<inputFileName<<endl;
                exit(1);
        }
		if (result)
			outputFile << "true";
		else
			outputFile << "false";

		outputFile .close();
        return 0;
}
