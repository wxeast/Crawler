#include"crawler.h"


void  Test()
{
//	//1、test GetHtml 函数
//	const string url = "http://blog.csdn.net/wx_east/article/details/77427224";
//	string html;
//	GetHtml(url,html);
//	cout<<html<<endl;
//	//2、test GetUrlList 函数
//	const  string  html= "<span class=\"link_title\"><a href=\"/wx_east/article/details/74942410\">TCP协议内部知识介绍</a></span></h1></div>";
//	const string  Regex="wx_east\\/article\\/details\\/\\d*\"";
//	const string  preface ="http://blog.csdn.net/";
//	vector<string> url_List;
//	GetUrlList(html,url_List,Regex,preface);
//	cout<<url_List.size()<<endl;
//	cout<<url_List[0]<<endl;
//	//3、test GetDetailTitle 函数
//	string  title;
//	string  str1 ="<title>";
//	string  str2 =" - 晓东的博客";
//	GetDetailTitle(html,title,str1,str2);
//	cout<<title<<endl;
//	//4、test Savetofile 函数
//	Savetofile(title,html,"./data/",1);
}

void  Run(const  string & url,const string & Regex,const string& preface,
 		  const  string &str1,const string &str2,
		  const  string &prefix)
{
	//1、先得到要爬网页url的主页代码 
	string  html;
	if(!GetHtml(url,html))
	{
		return  ;
	}
	//2、得到网页中的详情页url
	vector<string> url_List;
	if(!GetUrlList(html,url_List,Regex,preface))
	{
		return ;
	}
	//3、得到每一详情页
	for(int  i= 0 ;i < url_List.size();++i)
	{
		string detail_html;
		if(!GetHtml(url_List[i],detail_html))
		{
			cout<<"GetDetailHtml error!"<<endl;
			return;
		}
		string title;
		//4、得到每一个详情页的标题
		if(!GetDetailTitle(detail_html,title,str1,str2))
		{
			return  ;
		}
		//5、将得到的详情页的内容直接保存
		if(!Savetofile(title,detail_html,prefix,i+1))
		{
			return  ;
		}
	}

}
//实现一个防拷贝的智能指针
class ScopedHandler
{
public:
	ScopedHandler(CURL* handler)
		:_handler(handler)
	{}
	~ScopedHandler(){curl_easy_cleanup(_handler);}
private:
	ScopedHandler(const ScopedHandler&);
	ScopedHandler& operator=(const ScopedHandler&);
	CURL* _handler;
};
size_t WriteData(char  * ptr,size_t size,size_t nmemb,void  * usedata)
{
	if(ptr == NULL || usedata == NULL)
	{
		return 0;
	}
	string * data = (string *) usedata;
	data->append(ptr,size*nmemb);
	return  size*nmemb;
}
bool  GetHtml(const string &  url ,string  & html)
{
	CURL* handler = curl_easy_init();//先得到句柄
	ScopedHandler scoped_handler(handler);
	curl_easy_setopt(handler,CURLOPT_URL,url.c_str());//设置要爬的网页
	curl_easy_setopt(handler,CURLOPT_WRITEFUNCTION,WriteData);//设置回调函数
	curl_easy_setopt(handler,CURLOPT_WRITEDATA,&html);//设置缓冲区
	CURLcode  res = curl_easy_perform(handler);//发射
	if(res != CURLE_OK)
	{
		cout<<"GetHtml error!"<<endl;
		return false;
	}
	return true;
}
//得到url列表  从html中读到 一个个的url放到 url_List链表中，Regex为 查找的正则表达式
bool GetUrlList(const string& html,vector<string> & url_List,const string & Regex,const string & preface)
{
	boost::regex reg(Regex.c_str());
	boost::smatch what;
	string::const_iterator beg = html.begin();
	string::const_iterator end = html.end();
	while(boost::regex_search(beg,end,what,reg))
	{
		//string tmp = what[0];
		string tmp =  string(what[0].first,what[0].second-2);
		string url = preface+tmp;
		url_List.push_back(url);
		beg = what[0].second;
	}
	return true;
}
//从html中得到当前网页的 title ，内部的查找 标题区间 str1 str2;
bool GetDetailTitle(const string & html,string& title,const string& str1,const string& str2)
{
	size_t beg = html.find(str1.c_str());
	size_t end = html.find(str2.c_str());
	size_t begin = beg + str1.size();
	if(begin >= end)
	{
		cout<<"GetDetailTitle error!"<<endl;
		return false;
	}
	title = html.substr(begin,end-begin);
	return true;
}
bool Savetofile(const string& title,const string & html,const string & prefix,int pos)
{
	char num[1024] ={0};
	sprintf(num,"%d、",pos);
	string path_name = prefix +num + title + ".html";
	cout<<path_name<<endl;
	ofstream ofs;
	ofs.open(path_name.c_str());
//	if(ofs.is_open())
//	{
//		cout<<"Savetofile error!"<<endl;
//		return false;
//	}
	ofs<<html;
	return  true;
}
bool ConvertEncode(const string& in,string  & out)
{
	const  size_t BufSize = 1024*100;
	char *  input= (char*)in.c_str();
	size_t  inbytes_left = in.size();
	char  outbuf[BufSize]  = {0};
	char * output = outbuf;
	size_t outbytes_left = BufSize;
	iconv_t cd = iconv_open("utf-8//IGNORE","gdk");
	int ret= iconv(cd,&input,&inbytes_left,&output,&outbytes_left);
	if(ret < 0)
	{
		cout<<"ConvertEncode error!"<<endl;
		iconv_close(cd);
		return false;
	}
	out = outbuf;
	iconv_close(cd);
	return true;



}
