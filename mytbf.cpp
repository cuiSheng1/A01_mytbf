#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "mytbf.h"


/*********************************  变量区 ****************************************/

/* 用于存放令牌桶的数组 */
static struct mytbf_st* mytbf_array[MAX_TBF_SIZE] = {0};

/* 第一次初始化flag */
static int FIRST_INIT = 1;

/* 用于保存之前的signal动作 */
typedef void (*sighandler_t)(int);
sighandler_t save_alrm;


struct mytbf_st{
    int cps;    //每次恒定增加的token数量
    int burst;  //令牌桶上限
    int token;  
    int pos;    //记录当前令牌桶在数组的下标
};


/********************************************************
 *  功能：      alarm 的回调指针
 *  返回值：    void
 * 
 * *******************************************************/
void alrm_handler(int s)
{
    int index = 0;
    while(index < MAX_TBF_SIZE)
    {
        if(mytbf_array[index] != NULL)
        {
            mytbf_array[index]->token += mytbf_array[index]->cps;
            if(mytbf_array[index]->token >= mytbf_array[index]->burst)
                mytbf_array[index]->token = mytbf_array[index]->burst;
        }
        index++;
    }

    alarm(ALRM_INTERVAL);
}

/********************************************************
 *  功能：      卸载alarm模块信号
 *  返回值：    void
 * 
 * *******************************************************/
static void module_unload(void)
{
    int index = 0;
    signal(SIGALRM, save_alrm);
    alarm(0);
    while(index < MAX_TBF_SIZE)
    {
        free(mytbf_array[index]);
        index++;
    }

    FIRST_INIT = 1;
}

/********************************************************
 *  功能：      加载alarm模块信号
 *  返回值：    void
 * 
 * *******************************************************/
static void module_load(void)
{
    save_alrm = signal(SIGALRM, alrm_handler);
    alarm(ALRM_INTERVAL);
    FIRST_INIT = 0;

    /* 当有exit发生或者异常中止的时候，会调用 钩子函数*/
    atexit(module_unload); 
}





/********************************************************
 *  功能：      获取数组的空闲位置
 *  参数：      int cps: 恒定生成德token的数量
 *  返回值：    成功：pos, 失败: -1
 * 
 * *******************************************************/
static int get_free_pos(struct mytbf_st *array[])
{
    int pos = -1;
    int index = 0;

    while(index < MAX_TBF_SIZE)
    {
        if(array[index] == NULL)
        {
            pos = index;
            break;
        }
        index ++;
    }

    return pos;
}


/********************************************************
 *  功能：      令牌桶的初始化
 *  参数：      int cps: 恒定生成德token的数量
 *  参数：      int burst: 令牌桶的最大token值
 *  返回值：    成功：mytbf_t * ; 失败NULL
 * 
 * *******************************************************/
mytbf_t *mytbf_init(int cps, int burst)
{
    struct mytbf_st *mytbf;
    int pos = 0;

    if(cps <= 0 || burst <= 0)
        return NULL;
    
    if((pos = get_free_pos(mytbf_array)) < 0)
        return NULL;

    mytbf = (struct mytbf_st*)malloc(sizeof(struct mytbf_st));
    if(mytbf == NULL)
        return NULL;

    if(FIRST_INIT == 1)
        module_load();
    
    mytbf->cps = cps;
    mytbf->burst = burst;
    mytbf->token = 5;
    mytbf->pos = pos;

    mytbf_array[pos] = mytbf;
    return mytbf;
}

/********************************************************
 *  功能：      min
 *  返回值：    min
 * 
 * *******************************************************/
static int min(int a, int b)
{
    if(a < b)
        return a;
    return b;
}

/********************************************************
 *  功能：      令牌桶的token的获取
 *  参数：      mytbf_t *: 获取token的令牌桶指针
 *  参数：      int ： 想要获取的token数量
 *  返回值：    实际获取德token德数量
 * 
 * *******************************************************/
int mytbf_gettoken(mytbf_t *mytbf, int size)
{
    int num;
    struct mytbf_st *mt = (struct mytbf_st*)mytbf;

    if(size <= 0)
        return -1;

    while(mt->token <= 0)
        pause();
    num = min(mt->token, size);
    mt->token -= num;
    
    return num;
}


/********************************************************
 *  功能：      令牌桶的token的返回
 *  参数：      mytbf_t *: 返回token的令牌桶指针
 *  参数：      int ： 想要返回的token数量
 *  返回值：    实际返回德token德数量
 * 
 * *******************************************************/
int mytbf_returntoken(mytbf_t *mytbf, int size)
{
    struct mytbf_st *mt = (struct mytbf_st*)mytbf;
    
    if(size <= 0)
        return -1;
    
    mt->token += size;
    if(mt->token >= mt->burst)
        mt->token = mt->burst;
    
    return size;
}

/********************************************************
 *  功能：      查询剩余的token
 *  参数：      mytbf_t *: token的令牌桶指针
 *  返回值：    剩余的token德数量
 * 
 * *******************************************************/
int mytbf_lefttoken(mytbf_t *mytbf)
{
    struct mytbf_st *mt = (struct mytbf_st*)mytbf;
    
    return mytbf_array[mt->pos]->token;
}

/********************************************************
 *  功能：      令牌桶的销毁
 *  参数：      mytbf_t *: 要销毁的令牌桶的结构体指针
 *  返回值：    void
 * 
 * *******************************************************/
void mytbf_destory(mytbf_t *mytbf)
{
    struct mytbf_st *mt = (struct mytbf_st*)mytbf;

    mytbf_array[mt->pos] = NULL;
    free(mt);
}

