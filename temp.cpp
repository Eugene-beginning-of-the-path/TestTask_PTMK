#include <iostream>
#include <fstream>
#include <vector>
#include <string> //for std::getline();

#include <stdlib.h> //for std::srand() std::rand()
#include <ctime> //for std::time()

enum class Gender
{
    Male = 1,
    Female = 2
};

short getRandomValue(short min, short max)
{
    srand(time(NULL));
    return (min + (rand() % (max - min + 1)));
}

void insertPerson(Gender gender ,std::string fileName, std::string& query)
{
    //std::string query;
    std::ifstream fileReadNames;
    fileReadNames.open(fileName);

    if (fileReadNames.is_open())
    {
        std::string line;

        std::getline(fileReadNames, line); //getting whole line from file with spaces in variable 'line'
        std::string strToInt = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
        int volume = std::stoi(strToInt); //keeping a value, how much names are written in the file

        fileReadNames >> line; //copying a line to the first space or line break '\n'

        int solveName = getRandomValue(1, volume); //rand(1, volume);
        std::string wholeName;
        if (solveName == 1)
        {
            wholeName = line.substr(0, line.find(',', 0));
            //std::cout << wholeName;
        }
        else
        {
            int i = 0;
            size_t startIndexName = 0; //will keep the first index of the name
            size_t zapatayaBeforeName = solveName - 1; //index of the required comma before required the name
            while (zapatayaBeforeName != i)
            {
                startIndexName = line.find(',', startIndexName + 1);
                i++;
            }
            //0123456789
            //rrr,str,rr 
            startIndexName += 1; //now keeping the first index of the name 
            size_t endIndexName = line.find(',', startIndexName); //getting the index of the end name

            wholeName = line.substr(startIndexName, endIndexName - startIndexName);
            //std::cout << wholeName;
        }

        std::string dateOfBirth;  
        std::ifstream fileReadDates;
        fileReadDates.open("Dates.txt");
        if (fileReadDates.is_open())
        {
            std::getline(fileReadDates, line);
            int volume = std::stoi(line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1));
            int solveDate = getRandomValue(1, volume);

            fileReadDates >> line;

            if (solveDate == 1)
            {
                dateOfBirth = line.substr(0, line.find(','));
            }
            else
            {
                int i = 0;
                size_t startIndexDate = 0;
                size_t zapatayaBeforeDate = solveDate - 1;
                while (zapatayaBeforeDate != i)
                {
                    startIndexDate = line.find(',', startIndexDate + 1);
                    i++;
                }
                startIndexDate += 1;
                dateOfBirth = line.substr(startIndexDate, line.find(',', startIndexDate) - startIndexDate);
            }
        }

        std::string strGender;
        (gender == Gender::Male) ? strGender = "male" : strGender = "female";

        query = "INSERT INTO employee (fullname, birth, sex) values ('" + wholeName + "','" +
            dateOfBirth + "','" + strGender + "');";
        std::cout << "> " << query;

    }
}

int main()
{
    {
        std::string query;
        short solveGender = getRandomValue(1,2);

        (solveGender == 1) ? insertPerson(Gender::Male ,"MaleNames.txt", query) :
            insertPerson(Gender::Female, "FemaleNames.txt", query);

    }
    
}

