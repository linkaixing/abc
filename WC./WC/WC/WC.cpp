#include<iostream>
#include<string>
#include<fstream>
#include<windows.h>
#include<vector>
#include<cstringt.h>
#include<atlstr.h>
#include <commdlg.h> 
using namespace std;

//��ʼ������������
class InitInput{
public:
	bool c;
	bool w;
	bool l;
	bool s;
	bool a;
	bool x;
	string dir;
	InitInput(){
		c=false;
		w=false;
		l=false;
		s=false;
		a=false;
		x=false;
		dir="";
	}
	void init(int argc,char *argv[]){
		for(int i=1;i<argc;i++){
			if(argv[i][0]=='-' && strlen(argv[i])==2){
				switch(argv[i][1]){
					case 'c':c=true;break;
					case 'w':w=true;break;
					case 'l':l=true;break;
					case 's':s=true;break;
					case 'a':a=true;break;
					case 'x':x=true;break;
				}
			}
			else{
				dir=argv[i];
			}
		}
	}
};

//-c -w -l ģ��
class CWL{
public:
	int sumC;
	int sumW;
	int sumL;
	bool useC;
	bool useW;
	CWL(){
		sumC=0;
		sumW=0;
		sumL=0;
		useC=false;
		useW=false;
	}
	//�����ַ���
	void charCount(ifstream &file){
		string s;
		while(getline(file,s)){
			sumC+=s.length();
		}
		file.clear();
		file.seekg(0);
		useC=true;
		cout<<"�ļ��е��ַ���Ϊ��"<<sumC;
	}
	//���㵥����
	void wordCount(ifstream &file){
		string s;
		while(getline(file,s)){
			string str="";
			for(int i=0;i<s.length();i++){
				if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z')){
					str+=s[i];
				}
				else{
					if(str.length()>1){
						sumW++;
					}
					str="";
				}
			}
			if(str.length()>1){
				sumW++;
			}
		}
		file.clear();
		file.seekg(0);
		useW=true;
		if(useC)
			cout<<endl;
		cout<<"�ļ��еĵ�����Ϊ��"<<sumW;
	}
	//��������
	void lineCount(ifstream &file){
		string s;
		while(getline(file,s)){
			sumL++;
		}
		file.clear();
		file.seekg(0);
		if(useW || useC)
			cout<<endl;
		cout<<"�ļ��е�����Ϊ��"<<sumL;
	}
};

//-s -aģ��
class SA{
public:
	int codeRow;
	int nullRow;
	int annoRow;
	vector<string> statfileurllist;//�洢�ļ����б�
	SA(){
		codeRow=0;
		nullRow=0;
		annoRow=0;
	}
	//��ȡִ���ļ�·��
	string GetProgramDir()  
	{   
		char exeFullPath[MAX_PATH]; // Full path
		string strPath = "";
		GetModuleFileName(NULL,exeFullPath,MAX_PATH);
		strPath=(string)exeFullPath;    // Get full path of the file
		int pos = strPath.find_last_of('\\', strPath.length());
		strPath=strPath.substr(0, pos);
		int t=1,j=0;
		string s="";
		for(int i=0;i<strPath.length();i++){
			if(strPath[i]=='\\'){
				s+=strPath.substr(j,t);
				s+="\\";
				t=0;
				j=i+1;
			}
			t++;
		}
		s+=strPath.substr(j,t);
		strPath=s;
		strPath+="\\";
		strPath+="\\";
		return strPath;  // Return the directory without the file name
	}

	//Ѱ���ļ��������ļ�������vector
	void searchFile(CString path,string dir,int o){
		CString fPath=path+"*";
		//����Ŀ¼�µ���Ŀ¼
		if(o==0){
			WIN32_FIND_DATA FindFileData;//
			HANDLE hFind = FindFirstFile(fPath, &FindFileData);
			int i=0;
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //is directory
			{
				i++;
			}
			while(FindNextFile(hFind, &FindFileData) != 0) 
			{
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //is directory
				{
					if(i>1){
						CString nextFile=path+FindFileData.cFileName+'\\'+'\\';
						searchFile(nextFile,dir,1);
					}
					i++;
				}
			}
		}
		//����Ŀ¼��ָ����׺�����ļ�
		CString sPath =path; 
		sPath+=dir.c_str();
		WIN32_FIND_DATAA wfd; 
		HANDLE hFile = FindFirstFile(sPath.GetBuffer(),&wfd);  
		if(INVALID_HANDLE_VALUE == hFile){  
			//cout<<"���Ϸ�!";
			return;  
		}
		do{  
			char *chr=path.GetBuffer();
			string str=chr;
			statfileurllist.push_back(str+wfd.cFileName);
		}while(FindNextFile(hFile,&wfd));
	}

	//����
	void cal(ifstream &file){
		string s;
		bool front=false;
		while(getline(file,s)){
			//cout<<front<<"  "+s<<endl;
			if(!front && nullCount(s,front)){
				//cout<<"null  "<<endl;
				nullRow++;
				continue;
			}
			if(annoCount(s,front)){
				//cout<<"anno  "+s<<endl;
				annoRow++;
				continue;
			}
			codeRow++;
		}
	}

	//�жϿ���
	bool nullCount(string &s,bool front){
		int l=s.length();
		if(l<=1)
			return true;
		for(int i=0;i<s.length();i++){
			if(s[i]!=' ' && s[i]!='\t' && s[i]!='\n' && s[i]!='\r'){
				s=s.substr(i);
				return false;
			}
			l--;
		}
		if(l==0)
			return true;
	}

	//�ж�ע����
	bool annoCount(string s,bool &front){
		int l=0,l1=0,l2=0;
		string str,str1,str2;
		if(strstr(s.c_str(),"//"))
			l=strlen(strstr(s.c_str(),"//"));
		if(strstr(s.c_str(),"/*"))
			l1=strlen(strstr(s.c_str(),"/*"));
		if(strstr(s.c_str(),"*/"))
			l2=strlen(strstr(s.c_str(),"*/"));
		if(l){
			if(l==s.length() || l==s.length()-1)
				return true;
		}
		if(l1){
			if(!l2)
				front=true;
			if(l1==s.length() || l1==s.length()-1)
				return true;
		}
		else if(l2){
			if(front){
				front=false;
				return true;
			}
		}
		else if(front){
			return true;
		}
		return false;
	}
};

int main(int argc,char *argv[])
{
	InitInput initInput;
	initInput.init(argc,argv);
	bool useCWL=false;
	if(initInput.dir=="" && !initInput.x){
		cout<<"�����������";
		return 0;
	}
	//����-xʱ
	if(initInput.x){
		OPENFILENAME ofn;      // �����Ի���ṹ��     
		TCHAR szFile[MAX_PATH]; // �����ȡ�ļ����ƵĻ�������               
		// ��ʼ��ѡ���ļ��Ի���     
		ZeroMemory(&ofn, sizeof(OPENFILENAME));    
		ofn.lStructSize = sizeof(OPENFILENAME);    
		ofn.hwndOwner = NULL;    
		ofn.lpstrFile = szFile;    
		ofn.lpstrFile[0] = '\0';   
		ofn.nMaxFile = sizeof(szFile);    
		ofn.lpstrFilter = "All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";    
		ofn.nFilterIndex = 1;    
		ofn.lpstrFileTitle = NULL;    
		ofn.nMaxFileTitle = 0;    
		ofn.lpstrInitialDir = NULL;    
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		while(1){
			// ��ʾ��ѡ���ļ��Ի���
			if ( GetOpenFileName(&ofn) ){ 
				CWL cwl;
				SA sa;
				ifstream file;
				file.open(szFile,ios::in);
				if(!file.good()){
					cout<<"�ļ���ʧ�ܣ�"<<endl;
					continue;
				}
				cwl.charCount(file);
				cwl.wordCount(file);
				cwl.lineCount(file);
				useCWL=true;
				sa.cal(file);
				file.close();
				if(useCWL)
					cout<<endl;
				cout<<"����������"<<sa.codeRow<<endl;
				cout<<"��������"<<sa.nullRow<<endl;
				cout<<"ע��������"<<sa.annoRow;
			}
			else
				break;
		}
	}
	//���벻��-xʱ
	else{
		//����-c -w -lʱ
		if(!strstr(initInput.dir.c_str(),"*") && !strstr(initInput.dir.c_str(),"?") && (initInput.c || initInput.w || initInput.l)){
			CWL cwl;
			ifstream file;
			file.open(initInput.dir,ios::in);
			if(!file.good()){
				cout<<initInput.dir+"�ļ���ʧ�ܣ�";
				return 0;
			}
			if(initInput.c){
				cwl.charCount(file);
			}
			if(initInput.w){
				cwl.wordCount(file);
			}
			if(initInput.l){
				cwl.lineCount(file);
			}
			file.close();
			useCWL=true;
		}
		//����-s -aʱ
		if(initInput.s || initInput.a){
			SA sa;
			if((initInput.dir[0]=='*' || initInput.dir[0]=='?') && initInput.dir[1]=='.'){
				if(initInput.s)
					sa.searchFile(sa.GetProgramDir().c_str(),initInput.dir,0);
				else
					sa.searchFile(sa.GetProgramDir().c_str(),initInput.dir,1);
			}
			else if(strstr(initInput.dir.c_str(),"*") || strstr(initInput.dir.c_str(),"?")){
				string str;
				if(strstr(initInput.dir.c_str(),"*"))
					str=strstr(initInput.dir.c_str(),"*");
				else
					str=strstr(initInput.dir.c_str(),"?");
				if(str[1]=='.'){
					string s=initInput.dir.substr(0,initInput.dir.length()-str.length());
					if(initInput.s)
						sa.searchFile(s.c_str(),str,0);
					else
						sa.searchFile(s.c_str(),str,1);
				}
				else
					sa.statfileurllist.push_back(initInput.dir);
			}
			else
				sa.statfileurllist.push_back(initInput.dir);
			for(int i=0;i<sa.statfileurllist.size();i++){
				ifstream file;
				file.open(sa.statfileurllist[i],ios::in);
				if(!file.good()){
					cout<<sa.statfileurllist[i]+"�ļ���ʧ��";
					return 0;
				}
				sa.cal(file);
				file.close();
			}
			if(useCWL)
				cout<<endl;
			cout<<"����������"<<sa.codeRow<<endl;
			cout<<"��������"<<sa.nullRow<<endl;
			cout<<"ע��������"<<sa.annoRow;
		}
		else{
			if(!useCWL)
				cout<<"��������ȷ·����";
		}
	}
	return 0;
}