#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

char path[100];
struct stat s;
void list_all(char *mydir, int space)
{
    DIR* dir;
    struct dirent* str;
    int i;
    dir = opendir(mydir);   
    while((str = readdir(dir)) != NULL)
    {
        for (i = 0; i < space; ++i)
            printf("-");
        printf("%s\n", str->d_name);
        
        strcpy(path, mydir);
        strcat(path, str->d_name);
        strcat(path, "/");
        
        stat(path, &s);
        if( s.st_mode & S_IFDIR )               //it's a directory
        {
        	if(str->d_name[0] != '.')
            	list_all(path, space + 2);
        }
        else if( s.st_mode & S_IFREG )          //it's a file
        {
//            printf("file\n");
        }
        else                                    //something else
        {}
    }
}

int main()
{   
    char mydir[100] = "E:/Wallpaper/";				//	It prints out all directories and files
    
    list_all(mydir, 0);  //getcwd(mydir, 100);
  
    return 0;
}
