#include <iostream>
#include <fstream>
#include <vector>
#include <pqxx/pqxx>
#include <string> //for std::getline();

#include <stdlib.h> //for std::srand() std::rand()
#include <ctime> //for std::time()
#include <thread>
#include <chrono>

using namespace pqxx;

enum class Gender
{
    Male = 1,
    Female = 2
};

short getRandomValue(short min, short max)
{
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    //srand(time(NULL));
    return (min + (rand() % (max - min + 1)));
}

void insertPerson(Gender gender, std::string fileName, std::string& query)
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

        query += "('" + wholeName + "','" + dateOfBirth + "','" + strGender + "'),";
    }
}

int main(int argc, char *argv[])
{
    try
    {
        connection C("dbname = testtask1 user = postgres password = password \
      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open())
        {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
            //std::string number(argv[1]);
            //std::cout << std::stoi(number) << std::endl;
            //std::cout << (std::string)argv[1] << std::endl;

            //Depending on the parameter, operator switch will start 
            //some branch of the program 
            switch(std::stoi(argv[1])) //(int)(*argv[1]) - (int)'0' takes only one digit
            {
                //Creating a table with fields representing fullname, 
                //date of birth, gender.
                case(1):
                {
                    char *query = "CREATE TABLE employee("  \
                    "id serial," \
                    "fullname varchar(50)   not null," \
                    "birth    varchar(10)   not null," \
                    "sex      varchar(6)" \
                    ");";

                    work W(C);
                    W.exec(query);
                    W.commit();
                    
                    std::cout << "> The Table created successfully" << std::endl;
                }
                break;

                //Creating a record
                case(2):
                {                    
                    std::string query = "INSERT INTO HEROES (fullname, birth, sex) values ('";
                    query.append(argv[2]);
                    query.append("','");
                    query.append(argv[3]);
                    query.append("','");
                    query.append(argv[4]);
                    query.append("' );");
                    
                    std::cout << "> " << query << std::endl;

                    work W(C);
                    W.exec(query.c_str());
                    W.commit();
                }
                break;

                //Output of all rows with a unique value of fullame+birth, 
                //sorted by fullname
                case(3):
                {
                    const char* query = "select distinct fullname," \
                    "birth from heroes order by fullname;";
                    
                    nontransaction N(C);
                    result R(N.exec(query));

                    std::cout << ">--------" << std::endl;
                    for (auto el : R)
                    {
                        std::cout << "FULLNAME: " << el[0].as<std::string>() << std::endl;
                        std::cout << "BIRTH: " << el[1].as<std::string>() << std::endl;
                        std::cout << "---------" << std::endl;
                    }
                    std::cout << "> Operation done successfully" << std::endl;

                }
                break;

                case (4):
                {
                    std::string query = "INSERT INTO employee (fullname, birth, sex) values ";
                    
                    srand(time(NULL));

                    for (size_t i = 0; i < 100; i++)
                    {
                        short solveGender = getRandomValue(1,2);
                        (solveGender == 1) ? insertPerson(Gender::Male ,"MaleNames.txt", query) :
                        insertPerson(Gender::Female, "FemaleNames.txt", query);
                        
                        std::cout << "> " << i << " query: " << query << std::endl;
                        
                    }
                        query[query.size()-1] = ';';
                        //std::cout << query << std::endl;

                        work W(C);
                        W.exec(query.c_str());
                        W.commit();
                    
                }
                break;

                default:
                    std::cout << "> Invalid parametr" << std::endl;
            }

        }
        else
        {
            std::cout << "Can't open database" << std:: endl;
            return 1;
        }
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}