#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <regex>
#include <map>
#include <stdio.h>

int action(){

    std::string ser;
    //ser = " cat text.cpp|who|uniq |sort | wc -l ";
    char omar[1024]; //= (char *) " cat 4.txt|who|uniq |sort | wc -l ";
    std::cin.getline(omar, 1024);
    std::regex pipers("\\b[^|]+\\b");
    ser = omar;


    auto words_begin =
        std::sregex_iterator(ser.begin(),ser.end(),pipers);
    auto words_end = std::sregex_iterator();

    std::smatch sm;
    std::vector<std::string> veccommand;

    for(std::sregex_iterator i = words_begin;i!=words_end;++i){
        std::smatch a=*i;
        veccommand.emplace(veccommand.end(), a.str());

    }

    std::copy(veccommand.begin(), veccommand.end(),
              std::ostream_iterator<std::string>{std::cout," "});

    std::cout <<'\n';

    std::vector<std::vector<std::string>> resultvector;

    std::regex com("\\S+");

    for(auto it = veccommand.begin(); it!= veccommand.end(); it++){
        auto w_begin = std::sregex_iterator(it->begin(), it->end(),com);
        auto w_end = std::sregex_iterator();

        bool flag=0;

        for(auto i = w_begin; i!=w_end; ++i){
            std::smatch matches=*i;

            if (flag == 0){
                //std::cout <<matches.str() <<" ";
                resultvector.emplace(resultvector.end(),
                                     std::vector<std::string>(0));
                resultvector.back().push_back(matches.str());
                flag=1;
            }else{
                resultvector.back().push_back(matches.str());
                //std::cout <<matches.str() <<" ";
            }
        }
        //std::cout <<'\n';
    }

    for(auto &a:resultvector){
        for(auto &b:a){
            std::cout <<b <<" ";
        }
        std::cout <<"\n";
    }


    pid_t parent = getpid();

    for(int i=0; i < resultvector.size()-1; i++){

        int pipefd[2];
        pipe(pipefd);


        char* args[resultvector[i].size()+1];
        for (int k=0;k < resultvector[i].size(); ++k){
            std::cout <<k <<" ";
            args[k] = (char *)resultvector[i][k].c_str();
            //std::cout <<args[i] << " ";
            if(k==resultvector[i].size()-1){
                args[k+1]=(char *)NULL;
            }
        }


        if(!fork()){
            dup2(pipefd[1],STDOUT_FILENO);
            execvp(args[0],args);
            return 0;
        }
        dup2(pipefd[0],STDIN_FILENO);
        close(pipefd[1]);
    }

    if(getpid() == parent){


        char* args[resultvector.back().size()+1];
        for (int k=0;k < resultvector.back().size(); ++k){
            std::cout <<k <<" ";
            args[k] = (char *)resultvector.back()[k].c_str();
            //std::cout <<args[i] << " ";
            if(k==resultvector.back().size()-1){
                args[k+1]=(char *)NULL;
            }
        }

        int fd;
        fd =open("/home/box/result.out",
                 O_CREAT|O_RDWR);
        if(fd < 0){
            std::cout <<"ERROR\n";
        }
        dup2(fd,STDOUT_FILENO);
        execvp(args[0],args);
    }

    return 0;
}


int main(int argc, char *argv[]) {

    action();

//    char* args[] ={(char *)"cat",(char *)"--help",NULL};

  //  execvp("cat",args);



    return 0;
}
