// cmd: gcc -fPIC -shared -o libadd.so add.c
/*函数功能说明编译生成动态库文件
*/

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

/************************宏定义**************************/
typedef int(* FUN)(int,...);
const char* dllPath = "/mnt/libdynamic";
#define ERROR 	0
#define TRUE	1
int isIntPara(char *isIntPara)
{
	int i;
	//printf("i = %s  isIntPara = %ld \n",isIntPara, strlen(isIntPara));              //
	for (i = 0; i< strlen(isIntPara); i++)
	{
	//	printf("%s = %d \n",isIntPara, isIntPara[i]);
		if((isIntPara[i]<'0') || (isIntPara[i]>'9'))
	    {
			return ERROR;
	    }
	}
	return TRUE;
}
/*
 * 主函数入参说明：
 * Lib库名称 函数名 参数列表
 * example:
 * ./open_dynamic /mnt/libdynamic fun5 1 2 3 4
*/
int main(int argc,char *argv[])
{
    char* func;
    char* libName;
    int initPara[6] = {0};
    int i = 0;
    int Return = 0;

    /*获取LIB库名*/
    libName = argv[1];
    //if 判断库文件是否存在

    /*获取库函数名称*/
    func = argv[2];

    /*获取参数列表*/
    for(i = 3; i < argc; i++)
    {
           if(TRUE == isIntPara(argv[i]))
           {
        	   initPara[i-3] = atoi(argv[i]);
           }
           else
           {
        	   printf("函数入参类型错误，只支持十进制int 类型\n");
        	   return -1;
           }
    }

    /*open Lib库*/
    void* handle = dlopen(libName, RTLD_LAZY);
    if( !handle )
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        exit( EXIT_FAILURE );
    }

    /*获取 LIB库函数*/
	FUN add_func = (FUN)dlsym(handle, func);

	/*调用库函数*/
	Return = add_func(initPara[0], initPara[1], initPara[2], initPara[3], initPara[4], initPara[5]);

	//printf("%d \n", Return);
    dlclose( handle );
    return Return;
}
