#include<iostream>
#include<cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;
int main()
{
    ofstream outfile;

    outfile.open("/home/hadoop/counterlooptest/test",std::ofstream::app);
    //-------------------------------
    int l=10;
    int m=20;
    int counter_total=0;
    int counter_one=0;
    int counter_two=0;
    int count=0;
    int test=0;
    int counter=1;
    int insert_count=0;
    int test_count=0;
    // int BufferSize=2000;
    string str,laststr,spstr;
    stringstream ss;
    string tempString;
    int execute_count=8;
    typedef double* DynamicMatrix[l+m];
    // DynamicMatrix Count;
    typedef double* DynamicMatrix2[l+m];
    //-------------------------------pra
    for(int i=0; i<(l+m); i++)
    {
        for(int j=0; j<(l+m); j++)
        {

          if(insert_count<(floor(((l+m)*(l+m))/execute_count))){
               if(count==0){
               str = "INSERT INTO EM ( sourceindex,targetindex,source,target) VALUES";

                count++;
               }else if(count ==(execute_count-1)){
                    //throw mysql
                    insert_count++;
                    count=0;
               }else{
                     count++;

               }
           }else{
           if(counter==1){
           counter++;
           test_count++;
           }else if(counter==(((l+m)*(l+m))%execute_count)){
           counter_one++;
           cout<<"Wrong Way !!!"<<endl;
           }else{
           counter++;
           }
           }


            counter_total++;
        }
    }
    outfile<<"(((l+m)*(l+m))%execute_count)"<<(((l+m)*(l+m))%execute_count)<<endl;
    outfile<<"------------------------"<<endl;
    outfile<<"insert_count:"<<insert_count<<endl;
    outfile<<"counter_total:"<<counter_total<<endl;
    outfile<<"counter_one:"<<counter_one<<endl;
    outfile<<"counter_two:"<<counter_two<<endl;
    outfile<<"test_count:"<<test_count<<endl;




    system("pause");
}
