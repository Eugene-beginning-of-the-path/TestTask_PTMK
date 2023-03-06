#include <iostream>
#include <fstream>
#include <vector>
#include <string> //for std::getline();

int main()
{
    std::ifstream fileRead;
    fileRead.open("MaleNamesF.txt");

    if (fileRead.is_open())
    {
        std::string line;
        char devider = ',';

        
        std::getline(fileRead, line); //getting whole line from file with spaces in variable 'line'
        std::string strToInt = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
        int volume = std::stoi(strToInt); //keeping a value, how much names are written in the file

        fileRead >> line; //copying a line to the first space or line break '\n'

        int countName = 60; //rand(1, volume);
        if (countName == 1)
        {
            std::string wholeName = line.substr(0, line.find(',', 0));
        }
        else
        {
            int i = 0;
            size_t startName = 0; //will keep the first index of the name
            size_t zapyatayaNumberX = countName - 1; //index of the required comma before required the name
            while (zapyatayaNumberX != i)
            {
                startName = line.find(',', startName + 1);
                i++;
            }
            //0123456789
            //rrr,str,rr 
            startName += 1; //now keeping the first index of the name 
            size_t endName = line.find(',', startName); //getting the index of the end name

            std::string wholeName = line.substr(startName, endName - startName);
            std::cout << wholeName;
        }
        
    }
    
}

