/*
Process-based multitasking handles the concurrent execution of programs.
Thread-based multitasking deals with the concurrent execution of pieces of the same program.
*/


#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

int main()
{
    string mydir = "E:/Song/Video/BollyWood/";           // Directory Path in which you want to change name
    string path, tmp, old;
    DIR* dir = opendir(mydir.c_str());
    
    struct dirent* store;
    int i;
    while((store = readdir(dir)) != NULL)
    {
        path = string(store->d_name);
        if(path[0] == '.')
            continue;
            
        	
        for(i = 0; i < path.length(); i++)										//	remove numbers from starting of name
        {
            if((path[i] >= 'a' && path[i] <= 'z') || (path[i] >= 'A' && path[i] <= 'Z'))
                break;
        } 
		
//		i = 0;																	//	remove [] 
//		if(path[0] == '(' || path[0] == '[')
//		for(i = 0; i < path.length(); i++)
//		{
//			if(path[i] == ')' || path[i] == ']')
//			{
//				i++;
//				break;			
//			}
//		}
		
	old = mydir + path;
        path = mydir + path.substr(i);
        
        rename(old.c_str(), path.c_str());
        cout<<(path)<<endl;
    }
}
