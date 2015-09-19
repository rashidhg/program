#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

string path;
struct stat s;

void remove_all(string mydir)
{
    DIR* dir;
    struct dirent* str;
    dir = opendir(mydir.c_str());   
    
    while((str = readdir(dir)) != NULL)
    {
        if(str->d_name[0] == '.')
        	continue;
        
		path = mydir;
		path += string(str->d_name);
		path += "/";
		
        stat(path.c_str(), &s);
        if(s.st_mode & S_IFDIR)               		 //it's a directory
        {
        	if(str->d_name[0] != '.')
            {
            	string path1 = path;				
            	remove_all(path);	
            	rmdir(path1.c_str());
        	}
        }
        else if(s.st_mode & S_IFREG) 		         //it's a file
        {
        	path = path.substr(0, path.length() - 1);
			unlink(path.c_str());
        }
        else                                	    //something else
        {}
    }
}

int main()
{   
    string mydir = "E:/newf/";
    remove_all(mydir);
    rmdir(mydir.c_str());
    return 0;
}
