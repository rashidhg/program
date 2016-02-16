#include<iostream>
#include<math.h>
#include<conio.h>
using namespace std;
int main(){
int n,i,j,k,l;
cout<<"Enter the size of message";
cin>>n;
int r=(int)ceil(log10(n)/log10(2));
if(pow(2,r)<(n+r+1))
r++;
char a[r+n+2];
cout<<"Enter the message";
cin>>a;
int r_p[r+1];
j=1;
for(int i=1;i<=r;i++)
{r_p[i]=j;
j*=2;
   }
   k=r;
   a[n+r+1]='\0';
for(i=n+r;i>0;i--){
       if(r_p[k]!=i){
       a[i]=a[i-k-1];
       cout<<i<<" s"<<a[i]<<endl;
       }
       else{
       a[i]='0';
       k--;
       //cout<<i<<" b"<<a[i]<<endl;    
         }
         }
         for(i=1;i<=r;i++){
                           l=0;
                 for(j=pow(2,i-1);j<pow(2,i);j++){
                            for(k=j;k<=(n+r);k=k+pow(2,i)){
                                 if(a[k]=='1')
                                 l++;
                                 }
                     }
              //       cout<<i<<" "<<l<<" "<<r_p[i]<<endl;
             if(l%2!=0)
             a[r_p[i]]='1';              
              }
             cout<<"meaasge sent";     
              for(i=1;i<=(n+r);i++)
              cout<<a[i];
              int p=rand()%(n+r+3);
             // cout<<endl<<p<<endl;
              if(p<=(n+r)){
                        //   cout<<a[p]<<" ";
                      if(a[p]=='0')
                      a[p]='1';
                      else
                      a[p]='0';
                      //cout<<a[p]<<" ";
                       }
               cout<<"\nmeaasge recieve";     
              for(i=1;i<=(n+r);i++)
              cout<<a[i];
              p=0;
               for(i=1;i<=r;i++){
                           l=0;
                 for(j=pow(2,i-1);j<pow(2,i);j++){
                            for(k=j;k<=(n+r);k=k+pow(2,i)){
                                 if(a[k]=='1')
                                 l++;
                               }
                     }  
                   if(l%2!=0)
                   p+=1*r_p[i];                
                 }
                 if(p!=0)
                 cout<<"\nErroer detected at position"<<p<<endl;     
getch();
return 0;
}
