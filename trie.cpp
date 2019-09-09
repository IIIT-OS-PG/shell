//os assign
#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
// #include <unordered_m>
using namespace std;

struct trie 
{
	// true when node is a leaf node
	bool ln;

	// each node stores a m to its child nodes
	map<char, trie*> m;
};

bool have_chd(trie*curr)
{  
	trie*temp;
	temp=curr;
   map<char, trie*>::iterator itr; 

  for(itr=temp->m.begin();itr!=temp->m.end();itr++)
	{
        if(itr->second!=NULL)
        {
        	return true;
        }
	}
	return false;
   
}

trie* new_node()
{
trie* node;
node=new trie;
node->ln=false;
return node;
}


void insert(char*str,trie* &head)
{
trie*temp,*temp1,*pro;
if(head==NULL)
{
temp=new_node();
temp1=new_node();
temp->m[*str]=temp1;
head=temp;
}
pro=head;
while(*str)
{

if(pro->m.find(*str)==pro->m.end())
{
  pro->m[*str]=new_node();
}
pro=pro->m[*str];
str++;

}
pro->ln=true;

}


void rtrv(trie* &head,string str,vector<string> &v)
{
	trie*temp;
	temp=head;
	// stack<trie*> srh;

    map<char, trie*>::iterator itr; 
    if(head->m.empty())
    {
       v.push_back(str);
       return;
    }
    else if(temp->ln==true)
    {
     v.push_back(str);
    }
	
	for (itr=temp->m.begin();itr!=temp->m.end();itr++)
	{
        rtrv(itr->second,(str+itr->first),v);
	}
   
   return;
}



vector<string> search(char*str,trie* &head)
{   string input ="";
	vector<string> v;
	trie* temp;
	temp=head;
	while(*str)
	{
        input=input+*str;
		if(temp->m.find(*str)==temp->m.end() )
		{
			// v.push_back(input);
			return v;
		}
		temp=temp->m[*str];
		str++;

	}

   rtrv(temp,input,v);

  return v;
}

bool deletion(trie*& curr, char* str)
{
	
	if (curr == NULL)
		return false;

	
	if (*str)
	{

		if (curr != NULL &&  curr->m.find(*str) != curr->m.end() &&
			deletion(curr->m[*str], str + 1) && curr->ln == false)
		{
			if (!have_chd(curr))
			{
				delete curr;;
				curr = NULL;
				return true;
			}
			else {
				return false;
			}
		}
	}

	if (*str == '\0' && curr->ln)
	{
		
		if (!have_chd(curr))
		{
			delete curr;; 
			curr = NULL;
			return true; 
		}

	
		else
		{
			
			curr->ln= false;
			return false;	   
		}
	}

	return false;
}


int main()
{
string s1,s2,s3;
s1="helllooaaaabbbbbccc";
s2="helllofghi";
s3="p";
// cout<<"h"<<endl;
char*c1=(char*)calloc(100,sizeof(char));
char*c2=(char*)calloc(100,sizeof(char));
char*c3=(char*)calloc(100,sizeof(char));

strcpy(c1,s1.c_str());

strcpy(c2,s2.c_str());
strcpy(c3,s3.c_str());
trie * head,*hemp;
head=NULL;
vector<string> res;
hemp=new trie;

insert(c1,head);
// cout<<"hi"<<endl;
insert(c2,head);
deletion(head,c3);

res=search(c3,head);
cout<<res[0]<<endl;
// cout<<res[1]<<endl;




return 0;
}