#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include "mysql/mysql.h"
#include "omp.h"
#include <sstream>
#define BufferSize 600000
using namespace std;
class EM_initialize
{
public:
    inline vector<string> tokenize( const char* input )
    {
        vector< string > token;
        bool betweenWords = true;
        int start=0;
        int i=0;
        for(; input[i] != '\0'; i++)
        {
            bool isSpace = (input[i] == ' ' || input[i] == '\t');

            if (!isSpace && betweenWords)
            {
                start = i;
                betweenWords = false;
            }
            else if (isSpace && !betweenWords)
            {
                token.push_back( string( input+start, i-start ) );
                betweenWords = true;
            }
        }
        if (!betweenWords)
            token.push_back( string( input+start, i-start ) );
        return token;
    }

    void english_data_input(const string& fileName)
    {
        ifstream enfile;
        enfile.open(fileName.c_str());
        std::string line;
        //vector<string>english_token;
        int i=0;
        while(true)
        {
            i++;
            getline(enfile, line);
            if (enfile.eof()) break;
            if (!enfile)
            {
                cout<<"error in files";
            }
            vector<string>temp;
            temp = tokenize(line.c_str());

            for ( size_t j = 0; j < temp.size(); j++)
            {
                english_token.push_back(temp[j]);
            }
            if (english_token.size() < 2)
            {
                cerr << "line " << i << " in " << " too short, skipping\n";
                continue;
            }

        }

        //enflie.close();
    }
    void french_data_input(const string& fileName)
    {
        ifstream infile;
        infile.open(fileName.c_str());
        std::string line;
        //vector<string>english_token;
        int i=0;
        while(true)
        {
            i++;
            getline(infile, line);
            if (infile.eof()) break;
            if (!infile)
            {
                cout<<"error in files";
            }
            vector<string>temp;
            temp = tokenize(line.c_str());

            for ( size_t j = 0; j < temp.size(); j++)
            {
                french_token.push_back(temp[j]);
            }
            if (french_token.size() < 2)
            {
                cerr << "line " << i << " in " << " too short, skipping\n";
                continue;
            }

        }

        //inflie.close();
    }


    inline  vector <string>getenglishtoken()
    {
        return english_token;
    }

    inline  vector <string>getfrenchtoken()
    {
        return french_token;
    }


    void mysqlconnection()
    {
        MYSQL *conn_ptr;
        conn_ptr = mysql_init(NULL);

        if(!conn_ptr)
        {
            fprintf(stderr,"mysql_init failed\n");
            //return EXIT_FAILURE;
        }

        conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","nlpgroup","testdb",0,NULL,0);

        if(conn_ptr)
            printf("Connection success\n");

        else
            printf("Connection failed\n");

        mysql_close(conn_ptr);

    }
    void mysqlstart()
    {
        //MYSQL *conn_ptr;
        conn_ptr = mysql_init(NULL);

        if(!conn_ptr)
        {
            fprintf(stderr,"mysql_init failed\n");
            //return EXIT_FAILURE;
        }
        // bool reconnection =true;
        // mysql_options(conn_ptr,MYSQL_OPT_RECONNECT,&reconnection);
        conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","nlpgroup","testdb",0,NULL,0);
    }
    void mysqlclose()
    {
        mysql_close(conn_ptr);
    }
    bool mysqlinsert(string input)
    {
        bool reconnection =true;
        if(mysql_ping(conn_ptr))
        {
            mysqlclose();
            mysql_options(conn_ptr,MYSQL_OPT_RECONNECT,&reconnection);
        }
        else
        {
            cout<<"Ping Ok"<<endl;
        }
        if(conn_ptr)
        {
            int abc;
            if(abc = mysql_query(conn_ptr,input.c_str()))
            {
                fprintf(stderr, "%s\n", mysql_error(conn_ptr));
            }


            //connection succees
        }
        else
        {
            fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(conn_ptr));
            //return EXIT_SUCCESS;
        }

    }

    bool mysqlinsert2(int sourceindex,int targetindex,string source,string target)
    {
        MYSQL *conn_ptr;
        conn_ptr = mysql_init(NULL);

        if(!conn_ptr)
        {
            fprintf(stderr,"mysql_init failed\n");
            //return EXIT_FAILURE;
        }

        conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","nlpgroup","testdb",0,NULL,0);

        if(conn_ptr)
        {
            int abc;
            char sql_buffer[500] = {0};
            sprintf(sql_buffer,"insert into EM (sourceindex,targetindex,source,target) values(\"%d\",\"%d\",\"%s\",\"%s\")",sourceindex,targetindex,source.c_str(),target.c_str());
            if((abc = mysql_query(conn_ptr,sql_buffer)))
            {
                printf("error code:%s\n",mysql_error(conn_ptr));
            }
            mysql_close(conn_ptr);
            return true;
            //connection succees
        }
        else
        {
            printf("Error code: %d  %s %s\n",mysql_errno(conn_ptr),mysql_sqlstate(conn_ptr),mysql_error(conn_ptr));
            mysql_close(conn_ptr);
            return false;
            //return EXIT_SUCCESS;
        }
    }
    void retrive_data()
    {
        MYSQL *con = mysql_init(NULL);


        if (con == NULL)
        {
            fprintf(stderr, "mysql_init() failed\n");
            exit(1);
        }

        if (mysql_real_connect(con, "localhost", "root", "nlpgroup",
                               "testdb", 0, NULL, 0) == NULL)
        {
            finish_with_error(con);
        }
        //SELECT * FROM EM2 GROUP BY source, target HAVING count(*)>1 顯示超過一次的
        //SELECT source,target,COUNT(*) AS count FROM EM2 GROUP BY source, target HAVING count(*)>1
        {
            if (mysql_query(con, "SELECT source,target,COUNT(*) AS count FROM EM2 GROUP BY source, target HAVING count(*)>=1"))
            {
                finish_with_error(con);
            }

            MYSQL_RES *result = mysql_store_result(con);

            if (result == NULL)
            {
                finish_with_error(con);
            }

            int num_fields = mysql_num_fields(result);

            MYSQL_ROW row;

            while ((row = mysql_fetch_row(result)))
            {
                for(int i = 0; i < num_fields; i++)
                {
                    printf("%s ", row[i] ? row[i] : "NULL");
                }
                printf("\n");
            }

            mysql_free_result(result);
            mysql_close(con);
        }
    }
    void finish_with_error(MYSQL *con)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    double arraymultipy(vector<double>array)
    {

        double result,cal;
        for (size_t i = 0; i<10; i++)
        {
            cal+=array[i];
        }
        result=log(cal);
        return result;
    }

    void EM_start()
    {
        l=english_token.size();
        m=french_token.size();
        cout<<"english_token_size="<<l<<endl;
        cout<<"french_token_size="<<m<<endl;
        combine=english_token;
        combine.insert(combine.end(),french_token.begin(),french_token.end());
        cout<<"combine_vector_size="<<combine.size()<<endl;


        //mysqlconnection();
    }
    void write_token_to_data()
    {
        //==
        ofstream outfile;

        outfile.open("/home/hadoop/counterlooptest/output",std::ofstream::app);

        //==
        int counter_total=0;
        int counter_one=0;
        int counter_two=0;
        int counter_three=0;
        int counter_four=0;
        int counter_five=0;
        int counter_six=0;
        int count=0;
        int test=0;
        int counter=1;
        int insert_count=0;
        // int BufferSize=2000;
        string str,laststr,spstr;
        stringstream ss;
        string tempString;
        int execute_count=9000;
        typedef double* DynamicMatrix[l+m];
        // DynamicMatrix Count;
        typedef double* DynamicMatrix2[l+m];
        //DynamicMatrix2 Prob;
        for(int i=0; i<(l+m); i++)
        {
            for(int j=0; j<(l+m); j++)
            {
                counter_total++;
                //((l+m)*(l+m))/execute_count
                if(insert_count<(floor(((l+m)*(l+m))/execute_count)))
                {
                    if(count==0)
                    {
                        str = "INSERT INTO EM ( sourceindex,targetindex,source,target) VALUES";
                        char temp_buffer[BufferSize] = {0};
                        sprintf(temp_buffer,"(\"%d\",\"%d\",\"%s\",\"%s\"),",i,j,combine[i].c_str(),combine[j].c_str());
                        ss<<temp_buffer;
                        ss>>tempString;
                        str+=tempString;
                        ss.str("");
                        ss.clear();
                        count++;
                        counter_one++;

                    }
                    else if(count ==(execute_count-1))
                    {
                        char temp_buffer2[BufferSize] = {0};
                        sprintf(temp_buffer2,"(\"%d\",\"%d\",\"%s\",\"%s\");",i,j,combine[i].c_str(),combine[j].c_str());
                        ss<<temp_buffer2;
                        ss>>tempString;
                        str+=tempString;
                        ss.str("");
                        ss.clear();
                       // mysqlinsert(str);
                        //throw mysql
                        insert_count++;
                        str="";
                        count=0;
                        counter_two++;
                    }
                    else
                    {
                        char temp_buffer3[BufferSize] = {0};
                        sprintf(temp_buffer3,"(\"%d\",\"%d\",\"%s\",\"%s\"),",i,j,combine[i].c_str(),combine[j].c_str());
                        ss<<temp_buffer3;
                        ss>>tempString;
                        str+=tempString;
                        ss.str("");
                        ss.clear();
                        count++;
                        counter_three++;
                    }

                }// others are can't divide
                else
                {
                    if(counter==1)
                    {

                        laststr = "INSERT INTO EM ( sourceindex,targetindex,source,target) VALUES";
                        char temp_buffer4[BufferSize] = {0};
                        sprintf(temp_buffer4,"(\"%d\",\"%d\",\"%s\",\"%s\"),",i,j,combine[i].c_str(),combine[j].c_str());
                        ss<<temp_buffer4;
                        ss>>tempString;
                        laststr+=tempString;
                        ss.str("");
                        ss.clear();
                        counter++;
                        //outfile<<i<<","<<j<<","<<combine[i]<<","<<combine[j]<<endl;
                        counter_four++;
                    }
                    else if(counter==(((l+m)*(l+m))%execute_count))
                    {
                        char temp_buffer5[BufferSize] = {0};
                        sprintf(temp_buffer5,"(\"%d\",\"%d\",\"%s\",\"%s\");",i,j,combine[i].c_str(),combine[j].c_str());
                        ss<<temp_buffer5;
                        ss>>tempString;
                        laststr+=tempString;
                       // mysqlinsert(laststr);
                       
                        // outfile<<"II:"<<i<<","<<j<<","<<combine[i]<<","<<combine[j]<<endl;
                        counter_five++;
                    }
                    else
                    {
                        //outfile<<"III:"<<i<<","<<j<<","<<combine[i]<<","<<combine[j]<<endl;
                        char temp_buffer6[BufferSize] = {0};
                        sprintf(temp_buffer6,"(\"%d\",\"%d\",\"%s\",\"%s\"),",i,j,combine[i].c_str(),combine[j].c_str());
                        ss<<temp_buffer6;
                        ss>>tempString;
                        laststr+=tempString;
                        ss.str("");
                        ss.clear();
                        counter++;
                        counter_six++;
                    }
                }
            }
        }
        outfile<<"one:"<<counter_one<<endl;
                 outfile<<"two:"<<counter_two<<endl;
                 outfile<<"three:"<<counter_three<<endl;
                 outfile<<"four:"<<counter_four<<endl;
                 outfile<<"five:"<<counter_five<<endl;
                 outfile<<"six:"<<counter_six<<endl;
                 outfile<<"total:"<<counter_total<<endl;
                 outfile<<"insert_count:"<<insert_count<<endl;
                 outfile<<"(((l+m)*(l+m))%execute_count)"<<(((l+m)*(l+m))%execute_count)<<endl;
                 outfile<<"(floor(((l+m)*(l+m))/execute_count))"<<(floor(((l+m)*(l+m))/execute_count))<<endl;
        outfile.close();
    }//end of function

private:
    vector<int>count;
    vector<double>prob;
    vector<string>english_token;
    vector<string>french_token;
    int l; //english_token.size()
    int m;  //french_token.size()
    ifstream infile;
    ifstream enfile;
    vector<string>combine;
    MYSQL *conn_ptr;
    //MYSQL *conn_ptr = mysql_init(NULL);
}; //end of class


double ArrayGCD(double *a, int first, int last);
int main ()
{

    EM_initialize handler;
    vector<string> english_token;
    vector<string> french_token;

   // handler.english_data_input("/home/hadoop/simple_corpus/sim.en");
   // handler.french_data_input("/home/hadoop/simple_corpus/sim.fr");
    handler.english_data_input("/home/hadoop/corpus_wei/europarl_clean_1-9000.en");
    handler.french_data_input("/home/hadoop/corpus_wei/europarl_clean_1-9000.fr");

    handler.EM_start();
    handler.mysqlstart();
    handler.write_token_to_data();
    handler.mysqlclose();
    //handler.retrive_data();


    return 0;
}

double ArrayGCD(double *a, int first, int last)
{
    double x = 0, y = 0, gcd = 0;
    if(first == last)
    {
        gcd = a[first];
        return gcd;
    }
    else
    {
        x = ArrayGCD(a,first,(first+last)/2);
        y = ArrayGCD(a,(first+last)/2+1,last);
        if(x < 0)
        {
            x = -x;
        }
        if(y < 0)
        {
            y = -y;
        }

        double t;
        while(x)
        {
            t = x;
            x = fmod(y,x);
            y = t;
        }
        gcd = y;
        return gcd;
    }
}




