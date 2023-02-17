#include <iostream>
#include <string>
#include <pqxx/pqxx>

using namespace pqxx;

int main(int argc, char *argv[])
{
    try
    {
        connection C("dbname = testtask user = postgres password = password \
      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open())
        {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;

            //Depending on the parameter, operator switch will start 
            //some branch of the program 
            switch((int)(*argv[1]) - (int)'0')
            {
                //Creating a table with fields representing fullname, 
                //date of birth, gender.
                case(1):
                {
                    char *query = "create table HEROES("  \
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