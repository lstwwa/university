/*************************************************************************
*名称：频率计
*说明：实现0~5000hz频率计的统计
*作者：季浩原
*时间：11-8
*学号：1712220203
*************************************************************************/
#ifndef TARGET_H
#define TARGET_H 1
  #include "Include.h"
  #ifndef TARGET_GLOBAL
    #define TARGET_EXT extern
  #else 
    #define TARGET_EXT
  #endif
  TARGET_EXT void timer0_init(void);    //timer0初始化,MODEL1,timer 
  TARGET_EXT void int0_init(void);      //外部中断0初始化 
  TARGET_EXT void target_init(void);  
  TARGET_EXT uint get_fre_number(void);	//获取频率子函数 
#endif