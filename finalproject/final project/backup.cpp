#include<fstream>
#include<string>
#include <algorithm>
#include<cstring>
#include<vector>
#include<iostream>
#include<sys/stat.h>
#include <unordered_map>
#include <set>
#include <chrono>
#include <map>
using namespace std;
//qq
struct Trie {
    bool flag;
    struct Trie* son[26]={NULL};
    Trie() {
        flag = false;
    }
};
Trie* NewTrie() {
    Trie* temp = new Trie;
    return temp;
}
void Insert(Trie* root, string s) {
    Trie* temp = root;
    for(char ss:s)
	{
		if(ss<96) ss = ss+32;
        if (temp->son[ss - 'a'] == NULL) {
            temp->son[ss - 'a'] = NewTrie();
        }
        temp = temp->son[ss - 'a'];
    }
    temp->flag = true;
}
bool findTrie_pre(Trie* root, string pre)
{
	Trie* p = root;
	for(char c : pre)
	{
		if(p->son[c-'a']==NULL) return false;
		p = p->son[c-'a'];
	}
	return true;
}
bool findTrie_exat(Trie* root, string str)
{
    Trie* p = root;
    for (char ss:str) 
	{
        if (p->son[ss-'a'] == NULL)  //若為空集，表示不存以此為字首的串
            return false;
		p = p->son[ss - 'a'];
    }
	
    return p->flag;  //此串是字符集中某串的字首
}
bool findTrie_suff(Trie* root, string suff)
{
	Trie* p = root;
	for (int i = suff.length()-1;i>=0;i--) 
	{
        if (p->son[suff[i]-'a'] == NULL)  //若為空集，表示不存以此為字首的串
            return false;
		p = p->son[suff[i] - 'a'];
    }
    return true;  //此串是字符集中某串的字首
}
//qq
bool exists(const std::string& name)
{
	struct stat buffer;
	return(stat(name.c_str(),&buffer)==0);
}
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word)
		{
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}
vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}
bool Check(string str, Trie* Troot, Trie* RTroot)
{
	if(str[0]=='"') 
	{
		string copy(str);
		copy.erase(0,1);
		copy.pop_back();
		return findTrie_exat(Troot, copy);
	}
	else if(str[0]=='*') 
	{
		string copy(str);
		copy.erase(0,1);
		copy.pop_back();
		return findTrie_suff(RTroot, copy);
	}
	else if(str[0]=='@')
	{
		return false;
	}
	else if(str[0]=='~')
	{
		return true;
	}
	else return findTrie_pre(Troot, str);
}

Trie* Triemap[100001];
Trie* RTriemap[100001];
int main(int argc, char *argv[])
{
	//看跑多久
	double START,END; START = clock();
    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route
    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);
	// by me      get queries
	string tmpq;
	fstream qfi, fileout;
	
	fileout.open(output, ios::out | ios::trunc);
	qfi.open(query, ios::in);

	// Read File & Parser Example
	string title_name, tmp;
	fstream fi;
	vector<string> tmp_string, nowq_vector, nice_q_vec;
	int dataid=0, maxid=0;
	int haveans = 0;
	int beenhere =0;
	while(getline(qfi, tmpq))
	{
		nowq_vector = split(tmpq, " ");
		nice_q_vec = nowq_vector;
		while(dataid<maxid||exists(data_dir+to_string(dataid)+".txt")) // from data_dir get file ....
		{
			fi.open(data_dir+to_string(dataid)+".txt", ios::in);
			if(beenhere==0)
			{
				maxid++;
				Trie* nowTrie = NewTrie();
				Trie* nowrTrie = NewTrie();
			
				while(getline(fi, tmp))
				{
					// GET CONTENT WORD VECTOR
					tmp_string = split(tmp, " ");

					// PARSE CONTENT
					vector<string> content = word_parse(tmp_string);
					
					for(auto str:content)
					{
						string copy(str);
						reverse(copy.begin(), copy.end());
						Insert(nowTrie, str);
						Insert(nowrTrie, copy);
					}
				}
				Triemap[dataid]= nowTrie;
				RTriemap[dataid] =nowrTrie;
			}
			if(nowq_vector.size()==1)
			{
				if(Check(nowq_vector[0], Triemap[dataid], RTriemap[dataid]))
				{
					//cout<<dataid<<endl;
					fi.clear();
					fi.seekg(0);
					getline(fi, title_name);
					// GET TITLENAME WORD ARRAY
					
					fileout<<title_name<<endl;
					//cout<<title_name<<endl;
					haveans++;
				}
			}
			else
			{
				for(int i = 1;i<nowq_vector.size();i=i+2)
				{
					if(nowq_vector[i]=="+")
					{
						nowq_vector[i+1] = (Check(nowq_vector[i-1], Triemap[dataid], RTriemap[dataid])&&Check(nowq_vector[i+1], Triemap[dataid], RTriemap[dataid]))?"~":"@";
					}
					else
					{
						nowq_vector[i+1] = (Check(nowq_vector[i-1], Triemap[dataid], RTriemap[dataid])||Check(nowq_vector[i+1], Triemap[dataid], RTriemap[dataid]))?"~":"@";
					}
				}
				if(Check(nowq_vector[nowq_vector.size()-1], Triemap[dataid], RTriemap[dataid]))
				{
					//cout<<dataid<<endl;
					fi.clear();
					fi.seekg(0);
					getline(fi, title_name);
					// GET TITLENAME WORD ARRAY
					fileout<<title_name<<endl;
					//cout<<title_name<<endl;
					haveans++;
				}
			}

			
			dataid++;
			fi.close();
			nowq_vector = nice_q_vec;
		}
		if(haveans ==0) 
		{
			//cout<<"Not Found!"<<endl;
			fileout<<"Not Found!"<<endl;
		}
		dataid=0;
		haveans=0;
		beenhere++;
	}
	qfi.close();
	fileout.close();
	//看跑多久
	END = clock();
	cout << (END - START) / CLOCKS_PER_SEC << endl;
}
