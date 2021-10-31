// cmd: gcc -fPIC -shared -o libadd.so add.c
/*函数功能说明编译生成动态库文件
*/

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

/************************宏定义**************************/
typedef int(* FUN)(int,...);
const char* dllPath = "/mnt/libdynamic";
#define ERROR 	-1
#define TRUE    0
/*
* 
*/
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
int isInitPara_x(char *isIntPara)
{
	int i;
    // printf("isIntPara = %ld  isIntPara = %s \n", strlen(isIntPara), isIntPara); 
    // printf("isIntPara[0] = %d  \n",isIntPara[0]); 
    // printf("isIntPara[1] = %d  \n",isIntPara[1]); 
    // printf("isIntPara[2] = %d  \n",isIntPara[2]); 

    if (2 > strlen(isIntPara))
    {
        printf("isIntPara = %d \n", strlen(isIntPara)); 
        return ERROR;
    }

    if (!((isIntPara[1] == 'x' || isIntPara[1] == 'X') && isIntPara[0] == '0'))
    {
        printf("isIntPara[0] = %d  isIntPara[1] = %d isIntPara = %d \n",isIntPara[1], isIntPara[2], strlen(isIntPara)); 
        return ERROR;
    }
	for (i = 2; i < strlen(isIntPara); i++)
	{
	//	printf("%s = %d \n",isIntPara, isIntPara[i]);
		if(((isIntPara[i]<'0') || (isIntPara[i]>'9'))
            && ((isIntPara[i]<'a') || (isIntPara[i]>'f'))
            && ((isIntPara[i]<'A') || (isIntPara[i]>'F')))
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
            Return = sscanf(argv[i],"%d",&initPara[i-3]);
            //printf("Result = %d %d\n", Return, initPara[i-3]);
        }
        else if (TRUE == isInitPara_x(argv[i]))
        {
            sscanf(argv[i],"%x",&initPara[i-3]);
            //printf("Result = %d %d\n", Return, initPara[i-3]);
        } 
        else
        {
        	printf("函数入参类型错误，只支持int 类型\n");
        	return -1;
        }
        printf("in para is %d \n",initPara[i-3]);
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
