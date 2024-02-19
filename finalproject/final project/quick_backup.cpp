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
struct QNode {
    string data;
    QNode *left, *right;
};
QNode* newNode(string d)
{
    QNode* node = new QNode();
    node->data = d;

    node->left = node->right = NULL;
    return node;
}
QNode* BuildQTree(vector<string> qvecs)
{
	if(qvecs.size()==1)
	{
	
		return newNode(qvecs[0]);
	}
	QNode* root = newNode(qvecs[qvecs.size()-2]);
	QNode* right = newNode(qvecs[qvecs.size()-1]);
	qvecs.pop_back();qvecs.pop_back();
	QNode* left = BuildQTree(qvecs);
	root->right = right; root->left = left;
	
	return root;
}
//qq
struct Trie {
    set<int> flag;
	set<int> whose;
    struct Trie* son[26]={NULL};
    Trie() 
	{
        flag = {};
		whose = {};
    }
};
Trie* NewTrie() 
{
    Trie* temp = new Trie;
    return temp;
}
void Insert(Trie* root, string s, int who) {
    Trie* temp = root;
    for(char ss:s)
	{
		if(ss<96) ss = ss+32;
        if (temp->son[ss - 'a'] == NULL) 
		{	
            temp->son[ss - 'a'] = NewTrie();
			
			temp->son[ss - 'a']->whose.insert(who);
        }
		else temp->son[ss - 'a']->whose.insert(who);
        temp = temp->son[ss - 'a'];
		
    }
    temp->flag.insert(who);
}
set<int> findTrie_pre(Trie* root, string pre)
{
	Trie* p = root;
	for(char c : pre)
	{
		if(p->son[c-'a']==NULL) return {};
		p = p->son[c-'a'];
	}
	return p->whose;
}
set<int> findTrie_exat(Trie* root, string str)
{
    Trie* p = root;
	
    for (char ss:str) 
	{
        if (p->son[ss-'a'] == NULL)  //若為空集，表示不存以此為字首的串
            return {};
		p = p->son[ss - 'a'];
    }
	
    return p->flag;  //此串是字符集中某串的字首
}
set<int> findTrie_suff(Trie* root, string suff)
{
	Trie* p = root;
	
	for (int i = suff.length()-1;i>=0;i--) 
	{
        if (p->son[suff[i]-'a'] == NULL)  //若為空集，表示不存以此為字首的串
            return {};
		p = p->son[suff[i] - 'a'];
    }
    return p->whose;  //此串是字符集中某串的字首
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
set<int> Check(QNode* Qroot, Trie* Troot, Trie* RTroot)
{
	if(Qroot->data=="+")
	{
		set<int> tmpc, tmpa, tmpb;
		tmpa =  Check(Qroot->left, Troot, RTroot);
		tmpb = Check(Qroot->right, Troot, RTroot);
		set_intersection(tmpa.begin(),tmpa.end(),tmpb.begin(),tmpb.end(),inserter(tmpc, tmpc.end()));
		return tmpc;
	}
	else if(Qroot->data=="/")
	{
		set<int> tmpc, tmpa, tmpb;
		tmpa =  Check(Qroot->left, Troot, RTroot);
		tmpb = Check(Qroot->right, Troot, RTroot);
		set_union(tmpa.begin(),tmpa.end(),tmpb.begin(),tmpb.end(),inserter(tmpc, tmpc.end()));
		return tmpc;
	}
	else
	{
		if(Qroot->data[0]=='"') 
		{
			string copy(Qroot->data);
			copy.erase(0,1);
			copy.pop_back();
			return findTrie_exat(Troot, copy);
		}
		else if(Qroot->data[0]=='*') 
		{
			string copy(Qroot->data);
			copy.erase(0,1);
			copy.pop_back();
			return findTrie_suff(RTroot, copy);
		}
		else return findTrie_pre(Troot, Qroot->data);
	}
}

Trie* trie=NewTrie();
Trie* R_trie=NewTrie();
string title[100001];
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
	string tmpq;
	fstream qfi, fileout;
	
	fileout.open(output, ios::out | ios::trunc);
	qfi.open(query, ios::in);

	// Read File & Parser Example
	string tmp;
	fstream fi;
	vector<string> tmp_string, nowq_vector;
	int dataid=0;
	set<int> ans;
	while(exists(data_dir+to_string(dataid)+".txt")) // from data_dir get file ....
	{
		
		fi.open(data_dir+to_string(dataid)+".txt", ios::in);
		//
		getline(fi, tmp);
		title[dataid]=tmp;
		tmp_string = split(tmp, " ");
		// PARSE CONTENT
		vector<string> content = word_parse(tmp_string);
		for(auto str:content)
		{
			string copy(str);
			reverse(copy.begin(), copy.end());
			Insert(trie, str, dataid);
			Insert(R_trie, copy, dataid);
		}
		//
		while(getline(fi, tmp))
		{
			// GET CONTENT WORD VECTOR
			tmp_string = split(tmp, " ");
			// PARSE CONTENT
			content = word_parse(tmp_string);
			for(auto str:content)
			{
				string copy(str);
				reverse(copy.begin(), copy.end());
				Insert(trie, str, dataid);
				Insert(R_trie, copy, dataid);
			}
		}
		dataid++;
		fi.close();
	}
	while(getline(qfi, tmpq))
	{
		nowq_vector = split(tmpq, " ");
		QNode *nowqroot = BuildQTree(nowq_vector);
		ans = Check(nowqroot, trie, R_trie);
		if(ans.empty()) 
		{
			fileout<<"Not Found!"<<endl;
		}
		else
		{
			for(int i : ans)
			{
				fileout<<title[i]<<endl;
			}
		}
		ans.clear();
	}
	qfi.close();
	fileout.close();
	//看跑多久
	END = clock();
	cout << (END - START) / CLOCKS_PER_SEC << endl;
}
