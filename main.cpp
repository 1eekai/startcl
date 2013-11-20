//  This file is part of STARTCL.

//  STARTCL is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.

//  STARTCL is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with STARTCL.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <cstring>
#include <string>
#include <process.h>
#include <windows.h>
#include <io.h>
#include <winerror.h>
#include <tchar.h>

#ifdef _DEBUG
#define new DEBUG_NEW   //   ����ڴ�й¶�������������ǿ��֢������⡣= =
#endif

#pragma comment(lib,"Advapi32.lib")

using namespace std;

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL isWow64(){
    BOOL bIsWow64 = FALSE;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    if(NULL != fnIsWow64Process){
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)){
            //handle error
        }
    }
    return bIsWow64;
}

void clrchararray(char ch[]){           /* ����������ݣ�ԭ����ʹ��ѭ����'\0'�ַ����������ÿ��Ԫ��������ַ����顣   */ 
	int a;
	for (a=0;a<strlen(ch)+1;a++){
		ch[a]='\0';
	}
} 

void progra_exit(int exitcode){
	cout<<"STARTCL_KERNEL: ������ֹ���˳��룺"<<exitcode<<"."<<endl;
	exit(exitcode);
} 

char* pathconnect(char* path, char* pathvalue){
	char ch[255]="\0";
	strcpy(ch,path);
	strcat(ch,"=");
	strcat(ch,pathvalue);
	return ch;
}

bool registeryRead(char* MJKey, char* pathToReg_raw, char* keyWords_raw, unsigned char *resultvalue){
	HKEY MJkey, hkey;
	unsigned char *getvalue=new unsigned char[255];
	LPCTSTR pathToReg=(LPCTSTR)pathToReg_raw;
	LPCTSTR keyWords=(LPCTSTR)keyWords_raw;
	DWORD dwLong, ddwSize;
	DWORD dwtype, sl = 256, i=0;
	int counter;
	if (strcmp(MJKey,"HKLM")==0) MJkey=HKEY_LOCAL_MACHINE;
	if (strcmp(MJKey,"HKCU")==0) MJkey=HKEY_CURRENT_USER;
	if (strcmp(MJKey,"HKLM")!=0 && strcmp(MJKey,"HKCU")!=0){
		cout<<"��ȫ���棺�������֧���ⲿ�������á�"<<endl;
		progra_exit(10);
		}
	if (RegOpenKeyEx(MJkey,pathToReg,0,KEY_READ,&hkey)==ERROR_SUCCESS){
		if (RegQueryValueEx(hkey,keyWords,NULL,NULL,(LPBYTE)getvalue,&ddwSize)==ERROR_SUCCESS){
			strcat((char *)resultvalue,(char *)getvalue);
			delete [] getvalue;
			return true;
		}else{
			delete [] getvalue;
			return false;
			}
		}else{
			delete [] getvalue;
			return false;
		}
}

bool autofindEnvAndAssign(char* keyEnvWords, char* vsVersion, char* cpuArch){
	unsigned char* result=new unsigned char[1000];     //  �Զ���ע�����Ѱ����������
	unsigned char* result1=new unsigned char[1000];	   //  ��һ�δ���о�д�������õ�������~~~
	unsigned char* result2=new unsigned char[1000];
	unsigned char* dotnetname=new unsigned char[20];
	unsigned char* dotnetver=new unsigned char[20];
	if (strcmp((char *)keyEnvWords,"VS100COMNTOOLS")==0 || strcmp((char *)keyEnvWords,"VSINSTALLDIR")==0){
		strcpy((char *)result,"VS100COMNTOOLS=");
		strcpy((char *)result1,"VSINSTALLDIR=");
		if (isWow64()){
			if (registeryRead("HKLM","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result)==true && registeryRead("HKLM","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result1)==true){
				strcat((char *)result,"Common7\\Tools");
				putenv((char *)result1); // VS100COMNTOOLS �� VSINSTALLDIR ������Ŀ¼�������ֱ��strcat�Ϳ�����~
				putenv((char *)result);
				delete [] result;
				delete [] result1;
				delete [] result2;
				delete [] dotnetname;
				delete [] dotnetver;
				return true;
				}else{
					if (registeryRead("HKCU","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result)==true && registeryRead("HKCU","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result1)==true){
						strcat((char *)result,"Common7\\Tools");
						putenv((char *)result1);
						putenv((char *)result); //  ͬ��
						delete [] result;
						delete [] result1;
						delete [] result2;
						delete [] dotnetname;
						delete [] dotnetver;
						return true;
						}else{
							delete [] result;
							delete [] result1;
							delete [] result2;
							delete [] dotnetname;
							delete [] dotnetver;
							return false;
						}
					}
				}else{
					if (registeryRead("HKLM","SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result)==true && registeryRead("HKLM","SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result1)==true){
					strcat((char *)result,"Common7\\Tools");
					putenv((char *)result1);
					putenv((char *)result);
					delete [] result;
					delete [] result1;
					delete [] result2;
					delete [] dotnetname;
					delete [] dotnetver;
					return true;
					}else{
						if (registeryRead("HKCU","SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result)==true && registeryRead("HKCU","SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7",vsVersion,result1)==true){
							strcat((char *)result,"Common7\\Tools");
							putenv((char *)result1);
							putenv((char *)result);
							delete [] result;
							delete [] result1;
							delete [] result2;
							delete [] dotnetname;
							delete [] dotnetver;
							return true;
							}else{
								delete [] result;
								delete [] result1;
								delete [] result2;
								delete [] dotnetname;
								delete [] dotnetver;
								return false;
							}
						}
					}
	}
	if (strcmp(keyEnvWords,"WindowsSdkDir")==0){
		strcpy((char *)result,"WindowsSdkDir=");
			if (registeryRead("HKLM","SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A","InstallationFolder",result)==true){
				putenv((char *)result);
				delete [] result;
				delete [] result1;
				delete [] result2;   //  �����˺öද̬�ڴ档��������Щ���Ǳ�Ҫ�ġ�
				delete [] dotnetname;
				delete [] dotnetver;
				return true;
				}else{
					if (registeryRead("HKCU","SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A","InstallationFolder",result)==true || registeryRead("HKLM","SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A","InstallationFolder",result)==true){
						putenv((char *)result);
						delete [] result;
						delete [] result1;
						delete [] result2;
						delete [] dotnetname;
						delete [] dotnetver;
						return true;
						}else{
							if (getenv("VCINSTALLDIR")!=0){
								strcat((char *)result,getenv("VCINSTALLDIR"));
								strcat((char *)result,"\\PlatformSDK");
								putenv((char *)result);
								delete [] result;    //   ��һ�δ�����vcvars??.bat������ű��Ĵ��뷭������ġ�����Ҳ���WinSDK�ĵ�ַ��������ô�������Ǹ��˾���һ����ô����������CL.exe�����ʱ��ע��ʧ�ܣ��ڣ�����ɴ����ֱ��return false����....��
								delete [] result1;
								delete [] result2;
								delete [] dotnetname;
								delete [] dotnetver;
								return true;
								}else{
									delete [] result;
									delete [] result1;
									delete [] result2;
									delete [] dotnetname;
									delete [] dotnetver;
									return false;
								}
						}
				}
		}
	if (strcmp((char *)keyEnvWords,"VCINSTALLDIR")==0 || strcmp((char *)keyEnvWords,"FrameworkDir")==0){
		strcpy((char *)dotnetname,"FrameworkDir");
		strcpy((char *)dotnetver,"FrameworkVer");
		if (isWow64()){
			strcpy((char *)result,"VCINSTALLDIR=");
			strcpy((char *)result1,"FrameWorkDir");
			if (registeryRead("HKCU","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VC7","10.0",result)==true || registeryRead("HKLM","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VC7","10.0",result)==true){
				putenv((char *)result);
				delete [] result;
				if (strcmp((char *)cpuArch,"x86")==0 || strcmp((char *)cpuArch,"x86_amd64")==0 || strcmp((char *)cpuArch,"x86_ia64")==0){
				strcat((char *)dotnetname,"32");   // ��һ�δ���ƴ�ַ������������ɡ�
				strcat((char *)dotnetver,"32");
				}else{
					strcat((char *)dotnetname,"64");
					strcat((char *)dotnetver,"64");
				}
				if (registeryRead("HKCU","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VC7",(char *)dotnetname,result1)==true || registeryRead("HKCU","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VC7",(char *)dotnetver,result2)==true || registeryRead("HKLM","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VC7",(char *)dotnetname,result1)==true || registeryRead("HKLM","SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VC7",(char *)dotnetver,result2)==true){
					strcat((char *)dotnetname,"=");
					strcat((char *)dotnetver,"=");
					putenv((char *)result1);
					putenv((char *)result2);
					putenv((char *)dotnetname);
					putenv((char *)dotnetver);
					delete [] result1;
					delete [] result2;
					delete [] dotnetname;
					delete [] dotnetver;
					return true;
					}else{
					delete [] result1;
					delete [] result2;
					delete [] dotnetname;
					delete [] dotnetver;
					return false;
					}
				}else{
					delete [] result1;
					delete [] result2;
					delete [] dotnetname;
					delete [] dotnetver;
					return false;
			}
		}else{
			strcpy((char *)result,"VCINSTALLDIR=");
			strcpy((char *)result1,"FrameWorkDir=");
			if (registeryRead("HKCU","SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VC7","10.0",result)==true){
				putenv((char *)result);
				delete [] result;
				if (strcmp((char *)cpuArch,"x86")==0 || strcmp((char *)cpuArch,"x86_amd64")==0 || strcmp((char *)cpuArch,"x86_ia64")==0){
				strcat((char *)dotnetname,"32=");
				strcat((char *)dotnetver,"32=");
				}else{
					strcat((char *)dotnetname,"64=");
					strcat((char *)dotnetver,"64=");
				}
				if (registeryRead("HKCU","SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VC7",(char *)dotnetname,result1)==true && registeryRead("HKCU","SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VC7",(char *)dotnetver,result2)==true){
					putenv((char *)result1);
					putenv((char *)result2);
					putenv((char *)dotnetname);
					putenv((char *)dotnetver);
					delete [] result1;
					delete [] result2;
					delete [] dotnetname;
					delete [] dotnetver;
					return true;
				}else{
					delete [] result1;
					delete [] result2;
					delete [] dotnetname;
					delete [] dotnetver;
					return false;
				}
				}else{
					delete [] result1;
					delete [] result2;
					delete [] dotnetname;
					delete [] dotnetver;
					return false;
				}
		}
	}
	cout<<keyEnvWords<<" δ֪����"<<endl;   //    ��������֮�٣�Ϊ�˷�ֹ����dump core��
	delete [] result;
	delete [] result1;
	delete [] result2;
	delete [] dotnetname;
	delete [] dotnetver;
	return false;
}		
		
bool findEnvAndAssign(char** arg, char* keyEnvWords){   // �Ӷ�ά����������ַ�ƥ����ַ���
	char *tmpstr=new char[1000];			// ���Ѷ�ά�����һ���ַ�����Ϊ����������ֵע�ᵽϵͳ��
	char shorttmp[20]="\0";
	char shorttmpstr[20]="\0";
	int counter,counter_matrix=0;
	clrchararray(tmpstr);
	strcpy(tmpstr,keyEnvWords);
	strcat(tmpstr,"=");
	strcpy(shorttmp,tmpstr);
	while (1==1){
		if (arg[counter_matrix]!=0 && arg[counter_matrix]!=0){	// ������ά���顣ǰһ���ǻ�������������һ���Ǳ���ֵ��
		if (strcmp(arg[counter_matrix],keyEnvWords)==0){
			strcat(tmpstr,arg[counter_matrix+1]);
		}else{
		}
		counter_matrix++;
		clrchararray(shorttmpstr);
	}else{
		break;
	}
	}
	if (strcmp(shorttmp,tmpstr)==0){
		putenv(tmpstr);											// Assign the environment path.
		delete [] tmpstr; 
		return false;
	}else{
		cout<<keyEnvWords<<" ���óɹ���"<<endl;
		putenv(tmpstr);
		delete [] tmpstr;
		return true;
	}
}

int findStrInTheMatrix(char **matrix, char *keywords){  // ���ַ���ά�����в���ͷ�����ַ�ƥ����ַ����������ظ��ַ����Ľűꡣ
	int num=0;
	while (matrix[num]!=0){
		num++;
		if (matrix[num]!=0){
		if (matrix[num][0]==keywords[0] && matrix[num][1]==keywords[1]){
			return num;
		}else{
		}
	}else{
	}
	}
	return 65535;
} 

bool fileCouldbeused(char* filename){
	if (access(filename,00)==0 && access(filename,04)==0){
		return true;
	}else{
		return false;
	}
}

int findDotInThePath(char* filepath){     //   ��·�����в��ҡ�.����λ�ã���Ҫ��Ϊ�˺����ж��ļ��ĺ�׺��
	int strpin,finalresult=65535;
	int value=0;
	for (strpin=0;strpin<strlen(filepath)+1;strpin++){
		if (filepath[strpin]=='.') finalresult=strpin;
	}
	return finalresult;
}

bool getobjfile(char* input, char* output){
	int counter;
	if (findDotInThePath(input)==65535) return false;
	for (counter=0;counter<findDotInThePath(input);counter++)
		output[counter]=input[counter];
		return true;
}
		
void showhelp(char* argv0){
	cout<<"ʹ�÷�����"<<argv0<<" [srcfile]-o[output]-a[arch][arg][arg_value]\n\n"<<endl;
	cout<<"[srcfile]\t�����ļ�(*.c;*.cpp;*.h;*.hpp;*.obj;*.o; etc)."<<endl;
	cout<<"[output]\t����ļ�(*.exe;*.obj;*.dll; etc)."<<endl;
	cout<<"[arch]\tCPU�ܹ���"<<endl;
	cout<<"\t����ֵ��x86,x86_amd64,amd64,x86_ia64"<<endl;
	cout<<"����ѡ�����Чֵ����ѡ��:"<<endl;
	cout<<"\tCPMode: [Debug][Release][another], ���ñ�����ģʽ��"<<endl;
	cout<<"\tVCINSTALLDIR: [PATH],����ָ�����Visual C++��װ��ַ��"<<endl;
	cout<<"\tVSINSTALLDIR: [PATH],����ָ�����Visual Studio��װ��ַ��"<<endl;
	cout<<"\tVS100COMNTOOLS: [PATH],����ָ�����Visual C++ IDE��װ��ַ��"<<endl;
	cout<<"\tWindowsSdkDir: [PATH],����ָ�����Windows SDK��װ��ַ��"<<endl;
	cout<<"\tFrameworkDir: [PATH],����ָ�����.NET Framework��װ��ַ��"<<endl;
	cout<<"\n����bug��lkphantom1995@gmail.com"<<endl;
	exit(0);    // ��ʾ������ִ��exit(0)��һ���̶���ִ��showhelp()����ʾ������ֹ��
}

void version(void){
	cout<<"��ԴЭ�� GPLv3+: GNU GPL Version 3 or later (http://www.gnu.org/licenses/gpl.txt)"<<endl;
	cout<<"STARTCL ��һ����������������������޸Ĵ��벢�ٷ���������������Щ��ͬʱ��������GNU GPLv3Э��ĸ������"<<endl;
	cout<<"STARTCL ��Ϊ�������û�е����������Ȩ��Ϣ�������COPYING��"<<endl;
	cout<<"\n\nbuilt: i386-VS2010"<<endl;
	exit(0);  //  ��ʾ��Ȩ��Ϣ��ִ��exit(0)��һ���̶���ִ��version()����ʾ������ֹ��
}

int main(int argc, char* argv[]){
	char *outputf;
	char *outputfile=new char[255];
	char *outputfile_cl=new char[255];
	char *purefile=new char[255];
	char *objfile=new char[255];
	char *readargv;
	char* tmpchar;
	char* tmpstr;
	char *readcpuarch;
	char *filePoint;
	char *finalFile=new char[65500];
	bool inputFileisCpp=true;
	bool finalFile_Firstloop=true;
	bool isSdkDefined=true;
	bool onlyOutputObj=false;
	char vcExec[255]="\0";
	char ldExec[255]="\0";
	int counter=1;      //  ��ʼ����������
	cout<<"CL-to-Dev-C++ ������ v0.019beta      by 1eekai"<<endl;
	cout<<"Copyright 2013 1eekai"<<endl;
	cout<<"�鿴 ReadME.MD ����ȡ������\n"<<endl; 
	if (argv[1]==0){
		cout<<argv[0]<<": "<<"����������һ���Ϸ���C/C++Դ�롣������ֹ��"<<endl;
		progra_exit(1);    // ��ֹ����dump core��
	}else{
	if (strcmp(argv[1],"--version")==0){
		version();
		}
	if (strcmp(argv[1],"--help")==0){    
		showhelp(argv[0]);			
	}else{
		if (findStrInTheMatrix(argv,"-o")==65535){
			cout<<argv[0]<<": "<<"�����붨��һ���Ϸ�������ļ���ַ(���ǲ�������ʹ��\"-o\"ѡ�:)"<<endl;
			progra_exit(1);
		}else{
			outputf=&argv[findStrInTheMatrix(argv,"-o")+1][0];
			strcpy(outputfile,"/OUT:");   // Ϊ Link.exe ָ�����Ŀ¼��ƴ�ַ�����
			strcpy(outputfile_cl,"/Fe");  // Ϊ cl.exe ָ�����Ŀ¼��ƴ�ַ�����
			strcat(outputfile,outputf);
			strcat(outputfile_cl,outputf);
		}
	}
	for (counter=1;counter<findStrInTheMatrix(argv,"-o");counter++){
	if (strcmp(argv[counter],"-c")==0){
		onlyOutputObj=true;
		continue;
	}
	if (fileCouldbeused(argv[counter])==false){
		cout<<argv[0]<<": "<<argv[counter]<<endl;
		cout<<argv[0]<<": "<<"�ļ������ڻ����޷����ʡ�"<<endl;
		progra_exit(1);
	}else{
		filePoint=&argv[counter][findDotInThePath(argv[counter])+1];
		if (strcmp(filePoint,"c")==0 || strcmp(filePoint,"h")==0 || strcmp(filePoint,"cpp")==0 || strcmp(filePoint,"hpp")==0 || strcmp(filePoint,"c++")==0 || strcmp(filePoint,"cxx")==0 || strcmp (filePoint,"cc")==0 || strcmp(filePoint,"cp")==0){
			inputFileisCpp=true;
		}else{
			inputFileisCpp=false;
		}
}
if (finalFile_Firstloop==true){
	strcpy(finalFile,argv[counter]);
	cout<<finalFile<<endl;
}else{
	strcat(finalFile," ");
	strcat(finalFile,argv[counter]);
	cout<<finalFile<<endl;
}
}
}


counter=1;
	if  (argv[findStrInTheMatrix(argv,"-o")+1]==0){
		cout<<argv[0]<<": "<<"�����붨��һ���Ϸ�������ļ���ַ��"<<endl;
		progra_exit(1);
	}
	
	if (findStrInTheMatrix(argv,"-a")==65535){
		cout<<argv[0]<<": "<<"�����붨��һ���Ϸ��ұ�֧�ֵ�CPU�ܹ���E,g: x86,x86_amd64,amd64,x86_ia64����"<<endl;
		progra_exit(1); 
	}else{
		readcpuarch=&argv[findStrInTheMatrix(argv,"-a")+1][0];
	}
		cout<<argv[0]<<": "<<"Setting runtime path for CL.exe."<<endl;
		tmpchar=(char *)new char[65535];
		tmpstr=(char *)new char[65535];  
		if (findEnvAndAssign(argv,"VERSION")==false){
			cout<<argv[0]<<": VERSION����뱻���塣ȱʡʹ��10.0��"<<endl;
			putenv("VERSION=10.0");
			}
		if (findEnvAndAssign(argv,"VCINSTALLDIR")==false) 
			if (autofindEnvAndAssign("VCINSTALLDIR",getenv("VERSION"),readcpuarch)==false)
				cout<<argv[0]<<": VCINSTALLDIR δ��������Զ����ô���\n"<<argv[0]<<": ��������޷�������ɡ�"<<endl;
				else{
					clrchararray(vcExec);
					strcpy(vcExec,getenv("VCINSTALLDIR"));
					if (strcmp(readcpuarch,"x86")==0) strcat(vcExec,"bin\\cl.exe");
					if (strcmp(readcpuarch,"amd64")==0) strcat(vcExec,"bin\\amd64\\cl.exe");
					if (strcmp(readcpuarch,"x86_amd64")==0) strcat(vcExec,"bin\\x86_amd64\\cl.exe");
					if (strcmp(readcpuarch,"x86_ia64")==0) strcat(vcExec,"bin\\x86_ia64\\cl.exe");
					strcpy(ldExec,getenv("VCINSTALLDIR"));
					if (strcmp(readcpuarch,"x86")==0) strcat(ldExec,"bin\\link.exe");
					if (strcmp(readcpuarch,"amd64")==0) strcat(ldExec,"bin\\amd64\\link.exe");
					if (strcmp(readcpuarch,"x86_amd64")==0) strcat(ldExec,"bin\\x86_amd64\\link.exe");
					if (strcmp(readcpuarch,"x86_ia64")==0) strcat(ldExec,"bin\\x86_ia64\\link.exe");
				}
		if (findEnvAndAssign(argv,"VSINSTALLDIR")==false) 
			if (autofindEnvAndAssign("VSINSTALLDIR",getenv("VERSION"),readcpuarch)==false)
				cout<<argv[0]<<": VSINSTALLDIR δ��������Զ����ô���\n"<<argv[0]<<": ��������޷�������ɡ�"<<endl;
		if (findEnvAndAssign(argv,"VS100COMNTOOLS")==false) 
			if (autofindEnvAndAssign("VS100COMNTOOLS",getenv("VERSION"),readcpuarch)==false)
				cout<<argv[0]<<": VS100COMNTOOLS δ��������Զ����ô���\n"<<argv[0]<<": ��������޷�������ɡ�"<<endl;
		if (findEnvAndAssign(argv,"WindowsSdkDir")==false) 
			if (autofindEnvAndAssign("WindowsSdkDir",getenv("VERSION"),readcpuarch)==false){
				cout<<argv[0]<<": WindowsSdkDir δ��������Զ����ô���\n"<<argv[0]<<": ��������޷�������ɡ�"<<endl;
			isSdkDefined=false;
		}else{
			isSdkDefined=true;
		}
		if (findEnvAndAssign(argv,"FrameworkDir")==false) 
			if (autofindEnvAndAssign("FrameworkDir",getenv("VERSION"),readcpuarch)==false)
				cout<<argv[0]<<": δ��������Զ����ô���\n"<<argv[0]<<": ��������޷�������ɡ�"<<endl;
		if (findEnvAndAssign(argv,"CPMode")==false) {
		cout<<argv[0]<<": CPMode δ��������Զ����ô��󣡱�����ֹ��"<<endl;
		clrchararray(tmpchar);
		progra_exit(1);
		}else{
			if (strcmp(getenv("CPMode"),"Debug")==0){
				if (onlyOutputObj==true) putenv("CPMode=/c");
				else putenv("CPMode= ");
			}else{
			}
			if (strcmp(getenv("CPMode"),"Release")==0){
				if (onlyOutputObj==true) putenv("CPMode=/MD /D \"NDEBUG\" /c");
				else putenv("CPMode=/MD /D \"NDEBUG\"");
			}else{
			}
		}
		strcpy(tmpchar,"LIB=");      // Load the library for VS.
		strcpy(tmpstr,"LIBPATH=");
		if (getenv("VCINSTALLDIR")!=0){
			strcat(tmpchar,getenv("VCINSTALLDIR"));
			strcat(tmpstr,getenv("VCINSTALLDIR"));
			strcat(tmpchar,"lib");
			strcat(tmpstr,"lib");
			if (strcmp(readcpuarch,"x86")!=0){
				strcat(tmpchar,"\\");
				strcat(tmpstr,"\\");
				strcat(tmpchar,readcpuarch);
				strcat(tmpstr,readcpuarch);
			}
			strcat(tmpchar,";");
			strcat(tmpstr,";");
			strcat(tmpchar,getenv("VCINSTALLDIR"));
			strcat(tmpstr,getenv("VCINSTALLDIR"));
			strcat(tmpchar,"ATLMFC\\lib");
			strcat(tmpstr,"ATLMFC\\lib");
			if (strcmp(readcpuarch,"x86")!=0){
				strcat(tmpchar,"\\");
				strcat(tmpstr,"\\");
				strcat(tmpchar,readcpuarch);
				strcat(tmpstr,readcpuarch);
			}
			strcat(tmpchar,";");
			strcat(tmpstr,";");
		}
		if (getenv("WindowsSdkDir")!=0){
			strcat(tmpchar,getenv("WindowsSdkDir"));
			strcat(tmpstr,getenv("WindowsSdkDir"));
			if (strcmp(readcpuarch,"x86")==0){
				strcat(tmpchar,"Lib;");
				strcat(tmpstr,"Lib;");
			}
			if (strcmp(readcpuarch,"amd64")==0){
				strcat(tmpchar,"lib\\x64;");
				strcat(tmpstr,"lib\\x64;");
			}
			if (strcmp(readcpuarch,"ia64")==0){
				strcat(tmpchar,"lib\\IA64;");
				strcat(tmpstr,"lib\\IA64;");
			}
		}
		putenv(tmpchar);
		putenv(tmpstr);
		clrchararray(tmpchar);
		clrchararray(tmpstr); 
		strcpy(tmpchar,"INCLUDE=");
		if (getenv("VCINSTALLDIR")!=0){
			strcat(tmpchar,getenv("VCINSTALLDIR"));
			strcat(tmpchar,"INCLUDE;");
			strcat(tmpchar,getenv("VCINSTALLDIR"));
			strcat(tmpchar,"ATLMFC\\INCLUDE;");
		if (isSdkDefined==true){
			strcat(tmpchar,getenv("WindowsSdkDir"));
			strcat(tmpchar,"\\INCLUDE;");
			}
		}
		putenv(tmpchar);
		strcpy(tmpchar,"PATH=");
		strcat(tmpchar,getenv("PATH"));
		strcat(tmpchar,";");
		if (getenv("VSINSTALLDIR")!=0){
		strcat(tmpchar,getenv("VSINSTALLDIR"));
		strcat(tmpchar,"Common7\\Tools;");
		strcat(tmpchar,getenv("VSINSTALLDIR"));
		strcat(tmpchar,"Common7\\IDE;");
		strcat(tmpchar,getenv("VSINSTALLDIR"));
		strcat(tmpchar,"VCPackages;");
		if (strcmp(readcpuarch,"x86")==0){
			if (getenv("VCINSTALLDIR")!=0) strcat(tmpchar,getenv("VCINSTALLDIR"));
			strcat(tmpchar,"\\bin;");
		}else{
			if (getenv("VCINSTALLDIR")!=0) strcat(tmpchar,getenv("VCINSTALLDIR"));
			strcat(tmpchar,"\\bin");
			strcat(tmpchar,readcpuarch);
			strcat(tmpchar,";");
		}
	}
		if (getenv("WindowsSdkDir")!=0){ 
			if (strcmp(readcpuarch,"x86")==0){
			strcat(tmpchar,getenv("VSINSTALLDIR"));
			strcat(tmpchar,"bin\\NETFX 4.0 Tools;");
			strcat(tmpchar,getenv("VSINSTALLDIR"));
			strcat(tmpchar,"bin;");
			}else{
			}
		}else{
		}
		if (getenv("WindowsSdkDir")!=0){
			if (strcmp(readcpuarch,"amd64")==0){
			strcat(tmpchar,getenv("VSINSTALLDIR"));
			strcat(tmpchar,"bin\\NETFX 4.0 Tools\\x64;");
			strcat(tmpchar,getenv("VSINSTALLDIR"));
			strcat(tmpchar,"bin\\x64;");
			}else{
			}
		}else{
		}
		if (getenv("VS100COMNTOOLS")!=0){
				strcat(tmpchar,getenv("VS100COMNTOOLS"));
				tmpchar[strlen(tmpchar)-5]='\0';
				strcat(tmpchar,"IDE");
			}
		putenv(tmpchar);
		delete [] tmpstr;
		delete [] tmpchar;
		cout<<finalFile<<endl;
		if (outputfile!=0) cout<<outputfile<<endl;
		if (outputfile_cl!=0) cout<<outputfile_cl<<endl;
		if (inputFileisCpp==true){
			cout<<argv[0]<<": ����ʱ����������ɣ����ڽ�����CL.exe���������Ĵ��롣"<<endl;
			cout<<argv[0]<<": CL.exe �����������\n\n"<<endl;
		}else{
			cout<<argv[0]<<": ����ʱ����������ɣ����ڽ�����Link.exe���������Ķ������ļ���"<<endl;
			cout<<argv[0]<<": LINK.exe �����������\n\n"<<endl;
		}
		if (fileCouldbeused(vcExec)==false){    //   ��ֹ���� dump core��
			cout<<argv[0]<<": "<<vcExec<<endl;
			cout<<argv[0]<<": "<<"�ļ������ڻ��޷����ʣ�"<<endl;
			progra_exit(1);
		}else{
			if (inputFileisCpp==true){
			execl(vcExec,"cl.exe",getenv("CPMode"),finalFile,outputfile_cl,NULL); 			//    ִ��cl.exe
			if (onlyOutputObj==true){
				cout<<purefile<<endl;
				getobjfile(finalFile,purefile);
				strcpy(objfile,purefile);
				strcat(objfile,".obj");
				execl(getenv("COMSPEC"),getenv("COMSPEC"),"/c","ren",objfile,outputf,NULL);
				delete [] objfile;
				delete [] purefile;
			}
		}else{
			execl(ldExec,"link.exe",finalFile,outputfile,NULL); //    ִ�� link.exe
		}
		}
		delete [] outputfile;
		delete [] outputfile_cl;
		delete [] finalFile;
		return 0;
}