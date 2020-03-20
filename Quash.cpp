#include "Quash.h"
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include<cstdlib>
#include<stdlib.h>


using namespace std;

Quash::Quash()
{
m_path = getenv ("PATH");
m_home =getenv ("HOME");
m_dir = getenv("PWD");
}

Quash::~Quash()
{

}

void Quash::run()
{
  cout<<"home: "<<m_home<<endl;
  bool exitShell = false;
  string input = "";
  while(exitShell == false)
  {
    cout<<">";
    getline(cin,input);
    if(input == "exit" || input == "quit")
    {
      exitShell = true;
      return;
    }
    vector<string> test = splitArguments(input);
    if(test[0]=="set")
    {
      setPaths(test[1]);
    }else if(test[0]=="cd")
    {
      if(test.size() == 1)
      {
        changeDir(m_home, true); //if home sending true
      }else
      {
        changeDir(test[1], false);
      }

    }else
    {
      launch(test);
    }


  }


}

void Quash::launch(vector<string> args)
{
char** newArgs = new char*[args.size()+1];

  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0)
  {
    // Child process
    for(int i=0;i<args.size();i++)
    {
      newArgs[i] = const_cast<char*>(args[i].c_str());
    }
    newArgs[args.size()] = NULL;

      execvp(newArgs[0], newArgs);

  }
  else if (pid < 0)
  {
    // Error forking
    std::cout<<"\nError";
  }
  else
  {
    //Parent process
      wait(NULL);

  }
}

vector<string> Quash::splitArguments(string line)
{
  vector<string> myVector;
  string argument = "";
  for(int i=0;i<line.length();i++)
  {
    if(line[i] != ' ')
    {
      argument = argument + line[i];
    }else
    {
      myVector.push_back(argument);
      argument = "";
    }
  }
  myVector.push_back(argument);
  return myVector;
}

void Quash::setPaths(string mPath)
{
  int e = putenv(const_cast<char*>(mPath.c_str()));
  if(e)
  {
    std::cout<<"\nDidn't work";
  }else
  {
    cout<<"it works"<<endl;
    cout<<getenv("PATH")<<endl;
    cout<<getenv("HOME")<<endl;

  }
}

void Quash::changeDir(string mdir, bool homeOrNot)
{
      cout<<"before change: "<<getenv("PWD")<<endl;
  if(homeOrNot)
  {
    const char* name_of_dir = (mdir.c_str());
    cout<<"name of dir:"<<name_of_dir<<endl;
    if(chdir(name_of_dir)==0)
    {
      char* dir = getcwd(NULL, 1024);
      setenv("PWD",dir,1);
      cout<<dir<<endl;
    }else
    {
      cout<<"\nError"<<endl;
    }
  }else
  {
    char* name_of_dir = const_cast<char*>(mdir.c_str());
    char* cur_dir = getenv("PWD");
    string temp1 = "/";
    char* backslash= const_cast<char*>(temp1.c_str());
    char* dir_name = strcat(cur_dir,backslash);
    const char * to = strcat(dir_name,name_of_dir);
    if(chdir(to)==0)
    {
      char* dir = getcwd(NULL, 1024);
      setenv("PWD",dir,1);
      cout<<dir<<endl;
    }else
    {
      cout<<"\nError"<<endl;
    }
  }

}
