#ifndef  _CRAWLER_
#define  _CRAWLER_

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<boost/regex.hpp>
#include<vector>
#include<iconv.h>
#include<fstream>
using namespace  std;

void  Test();
void  Run(const  string & url,const string & Regex,const string& preface,
 		  const  string &str1,const string &str2,
		  const  string &prefix);
void  Run(const  string & url);
bool  GetHtml(const string &  url ,string  & html);
bool GetUrlList(const string& html,vector<string> & url_List,const string & Regex,const string & preface);

bool GetDetailTitle(const string & html,string& title,const string& str1,const string& str2);

bool Savetofile(const string& title,const string & html,const string & preface,int pos);
bool ConvertEncode(const string& in,string  & out);
#endif 

