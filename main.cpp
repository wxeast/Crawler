#include"crawler.h"


int main()
{ 
	const string url = "http://blog.csdn.net/wx_east";
	const string  Regex="wx_east\\/article\\/details\\/\\d*\">";
	const string  preface ="http://blog.csdn.net/";
	string  str1 ="<title>";
	string  str2 =" - 晓东的博客";
	Run(url,Regex,preface,str1,str2,"./data/");
	return  0;
}
