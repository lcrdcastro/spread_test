#include <iostream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;

Json::Value json_decode(string text){
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(text, root);

    if (!parsingSuccessful){
        cout << "Erro" << text << endl;
    }

    return root;
}

static size_t writeCallback(char* contents, size_t size, size_t nmemb, char* buffer_in){
    ((string*) buffer_in)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string getJsonFromApi(string url){
    CURL *curl;

    CURLcode res;

    string readBuffer;

    curl = curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res!=0){
            cout << "Error" << endl;
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}



int main(int argv, char** argc){   
        
    //Json::Value rootJsonValue;
    //rootJsonValue["mensagem"] = 

    //http://www.omdbapi.com/?t=fight+club&apikey=c2d2794b
    
    string url = (string)"http://www.omdbapi.com/?t=" + (string)"fight+club" + (string)"&apikey=c2d2794b";
    string readBuffer;

    readBuffer = getJsonFromApi(url);
    Json::Value result = json_decode(readBuffer);
    //cout << result.toStyledString();
    vector<string> members = result.getMemberNames();

    for (unsigned int i=0; i< members.size();i++){
        if (result[members[i]].size()>0){
            cout << members[i] << ":"<< endl;
            Json::Value ratings = result[members[i]];

            for (unsigned int j=0;j<ratings.size();j++){
                cout << "    " << (ratings[j])["Source"].asString() << " - "<< (ratings[j])["Value"].asString() << endl;
            }
        }
        else{
            cout << members[i] << " - " << result[members[i]].asString() <<  endl;
        }
    }

    
    //cout << readBuffer << endl;
    // cout << "number of arguments " << argv << " arguments: " <<  endl;
    // for (int i=0;i< sizeof(argc);i++)
    // {
    //     cout<< argc[i] << endl;
    // }

    return 0;
}