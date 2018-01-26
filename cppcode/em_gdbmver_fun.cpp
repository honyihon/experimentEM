/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include<algorithm>
#include<cmath>
#include<cfloat>
#include<iomanip>
#include <boost/algorithm/string.hpp>
//below database using
#define GDBM_STATIC
#include<gdbm.h>
#include <stdio.h>
using namespace std;
//======================================
template <typename T>
void FreeAll( T & t ) {
	T tmp;
	t.swap( tmp );
}
bool is_inf_or_nan(double x) 
{
	return !(x <= DBL_MAX && x >= -DBL_MAX); 
}
double fixed_value(double x){
	if(1.7e-307 > x){
		return 1.7e-307;
	}else{
		return 1.7e+307;
	}
}    
void store(string pair,double count){
	GDBM_FILE database = gdbm_open("prob.db",512,GDBM_WRCREAT,0644,0);
	assert(database!= nullptr);
	datum key;
	datum data;
	ostringstream strs;
	strs<<count;
	string count_string = strs.str();
	char *count_char = strdup(count_string.c_str());
	char *pair_char = strdup(pair.c_str());
	key.dptr=pair_char;
	key.dsize=pair.size()+1;
	data.dptr = count_char;
	data.dsize = count_string.size()+1;
	int result = gdbm_store(database,key,data,GDBM_REPLACE);
	assert(result == 0);

	gdbm_close(database);
	//cout<<"Store data ok!!!"<<endl;

}
void import_file_to_db(){
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
}
void open(){
	GDBM_FILE database = gdbm_open("trial.db",512,GDBM_WRCREAT,0600,NULL);
	assert(database!= nullptr);
	cout<<"database OPNE ok!!"<<endl;
	gdbm_close(database);
}

void show(){
	GDBM_FILE database = gdbm_open("db.dir",512,GDBM_READER,0600,NULL);
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
void update_count(GDBM_FILE dbr,string pair,double update_count){
	//GDBM_FILE dbr = gdbm_open("count.db",512,GDBM_WRITER,0,0);
	if(dbr==0){
		cout<<gdbm_strerror(gdbm_errno)<<endl;
	}
	assert(dbr!=0);
	datum key,content;
	
	key.dptr = strdup(pair.c_str());
	key.dsize = pair.size()+1;
	//datum content;
	content = gdbm_fetch (dbr, key);
	if (content.dptr == NULL)
	{
		fprintf(stderr, "key not found\n");
	}
	else
	{
		/* do something with content.dptr */
		//cout<<"I found the data!!!!"<<endl;
		double final_count = update_count+atof(content.dptr);
		bool check_final_count=is_inf_or_nan(final_count);
		//assert(check_final_count!=true);
		if(check_final_count){final_count=fixed_value(final_count);}
		key.dptr = strdup(pair.c_str());
		key.dsize = pair.size()+1;
		ostringstream strs;
		strs<<final_count;
		string count_string = strs.str();
		content.dptr = strdup(count_string.c_str());
		content.dsize = strlen(content.dptr)+1;
		int result = gdbm_store(dbr,key,content,GDBM_REPLACE);
		assert(result == 0);
		free(key.dptr);
		free(content.dptr);
	}
	//gdbm_close(dbr);
}
double get_prob(string pair){
	GDBM_FILE pb_r_db = gdbm_open("prob.db",512,GDBM_READER,0644,NULL);
	if(pb_r_db==0){
		cout<<gdbm_strerror(gdbm_errno)<<endl;
	}

	assert(pb_r_db!=0);
	datum key;
	key.dptr = strdup(pair.c_str());
	key.dsize = pair.size()+1;
	//cout<<"key.dptr:"<<key.dptr<<endl;
	datum content;
	cout<<key.dptr<<";"<<key.dsize<<endl;
	
	content = gdbm_fetch (pb_r_db, key);
	if (content.dptr == NULL)
	{
		fprintf(stderr, "key not found\n");
	}else{
		return atof(content.dptr);
		free(key.dptr);
		free(content.dptr);
	}
	gdbm_close(pb_r_db);
}
double count_sum(GDBM_FILE database,string source){
	//GDBM_FILE database = gdbm_open("trial.db",512,GDBM_READER,0600,NULL);
	assert(database != nullptr);
	double sum=0.0;
	datum key;
	datum content;
	//char* source_char = strdup(source.c_str());
	key = gdbm_firstkey(database);
	while ( key.dptr != nullptr ) {
		datum nextkey;
		char* tmp = key.dptr;
		string tmpstr = string(key.dptr);
		content = gdbm_fetch(database, key);
		//outfile << key.dptr << "\t" << data.dptr << endl;
		vector<string>strs;
		boost::split(strs,tmpstr,boost::is_any_of("|||"));
		//cout<<"key.dptr"<<key.dptr<<endl;
		if( strs[0]==source){
			sum+=atof(content.dptr);
		}
		//free(content.dptr);
		nextkey = gdbm_nextkey(database, key);
		free(content.dptr);
		free(key.dptr);
		FreeAll(strs);
		key = nextkey;
		
	}
	
	//gdbm_close(database);
	bool check_sum_inf=is_inf_or_nan(sum);
	if(check_sum_inf){sum=fixed_value(sum);}
	//assert(check_sum_inf!=true);
	return sum;
}
void  prob_calculation(GDBM_FILE database,GDBM_FILE database2,string source,double sum){
	//GDBM_FILE database = gdbm_open("count.db",512,GDBM_READER,0600,NULL);
	//GDBM_FILE database2 = gdbm_open("prob.db",512,GDBM_WRITER,0600,NULL);
	datum key,key2;
	datum content,content2;
	// char* source_char = strdup(source.c_str());
	key = gdbm_firstkey(database);
	key2 = gdbm_firstkey(database2);
	
	while ( key.dptr != nullptr &&key2.dptr!=nullptr ) {
		
		datum nextkey1,nextkey2;
		string tmpstr = string(key.dptr);
		content = gdbm_fetch(database, key);
		content2 = gdbm_fetch(database,key2);
		vector<string>strs;
		boost::split(strs,tmpstr,boost::is_any_of("|||"));

		if( strs[0] == source){
			double last_prob = atof(content.dptr)/sum;
			bool check_last_prob=is_inf_or_nan(last_prob);
			if(check_last_prob){last_prob=fixed_value(last_prob);}
			//assert(check_last_prob!=true);
			
			assert(last_prob!=0);
			ostringstream strs;
			strs<<last_prob;
			string last_prob_string = strs.str();
			content2.dptr = strdup(last_prob_string.c_str());
			content2.dsize = strlen(content2.dptr)+1;
			int result = gdbm_store(database2,key2,content2,GDBM_REPLACE);
			assert(result == 0);
		}

		nextkey1 = gdbm_nextkey(database, key);
		nextkey2 = gdbm_nextkey(database2, key2);
		FreeAll(strs);
		free(key.dptr);
		free(key2.dptr);
		free(content.dptr);
		free(content2.dptr);
		key = nextkey1;
		key2 = nextkey2;
		
	}
	
}
//====================================

vector<string> string_tokenizer(string str) {
	istringstream f(str);
	vector<string> strings;
	string s;
	while (getline(f, s, ' ')) {
		//cout << s << endl;
		strings.push_back(s);
	}
	return strings;
}
double value_plus(vector <string>&results, string target,vector<double>&db_prob) {

	double res = 0;
	vector<string>::const_iterator iter = results.begin();
	vector<double>::const_iterator iter2 = db_prob.begin();
	for(; (iter!=results.end())&&(iter2!=db_prob.end()); ++iter,++iter2) {
		string key = *iter;
		vector<string> strs;
		boost::split(strs,key, boost::is_any_of("|"));

		if(strs[0] == target) {
			res += *iter2;
		}
		FreeAll(strs);
	}
	
	return res;
}
double log_calculate(vector<double>value) {
	double sum;
	for(int i=0; i<value.size(); i++) {
		sum+=log(value[i]);
	}

	return exp(sum);
}
void value_multi( vector<string>&results,vector<double>&expect_count, string target,double alva) {

	
	//map<string, double>::iterator iter = myMap.begin();
	vector<string>::const_iterator iter = results.begin();
	vector<double>::iterator iter2 = expect_count.begin();
	for(; (iter!=results.end())&&(iter2!=expect_count.end()); ++iter,++iter2) {
		string key = *iter;
		vector<string>strs;
		boost::split(strs,key, boost::is_any_of("|"));

		if(strs[0] == target) {
			//res += iter->second;
			double tmp=(*iter2)*alva;
			bool check_tmp = is_inf_or_nan(tmp);
			if(check_tmp){tmp=fixed_value(tmp);}
			*iter2=tmp;
		}
		FreeAll(strs);
	}
	

	//return res;
}



int main(void)
{

	
	ofstream  debug_file;
	debug_file.open("./debug_file",ios::app);
	for(int loop=0;loop<1;loop++){

		ifstream fin("./europarl_clean_1-5000.en");
		ifstream fin2("./europarl_clean_1-5000.fr");
		while(!(fin.eof()&&fin2.eof())){
			//==================vector declare
			vector<string>results;// sort alignment pair
			vector<double>db_prob;//get probability form database
			vector<string>source; //array store all source word 
			vector<double>expect_count;  //store Pa/value_sum
			//==================
			string english,chinese;
			getline(fin,english);
			getline(fin2,chinese);
			string combine=english+" "+chinese;
			vector<string>corpus_combine=string_tokenizer(combine);
			int combine_size=corpus_combine.size();
			//combine_len=combine_size;
			source.insert(source.end(),corpus_combine.begin(),corpus_combine.end());
			//cout<<"combine.size="<<combine_size<<endl;
			//handler.prob_db_read_open();
			GDBM_FILE pb_r_db = gdbm_open("prob.db",512,GDBM_READER,0644,NULL);
			if(pb_r_db==0){
				cout<<gdbm_strerror(gdbm_errno)<<endl;
			}

			assert(pb_r_db!=0);
			for(int i=0; i<combine_size; i++) {
				for(int j=0; j<combine_size; j++) {
					string result=corpus_combine[i]+"|||"+corpus_combine[j];
					double prob =0.0;
					//for loop , source|target
					datum key;
					key.dptr = strdup(result.c_str());
					key.dsize = result.size()+1;
					
					datum content;
					content = gdbm_fetch (pb_r_db, key);
					if (content.dptr == NULL)
					{
						fprintf(stderr, "key not found\n");
					}else{
						prob=atof(content.dptr);
						free(key.dptr);
					}
					
					results.push_back(result);
					assert(prob!=0);
					db_prob.push_back(prob);

				}
			}
			gdbm_close(pb_r_db);
			//handler.prob_db_read_close();
			//----
			//lunda calculation
			
			vector<double>lunda;
			for(int i=0; i<source.size(); i++) {
				double sum=value_plus(results, source[i],db_prob);
				
				assert(sum!=0);
				lunda.push_back(sum);
			}
			for(int i=0;i<lunda.size();i++){
				/* if(lunda[i]==0){
				debug_file<<"source[i]"<<endl;
				}*/

				cout<<"source[i]="<<source[i]<<",lunda[i]"<<lunda[i]<<endl;
			}
			//-------
			//pa calculation
			double Pa=log_calculate(lunda);
			cout<<"pa="<<Pa<<endl;
			//----------
			//alva calculation
			expect_count.insert(expect_count.end(),db_prob.begin(),db_prob.end()); //copy probability into expect_count vector
			for(int i=0; i<source.size(); i++) {
				value_multi(results,expect_count,source[i],1/lunda[i]);
			}
			GDBM_FILE dbr = gdbm_open("count.db",512,GDBM_WRITER,0,0);
			for(int i=0 ;i<expect_count.size();i++){
				
				if(1.7e-307 >expect_count[i]){
					expect_count[i]=1.7e-307;
				}
				
				
				bool check_expect_count=is_inf_or_nan(expect_count[i]);	
				if(check_expect_count){expect_count[i]=fixed_value(expect_count[i]);}
				
				// assert(insert_expect_count!=0);
				update_count(dbr,results[i],expect_count[i]);
				assert(expect_count[i]!=0);
			}
			gdbm_close(dbr);
			FreeAll(results);
			FreeAll(db_prob);
			FreeAll(source); 
			FreeAll(expect_count);  
			FreeAll(corpus_combine);
			FreeAll(lunda);				
			cout<<"================================="<<endl;
		}
		ifstream fin3("./unique_source");
		vector<string>source;
		while(!fin3.eof()){
			string uni_source;
			getline(fin3,uni_source);
			vector<string>single_source=string_tokenizer(uni_source);
			for(int i=0;i<single_source.size();i++){
				source.push_back(single_source[i]);
			}
                        FreeAll(single_source);
		}
		vector<double>sum_of_count;
		GDBM_FILE database_count = gdbm_open("count.db",512,GDBM_READER,0600,NULL);
		for(int i=0;i<source.size();i++){
			double total = count_sum(database_count,source[i]);
			sum_of_count.push_back(total);
		}
		
		GDBM_FILE database_prob = gdbm_open("prob.db",512,GDBM_WRITER,0600,NULL);
		for(int i=0;i<source.size();i++){
			prob_calculation(database_count,database_prob,source[i],sum_of_count[i]);
		}
		gdbm_close(database_count);
		gdbm_close(database_prob);

		if(loop%10==0){  
			show_and_dumpfile();
		}
		//FreeAll(single_source);
		FreeAll(source);
		FreeAll(sum_of_count);
		// close_connection();
		cout << endl;
		// exit (EXIT_FAILURE);
		
	}
	return EXIT_SUCCESS;
}
