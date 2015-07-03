#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int main()
{
char ch;
int st=28;
int l=31;
string s="3 LITTLE PIGS WENT TO A CLASS.\r$$$$$$$$";
string s1=s.substr(st,(l-st));
cout<<s1;
return 0;
}