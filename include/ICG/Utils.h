#include <Common/Common.h>

using namespace std;

/*分隔符为 $$$$$$$$$ */

string ctx_encoder(vector<string> & values){
    string ret="";
    for(auto s:values){
        ret+=s;
        ret+=DELIMITER;
    }
    return ret;
}

std::shared_ptr<vector<string>> ctx_decoder(string s){
    string each;
    stringstream ss;
    ss<<s;

    std::shared_ptr<vector<string>> ret=make_shared<vector<string>>();

    while(getline(ss,each,DELIMITER)){
        ret->push_back(each);
    }
    return ret;
}