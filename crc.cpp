#include<iostream>
#include<conio.h>
#include<math.h>
using namespace std;
int main(){
    int n,i,j,k;
    cout<<"Enter message size";
    cin>>n;
    char a[n+4];
    cout<<"Enter the input\n";
    cin>>a;
    
    int d;
    cout<<"Enter the degee of polynomial\n";
    cin>>d;
    for(i=n;i<n+d;i++)
    a[i]='0';
    a[n+d]='\0';
    cout<<a<<endl;
    int b[d+1];
    for(i=d;i>=0;i--)
    {cout<<"Enter the coefficient of x^"<<i<<" 1/0\n";
    cin>>b[d-i];
           }
    int rem[d];
    for(i=0;i<(d);i++){
    rem[i]=a[i]-'0';
    }
    for(int i=0;i<n;i++){
            rem[d]=a[i+(d)]-'0';
            
            if(rem[0]==1){
              
               for(k=1;k<=d;k++){
                                if(rem[k]==b[k])
                                rem[k-1]=0;
                                else 
                                rem[k-1]=1;
                                }           
                          }
                          else{
                               for(j=0;j<d;j++)
                               rem[j]=rem[j+1];
                               }
            }
            for(i=0;i<d;i++){
           // cout<<rem[i]<<" ";
           a[i+n]=rem[i]+'0';}
           cout<<"message sent "<<a<<endl;
           int p=rand()%(n+d+10);
          
           if(p<(n+d)){
                      if(a[i]=='0')
                      a[i]='1';
                      else
                      a[i]='0';
                       }
           cout<<"message receive "<<a<<endl;
           for(i=0;i<(d);i++){
          rem[i]=a[i]-'0';
        }
       for(int i=0;i<n;i++){
            rem[d]=a[i+(d)]-'0';
            
            if(rem[0]==1){
              
               for(k=1;k<=d;k++){
                                if(rem[k]==b[k])
                                rem[k-1]=0;
                                else 
                                rem[k-1]=1;
                                }           
                          }
                          else{
                               for(j=0;j<d;j++)
                               rem[j]=rem[j+1];
                               }
            } 
            j=0;
            for(i=0;i<d;i++){
                             if(rem[i]!=0){
                                           j=1;break;}
                             }  
                             if(j==1)
                             cout<<"Error detected";
            getch();
    return 0;
            }
    
    
