#include <iostream>
#include <map>
#include <string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<fstream>
#include <boost/algorithm/string.hpp>
#include<cmath>
using namespace std;
/*
ofstream  file;
	//file.open("/home/nlp/em_co/output",ios::app);
	file.open("./em_co/output",ios::app);
	file <<"strs[0]:"<<strs[0]<<"res:"<<res<<endl;
	file.close();
*/

double value_plus(const map<string, double>& myMap, string target) {

	double res = 0;
	map<string, double>::const_iterator iter = myMap.begin();
	for(; iter!=myMap.end(); ++iter) {
		string key = iter->first;
		vector<string> strs;
		boost::split(strs,key, boost::is_any_of("|"));

		if(strs[0] == target) {
			res += iter->second;
		}
	}

	return res;
}
void value_multi( map<string, double>& myMap, string target,double alva) {

	double res = 0;
	map<string, double>::iterator iter = myMap.begin();
	for(; iter!=myMap.end(); ++iter) {
		string key = iter->first;
		vector<string> strs;
		boost::split(strs,key, boost::is_any_of("|"));

		if(strs[0] == target) {
			//res += iter->second;
			double tmp=iter->second*alva;
			iter->second=tmp;
		}
	}

	//return res;
}
double update_count( map<string, double>& myMap, string target,double count) {

	double res=0;
	map<string, double>::iterator iter = myMap.begin();
	for(; iter!=myMap.end(); ++iter) {
		string key = iter->first;
		if(key==target) {
			double tmp=iter->second+count;
			iter->second=tmp;
			res=iter->second;
		}
	}
    return res;
}
double cal_prob( map<string, double>& myMap, string target) {
	
	double sum=0;
	double res=0;
	map<string, double>::iterator iter = myMap.begin();
    map<string, double>::iterator iter2 = myMap.begin(); 
	for(; iter!=myMap.end(); ++iter) {                               //prob p(t|s)=c(t and s)/c(s)
		string key = iter->first;
		vector<string> strs;
		boost::split(strs,key, boost::is_any_of("|"));
		vector<string> strs2;
		boost::split(strs2,target, boost::is_any_of("|"));
		if(strs[0] == strs2[0]) {
			sum+=iter->second;                                       //add all count who has same source
		}
	}
	for(; iter2!=myMap.end(); ++iter2) {
		string key = iter2->first;
		if(key==target) {
			res=iter2->second/sum;                                  //count/sigma(count)
			//iter2->second=tmp;
		}
	}
	return res;
    
}
void update_prob( map<string, double>& myMap, string target,double prob) {

	map<string, double>::iterator iter = myMap.begin();
	for(; iter!=myMap.end(); ++iter) {
		string key = iter->first;
		if(key==target) {
			iter->second=prob;			                         //update count to myMap
		}
    }
}


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
double log_calculate(vector<double>value) {
	double sum;
	for(int i=0; i<value.size(); i++) {
		sum+=value[i];
	}
	//double log_parm=log1p(sum);
	double log_parm=log(sum);
	return exp(log_parm);
}

int main() {
	/*ofstream  file;
	//file.open("/home/nlp/em_co/output",ios::app);
	file.open("./em_co/output",ios::app);*/

	string english ="This is a book This is a pen";
	string chinese="這 是 一 本 書 這 是 一 支 筆";
	string combine=english+" "+chinese;
	vector<string>corpus_combine=string_tokenizer(combine); //corpus_combine store english and chinese corpus into vector
	int combine_size = corpus_combine.size();

	vector<string>results;
	for(int i=0; i<combine_size; i++) {
		for(int j=0; j<combine_size; j++) {
			string result=corpus_combine[i]+"|"+corpus_combine[j];       //for loop , source|target
			results.push_back(result);
		}
	}
	sort(results.begin(),results.end());
	unique(results.begin(),results.end());
	vector<int>count_value;
	for(int l=0; l<results.size(); l++) {
		int mycount=count(results.begin(),results.end(),results[l]);
		count_value.push_back(mycount);                       //store count for each pair
	}
	// count the prob
	vector<double>value;
	for(int m=0; m<count_value.size(); m++) {                            
		double prob=(double)count_value[m]/(double)combine_size;           
		value.push_back(prob);
	}
	//===============================================initialize


	vector<string>key;

	key=results;


	map<string, double> myMap;
	for(int i = 0; i < key.size(); i++) {
		myMap.insert(std::pair<string, double>(key[i], value[i]));
	}
	//============ print the map after initialize
	for(map<string, double>::const_iterator  it = myMap.begin();
	        it != myMap.end(); ++it) {
		cout << it->first << " " << it->second <<  "\n";
	}

	//==============
      vector<double>after_count;
//em loop start
	//pa caculate
	for(int em_loop=1; em_loop<=3; em_loop++) {
		cout<<"em loop number:"<<em_loop<<"===================="<<endl;
		vector<double>value_sum;
		for(int i=0; i<corpus_combine.size(); i++) {
			//cout<<"copus_combine:"<<corpus_combine[i]<<endl;
			double sum=value_plus(myMap, corpus_combine[i]);
			value_sum.push_back(sum);
		}
		double Pa=log_calculate(value_sum);
		cout<<"Pa="<<Pa<<endl;

		for(int i=0; i<corpus_combine.size(); i++) {
			value_multi(myMap,corpus_combine[i],Pa/value_sum[i]);
		}
		//============print the original_prob*(pa/value_sum)  -->alva
		cout<<"after caculate alva value =================================="<<endl;
		for(map<string, double>::const_iterator  it = myMap.begin();
		        it != myMap.end(); ++it) {
			cout << it->first << " " << it->second <<  "\n";
		}
		//============ caculate  original count+ + original_prob*(pa/value_sum)
		
		for(int i=0; i<key.size(); i++) {
			if(em_loop==1) {          //plus initialize count
				double new_count=update_count(myMap,key[i],(double)count_value[i]);
				after_count.push_back(new_count);
			} else {                  //plus last time count and store result to vector for next time add
				double next_count=update_count(myMap,key[i],(double)after_count[i]);
				after_count[i]=next_count;
			}

		}

		//============print the map         // show count after em
		cout<<"third map =================================="<<endl;
		for(map<string, double>::const_iterator  it = myMap.begin();
		        it != myMap.end(); ++it) {
			cout << it->first << " " << it->second <<  "\n";
		}
		//============
		vector<double>new_prob;
		for(int i=0; i<key.size(); i++) {
			double prob=cal_prob(myMap,key[i]);            
			new_prob.push_back(prob);
		}
		for(int i=0;i<key.size();i++){
			update_prob(myMap,key[i],new_prob[i]);
		}
		cout<<"forth map =================================="<<endl;  //calculate prob
		for(map<string, double>::const_iterator  it = myMap.begin();
		        it != myMap.end(); ++it) {
			cout << it->first << " " << it->second <<  "\n";
		}

	}

	//cout<<"front_element = This "<<value_plus(myMap, "This")<<endl;
	//cout<<"front_element = is "<<value_plus(myMap, "is")<<endl;
	return 0;
}
