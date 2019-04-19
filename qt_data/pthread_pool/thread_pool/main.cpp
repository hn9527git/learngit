#include <iostream>
#include "thread_pool.h"
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class CMyTask:public CTask
{
public:
    CMyTask()=default;
    int Run()
    {
//        cout<<(char *)m_ptrDate<<endl;
        printf("%s\n",(char *)m_ptrDate);
        int x=rand()%4+1;
        sleep(x);
        return 0;
    }
    ~CMyTask()
    {}
};

int main()
{
    CMyTask taskobj;
    char sztmp[]="hello world!";
    taskobj.setData((void *)sztmp);
    CThreadPool threadpool(5);
    for(int i=0;i<10;i++)
    {
       threadpool.AddTask(&taskobj);//添加任务
    }
    while(1)
    {
//        cout<<"there are still "<<threadpool.getTaskSize()<<" task need to handle"<<endl;
        printf("there are still %d tasks need to handle\n",threadpool.getTaskSize());
        if(threadpool.getTaskSize()==0)//没有任务了
        {
            if(threadpool.StopAll()==-1)//关闭线程池
            {
                cout<<"thread pool clear,exit"<<endl;
                exit(0);
            }
        }
        sleep(2);
        cout<<"2 seconds later"<<endl;
    }
    return 0;
}
