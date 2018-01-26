#include<iostream>
#include<cassert>
#include<array>
#include<cstring>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include <boost/algorithm/string.hpp>
#define GDBM_STATIC
#include<gdbm.h>
#include <stdio.h>
using namespace std;
void store(){
	GDBM_FILE database = gdbm_open("count.db",512,GDBM_WRCREAT,0644,0);
	assert(database!= nullptr);
	ifstream fin("./count.flat");
	while(!fin.eof()){
		string line;
		getline(fin,line);
		vector<string>strs;
		boost::split(strs,line,boost::is_any_of(" "));
		double count = atof(strs[1].c_str());               
		//store(strs[0],count);
		datum key;
		datum data;
		ostringstream ss;
		ss<<count;
		string count_string = ss.str();
		char *count_char = strdup(count_string.c_str());
		char *pair_char = strdup(strs[0].c_str());
		key.dptr=pair_char;
		key.dsize=strs[0].size()+1;
		data.dptr = count_char;
		data.dsize = count_string.size()+1;
		int result = gdbm_store(database,key,data,GDBM_REPLACE);
		assert(result == 0);
	}
	
	gdbm_close(database);
	

}
/*void import_file_to_db(){
	ifstream fin("./prob.flat");
	while(!fin.eof()){
		string line;
		getline(fin,line);
		vector<string>strs;
		boost::split(strs,line,boost::is_any_of(" "));
		//cout<<"strs[0]:"<<strs[0]<<",strs[1]:"<<strs[1]<<endl;
		double count = atof(strs[1].c_str());
		//cout<<"strs[0]:"<<strs[0]<<",strs[1]:"<<count<<endl;
		store(strs[0],count);
	}
}*/
void open(){
	GDBM_FILE database = gdbm_open("trial.db",512,GDBM_WRCREAT,0600,NULL);
	assert(database!= nullptr);
	cout<<"database OPNE ok!!"<<endl;
	gdbm_close(database);
}

void show(){
	GDBM_FILE database = gdbm_open("count.db",512,GDBM_READER,0600,NULL);
	datum key;
	datum data;
	key = gdbm_firstkey(database);
	while ( key.dptr != nullptr ) {
		char* tmp = key.dptr;
		data = gdbm_fetch(database, key);
		cout << key.dptr << " = " << data.dptr << endl;
		free(data.dptr);
		key = gdbm_nextkey(database, key);
		free(tmp);
	}
	gdbm_close(database);
}
void show_and_dumpfile(){
	ofstream outfile;
	outfile.open("./output",ios::app);
	GDBM_FILE database = gdbm_open("prob.db",512,GDBM_READER,0600,NULL);
	datum key;
	datum data;
	key = gdbm_firstkey(database);
	while ( key.dptr != nullptr ) {
		char* tmp = key.dptr;
		data = gdbm_fetch(database, key);
		outfile << key.dptr << "\t" << data.dptr << endl;
		free(data.dptr);
		key = gdbm_nextkey(database, key);
		free(tmp);
	}
	gdbm_close(database);
	outfile.close();
}
void update(string pair,double update_count){
	GDBM_FILE database = gdbm_open("count.db",512,GDBM_WRITER,0600,NULL);
	datum key;
	key.dptr = strdup(pair.c_str());
	key.dsize = pair.size()+1;
	datum content;
	content = gdbm_fetch (database, key);
	if (content.dptr == NULL)
	{
		fprintf(stderr, "key not found\n");
	}
	else
	{
		/* do something with content.dptr */
		cout<<"I found the data!!!!"<<endl;
		double final_count = update_count+atof(content.dptr);
		key.dptr = strdup(pair.c_str());
		key.dsize = pair.size()+1;
		ostringstream strs;
		strs<<final_count;
		string count_string = strs.str();
		content.dptr = strdup(count_string.c_str());
		content.dsize = strlen(content.dptr)+1;
		int result = gdbm_store(database,key,content,GDBM_REPLACE);
		assert(result == 0);
	}
	gdbm_close(database);
}
double get_prob(string pair){
	GDBM_FILE database = gdbm_open("prob.db",512,GDBM_READER,0600,NULL);
	datum key;
	key.dptr = strdup(pair.c_str());
	key.dsize = pair.size()+1;
	cout<<"key.dptr:"<<key.dptr<<endl;
	datum content;
	content = gdbm_fetch (database, key);
	if (content.dptr == NULL)
	{
		fprintf(stderr, "key not found\n");
	}else{
		return atof(content.dptr);
	}
}
double count_sum(string source){
	GDBM_FILE database = gdbm_open("trial.db",512,GDBM_READER,0600,NULL);
	assert(database != nullptr);
	double sum=0;
	datum key;
	datum content;
	//char* source_char = strdup(source.c_str());
	key = gdbm_firstkey(database);
	while ( key.dptr != nullptr ) {
		char* tmp = key.dptr;
		string tmpstr = string(key.dptr);
		content = gdbm_fetch(database, key);
		//outfile << key.dptr << "\t" << data.dptr << endl;
		vector<string>strs;
		boost::split(strs,tmpstr,boost::is_any_of("|||"));
		cout<<"key.dptr"<<key.dptr<<endl;
		if( strs[0]==source){
			sum+=atof(content.dptr);
		}
		free(content.dptr);
		key = gdbm_nextkey(database, key);
		free(tmp);
		
	}
	gdbm_close(database);
	return sum;
}
void  prob_calculation(string source,double sum){
	GDBM_FILE database = gdbm_open("trial.db",512,GDBM_READER,0600,NULL);
	GDBM_FILE database2 = gdbm_open("trial2.db",512,GDBM_WRITER,0600,NULL);
	datum key,key2;
	datum content,content2;
	// char* source_char = strdup(source.c_str());
	key = gdbm_firstkey(database);
	key2 = gdbm_firstkey(database2);
	
	while ( key.dptr != nullptr &&key2.dptr!=nullptr ) {
		//char* tmp = key.dptr;
		//char* tmp2 = key2.dptr;
		string tmpstr = string(key.dptr);
		content = gdbm_fetch(database, key);
		content2 = gdbm_fetch(database,key2);
		vector<string>strs;
		boost::split(strs,tmpstr,boost::is_any_of("|||"));

		if( strs[0] == source){
			double last_prob = atof(content.dptr)/sum;
			ostringstream strs;
			strs<<last_prob;
			string last_prob_string = strs.str();
			content2.dptr = strdup(last_prob_string.c_str());
			content2.dsize = strlen(content2.dptr)+1;
			int result = gdbm_store(database2,key2,content2,GDBM_REPLACE);
			assert(result == 0);
		}
		free(content.dptr);
		key = gdbm_nextkey(database, key);
		key2 = gdbm_nextkey(database2, key2);
		//free(tmp);
	}
}
void test(){
GDBM_FILE database = gdbm_open("count.db",512,GDBM_WRITER,0600,NULL);
assert(database!=0);
gdbm_close(database);
}
int main(){
	string leftside ="a";
	double div = 2.0;
	
	//double answer = count_sum(leftside);
	//cout<<"answer"<<answer<<endl;
	//prob_calculation(leftside,div);
	//open();
	string pair = "abc";
	double count = 1.2;
	double addcount= 1.3;
        string input="17|||que";
	store();
	//update(pair,addcount);
        //show();
        //test();
	//show_and_dumpfile();
	//import_file_to_db();
	//string input = "­|||­";
	//double prob=get_prob(input);
	//cout<<"prob:"<<prob<<endl;
}

