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

void printDatafromJson(Json::Value data, bool checkType)
{
    vector<string> members = data.getMemberNames();
    if (members.size()<=2){
        checkType = false;
    }
    if (!checkType){
        for (unsigned int i=0; i< members.size();i++){
            if (data[members[i]].size()>0){
                cout << members[i] << ":"<< endl;
                Json::Value ratings = data[members[i]];

                for (unsigned int j=0;j<ratings.size();j++){
                    cout << "    " << (ratings[j])["Source"].asString() << " - ";
                    cout << (ratings[j])["Value"].asString() << endl;
                }
            }
            else{
                cout << members[i] << " - " << data[members[i]].asString() <<  endl;
            }
        }
    }
    else{
        cout << data["Title"] << " - " << data["Type"] << endl;
    }
}

void printHelp(){
    cout << "Usage:" << endl;
    cout << "    moviesearch -s PATTERN" << endl;
    cout << "    moviesearch --search PATTERN" << endl;
    cout << "    moviesearch -s PATTERN -t" << endl;
    cout << "    moviesearch --search PATTERN -type" << endl << endl;
    cout << "Where:" << endl;
    cout << "    PATTERN - name of a movie in double quotes  Ex: \"Fight Club\"." << endl;
    cout << "    -t, --type - Returns the type of media Ex: movie , series, ... " << endl;

}

int main(int argv, char** argc){   
    string movieName;
    bool checkType = false;
    if (argv>1){
        for (int i=0;i<argv;i++){
            string arg = argc[i]; 
            if (arg.compare("-h") ==0 || arg.compare("--help") ==0){
                printHelp();
                exit(0);
            }
            if (arg.compare("-s") ==0 || arg.compare("--search") ==0){
                i++;
                movieName = argc[i];
            }
            if (arg.compare("-t") ==0 || arg.compare("--type") ==0){
                checkType = true;
            }
        }
    }
    else{
        cout << "Usage: moviesearch -s PATTERN -t" << endl;
        cout << "Try: moviesearch --help for more information." << endl;
        exit(0);

    }
    
    
    string url = (string)"http://www.omdbapi.com/?t=" + movieName + (string)"&apikey=c2d2794b";
    string readBuffer;

    readBuffer = getJsonFromApi(url);
    Json::Value result = json_decode(readBuffer);
    printDatafromJson(result,checkType);
    return 0;
}
