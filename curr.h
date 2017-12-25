#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>

#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#define SCR_ROW 25               /*屏幕行数*/
#define SCR_COL 80               /*屏幕列数*/
typedef struct curr_num
{
	char currnam[10];
	struct curr_num *next;
} CURR_NUM;
CURR_NUM *PCN=NULL; 
typedef struct school_num
{
	char schoolnam[10];
	struct school_num *next;
} SCHOOL_NUM;
SCHOOL_NUM *PSN=NULL; 
typedef struct building_arrange_info
{
	char buildingnam[20];
	int  s_subj_num;
	int  s_total_hour;
	int  s_stu_num;
	int  s_school_num;
	struct building_arrange_info *next;
} BUILDING_ARRANGE_INFO;
BUILDING_ARRANGE_INFO *PBAI=NULL;
/**
 *教室排课信息链结点结构
 */
typedef struct arrange_node {
    char class_No[10];            /**< 教室编号*/
    char semester[10];            /**< 学期*/
    char week_No[30];             /**< 周次*/
    int day;                     /**< 星期*/
    char curr_name[30];           /**<课程名*/
	char  time[6];                /*<节次*/
	int  Stu_No;                  /*<学生人数*/
	char  school[30]; 
	            /*<所属学院*/
    struct arrange_node *next;    /**< 指向下一结点的指针*/
} ARRANGE_NODE;

/**
 教室基本信息链结点结构
 */
typedef struct class_node {
	char building_No[10];         /**< 教学楼编号*/
    char class_No[10];            /**< 教室编号*/
    int  floor;                   /**< 楼层*/
    float class_area;             /**< 教室面积*/
    int contain_Num;              /**< 容纳学生数*/
    char type[4];                    /**< 是否是多媒体教室*/
  
    struct arrange_node *anext;   /**< 指向缴费信息支链的指针*/
    struct class_node *next;      /**< 指向下一结点的指针*/
} CLASS_NODE;

/**
 *教学楼信息链结点结构
 */
typedef struct building_node {
    char building_No[10];         /**< 教学楼编号*/
    char name[20];                /**< 教学楼名称*/
    char telphone[15];            /**< 管理员联系电话*/
    int  class_amount;            /**< 教室数目*/
    int stu_amount;               /**< 容纳学生数目*/
    float total_area; 
	                              /**< 教室总面积*/
    struct class_node *cnext;     /**< 指向教室基本信息支链的指针*/
    struct building_node *next;   /**< 指向下一结点的指针*/
} BUILDING_NODE;

/**
 *屏幕窗口信息链结点结点结构
 */
typedef struct layer_node {
    char LayerNo;            /**< 弹出窗口层数*/
    SMALL_RECT rcArea;       /**< 弹出窗口区域坐标*/
    CHAR_INFO *pContent;     /**< 弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;           /**< 弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next; /**< 指向下一结点的指针*/
} LAYER_NODE;

/**
 *标签束结构
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< 标签字符串数组首地址*/
    COORD *pLoc;           /**< 标签定位数组首地址*/
    int num;               /**< 标签个数*/
} LABEL_BUNDLE;

/**
 *热区结构
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< 热区定位数组首地址*/
    char *pSort;           /**< 热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;            /**< 热区序号数组首地址*/
    int num;               /**< 热区个数*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*弹出窗口信息链链头*/
BUILDING_NODE *gp_head = NULL;                     /*主链头指针*/
const char check[3]="Yy"; 

char *gp_sys_name = "教室排课信息管理系统";    /*系统名称*/
char *gp_class_info_filename = "class.dat";        /*学生基本信息数据文件*/
char *gp_arrangement_info_filename = "arrange.dat";  /*住宿缴费信息数据文件*/
char *gp_building_info_filename = "building.dat";      /*宿舍楼信息数据文件*/
char *gp_backup_info_filename="backup.dat";     /*备份信息数据文件*/ 
char *ga_main_menu[] = {"文件(F)",             /*系统主菜单名*/
    	                "数据维护(M)",
                        "数据查询(Q)",
                        "数据统计(S)",
                        "帮助(H)"
                       };

char *ga_sub_menu[] = {"[S] 数据保存",          /*系统子菜单名*/
                       "[B] 数据备份",
                       "[R] 数据恢复",
                       "[X] 退出    Alt+X",
                       "[D] 教学楼信息",
                       "[P] 教室基本信息",
                       "[C] 教室排课信息",
                       "[D] 教学楼信息",
                       "[P] 教室基本信息",
                       "[C] 教室排课信息",
                       "[I] 教学楼相关信息统计",
                       "[T] 教学楼开课信息统计",
                       "[C] 教室开课信息统计",
                       "[U] 课程相关信息统计",
                       "[Z] 教室相关信息统计",
	                   "[E] 排课相关信息统计",
                       "[T] 帮助主题",
                       "[A] 关于...",
                      };

int ga_sub_menu_count[] = {4, 3, 3, 6, 2};  /*各主菜单项下子菜单的个数*/
int gi_sel_menu = 1;                        /*被选中的主菜单项号,初始为1*/
int gi_sel_sub_menu = 0;                    /*被选中的子菜单项号,初始为0,表示未选中*/

CHAR_INFO *gp_buff_menubar_info = NULL;     /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/

char *gp_scr_att = NULL;    /*存放屏幕上字符单元属性值的缓冲区*/
char gc_sys_state = '\0';   /*用来保存系统状态的字符*/


HANDLE gh_std_out;          /*标准输出设备句柄*/
HANDLE gh_std_in;           /*标准输入设备句柄*/

int LoadCode(char *filename, char **ppbuffer);  /*代码表加载*/
int CreatList(BUILDING_NODE **pphead);              /*数据链表初始化*/
void InitInterface(void);                 /*系统界面初始化*/
void ClearScreen(void);                         /*清屏*/
void ShowMenu(void);                            /*显示菜单栏*/
void PopMenu(int num);                          /*显示下拉菜单*/
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *);  /*弹出窗口屏幕信息维护*/
void PopOff(void);                              /*关闭顶层弹出窗口*/
void DrawBox(SMALL_RECT *parea);                /*绘制边框*/
void LocSubMenu(int num, SMALL_RECT *parea);    /*主菜单下拉菜单定位*/
void ShowState(void);                           /*显示状态栏*/
void TagMainMenu(int num);                      /*标记被选中的主菜单项*/
void TagSubMenu(int num);                       /*标记被选中的子菜单项*/
int DealConInput(HOT_AREA *phot_area, int *pihot_num);  /*控制台输入处理*/
void SetHotPoint(HOT_AREA *phot_area, int hot_num);     /*设置热区*/
void RunSys(BUILDING_NODE **pphd);                  /*系统功能模块的选择和运行*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);  /*功能模块的调用*/
void CloseSys(BUILDING_NODE *phd);                  /*退出系统*/
BOOL ShowModule(char **pString, int n);

BOOL LoadData(void);           /*数据加载*/
BOOL SaveData(void);           /*保存数据*/
BOOL BackupData(void);         /*备份数据*/
BOOL RestoreData(void);        /*恢复数据*/
BOOL ExitSys(void);            /*退出系统*/

BOOL HelpTopic(void);          /*帮助主题*/
BOOL AboutDorm(void);          /*关于系统*/

BOOL MaintainbuildingInfo(void);   /*维护宿舍楼信息*/
BOOL MaintainclassInfo(void);    /*维护学生基本信息*/
BOOL MaintainarrangeInfo(void); /*维护学生缴费信息*/

BOOL QueryBuildingInfo(void);      /*查询宿舍楼信息*/
BOOL QueryClassInfo(void);       /*查询学生基本信息*/
BOOL QueryArrangeInfo(void);    /*查询缴费信息*/

BOOL Statbuildinginfo(void);       /*统计教学楼相关信息*/
BOOL StatBuildingClassQ(void);        /*学生分类统计*/
BOOL StatCharge(void);         /*学生缴费信息统计*/
BOOL StatUncharge(void);       /*学生欠费信息统计*/

BOOL InsertArrangeNode(BUILDING_NODE *phd, ARRANGE_NODE *pARRANGE_NODE);/*插入缴费信息结点*/
BOOL DelArrangeNode(BUILDING_NODE *hd, ARRANGE_NODE *pa);/*删除缴费信息结点*/
BOOL ModifChargeNode(BUILDING_NODE *phd, char *stu_id, char *date, ARRANGE_NODE *pARRANGE_NODE);/*修改缴费信息结点*/
CLASS_NODE *SeekClassNode(BUILDING_NODE *phd, char *stu_id);  /*查找学生基本信息结点*/
ARRANGE_NODE *SeekArrangeNode(BUILDING_NODE *hd, ARRANGE_NODE *pa);/*查找缴费信息结点*/
CLASS_NODE *SeekStuNodeM (BUILDING_NODE *phd, int cond_num, ...);/*模糊查询学生信息结点*/
BOOL JudgeStuNodeItem(CLASS_NODE *pCLASS_NODE, char *pcond);/*判断学生信息数据项是否满足查询条件*/

BOOL SaveSysData(BUILDING_NODE *phd);                       /*保存系统数据*/
BOOL BackupSysData(BUILDING_NODE *phd, char *filename);     /*备份系统数据*/
BOOL RestoreSysData(BUILDING_NODE **pphd, char *filename);  /*恢复系统数据*/
BOOL readtext(char *info_k,SMALL_RECT area);/*读取用户所输入的文本*/ 
BOOL InsertBuildingNode(BUILDING_NODE **hd, BUILDING_NODE *BUILDING_NODE);/*向主链中插入一个教学楼信息节点*/ 
BOOL Del_building_info(void);/*删除教学楼相关信息功能函数*/ 
BOOL Del_Building_node(BUILDING_NODE **hd, char *building_no);/*删除给定编号的教学楼信息节点*/ 
BOOL Modify_building_info(void);/*修改给定编号的教学楼信息*/ 
int statbuildingno(BUILDING_NODE *hd);/*统计主链中节点数目*/
int* statbuildinginfo(BUILDING_NODE *hd);/*统计教学楼中空教室数*/ 
BOOL Query_by_No(void);/*以教学楼编号为关键字输出教学楼相关信息*/ 
BUILDING_NODE* SeekBuildingNode(BUILDING_NODE *hd,char* No);/*查找给定编号的教学楼节点*/
BOOL readtext(char *info_k,SMALL_RECT area);/*读取用户所输入的文本类型数据*/
BOOL readnum(int *num,SMALL_RECT area);/*读取所输入的int类型数据*/
BOOL readfloat(float *num,SMALL_RECT area);/*读取所输入的float类型数据*/ 
BOOL Modify_building_node(BUILDING_NODE *pb);/*修改节点信息*/ 
BOOL modify_building_node(BUILDING_NODE *pb);/*修改节点信息*/
BOOL Insert_class_info(void);/*插入教室信息节点*/ 
BOOL InsertClassNode(BUILDING_NODE *hd,CLASS_NODE *pc);/*插入教室信息节点*/
BOOL Del_class_info(void);/*删除教室信息节点*/ 
BOOL Del_class_from_building(CLASS_NODE* pc);/*删除教室信息节点*/ 
BOOL Modify_class_info(void);/*修改教室信息节点*/
BOOL Modify_class_node(CLASS_NODE *pc);/*修改教室信息节点*/
BOOL modify_class_node(CLASS_NODE *pc);/*修改教室信息节点*/
BOOL Insert_arrange_info(void);/*插入缴费节点功能函数*/ 
BOOL Del_arrange_info();/*删除安排节点功能函数*/ 
BOOL Modify_arrange_info();/*修改安排节点功能函数*/
BOOL Modify_arrange_node(ARRANGE_NODE *pa);/*修改安排节点函数*/
BOOL ModifyArrangeNode(BUILDING_NODE *hd,ARRANGE_NODE *painfo,ARRANGE_NODE *pARRANGE_NODE);/*修改安排节点函数*/
BOOL show_building_info(BUILDING_NODE *pb);/*显示教学楼信息函数*/ 
BOOL Query_by_minfo(void);/*模糊查询教学楼相关信息*/ 
BOOL show_building_minfo(BUILDING_NODE *hd,char *tmp);/*根据关键字显示教学楼信息函数*/ 
BOOL Queryclass_by_No();/*根据教室编号查询教室信息函数*/
BOOL show_class_info(CLASS_NODE *p);/*显示教室信息函数*/ 
BOOL Queryclass_by_minfo();/*模糊查询教室相关信息*/
BOOL show_class_minfo(CLASS_NODE *pc,int min,int max);/*根据信息显示教室信息*/
BOOL Queryarrange_by_info1();/*根据信息类型一查询安排相关信息*/ 
BOOL show_arrange_minfo1(ARRANGE_NODE *pa,CLASS_NODE *pc);/*显示安排信息1*/ 
BOOL Queryarrange_by_info2();/*根据信息类型二查询安排相关信息*/
BOOL show_arrange_minfo2(ARRANGE_NODE *pa,BUILDING_NODE *hd);/*根据信息类型二查询安排相关信息*/
BOOL StatBuildingClassQ();/*统计教学楼排课数据*/ 
BOOL Showbuildingarrangeinfo(char *tmp);/*显示教学楼排课信息*/
BOOL StatClassArrangeQ();/*显示教室教学安排*/ 
BOOL Showclassarrangeinfo(char *tmp,ARRANGE_NODE *parrange);/*显示教室教学安排*/ 
BOOL StatCurrArrangeQ();/*显示课程安排函数*/ 
BOOL Showcurrarrangeinfo(char* tmp1,char *tmp2);/*显示课程安排函数*/ 
BOOL Query_class_by_No(void);/*查找教室信息函数*/ 
BOOL ModifyArrangeNode(BUILDING_NODE *hd,ARRANGE_NODE *painfo,ARRANGE_NODE *pARRANGE_NODE);/*修改课程信息函数*/ 
BOOL Insert_buildingnode_window(void);/*弹出插入教学楼信息窗口函数*/ 
int statclassno(BUILDING_NODE *hd);/*统计教室数目*/
BOOL Statclassinfo();/*统计教室信息*/ 
BOOL Statarrangeinfo();/*统计排课信息*/
int statarrangeno(BUILDING_NODE *hd);/*统计排课信息条数*/


#endif /**< TYPE_H_INCLUDED*/
