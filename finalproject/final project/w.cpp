#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<algorithm>
//#include <experimental/filesystem>
#include <chrono>
using namespace std::chrono;
#define alphabet_size 26

using namespace std;
//namespace fs = std::experimental::filesystem;
enum{
    Exact_Search = 0, Prefix_Search = 1, Suffix_Search = 2, And_oper = 3, Or_oper = 4
};
     

class Trie{
    public:
        Trie* children[alphabet_size];
        Trie* parent[alphabet_size];
        int* depth[alphabet_size];
        vector<string> root_name;
        bool isEnd;
        Trie(){
            this->isEnd = false;
            for(int i = 0;i < alphabet_size;i ++){
                this->children[i] = nullptr;
                this->parent[i] = nullptr;
                this->depth[i] = 0;
            }
        }
        void insert(string s);
        bool search(string s);
        bool search_prefix(string s);
        bool search_suffix(string s);

};
void Trie::insert(string s){
    Trie* curr = this;
    for(int i = 0;i < s.length();i ++){
        if(s[i] >= 'a' && s[i] <= 'z'){
            if(curr->children[s[i] - 'a'] == nullptr){
                curr->children[s[i] - 'a'] = new Trie();
            }
            curr = curr->children[s[i] - 'a'];
        }
        else{
            if(curr->children[s[i] - 'A'] == nullptr){
                curr->children[s[i] - 'A'] = new Trie();
            }
            curr = curr->children[s[i] - 'A'];
        }
        
        
    }
    curr->isEnd = true;
}
bool Trie::search(string s){
    Trie* curr = this;
    for(int i = 0; i< s.length();i ++){
        if(s[i] >= 'a' && s[i] <= 'z')
            curr = curr->children[s[i] - 'a'];
        else if(s[i] >= 'A' && s[i] <= 'Z')
            curr = curr->children[s[i] - 'A'];
        if(curr == nullptr)return false;
    }
    return curr->isEnd;
}
bool Trie::search_prefix(string s){
    Trie* curr = this;
    for(int i = 0; i< s.length();i ++){
        if(s[i] >= 'a' && s[i] <= 'z')
            curr = curr->children[s[i] - 'a'];
        else if(s[i] >= 'A' && s[i] <= 'Z')
            curr = curr->children[s[i] - 'A'];
        if(curr == nullptr)return false;
    }
    return true;
}
bool Trie::search_suffix(string s){
    if(s.size() == 1 && s[s.at(0)] == '*')return true;
    Trie* curr = this;
    if(s[s.at(0)] == '*')
    {
        s = s.substr(1);
        
    }
    
    else if(s[s.at(0)] >= 'a' && s[s.at(0)] <= 'z'){
        if(curr->children[s[s.at(0)] - 'a']){
            return curr->children[s[s.at(0)] - 'a']->search_suffix(s.substr(1));
        }
        else
            return false;
    }
    else{
        if(curr->children[s[s.at(0)] - 'A']){
            return curr->children[s[s.at(0)] - 'A']->search_suffix(s.substr(1));
        }
        else
            return false;
    }
    return true;
    
}
// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
    vector<string> parse_string;
    for(auto& word : tmp_string){
        string new_str;
        for(auto &ch : word){
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


int main(int argc, char *argv[])
{

    // INPUT :
    // 1. data directory in data folder
    // 2. number of txt files
    // 3. output route
    auto start = high_resolution_clock::now();
    string data_dir = argv[1] + string("/");
    string query = string(argv[2]);
    string output = string(argv[3]);
    //string data_dir = "data/";
    //string query = "query-more";
    //string output = string(argv[3]);
    // Read File & Parser Example


    string file, title_name, tmp;
    fstream fi;
    vector<string> tmp_string;
    vector<Trie>data;
    vector<Trie>reverse_data;
    // from data_dir get file ....
    // eg : use 0.txt in data directory
    int num = 0;
    int a = 1000;
    int t = 0;
    //
    //file = data_dir + to_string(num);
    int aa = 99;
    while(aa--){
    //for(int i = 0;i < a;i ++){
        Trie* new_data = new Trie;
        Trie* rev_data = new Trie;
        //fi.open(file, ios::in);
        string num_str = to_string(t);
        t++;
        file = data_dir + num_str +".txt";
        fi.open(file, ios::in);
        // GET TITLENAME
        getline(fi, title_name);
        // GET TITLENAME WORD ARRAY
        tmp_string = split(title_name, " ");
        vector<string> title = word_parse(tmp_string);
        new_data->root_name = title;
        rev_data->root_name = title;
        //cout << "test" << endl;
        //for(int j = 0;j < new_data->root_name.size();j++){
        //    cout << new_data->root_name[j];
        //}
        //cout << endl;
        // GET CONTENT LINE BY LINE
        while(getline(fi, tmp)){

        // GET CONTENT WORD VECTOR
            tmp_string = split(tmp, " ");

            // PARSE CONTENT
            vector<string> content = word_parse(tmp_string);
            for(int i = 0;i < content.size();i ++){
                char* strs = new char[content[i].length() + 1];
                strcpy(strs, content[i].c_str());
                reverse(&strs[0], &strs[content[i].length()]);
                //cout << "content: "<<content[i] << endl;
                //cout << "strs: "<<strs << endl;
                new_data->insert(content[i]);
                rev_data->insert(strs);
            }
            
        }
        // CLOSE FILE
        fi.close();
        data.push_back(*new_data);
        reverse_data.push_back(*rev_data);
        num++;
        //file = data_dir + to_string(num);
    }
    //}
    //read in query
    vector<string> order;
    int* record = (int*)malloc(sizeof(int)*data.size());
    
    string query_name = query;
    fi.open(query_name, ios::in);
    while (getline(fi, tmp)) {
        order = split(tmp, " ");
        //vector<string>order = word_parse(query_str);
        vector<int>ans;
        memset(record, 0, sizeof(data.size()));
        string now;
        for(int i = 0;i < order.size();i ++){
            cout << order[i] << endl;
        }
        for(int i = 0;i < order.size();i ++){
            //for(int j = 0;j < data.size();j ++){
            //    cout << record[j] << " " << j << endl;
            //}
            now = order[i];
            
            if(order[i] == "+"){
                //printf("hello\n");
                i++;
                now = order[i];
                
                if(now[0] == '"'){
                    for(int j = 0;j < data.size();j ++){
                        if(record[j] == 1){
                            if(data[j].search(now)){
                                record[j] = 1;
                            }
                                
                            else
                                record[j] = 0;
                        }
                        else
                            continue;
                    }
                }
                //suffix
                else if(now[0] == '*'){
                    char* strs = new char[now.length()];
                    strcpy(strs, now.c_str());
                    reverse(&strs[0], &strs[now.length()]);
                    for(int j = 0;j < reverse_data.size();j ++){
                        if(record[j] == 1){
                            
                            if(reverse_data[j].search_prefix(strs)){
                                record[j] = 1;
                                //printf("hello\n");
                            }
                                
                            else
                                record[j] = 0;
                        }
                        else
                            continue;
                        
                    }
                }
                //prefix
                else{
                    for(int j = 0;j < data.size();j ++){
                        if(record[j] == 1){
                            if(data[j].search_prefix(now)){
                                record[j] = 1;
                            }
                            else
                                record[j] = 0;
                        }
                        else
                            continue;
                    }
                }
            }
            else if(order[i] == "/"){
                i++;
                now = order[i];
                if(now[0] == '"'){
                    for(int j = 0;j < data.size();j ++){
                        if(data[j].search(now)){
                            record[j] = 1;
                            
                        }

                    }
                }
                //suffix
                else if(now[0] == '*'){
                        //cout << "test" << endl;
                        //cout << now << endl;
                    char* strs = new char[now.length()];
                    strcpy(strs, now.c_str());
                    reverse(&strs[0], &strs[now.length()]);
                    for(int j = 0;j < reverse_data.size();j ++){
                        
                        if(reverse_data[j].search_prefix(strs))
                            record[j] = 1;
                            //printf("hello\n");
                    }
                }
                //prefix
                else{
                    for(int j = 0;j < data.size();j ++){
                        if(data[j].search_prefix(now))
                            record[j] = 1;
                    }
                }
            }
            else{
                //exact search
                if(now[0] == '"'){
                    for(int j = 0;j < data.size();j ++){
                        if(data[j].search(now)){
                            record[j] = 1;
                        }
                        else
                            record[j] = 0;
                    }
                }
                //suffix
                else if(now[0] == '*'){
                    char* strs = new char[now.length()];
                    strcpy(strs, now.c_str());
                    reverse(&strs[0], &strs[now.length()]);
                    for(int j = 0;j < reverse_data.size();j ++){
                        if(reverse_data[j].search_prefix(strs)){
                            record[j] = 1;
                            //printf("hello\n");
                        }
                            
                        else
                            record[j] = 0;
                    }
                }
                //prefix
                else{
                    for(int j = 0;j < data.size();j ++){
                        if(data[j].search_prefix(now)){
                            record[j] = 1;
                            
                        }
                            
                        else
                            record[j] = 0;
                    }
                }
                
            }
            
        }
        for(int i = 0;i < data.size();i ++){
            if(record[i])
            ans.push_back(i);
        }
        fstream fileout;
        fileout.open(output, ios::out | ios::trunc);
        if(ans.size() == 0)
            fileout << "Not Found!" << endl;
        for(int i = 0;i < ans.size();i ++){
            for(int j = 0;j < data[ans[i]].root_name.size();j ++)
                fileout << "a" << " ";
            fileout << endl;
            
        }
        //cout << endl;
        fileout.close();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
  
    // To get the value of duration use the count()
        // member function on the duration object
        //cout << duration.count()/CLOCKS_PER_SEC << endl;
    }
    

    

    // for(auto &word : title){
    //     cout << word << endl;
    // }

    
    

    
    
}