/********************************************************
 *  文件：mytbf令牌桶的实现 libmytbf.so
 *  功能：实现令牌桶的一个简单原型
 *  作者： kmist 
 *  date: 2020/3/8
 * 
 * *******************************************************/
#ifndef MYTBF_H__
#define MYTBF_H__

#define MAX_TBF_SIZE    (1024)
#define ALRM_INTERVAL    10
typedef void mytbf_t;

/********************************************************
 *  功能：      令牌桶的初始化
 *  参数：      int cps: 恒定生成德token的数量
 *  参数：      int burst: 令牌桶的最大token值
 *  返回值：    成功：mytbf_t * ; 失败NULL
 * 
 * *******************************************************/
mytbf_t *mytbf_init(int, int);


/********************************************************
 *  功能：      令牌桶的token的获取
 *  参数：      mytbf_t *: 获取token的令牌桶指针
 *  参数：      int ： 想要获取的token数量
 *  返回值：    实际获取德token德数量
 * 
 * *******************************************************/
int mytbf_gettoken(mytbf_t *, int);


/********************************************************
 *  功能：      令牌桶的token的返回
 *  参数：      mytbf_t *: 返回token的令牌桶指针
 *  参数：      int ： 想要返回的token数量
 *  返回值：    实际返回德token德数量
 * 
 * *******************************************************/
int mytbf_returntoken(mytbf_t *, int);

/********************************************************
 *  功能：      查询剩余的token
 *  参数：      mytbf_t *: token的令牌桶指针
 *  返回值：    剩余的token德数量
 * 
 * *******************************************************/
int mytbf_lefttoken(mytbf_t *mytbf);

/********************************************************
 *  功能：      令牌桶的销毁
 *  参数：      mytbf_t *: 要销毁的令牌桶的结构体指针
 *  返回值：    void
 * 
 * *******************************************************/
void mytbf_destory(mytbf_t *);








#endif /* MYTBF_H__ */
