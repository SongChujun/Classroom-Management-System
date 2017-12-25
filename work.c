#define _CRT_SECURE_NO_WARNINGS
#include "curr.h" 
#define open _open
#define write _write
#define read _read
#define close _close

unsigned long ul;



/**
 * 函数名称: LoadData
 * 函数功能: 将三类基础数据从数据文件载入到内存缓冲区和十字链表中.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 功能函数中除了函数ExitSys的返回值可以为FALSE外,
 *           其他函数的返回值必须为TRUE.
 *
 * 调用说明: 为了能够以统一的方式调用各功能函数, 将这些功能函数的原型设为
 *           一致, 即无参数且返回值为BOOL. 返回值为FALSE时, 结束程序运行.
 */
BOOL LoadData()
{
    int Re = 0;
    Re = CreatList(&gp_head);
	if (Re < 28)
	{  /*数据加载提示信息*/
		printf("\n系统基础数据不完整!\n");
		printf("\n按任意键继续..0.\n");
	}
    else
	{
		printf("\n系统基础数据加载成功!\n");
        printf("\n按任意键继续..0.\n");
	 } 

    return TRUE;
}



/**
 * 函数名称: CreatList
 * 函数功能: 从数据文件读取基础数据, 并存放到所创建的十字链表中.
 * 输入参数: 无
 * 输出参数: phead 主链头指针的地址, 用来返回所创建的十字链.
 * 返 回 值: int型数值, 表示链表创建的情况.
 *           0  空链, 无数据
 *           4  已加载宿舍楼信息数据，无学生基本信息和缴费信息数据
 *           12 已加载宿舍楼信息和学生基本信息数据，无缴费信息数据
 *           28 三类基础数据都已加载
 *
 * 调用说明:
 */
int CreatList(BUILDING_NODE **phead)
{
    BUILDING_NODE *hd=NULL, *pBuildingNode = NULL;
    CLASS_NODE *pClassNode = 0;
    ARRANGE_NODE *pArrangeNode = 0;
    FILE *pFile = 0;
    int find;
    int re = 0;

    if ((pFile = fopen(gp_building_info_filename, "r")) == NULL)
    {
        printf("宿舍楼信息数据文件打开失败!\n");
        return re;
    }
    printf("宿舍楼信息数据文件打开成功!\n");
	int num = 0;
	int i = 0;
	fscanf(pFile, "%d", &num);
    /*从数据文件中读宿舍楼信息数据，存入以后进先出方式建立的主链中*/
    for(i=0;i<num;i++)
    {
    	pBuildingNode = (struct building_node*)malloc(sizeof(BUILDING_NODE));
    	if(pBuildingNode==NULL)
		{
			printf("Fatel Error Out of space!!!");
			return re;
		 }                               
    	fscanf(pFile,"%s %d %s %d %s %f",pBuildingNode->building_No,&(pBuildingNode->class_amount),pBuildingNode->name,&(pBuildingNode->stu_amount),pBuildingNode->telphone,&(pBuildingNode->total_area));
		pBuildingNode->cnext = NULL;
        pBuildingNode->next = hd;
        hd = pBuildingNode;
    }
    fclose(pFile);
    if (hd == NULL)
    {
        printf("宿舍楼信息数据文件加载失败!\n");
        return re;
    }
    printf("宿舍楼信息数据文件加载成功!\n");
    *phead = hd;
    re += 4;

    if ((pFile = fopen(gp_class_info_filename, "r")) == NULL)
    {
        printf("学生基本信息数据文件打开失败!\n");
        return re;
    }
    printf("学生基本信息数据文件打开成功!\n");
    re += 8;

    /*从数据文件中读取学生基本信息数据，存入主链对应结点的学生基本信息支链中*/
    while (!feof(pFile))
    {    /*创建结点，存放从数据文件中读出的学生基本信息*/
        pClassNode = (CLASS_NODE *)malloc(sizeof(CLASS_NODE));
        if(pClassNode==NULL)
        {
        	printf("Fatel Error Out of space!!!");
        	return re;
		}                                //pCLASS_NODE->building_No, pCLASS_NODE->class_area, pCLASS_NODE->class_No, pCLASS_NODE->contain_Num, pCLASS_NODE->floor, pCLASS_NODE->type
        fscanf(pFile,"%s %f %s %d %d %s",pClassNode->building_No,&(pClassNode->class_area),pClassNode->class_No,&(pClassNode->contain_Num),&(pClassNode->floor),pClassNode->type);
        pClassNode->anext = NULL;

        /*在主链上查找该学生所住宿舍楼对应的主链结点*/
        pBuildingNode = hd;
        while (pBuildingNode != NULL&& strcmp(pBuildingNode->building_No, pClassNode->building_No) != 0)
        {
            pBuildingNode = pBuildingNode->next;
        }
        if (pBuildingNode != NULL) /*如果找到，则将结点以后进先出方式插入学生信息支链*/
        {
            pClassNode->next = pBuildingNode->cnext;
            pBuildingNode->cnext = pClassNode;
        }
        else  /*如果未找到，则释放所创建结点的内存空间*/
        {
            free(pClassNode);
        }
    }
    fclose(pFile);

    if ((pFile = fopen(gp_arrangement_info_filename, "r")) == NULL)
    {
        printf("教师排课信息文件打开失败!\n");
        return re;
    }
    printf("教师排课信息数据文件打开成功!\n");
    re += 16;

    /*从数据文件中读取排课信息数据，存入学生基本信息支链对应结点的缴费支链中*/
    while (!feof(pFile))
    {
        /*创建结点，存放从数据文件中读出的排课信息*/
        pArrangeNode = (ARRANGE_NODE *)malloc(sizeof(ARRANGE_NODE));
        if(pArrangeNode==NULL)
        {
        	printf("Fatel Error Out of space！！！");
        	return re-16; 
		}                                      //pARRANGE_NODE->class_No, pARRANGE_NODE->curr_name, pARRANGE_NODE->day, pARRANGE_NODE->school, pARRANGE_NODE->semester, pARRANGE_NODE->Stu_No, pARRANGE_NODE->time, pARRANGE_NODE->week_No
        fscanf(pFile,"%s %s %d %s %s %d %s %s",pArrangeNode->class_No,pArrangeNode->curr_name,&(pArrangeNode->day),pArrangeNode->school,pArrangeNode->semester,&(pArrangeNode->Stu_No),pArrangeNode->time,pArrangeNode->week_No);
        /*查找学生信息支链上对应学生信息结点*/
        pBuildingNode = hd;
        find = 0;
        while (pBuildingNode != NULL && find == 0)
        {
            pClassNode = pBuildingNode->cnext;
            while (pClassNode != NULL && find == 0)
            {
                if (strcmp(pClassNode->class_No, pArrangeNode->class_No) == 0)
                {
                    find = 1;
					goto FLAG;
                }
                pClassNode = pClassNode->next;
            }
            pBuildingNode = pBuildingNode->next;
        }
		FLAG:
        if (find)  /*如果找到，则将结点以后进先出方式插入学生缴费信息支链中*/
        {
            pArrangeNode->next = pClassNode->anext;
            pClassNode->anext = pArrangeNode;
        }
        else /*如果未找到，则释放所创建结点的内存空间*/
        {
            free(pArrangeNode);
        }
    }
    fclose(pFile);

    return re;
}

/**
 * 函数名称: InitInterface
 * 函数功能: 初始化界面.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
int main()
{
	COORD size = { SCR_COL, SCR_ROW };              /*窗口缓冲区大小*/

	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* 获取标准输出设备句柄*/
	gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* 获取标准输入设备句柄*/
	SetConsoleTitle(gp_sys_name);                 /*设置窗口标题*/
	SetConsoleScreenBufferSize(gh_std_out, size); /*设置窗口缓冲区大小80*25*/

	LoadData();                   /*数据加载*/
	InitInterface();          /*界面初始化*/
	AboutDorm();              /*弹出提示信息*/
	RunSys(&gp_head);             /*系统功能模块的选择及运行*/
	CloseSys(gp_head);            /*退出系统*/

	return 0;
}

/**
* 函数名称: LoadData
* 函数功能: 将三类基础数据从数据文件载入到内存缓冲区和十字链表中.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: BOOL类型, 功能函数中除了函数ExitSys的返回值可以为FALSE外,
*           其他函数的返回值必须为TRUE.
*
* 调用说明: 为了能够以统一的方式调用各功能函数, 将这些功能函数的原型设为
*           一致, 即无参数且返回值为BOOL. 返回值为FALSE时, 结束程序运行.
*/

BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*函数指针数组，用来存放所有功能函数的入口地址*/
    BOOL (*pFunction[18])(void);
    int i, loc;

    /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
    pFunction[0] = SaveData;
    pFunction[1] = BackupData;
    pFunction[2] = RestoreData;
    pFunction[3] = ExitSys;
    
    pFunction[4] = MaintainbuildingInfo;
    pFunction[5] = MaintainclassInfo;
    pFunction[6] = MaintainarrangeInfo;
    
    pFunction[7] = QueryBuildingInfo;
    pFunction[8] = QueryClassInfo;
    pFunction[9] = QueryArrangeInfo;
    
    pFunction[10] = Statbuildinginfo;
    pFunction[11] = StatBuildingClassQ;
    pFunction[12] = StatClassArrangeQ;
    pFunction[13] = StatCurrArrangeQ;
    pFunction[14] = Statclassinfo;
	pFunction[15] = Statarrangeinfo;

    
    pFunction[16] = HelpTopic;
    pFunction[17] = AboutDorm;
    for (i=1,loc=0; i<m; i++)  /*根据主菜单号和子菜单号计算对应下标*/
    {
        loc += ga_sub_menu_count[i-1];
    }
    loc += s - 1;

    if (pFunction[loc] != NULL)
    {
        bRet = (*pFunction[loc])();  /*用函数指针调用所指向的功能函数*/
    }

    return bRet;
}

BOOL SaveData(void)
{
    BOOL bRet = TRUE;
    SaveSysData(gp_head); 
    char *plabel_name[] = {"数据保存成功",
                           
                           "确认"
                          };

    ShowModule(plabel_name, 2);

    return bRet;
}

BOOL BackupData(void)
{
	BackupSysData(gp_head,gp_backup_info_filename);
    BOOL bRet = TRUE;
    char *plabel_name[] = {"当前数据已备份至",
                             "   backup.dat",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return TRUE;
}
BOOL ExitSys(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"确认退出系统吗？", "确定    取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+5, rcPop.Top+5}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {{rcPop.Left + 5, rcPop.Top + 5,
                           rcPop.Left + 8, rcPop.Top + 5},
                          {rcPop.Left + 13, rcPop.Top + 5,
                           rcPop.Left + 16, rcPop.Top + 5}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();

    return bRet; 
}
BOOL QueryClassInfo(void)
{
    int i;
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"按教室编号查询", "按教室信息查询","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 4;
    pos.Y = 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[100];
	for(i=0;i<labels.num-1;i++)
	{
		aLoc[i].X=rcPop.Left+2;
		aLoc[i].Y=rcPop.Top+i+1;
	 } 
	aLoc[labels.num-1].X=rcPop.Left+7;
	aLoc[labels.num-1].Y=rcPop.Top+labels.num-1+1;
    labels.pLoc = aLoc;

    areas.num = labels.num;
    SMALL_RECT aArea[100];
    char aSort[100];
    char aTag[100];
     for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right-2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    aArea[areas.num-1].Left = rcPop.Left + 7;  /*热区定位*/
    aArea[areas.num-1].Right = aArea[areas.num-1].Left+3;
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);

    if (DealInput(&areas, &iHot) == 13 )
    {
    	switch(iHot)
    	{
    		case 1:Query_class_by_No();
			break;
    		case 2:Queryclass_by_minfo();
    		break;
    		case 3:
			break;
		}
	}
    PopOff();

    return TRUE;
}
BOOL QueryBuildingInfo(void)
{
    int i;
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"按教学楼编号查询", "按教学楼信息查询","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 4;
    pos.Y = 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[100];
	for(i=0;i<labels.num-1;i++)
	{
		aLoc[i].X=rcPop.Left+2;
		aLoc[i].Y=rcPop.Top+i+1;
	 } 
	aLoc[labels.num-1].X=rcPop.Left+8;
	aLoc[labels.num-1].Y=rcPop.Top+labels.num-1+1;
    labels.pLoc = aLoc;

    areas.num = labels.num;
    SMALL_RECT aArea[100];
    char aSort[100];
    char aTag[100];
     for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    aArea[areas.num-1].Left = rcPop.Left + 8;  /*热区定位*/
    aArea[areas.num-1].Right = aArea[areas.num-1].Left+3;
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);

    if (DealInput(&areas, &iHot) == 13 )
    {
    	switch(iHot)
    	{
    		case 1:Query_by_No();
			break;
    		case 2:Query_by_minfo();
    		break;
    		case 3:
			break;
		}
	}
    PopOff();

    return TRUE;
}
BOOL MaintainclassInfo(void)
{
	int i;
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"录入教室基本信息", "删除教室基本信息","修改教室基本信息","       取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 6;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[100];
	for(i=0;i<labels.num;i++)
	{
		aLoc[i].X=rcPop.Left+2;
		aLoc[i].Y=rcPop.Top+i+1;
	 } 
    labels.pLoc = aLoc;

    areas.num = labels.num;
    SMALL_RECT aArea[100];
    char aSort[100];
    char aTag[100];
     for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);

    if (DealInput(&areas, &iHot) == 13 )
    {
    	switch(iHot)
    	{
    		case 1:Insert_class_info(); 
			break;
    		case 2:Del_class_info();
    		break;
    		case 3:Modify_class_info();
			break;
			case 4:
			break; 
		}
	}
    PopOff();

    return TRUE;
}

BOOL MaintainarrangeInfo(void)
{
	int i;
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"录入排课基本信息", "删除排课基本信息","修改排课基本信息","       取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 6;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[100];
	for(i=0;i<labels.num;i++)
	{
		aLoc[i].X=rcPop.Left+2;
		aLoc[i].Y=rcPop.Top+i+1;
	 } 
    labels.pLoc = aLoc;

    areas.num = labels.num;
    SMALL_RECT aArea[100];
    char aSort[100];
    char aTag[100];
     for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);

    if (DealInput(&areas, &iHot) == 13 )
    {
    	switch(iHot)
    	{
    		case 1:Insert_arrange_info();
			break;
    		case 2:Del_arrange_info();
    		break;
    		case 3:Modify_arrange_info();
			break;
			case 4:
			break; 
		}
	}
    PopOff();

    return TRUE;
}
BOOL QueryArrangeInfo(void)
{
    int i;
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"按排课信息查询1", "按排课信息查询2","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 4;
    pos.Y = 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X-1 ;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[100];
	for(i=0;i<labels.num-1;i++)
	{
		aLoc[i].X=rcPop.Left+2;
		aLoc[i].Y=rcPop.Top+i+1;
	 } 
	aLoc[labels.num-1].X=rcPop.Left+8;
	aLoc[labels.num-1].Y=rcPop.Top+labels.num-1+1;
    labels.pLoc = aLoc;

    areas.num = labels.num;
    SMALL_RECT aArea[100];
    char aSort[100];
    char aTag[100];
     for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    aArea[areas.num-1].Left = rcPop.Left + 8;  /*热区定位*/
    aArea[areas.num-1].Right = aArea[areas.num-1].Left+3;
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);

    if (DealInput(&areas, &iHot) == 13 )
    {
    	switch(iHot)
    	{
    		case 1:Queryarrange_by_info1();
			break;
    		case 2:Queryarrange_by_info2();
    		break;
    		case 3:
			break;
		}
	}
    PopOff();

    return TRUE;
}
/**
 * 函数名称: Statbuildinginfo
 * 函数功能: 统计教学楼相关信息.
 * 输入参数：无 
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE 或 FALSE
 *
 * 调用说明: Exefunction函数调用。 
 */
BOOL Statbuildinginfo()
{
    PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼相关信息统计","教学楼   教室间数 容纳学生数 教室总面积 多媒体间数" ,"确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 65;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;
    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top +3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    
    struct building_node *pb;
    struct class_node *pc;
    int n;
    n=statbuildingno(gp_head);
    if(n==0)
    {
    	PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "没有教学楼信息",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;
	}
	int multi_num[100]; 
	int i=0;
	char check1[3] = "y";
	char check2[3] = "Y";
    for(pb=gp_head;pb!=NULL;pb=pb->next)
    {
    	multi_num[i]=0;
    	
    	for(pc=pb->cnext;pc!=NULL;pc=pc->next)
    	{
    		if((strcmp(pc->type,check1)==0)||(strcmp(pc->type,check2)==0))
    		multi_num[i]++;
		}
		i++;
	}
	pb=gp_head;
	for(i=0;i<n;i++)
	{
		pos.X=rcPop.Left + 10;
        pos.Y=rcPop.Top + 5+i;
        SetConsoleCursorPosition(gh_std_out,pos);
    	printf("%-6s%-9d%-11d%-12f%-6d",pb->name,pb->class_amount,pb->stu_amount,pb->total_area,multi_num[i]);
	    pb=pb->next;
	}
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        goto FLAG;
    }
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}




BOOL HelpTopic(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"Powered By Chujun Song",
                           "     HUST CS1505",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL AboutDorm(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"教室类型输入Y(y)表示是多媒体教室,N(n)表示非多媒体教室",
	                       "课程节次为请输入1-2或3-4或5-6或7-8或9-10或11-12",
		                   "测试软件请不要键入太多数据",
		                   "向文本框中输入字符请先按下回车键,结束输入请按下相应按键两次",
		                   "请尽量输入完整数据,避免输入不完整数据",
		                   "有任何问题欢迎与作者联系",
                           "邮箱：925504340@qq.com",
                                   "确认"
                          };

    ShowModule(plabel_name, 8);

    return bRet;
}

/**
 * 函数名称: InsertArrangeNode
 * 函数功能: 在十字链表中插入一个排课信息结点.
 * 输入参数: hd 主链头指针
 *           pARRANGE_NODE 指向所要插入结点的指针
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE表示插入成功, FALSE表示插入失败
 *
 * 调用说明:
 */
BOOL InsertArrangeNode(BUILDING_NODE *hd, ARRANGE_NODE *pARRANGE_NODE)
{

    CLASS_NODE *pCLASS_NODE;
	/*首先在十字链中找到对应的学生基本信息节点*/
	pCLASS_NODE=SeekClassNode(hd,pARRANGE_NODE->class_No);
	if(pCLASS_NODE!=NULL)/*如果找到，则插入该节点的缴费信息支链*/
	{
		pARRANGE_NODE->next=pCLASS_NODE->anext;
		pCLASS_NODE->anext=pARRANGE_NODE;
		return TRUE;
	 } 
	else
	{
		return FALSE;
	}
}

/**
 * 函数名称: DelArrangeNode
 * 函数功能: 从十字链表中删除指定的安排信息结点.
 * 输入参数: hd 主链头指针
 *           class_No 教室号 
 *           date 缴费日期
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE表示删除成功, FALSE表示删除失败
 *
 * 调用说明: 根据学号和缴费日期可以确定唯一的缴费信息
 */
BOOL DelArrangeNode(BUILDING_NODE *hd, ARRANGE_NODE *pa)
{

    CLASS_NODE *pCLASS_NODE;
    ARRANGE_NODE *pARRANGE_NODE_prior;
    ARRANGE_NODE *pARRANGE_NODE_current;
    BOOL bRet=FALSE;
    
    pCLASS_NODE=SeekClassNode(hd,pa->class_No);
    CONSOLE_CURSOR_INFO lpCur;
    if(pCLASS_NODE==NULL)
    {
    	PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教室",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	}
	pARRANGE_NODE_prior=NULL;
	pARRANGE_NODE_current=pCLASS_NODE->anext;
	if((strcmp(pARRANGE_NODE_current->class_No,pa->class_No)==0)&&(strcmp(pARRANGE_NODE_current->curr_name,pa->curr_name)==0)&&(strcmp(pARRANGE_NODE_current->school,pa->school)==0)&&(strcmp(pARRANGE_NODE_current->semester,pa->semester)==0)&&(strcmp(pARRANGE_NODE_current->time,pa->time)==0)&&(strcmp(pARRANGE_NODE_current->week_No,pa->week_No)==0)&&(pARRANGE_NODE_current->day==pa->day)&&(pARRANGE_NODE_current->Stu_No==pa->Stu_No))
	{
		pCLASS_NODE->anext=pARRANGE_NODE_current->next;
		free(pARRANGE_NODE_current);
		return TRUE;
	 } 
	while((pARRANGE_NODE_current!=NULL)&&(strcmp(pARRANGE_NODE_current->class_No,pa->class_No)!=0)&&(strcmp(pARRANGE_NODE_current->curr_name,pa->curr_name)!=0)&&(strcmp(pARRANGE_NODE_current->school,pa->school)!=0)&&(strcmp(pARRANGE_NODE_current->semester,pa->semester)!=0)&&(strcmp(pARRANGE_NODE_current->time,pa->time)!=0)&&(strcmp(pARRANGE_NODE_current->week_No,pa->week_No)!=0)&&(pARRANGE_NODE_current->day!=pa->day)&&(pARRANGE_NODE_current->Stu_No!=pa->Stu_No))
	{
		pARRANGE_NODE_prior=pARRANGE_NODE_current;
		pARRANGE_NODE_current=pARRANGE_NODE_current->next; 
	} 
	if(pARRANGE_NODE_current==NULL)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "排课信息不存在",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	 } 
	bRet=TRUE;	
	pARRANGE_NODE_prior->next=pARRANGE_NODE_current->next;
	free(pARRANGE_NODE_current);	
	return TRUE;
}

/**
 * 函数名称: ModifArrangeNode
 * 函数功能: 对指定的缴费信息结点内容进行修改.
 * 输入参数: hd 主链头指针
 *           class_No 缴费学生学号
 *           date 缴费日期
 *           pARRANGE_NODE 指向存放修改内容结点的指针
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE表示修改成功, FALSE表示修改失败
 *
 * 调用说明:
 */
BOOL ModifArrangeNode(BUILDING_NODE *hd, ARRANGE_NODE *pARRANGE_NODE)
{

    ARRANGE_NODE *pARRANGE_NODE_temp;
    ARRANGE_NODE *pARRANGE_NODE_next;
    pARRANGE_NODE_temp=SeekArrangeNode(hd,pARRANGE_NODE);/*查找指定缴费信息节点*/ 
    if(pARRANGE_NODE_temp!=NULL)/*如果找到，则进行修改*/ 
    {
    	pARRANGE_NODE_next=pARRANGE_NODE_temp->next;/*赋值前保存指针域*/ 
		*pARRANGE_NODE_temp=*pARRANGE_NODE;
		pARRANGE_NODE_temp->next=pARRANGE_NODE_next; /*恢复指向关系*/
		return TRUE; 
	}
	else
	{
		return FALSE;
	}
}

/**
 * 函数名称: SeekClassNode
 * 函数功能: 按学号查找学生基本信息结点.
 * 输入参数: hd 主链头指针
 *           class_No 学生学号
 * 输出参数: 无
 * 返 回 值: 查中时返回结点的地址, 否则返回NULL
 *
 * 调用说明:
 */
CLASS_NODE *SeekClassNode(BUILDING_NODE *hd, char *class_No)
{

    BUILDING_NODE *pBUILDING_NODE = 0;
    CLASS_NODE *pCLASS_NODE = 0;
    int find=0;
    
    for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next)
    {/*二重循环依次搜索主链上每个结点的学生信息支链*/
	   pCLASS_NODE=pBUILDING_NODE->cnext;
	   while(pCLASS_NODE!=NULL)
	   {
		   if(strcmp(pCLASS_NODE->class_No,class_No)==0)
		   {
			find=1;
			break;
		   }
		   pCLASS_NODE=pCLASS_NODE->next;
	   } 
	   if(find)
	   {
	 	break;
	   }
    }
	if(find)
	{ 
		return pCLASS_NODE;
	}
	else
	{
		return NULL;
	}
}

/**
 * 函数名称: SeekArrangeNode
 * 函数功能: 按学号和缴费日期查找缴费信息结点.
 * 输入参数: hd 主链头指针
 *           class_No 学生学号
 *           date 缴费日期
 * 输出参数: 无
 * 返 回 值: 查中时返回结点的地址, 否则返回NULL
 *
 * 调用说明:
 */
ARRANGE_NODE *SeekArrangeNode(BUILDING_NODE *hd, ARRANGE_NODE *pa)
{
    CLASS_NODE *pCLASS_NODE = 0;
    ARRANGE_NODE *pARRANGE_NODE_current = 0;
    int find=0;
    pCLASS_NODE=SeekClassNode(hd,pa->class_No);/*首先查找对应的学生基本信息节点*/
	/*如果找到，则在缴费信息支链上继续找指定缴费信息节点*/
	if(pCLASS_NODE!=NULL)
	{
		pARRANGE_NODE_current=pCLASS_NODE->anext;
		while(pARRANGE_NODE_current!=NULL)
		{
			if((strcmp(pARRANGE_NODE_current->class_No,pa->class_No)==0)&&(strcmp(pARRANGE_NODE_current->curr_name,pa->curr_name)==0)&&(strcmp(pARRANGE_NODE_current->school,pa->school)==0)&&(strcmp(pARRANGE_NODE_current->semester,pa->semester)==0)&&(strcmp(pARRANGE_NODE_current->time,pa->time)==0)&&(strcmp(pARRANGE_NODE_current->week_No,pa->week_No)==0)&&(pARRANGE_NODE_current->day!=pa->day)&&(pARRANGE_NODE_current->Stu_No==pa->Stu_No))
			{
				find=1;
				break;
			}
			pARRANGE_NODE_current=pARRANGE_NODE_current->next; 
		}
	 } 
	 if(find)
	 {
	 	return pARRANGE_NODE_current;
	 }
	 else
	 {
	 	return NULL;
	 }
}



/**
 * 函数名称: SaveSysData
 * 函数功能: 保存三类基础数据.
 * 输入参数: hd 主链头结点指针
 * 输出参数:
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL SaveSysData(BUILDING_NODE *hd)/*CHECK*/ 
{
     	BUILDING_NODE *pBUILDING_NODE;
     	CLASS_NODE *pCLASS_NODE;
     	ARRANGE_NODE *pARRANGE_NODE;
     	FILE *pfout;
     	
     	pfout=fopen(gp_building_info_filename,"w");
		int num = 0;
		for (pBUILDING_NODE = hd; pBUILDING_NODE != NULL; pBUILDING_NODE = pBUILDING_NODE->next)
		{
			num++;
		}
		fprintf(pfout, "%d\n", num);
     	for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next)
     	{   /*保存教学楼信息*/
		   fprintf(pfout,"%s %d %s %d %s %f\n",pBUILDING_NODE->building_No,pBUILDING_NODE->class_amount,pBUILDING_NODE->name,pBUILDING_NODE->stu_amount,pBUILDING_NODE->telphone,pBUILDING_NODE->total_area);
        }
		fclose(pfout);
		int i;
		
		pfout=fopen(gp_class_info_filename,"w");
		for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next)
		{  /*保存学生基本信息*/ 
			for(pCLASS_NODE=pBUILDING_NODE->cnext; pCLASS_NODE; pCLASS_NODE = pCLASS_NODE->next)
			{
				fprintf(pfout,"%s %f %s %d %d %s\n",pCLASS_NODE->building_No,pCLASS_NODE->class_area,pCLASS_NODE->class_No,pCLASS_NODE->contain_Num,pCLASS_NODE->floor,pCLASS_NODE->type);
			} 
		} 
		fclose(pfout);

		pfout=fopen(gp_arrangement_info_filename,"w");
		for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next)
		{/*保存学生缴费信息*/ 
			pCLASS_NODE=pBUILDING_NODE->cnext;
			while(pCLASS_NODE!=NULL)
			{
				pARRANGE_NODE=pCLASS_NODE->anext;
				while(pARRANGE_NODE!=NULL)
				{
					fprintf(pfout,"%s %s %d %s %s %d %s %s\n",pARRANGE_NODE->class_No,pARRANGE_NODE->curr_name,pARRANGE_NODE->day,pARRANGE_NODE->school,pARRANGE_NODE->semester,pARRANGE_NODE->Stu_No,pARRANGE_NODE->time,pARRANGE_NODE->week_No);
					pARRANGE_NODE=pARRANGE_NODE->next;
				}
				pCLASS_NODE=pCLASS_NODE->next;
			 } 
		}
		fclose(pfout);
		
		return TRUE;
}

/**
 * 函数名称: BackupSysData
 * 函数功能: 将系统代码表和三类基础数据备份到一个数据文件.
 * 输入参数: hd 主链头结点指针
 *           filename 数据文件名
 * 输出参数:
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL BackupSysData(BUILDING_NODE *hd, char *filename)
{
     BUILDING_NODE *pBUILDING_NODE;
     CLASS_NODE *pCLASS_NODE;
     ARRANGE_NODE *pARRANGE_NODE;
     unsigned long BUILDING_NODE_num=0;
     unsigned long CLASS_NODE_num=0;
     unsigned long ARRANGE_NODE_num=0;
     //int handle;
     
     /*遍历十字链，统计三种基础数据的记录总数*/ 
     for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next)
     {
     	BUILDING_NODE_num++;
     	pCLASS_NODE=pBUILDING_NODE->cnext;
     	while(pCLASS_NODE!=NULL)
     	{
     		CLASS_NODE_num++;
     		pARRANGE_NODE=pCLASS_NODE->anext;
     		while(pARRANGE_NODE!=NULL)
     		{
     			ARRANGE_NODE_num++;
     			pARRANGE_NODE=pARRANGE_NODE->next;
			 }
			pCLASS_NODE=pCLASS_NODE->next;
		 }
	 }
	 
	 /*if(handle=open(filename,O_WRONLY|O_BINARY)==-1) 
	 {
	 	handle=open(filename,O_CREAT|O_BINARY,S_IWRITE);
	 }
	 write(handle,(char*)&BUILDING_NODE_num,sizeof(BUILDING_NODE_num));
	 write(handle,(char*)&CLASS_NODE_num,sizeof(CLASS_NODE_num));
	 write(handle,(char*)&ARRANGE_NODE_num,sizeof(ARRANGE_NODE_num));*/
	 FILE *pfout;
	 pfout = fopen("backup.dat", "w");
	 CONSOLE_CURSOR_INFO lpCur;

	 fprintf(pfout,"%lu %lu %lu\n", BUILDING_NODE_num, CLASS_NODE_num, ARRANGE_NODE_num);
	 for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next)
	 {/*保存教学楼信息*/ 
		 fprintf(pfout, "%s %d %s %d %s %f\n", pBUILDING_NODE->building_No, pBUILDING_NODE->class_amount, pBUILDING_NODE->name, pBUILDING_NODE->stu_amount, pBUILDING_NODE->telphone, pBUILDING_NODE->total_area);
	 }  
	 for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next) 
	 {  /*保存教室基本信息*/ 
	 	pCLASS_NODE=pBUILDING_NODE->cnext;
		 while(pCLASS_NODE!=NULL) 
		 {
			 fprintf(pfout, "%s %f %s %d %d %s\n", pCLASS_NODE->building_No, pCLASS_NODE->class_area, pCLASS_NODE->class_No, pCLASS_NODE->contain_Num, pCLASS_NODE->floor, pCLASS_NODE->type);
			 pCLASS_NODE=pCLASS_NODE->next; 
		 }
	 }
	 for(pBUILDING_NODE=hd;pBUILDING_NODE!=NULL;pBUILDING_NODE=pBUILDING_NODE->next) 
	 {  /*保存排课基本信息*/ 
	 	pCLASS_NODE=pBUILDING_NODE->cnext;
		 while(pCLASS_NODE!=NULL) 
		 {
		 	pARRANGE_NODE=pCLASS_NODE->anext;
			 while(pARRANGE_NODE!=NULL)
			 {
			 	//write(handle,(char*)pARRANGE_NODE,sizeof(pARRANGE_NODE));
				fprintf(pfout, "%s %s %d %s %s %d %s %s\n", pARRANGE_NODE->class_No, pARRANGE_NODE->curr_name, pARRANGE_NODE->day, pARRANGE_NODE->school, pARRANGE_NODE->semester, pARRANGE_NODE->Stu_No, pARRANGE_NODE->time, pARRANGE_NODE->week_No);
			 	pARRANGE_NODE=pARRANGE_NODE->next;
			  } 
			pCLASS_NODE=pCLASS_NODE->next; 
		 }
	 }
	 fclose(pfout);
	 return TRUE;
}

/**
 * 函数名称: RestoreSysData
 * 函数功能: 从指定数据文件中恢复系统代码表和三类基础数据.
 * 输入参数: phead 主链头结点指针的地址
 *           filename 存放备份数据的数据文件名
 * 输出参数:
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL RestoreSysData(BUILDING_NODE **phead, char *filename)
{
	BUILDING_NODE *hd = NULL;
	BUILDING_NODE *pBUILDING_NODE = 0;
	CLASS_NODE *pCLASS_NODE = 0;
	ARRANGE_NODE *pARRANGE_NODE = 0;
	unsigned long BUILDING_NODE_num = 0;
	unsigned long CLASS_NODE_num = 0;
	unsigned long ARRANGE_NODE_num = 0;
	unsigned long ulloop = 0;
	int handle;
	int find;
	FILE *pFile = 0;
	CONSOLE_CURSOR_INFO lpCur;

	BUILDING_NODE *pBuildingNode1 = hd, *pBuildingNode2;
	CLASS_NODE *pClassNode1, *pClassNode2;
	ARRANGE_NODE *pArrangeNode1, *pArrangeNode2;


	if ((pFile = fopen(filename, "r")) == NULL)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = FALSE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
			"无法找到文件",
			"确认"
		};

		ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	}
	/*if(handle=open(filename,O_RDONLY|O_BINARY)==-1)
	{
	   handle=open(filename,O_CREAT|O_BINARY,S_IREAD);
	}
	/*读取三种基本信息的长度*/
	/*read(handle,(char*)&BUILDING_NODE_num,sizeof(BUILDING_NODE_num));
	read(handle,(char*)&CLASS_NODE_num,sizeof(CLASS_NODE_num));
	read(handle,(char*)&ARRANGE_NODE_num,sizeof(ARRANGE_NODE_num));

	/*读取教学楼信息，建立十字链主链*/
	while (pBuildingNode1 != NULL) /*释放十字交叉链表的动态存储区*/
	{
		pBuildingNode2 = pBuildingNode1->next;
		pClassNode1 = pBuildingNode1->cnext;
		while (pClassNode1 != NULL) /*释放学生基本信息支链的动态存储区*/
		{
			pClassNode2 = pClassNode1->next;
			pArrangeNode1 = pClassNode1->anext;
			while (pArrangeNode1 != NULL) /*释放缴费信息支链的动态存储区*/
			{
				pArrangeNode2 = pArrangeNode1->next;
				free(pArrangeNode1);
				pArrangeNode1 = pArrangeNode2;
			}
			free(pClassNode1);
			pClassNode1 = pClassNode2;
		}
		free(pBuildingNode1);  /*释放主链结点的动态存储区*/
		pBuildingNode1 = pBuildingNode2;
	}
	fscanf(pFile, "%lu %lu %lu", &BUILDING_NODE_num, &CLASS_NODE_num, &ARRANGE_NODE_num);
	for (ulloop = 1; ulloop <= BUILDING_NODE_num; ulloop++)
	{
		pBUILDING_NODE = (BUILDING_NODE*)malloc(sizeof(BUILDING_NODE));
		fscanf(pFile, "%s %d %s %d %s %f", pBUILDING_NODE->building_No, &(pBUILDING_NODE->class_amount), pBUILDING_NODE->name, &(pBUILDING_NODE->stu_amount), pBUILDING_NODE->telphone, &(pBUILDING_NODE->total_area));
		pBUILDING_NODE->cnext = NULL;
		pBUILDING_NODE->next = hd;
		hd = pBUILDING_NODE;
	}
	*phead = hd;

	  /*读取学生基本信息，建立学生信息支链*/
    for(ulloop=1;ulloop<=CLASS_NODE_num;ulloop++)
	 {
		//fprintf(pfout, "%s %s %d %s %s %d %s %s\n", pARRANGE_NODE->class_No, pARRANGE_NODE->curr_name, pARRANGE_NODE->day, pARRANGE_NODE->school, pARRANGE_NODE->semester, pARRANGE_NODE->Stu_No, pARRANGE_NODE->time, pARRANGE_NODE->week_No);

	  	pCLASS_NODE=(CLASS_NODE*)malloc(sizeof(CLASS_NODE));
		fscanf(pFile, "%s %f %s %d %d %s", pCLASS_NODE->building_No, &(pCLASS_NODE->class_area), pCLASS_NODE->class_No, &(pCLASS_NODE->contain_Num), &(pCLASS_NODE->floor), pCLASS_NODE->type);
	  	pCLASS_NODE->anext=NULL;
	  	pBUILDING_NODE=hd;
	  	while(pBUILDING_NODE!=NULL&&strcmp(pBUILDING_NODE->building_No,pCLASS_NODE->building_No)!=0)
	  	{
	  		pBUILDING_NODE=pBUILDING_NODE->next;
		}
		  
		if(pBUILDING_NODE!=NULL)
		{
		  	pCLASS_NODE->next=pBUILDING_NODE->cnext;  
		  	pBUILDING_NODE->cnext=pCLASS_NODE;
		}
		else
		{
			free(pCLASS_NODE);
		}
	  } 
	/*读取学生缴费信息，建立缴费信息支链*/
	for (ulloop = 1; ulloop <= ARRANGE_NODE_num; ulloop++)
	{
		//fprintf(pfout, "%s %s %d %s %s %d %s %s\n", pARRANGE_NODE->class_No, pARRANGE_NODE->curr_name, pARRANGE_NODE->day, pARRANGE_NODE->school, pARRANGE_NODE->semester, pARRANGE_NODE->Stu_No, pARRANGE_NODE->time, pARRANGE_NODE->week_No);

		pARRANGE_NODE = (ARRANGE_NODE*)malloc(sizeof(ARRANGE_NODE));
		fscanf(pFile, "%s %s %d %s %s %d %s %s", pARRANGE_NODE->class_No, pARRANGE_NODE->curr_name, &(pARRANGE_NODE->day), pARRANGE_NODE->school, pARRANGE_NODE->semester, &(pARRANGE_NODE->Stu_No), pARRANGE_NODE->time, pARRANGE_NODE->week_No);
		pBUILDING_NODE = hd;
		find = 0;

		while (pBUILDING_NODE != NULL&&find == 0)
		{
			pCLASS_NODE = pBUILDING_NODE->cnext;
			while (pCLASS_NODE != NULL&&find == 0)
			{
				if (strcmp(pCLASS_NODE->class_No, pARRANGE_NODE->class_No) == 0)
				{
					find = 1;
					goto FLAG;
				}
				pCLASS_NODE = pCLASS_NODE->next;
			}
			pBUILDING_NODE = pBUILDING_NODE->next;
		}
		FLAG:
		if (find)
		{
			pARRANGE_NODE->next = pCLASS_NODE->anext;
			pCLASS_NODE->anext = pARRANGE_NODE;
		}
		else
		{
			free(pARRANGE_NODE);
		}
	}
  SaveSysData(hd);
  return TRUE;
}
BOOL ShowModule(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len;

    for (i=0,maxlen=0; i<n; i++) {
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }

    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[100];

    for (i=0; i<n; i++) {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;

    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;

    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {{aLoc[n-1].X, aLoc[n-1].Y,
                           aLoc[n-1].X + 3, aLoc[n-1].Y}};

    char aSort[] = {0};
    char aTag[] = {1};

    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    DealInput(&areas, &iHot);
    PopOff();

    return bRet;

}
BOOL Insert_buildingnode_window(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼编号：", "教学楼名称：","管理员电话：","教室间数:","容纳学生数:","教室总面积:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 16;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 7;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+7, rcPop.Top+14}};
    labels.pLoc = aLoc;

    areas.num = 8;
    SMALL_RECT aArea[] = {{rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 2,
                           rcPop.Left + 8+strlen(pCh[0]), rcPop.Top + 2},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + 8+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 8+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 8+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 8+strlen(pCh[1]), rcPop.Top + 10},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 12,
                           rcPop.Left + 8+strlen(pCh[2]), rcPop.Top + 12},
	                       {rcPop.Left + 7, rcPop.Top + 14,
                           rcPop.Left + 10, rcPop.Top + 14},
                           {rcPop.Left + 15, rcPop.Top + 14,
                           rcPop.Left + 18, rcPop.Top + 14}};
    char aSort[] = {1, 1, 1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6, 7, 8};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    BUILDING_NODE *pbuildingnode;
	pbuildingnode=(BUILDING_NODE*)calloc(1,sizeof(BUILDING_NODE));
	//pbuildingnode = NULL;

    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:
	   	readtext(pbuildingnode->building_No,aArea[0]);
	   	break;
	   case 2:
	   	readtext(pbuildingnode->name,aArea[1]);
	   	break;
	   case 3: 
	    readtext(pbuildingnode->telphone,aArea[2]);
	    break;
	   case 4:
	   	readnum(&(pbuildingnode->class_amount),aArea[3]);
	   	break;
	   case 5:
	   	readnum(&(pbuildingnode->stu_amount),aArea[4]);
	   	break;
	   case 6: 
	    readfloat(&(pbuildingnode->total_area),aArea[5]);
	    break;
	   case 7: 
	   	InsertBuildingNode(&gp_head,pbuildingnode);
	    goto FLAG;
	    break;
	   case 8:
	    goto FLAG;	
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
}
/**
 * 函数名称: readtext
 * 函数功能: 读取用户输入到文本框中的信息.
 * 输入参数: 文本框的坐标       
 * 输出参数: 所读取的用户的输入
 * 返 回 值: 布尔值 
 *
 * 调用说明:
 */
BOOL readtext(char *info_k,SMALL_RECT area)
{
	CONSOLE_CURSOR_INFO lpCur;
    DWORD res;
    INPUT_RECORD inRec;
    COORD mouse,pos;
	pos.X=area.Left;
	pos.Y=area.Top;
	char asc,vkc; 
	WORD att = BACKGROUND_BLUE | BACKGROUND_GREEN;
	char tmp[100]="\0";/*tmp临时储存当次输入内容, c临时储存一个汉字*/
	char c[100]="\0";
	int i=0;
	while(TRUE)
	    {
	    	FillConsoleOutputAttribute(gh_std_out, att, area.Right-area.Left+1, pos, &ul);
		    ReadConsoleInput(gh_std_in, &inRec, 1, &res);/*读入一条输入记录*/
		    if(strlen(tmp)>area.Right-area.Left+1)
		    {/*超出输入框*/
			break;
		    }
			if(inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
		    {
			  asc = inRec.Event.KeyEvent.uChar.AsciiChar;
			  vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
			  if(asc>='a'&&asc<='z' || asc>='A'&&asc<='Z' || asc>='0'&&asc<='9' || asc=='-' || asc=='.' || asc==',')
			  {/*正确的字符读入*/
				tmp[i]=asc;
				i+=1;
				tmp[i]='\0';
				SetConsoleCursorPosition(gh_std_out, pos);
				printf("%s",tmp);
			  }
			  else if(asc=='\b' && i!=0)
			  {/*退格处理*/
				i--;
				if((int)tmp[i]<0)
				{/*汉字*/
					tmp[i]='\0';
					tmp[--i]='\0';
				}
				else
				{	tmp[i]='\0';
			    }
				SetConsoleCursorPosition(gh_std_out, pos);
				printf("%s  ",tmp);
				printf("%c%c",'\b','\b');
			  }
			  else if(vkc==0&&asc<0)
			  {/*汉字读入*/
				c[0]=asc;
				ReadConsoleInput(gh_std_in, &inRec, 1, &res);
				c[1] = inRec.Event.KeyEvent.uChar.AsciiChar;
				c[2]='\0';
				strcat(tmp,c);
				i+=2;
				SetConsoleCursorPosition(gh_std_out, pos);
			 	printf("%s",tmp);
			  }
			  else if(vkc==38 || vkc==40 || vkc==27||vkc==39||vkc==37)
			  {/*方向键和ESC*/
				break;
			  }	
		    }
		    else if(inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		    {/*鼠标点击*/
			mouse= inRec.Event.MouseEvent.dwMousePosition;
			if(mouse.X<area.Left || mouse.X>area.Right || mouse.Y!=area.Top)
			{
				/*GetConsoleCursorInfo(gh_std_out, &lpCur);
		        lpCur.bVisible = FALSE;
		        SetConsoleCursorInfo(gh_std_out, &lpCur);*/
				break;
			}
		    }
	    }
	    strcpy(info_k,tmp);
	    return TRUE;
}
/**
 * 函数名称: InsertBuildingNode
 * 函数功能: 在十字链表中插入一个教学楼信息结点.
 * 输入参数: hd 主链头指针
 *           tmp1 指向所要插入教学楼信息结点的指针
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE表示插入成功, FALSE表示插入失败
 *
 * 调用说明:
 */
BOOL InsertBuildingNode(BUILDING_NODE **hd, BUILDING_NODE *p)
{
    struct building_node *current;
    current=(struct building_node*)malloc(sizeof(BUILDING_NODE));
    CONSOLE_CURSOR_INFO lpCur;
    if(current==NULL)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "Fatal Error Out of space！！",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
		
	 } 
	current->next=*hd;
	strcpy(current->building_No,p->building_No);
	strcpy(current->name,p->name);
	strcpy(current->telphone,p->telphone);
	current->class_amount=p->class_amount;
	current->stu_amount=p->stu_amount;
	current->cnext = 0;
	current->total_area=p->total_area;
	*hd=current;
	return TRUE;
}
/**
 * 函数名称: Del_building_info
 * 函数功能: 弹出删除教学楼信息节点的界面.
 * 输入参数: 无 
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE表示插入成功, FALSE表示插入失败
 *
 * 调用说明： 
 */
BOOL Del_building_info(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入待删除教学楼编号", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top + 6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = {1, 0, 0};
    char aTag[] = {1, 2, 3}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
     FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
     
    struct building_node *p;
    p=(struct building_node*)malloc(sizeof(BUILDING_NODE)); 
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:
	   readtext(p->building_No,aArea[0]);
	   	break;
	   case 2:if (SeekBuildingNode(gp_head, p->building_No) == NULL)
	   {
		   PopOff();
		   GetConsoleCursorInfo(gh_std_out, &lpCur);
		   lpCur.bVisible = FALSE;
		   SetConsoleCursorInfo(gh_std_out, &lpCur);
		   char *plabel_name[] = {
			   "无法找到教学楼",
			   "确认"
		   };

		   ShowModule(plabel_name, 2);
		   PopOff();
		   return FALSE;
	   }
		   Del_Building_node(&gp_head,p->building_No);
	   goto FLAG;
	   case 3: 
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
/**
 * 函数名称: Del_Building_node
 * 函数功能: 删除给定编号的教学楼节点.
 * 输入参数: phead主链头指针，教学楼编号。 
 * 输出参数: 五； 
 * 返 回 值: 布尔值 FALSE表示删除失败，TRUE表示删除成功。 
 * 调用说明:
 */
BOOL Del_Building_node(BUILDING_NODE **hd, char *building_no)
{
    struct building_node *current=*hd;
    struct building_node *prior=*hd;
    if(strcmp(current->building_No,building_no)==0)
    {
    	*hd=current->next;
    	return TRUE;
	}
    while((strcmp(current->building_No,building_no)!=0)&&current!=NULL)
    {
    	prior=current;
    	current=current->next;
	}
	CONSOLE_CURSOR_INFO lpCur;
	if (!current)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = FALSE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
			"无法找到文件",
			"确认"
		};

		ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;

	}
	prior->next=current->next;
	free(current);
	return TRUE;
}
BOOL Modify_building_info(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入待修改教学楼编号：", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top + 6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = {1, 0, 0};
    char aTag[] = {1, 2, 3}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
    struct building_node *p;
	p=(struct building_node*)malloc(sizeof(BUILDING_NODE));
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:
	    readtext(p->building_No,aArea[0]);
	   	break;
	   case 2:
	   	Modify_building_node(p);
	   goto FLAG;
	   case 3: 
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
/*统计主链中节点数目*/ 
int statbuildingno(BUILDING_NODE *hd)
{
	struct building_node *p=hd;
	int num=0;
	while(p)
	{
		num++;
		p=p->next;
	}
	return num;
}
BOOL MaintainbuildingInfo(void)
{
    int i;
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"录入教学楼相关信息", "删除教学楼相关信息","修改教学楼相关信息","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 6;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[100];
	for(i=0;i<labels.num;i++)
	{
		aLoc[i].X= rcPop.Left+2;
		aLoc[i].Y=rcPop.Top+i+1;
	 } 
	aLoc[3].X = rcPop.Left + 10;
    labels.pLoc = aLoc;

    areas.num = labels.num;
    SMALL_RECT aArea[100];
    char aSort[100];
    char aTag[100];
     for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);

    if (DealInput(&areas, &iHot) == 13 )
    {
    	switch(iHot)
    	{
    		case 1:Insert_buildingnode_window();
			break;
    		case 2:Del_building_info();
    		break;
    		case 3:Modify_building_info();
			break;
		}
	}
    PopOff();

    return TRUE;
}
BOOL Query_by_No(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入待查询教学楼编号", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top + 6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = {1, 0, 0};
    char aTag[] = {1, 2, 3}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
    BUILDING_NODE* pb;
	pb=(struct building_node*)malloc(sizeof(BUILDING_NODE)); 
	//pb=NULL;
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pb->building_No,aArea[0]);
	   break;
	   case 2:show_building_info(pb);
	   goto FLAG;
	   case 3: 
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BUILDING_NODE* SeekBuildingNode(BUILDING_NODE *hd,char* No)
{
	int find=0;
	struct building_node *p;
	for(p=hd;p!=NULL;p=p->next)
	{
		if(strcmp(p->building_No,No)==0)
		{
			find=1;
			break;
		}
	}
	if(find)
	{
		return p;
	}
	else
	{
		return NULL;
	}
}
BOOL readfloat(float *num,SMALL_RECT area)
{CONSOLE_CURSOR_INFO lpCur;
    DWORD res;
    INPUT_RECORD inRec;
    COORD mouse,pos;
	pos.X=area.Left;
	pos.Y=area.Top;
	char asc,vkc; 
	WORD att = BACKGROUND_BLUE | BACKGROUND_GREEN;
	char tmp[100]="\0";/*tmp临时储存当次输入内容, c临时储存一个汉字*/
	char c[100]="\0";
	int i=0;
	while(TRUE)
	    {
	    	FillConsoleOutputAttribute(gh_std_out, att, area.Right-area.Left+1, pos, &ul);
		    ReadConsoleInput(gh_std_in, &inRec, 1, &res);/*读入一条输入记录*/
		    if(strlen(tmp)>area.Right-area.Left+1)
		    {/*超出输入框*/
			break;
		    }
			if(inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
		    {
			  asc = inRec.Event.KeyEvent.uChar.AsciiChar;
			  vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
			  if(asc>='a'&&asc<='z' || asc>='A'&&asc<='Z' || asc>='0'&&asc<='9' || asc=='-' || asc=='.' || asc==',')
			  {/*正确的字符读入*/
				tmp[i]=asc;
				i+=1;
				tmp[i]='\0';
				SetConsoleCursorPosition(gh_std_out, pos);
				printf("%s",tmp);
			  }
			  else if(asc=='\b' && i!=0)
			  {/*退格处理*/
				i--;
				if((int)tmp[i]<0)
				{/*汉字*/
					tmp[i]='\0';
					tmp[--i]='\0';
				}
				else
				{	tmp[i]='\0';
			    }
				SetConsoleCursorPosition(gh_std_out, pos);
				printf("%s  ",tmp);
				printf("%c%c",'\b','\b');
			  }
			  else if(vkc==0&&asc<0)
			  {/*汉字读入*/
				c[0]=asc;
				ReadConsoleInput(gh_std_in, &inRec, 1, &res);
				c[1] = inRec.Event.KeyEvent.uChar.AsciiChar;
				c[2]='\0';
				strcat(tmp,c);
				i+=2;
				SetConsoleCursorPosition(gh_std_out, pos);
			 	printf("%s",tmp);
			  }
			  else if(vkc==38 || vkc==40 || vkc==27)
			  {/*方向键和ESC*/
				break;
			  }	
		    }
		    else if(inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		    {/*鼠标点击*/
			mouse= inRec.Event.MouseEvent.dwMousePosition;
			if(mouse.X<area.Left || mouse.X>area.Right || mouse.Y!=area.Top)
			{
				/*GetConsoleCursorInfo(gh_std_out, &lpCur);
		        lpCur.bVisible = FALSE;
		        SetConsoleCursorInfo(gh_std_out, &lpCur);*/
				break;
			}
		    }
	    }
	
	    *num=atof(tmp);
	    return TRUE;
}
BOOL readnum(int *num,SMALL_RECT area)
{
	    CONSOLE_CURSOR_INFO lpCur;
    DWORD res;
    INPUT_RECORD inRec;
    COORD mouse,pos;
	pos.X=area.Left;
	pos.Y=area.Top;
	char asc,vkc; 
	WORD att = BACKGROUND_BLUE | BACKGROUND_GREEN;
	char tmp[100]="\0";/*tmp临时储存当次输入内容, c临时储存一个汉字*/
	char c[100]="\0";
	int i=0;
	while(TRUE)
	    {
	    	FillConsoleOutputAttribute(gh_std_out, att, area.Right-area.Left+1, pos, &ul);
		    ReadConsoleInput(gh_std_in, &inRec, 1, &res);/*读入一条输入记录*/
		    if(strlen(tmp)>area.Right-area.Left+1)
		    {/*超出输入框*/
			break;
		    }
			if(inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
		    {
			  asc = inRec.Event.KeyEvent.uChar.AsciiChar;
			  vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
			  if(asc>='a'&&asc<='z' || asc>='A'&&asc<='Z' || asc>='0'&&asc<='9' || asc=='-' || asc=='.' || asc==',')
			  {/*正确的字符读入*/
				tmp[i]=asc;
				i+=1;
				tmp[i]='\0';
				SetConsoleCursorPosition(gh_std_out, pos);
				printf("%s",tmp);
			  }
			  else if(asc=='\b' && i!=0)
			  {/*退格处理*/
				i--;
				if((int)tmp[i]<0)
				{/*汉字*/
					tmp[i]='\0';
					tmp[--i]='\0';
				}
				else
				{	tmp[i]='\0';
			    }
				SetConsoleCursorPosition(gh_std_out, pos);
				printf("%s  ",tmp);
				printf("%c%c",'\b','\b');
			  }
			  else if(vkc==0&&asc<0)
			  {/*汉字读入*/
				c[0]=asc;
				ReadConsoleInput(gh_std_in, &inRec, 1, &res);
				c[1] = inRec.Event.KeyEvent.uChar.AsciiChar;
				c[2]='\0';
				strcat(tmp,c);
				i+=2;
				SetConsoleCursorPosition(gh_std_out, pos);
			 	printf("%s",tmp);
			  }
			  else if(vkc==38 || vkc==40 || vkc==27)
			  {/*方向键和ESC*/
				break;
			  }	
		    }
		    else if(inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		    {/*鼠标点击*/
			mouse= inRec.Event.MouseEvent.dwMousePosition;
			if(mouse.X<area.Left || mouse.X>area.Right || mouse.Y!=area.Top)
			{
				/*GetConsoleCursorInfo(gh_std_out, &lpCur);
		        lpCur.bVisible = FALSE;
		        SetConsoleCursorInfo(gh_std_out, &lpCur);*/
				break;
			}
		    }
	    }
	    *num=atoi(tmp);
	    return TRUE;
}
BOOL Modify_building_node(BUILDING_NODE *pb)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼编号","教学楼名称:","管理员电话：","教室间数:","容纳学生数:","教室总面积:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 16;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 7;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+7, rcPop.Top+14}};
    labels.pLoc = aLoc;

    areas.num = 7;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + 8+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 13+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 8+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 8+strlen(pCh[1]), rcPop.Top + 10},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 12,
                           rcPop.Left + 8+strlen(pCh[2]), rcPop.Top + 12},
	                       {rcPop.Left + 7, rcPop.Top + 14,
                           rcPop.Left + 10, rcPop.Top + 14},
                           {rcPop.Left + 15, rcPop.Top + 14,
                           rcPop.Left + 18, rcPop.Top + 14}};
    char aSort[] = { 1, 1, 1, 1, 1, 0, 0};
    char aTag[] =  { 1, 2, 3, 4, 5, 6, 7};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pb->building_No);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 11, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    int i,n;
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:
	   	readtext(pb->name,aArea[0]);
	   	break;
	   case 2: 
	    readtext(pb->telphone,aArea[1]);
	    break;
	   case 3:
	   	readnum(&(pb->class_amount),aArea[2]);
	   	break;
	   case 4:
	   	readnum(&(pb->stu_amount),aArea[3]);
	   	break;
	   case 5: 
	    readfloat(&(pb->total_area),aArea[4]);
	    break;
	   case 6: modify_building_node(pb);
	    goto FLAG;
	    break;
	   case 7:
	   	goto FLAG;
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
}
BOOL Insert_class_info(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼编号：", "教室编号：","楼层：","教室面积:","容纳学生数:","是否是多媒体教室:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 16;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 7;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+7, rcPop.Top+14}};
    labels.pLoc = aLoc;

    areas.num = 8;
    SMALL_RECT aArea[] = {{rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 2,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 2},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + +13+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 17+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 13+strlen(pCh[1]), rcPop.Top + 10},
                           {rcPop.Left + 14+strlen(pCh[2]), rcPop.Top + 12,
                           rcPop.Left + 17+strlen(pCh[2]), rcPop.Top + 12},
	                       {rcPop.Left + 7, rcPop.Top + 14,
                           rcPop.Left + 10, rcPop.Top + 14},
                           {rcPop.Left + 15, rcPop.Top + 14,
                           rcPop.Left + 18, rcPop.Top + 14}};
    char aSort[] = {1, 1, 1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6, 7, 8};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 11, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 15, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 10, pos, &ul);
    pos.X=rcPop.Left + 14+strlen(pCh[2]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att,4, pos, &ul);
	
	CLASS_NODE *pc;
	pc=(struct class_node*)malloc(sizeof(CLASS_NODE));
	char check2[3] = "Nn";
    
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:
	   	readtext(pc->building_No,aArea[0]);
	   	break;
	   case 2:
	   	readtext(pc->class_No,aArea[1]);
	   	break;
	   case 3: 
	    readnum(&(pc->floor),aArea[2]);
	    break;
	   case 4:
	   	readfloat(&(pc->class_area),aArea[3]);
	   	break;
	   case 5:
	   	readnum(&(pc->contain_Num),aArea[4]);
	   	break;
	   case 6: 
	    readtext(pc->type,aArea[5]);
	    break;
	   case 7:
		   if ((strstr(check, pc->type) == NULL)&&(strstr(check2,pc->type)==NULL))
		   {
			   PopOff();
			   GetConsoleCursorInfo(gh_std_out, &lpCur);
			   lpCur.bVisible = FALSE;
			   SetConsoleCursorInfo(gh_std_out, &lpCur);
			   char *plabel_name[] = {
				   "教室类型输入不合法",
				   "确认"
			   };

			   ShowModule(plabel_name, 2);
			   PopOff();
			   return FALSE;
		   }
		   InsertClassNode(gp_head,pc);
	    goto FLAG;
	    break;
	    case 8:
	    goto FLAG;	
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
}
BOOL InsertClassNode(BUILDING_NODE *hd,CLASS_NODE *pc)
{
    CONSOLE_CURSOR_INFO lpCur;
    BUILDING_NODE *pb;
    CLASS_NODE *p;
    p=(struct class_node*)malloc(sizeof(CLASS_NODE));
	/*首先在十字链中找到对应的学生基本信息节点*/
	pb=SeekBuildingNode(hd,pc->building_No);
	if(pb==NULL)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	}
	else/*如果找到，则插入该节点的缴费信息支链*/
	{
		strcpy(p->building_No,pc->building_No);
		strcpy(p->class_No,pc->class_No);
		strcpy(p->type,pc->type);
		p->class_area=pc->class_area;
		p->contain_Num=pc->contain_Num;
		p->floor=pc->floor;
		p->anext = 0;
		p->next=pb->cnext;
		pb->cnext=p;
		return TRUE;
	 } 
}
BOOL Del_class_info(void)
{
	PopOff();
	struct class_node *p;
	p=(CLASS_NODE*)malloc(sizeof(CLASS_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入教学楼编号：","  请输入教室编号：", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+6},
                    {rcPop.Left+7, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 10, rcPop.Top + 8,
                           rcPop.Left + 14, rcPop.Top + 8},
                           {rcPop.Left + 7, rcPop.Top + 10,
                           rcPop.Left + 10, rcPop.Top + 10},
                           {rcPop.Left + 15, rcPop.Top + 10,
                           rcPop.Left + 18, rcPop.Top + 10}
                           };
    char aSort[] = {1, 1, 0, 0};
    char aTag[] = {1, 2,  3, 4}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
     FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
     pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 8;
     FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(p->building_No,aArea[0]);
	   	break;
	   case 2:readtext(p->class_No,aArea[1]);
	   break;
	   case 3:Del_class_from_building(p);
	   goto FLAG;
	   case 4:
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Del_class_from_building(CLASS_NODE* pc)
{
    struct building_node *pb;
	pb=SeekBuildingNode(gp_head,pc->building_No);
	CONSOLE_CURSOR_INFO lpCur;
	if(pb==NULL)
	{
	  PopOff();
	  GetConsoleCursorInfo(gh_std_out, &lpCur);
      lpCur.bVisible = FALSE;
	  SetConsoleCursorInfo(gh_std_out, &lpCur);
	  char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
      ShowModule(plabel_name, 2);
	  PopOff();
	  return FALSE;	
	} 
	struct class_node *current=pb->cnext;
    struct class_node *prior=NULL; 
    if(strcmp(current->class_No,pc->class_No)==0)
    {
		pb->cnext = current->next;
		free(current);
    	return TRUE;
	}
    while((current != NULL)&&(strcmp(current->class_No,pc->class_No)!=0))
    {
    	prior=current;
    	current=current->next;
	}
	if(!current)
	{
		PopOff();
	  GetConsoleCursorInfo(gh_std_out, &lpCur);
      lpCur.bVisible = FALSE;
	  SetConsoleCursorInfo(gh_std_out, &lpCur);
	  char *plabel_name[] = {
                           "无法找到该教室",
                           "确认"
                          };
         
      ShowModule(plabel_name, 2);
	  PopOff();
	  return FALSE;
	}
	else
	prior->next=current->next;
	free(current);
	return TRUE;
}
BOOL Modify_class_info(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入教学楼编号：","  请输入教室编号：", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+6},
                    {rcPop.Left+7, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 10, rcPop.Top + 8,
                           rcPop.Left + 14, rcPop.Top + 8},
                           {rcPop.Left + 7, rcPop.Top + 10,
                           rcPop.Left + 10, rcPop.Top + 10},
                           {rcPop.Left + 15, rcPop.Top + 10,
                           rcPop.Left + 18, rcPop.Top + 10}
                           };
    char aSort[] = {1, 1, 0, 0};
    char aTag[] = {1, 2,  3, 4}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
     FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
     pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 8;
     FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
     struct class_node *p;
	p=(CLASS_NODE*)malloc(sizeof(CLASS_NODE));
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(p->building_No,aArea[0]);
	   	break;
	   case 2:readtext(p->class_No,aArea[1]);
	   break;
	   case 3:if(SeekBuildingNode(gp_head,p->building_No)==NULL)
	   {
	   	PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	   }
	   if(SeekClassNode(gp_head,p->class_No)==NULL)
	   {
	   	PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教室",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
		} 

	   Modify_class_node(p);
	   goto FLAG;
	   case 4:
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Modify_class_node(CLASS_NODE *pclass)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室编号","教学楼编号","楼层：","教室面积:","容纳学生数:","是否是多媒体教室:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 16;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 7;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+7, rcPop.Top+14}};
    labels.pLoc = aLoc;

    areas.num = 6;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 8+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[3]), rcPop.Top + 8,
                           rcPop.Left + 8+strlen(pCh[3]), rcPop.Top + 8},
                           {rcPop.Left + 3+strlen(pCh[4]), rcPop.Top + 10,
                           rcPop.Left + 8+strlen(pCh[4]), rcPop.Top + 10},
                           {rcPop.Left + 3+strlen(pCh[5]), rcPop.Top + 12,
                           rcPop.Left + 8+strlen(pCh[5]), rcPop.Top + 12},
	                       {rcPop.Left + 7, rcPop.Top + 14,
                           rcPop.Left + 10, rcPop.Top + 14},
                           {rcPop.Left + 15, rcPop.Top + 14,
                           rcPop.Left + 18, rcPop.Top + 14}};
    char aSort[] = {1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pclass->class_No);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pclass->building_No);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 14+strlen(pCh[2]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    CLASS_NODE *pc;
    pc=(struct class_node*)malloc(sizeof(CLASS_NODE));
    strcpy(pc->building_No,pclass->building_No);
    strcpy(pc->class_No,pclass->class_No);
	char check2[3] = "Nn";
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1: 
	    readnum(&(pc->floor),aArea[0]);
	    break;
	   case 2:
	   	readfloat(&(pc->class_area),aArea[1]);
	   	break;
	   case 3:
	   	readnum(&(pc->contain_Num),aArea[2]);
	   	break;
	   case 4: 
	    readtext(pc->type,aArea[3]);
	    break;
	   case 5:if ((strstr(check, pc->type) == NULL) && (strstr(check2, pc->type) == NULL))
	   {
		   PopOff();
		   GetConsoleCursorInfo(gh_std_out, &lpCur);
		   lpCur.bVisible = FALSE;
		   SetConsoleCursorInfo(gh_std_out, &lpCur);
		   char *plabel_name[] = {
			   "教室类型输入不合法",
			   "确认"
		   };

		   ShowModule(plabel_name, 2);
		   PopOff();
		   return FALSE;
	   }
		   modify_class_node(pc);
	    goto FLAG;
	    break;
	    case 6:
	    goto FLAG;	
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
}
BOOL modify_class_node(CLASS_NODE *pc)
{
	struct class_node *pclass;
	pclass=SeekClassNode(gp_head, pc->class_No);
	CONSOLE_CURSOR_INFO lpCur;
	if(pclass==NULL)
	{   
	    PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教室",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;	
	 } 
	strcpy(pclass->building_No,pc->building_No);
	pclass->class_area=pc->class_area;
	strcpy(pclass->class_No,pc->class_No);
	pclass->contain_Num=pc->contain_Num;
	strcpy(pclass->type,pc->type);
	pclass->floor=pc->floor;
	return TRUE; 
}
BOOL modify_building_node(BUILDING_NODE *pb)
{
	struct building_node *pbuilding;
	pbuilding=(struct building_node*)malloc(sizeof(BUILDING_NODE));
	pbuilding=SeekBuildingNode(gp_head, pb->building_No);
	CONSOLE_CURSOR_INFO lpCur;
	if(pbuilding==NULL)
	{
		PopOff(); 
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
    ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	 } 
	strcpy(pbuilding->building_No,pb->building_No);
	pbuilding->class_amount=pb->class_amount;
	strcpy(pbuilding->name,pb->name);
	pbuilding->stu_amount=pb->stu_amount;
	strcpy(pbuilding->telphone,pb->telphone);
	pbuilding->total_area=pb->total_area;
}
BOOL Insert_arrange_info(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室编号：", "学期：","周次:","星期:","节次:","课程名:","人数:","所属学院:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 20;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 9;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+3, rcPop.Top+14},
					{rcPop.Left+3, rcPop.Top+16},
					{rcPop.Left+7, rcPop.Top+18}}; 
					
    labels.pLoc = aLoc;

    areas.num = 10;
    SMALL_RECT aArea[] = {{rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 2,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 2},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + +15+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 10},
                           {rcPop.Left + 5+strlen(pCh[2]), rcPop.Top + 12,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 12},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 14,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 14},
                           {rcPop.Left + 7+strlen(pCh[2]), rcPop.Top + 16,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 16},
	                       {rcPop.Left +7, rcPop.Top + 18,
                           rcPop.Left + 10, rcPop.Top + 18},
                           {rcPop.Left + 15, rcPop.Top + 18,
                           rcPop.Left + 18, rcPop.Top + 18}};
    char aSort[] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 13, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 14, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att,12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 14;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 7+strlen(pCh[2]);
    pos.Y=rcPop.Top + 16;
    FillConsoleOutputAttribute(gh_std_out, att,10, pos, &ul);
    struct arrange_node *pa;
    pa=(ARRANGE_NODE*)malloc(sizeof(ARRANGE_NODE)); 
    
    char check1[100]="1-2";
	char check2[100]="3-4";
	char check3[100]="5-6";
	char check4[100]="7-8";
	char check5[100]="9-10";
	char check6[100]="11-12"; 
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pa->class_No,aArea[0]);
	   	break;
	   case 2:readtext(pa->semester,aArea[1]);
	   	break;
	   case 3:readtext(pa->week_No,aArea[2]);
	   	break;
	   case 4:readnum(&(pa->day),aArea[3]);
	   	break;
	   case 5:readtext(pa->time,aArea[4]);
	   	break;
	   case 6:readtext(pa->curr_name,aArea[5]);
	    break;
	   case 7:readnum(&(pa->Stu_No),aArea[6]);
	    break;
	   case 8:readtext(pa->school,aArea[7]);
	   	break;
	   case 9:
	   if(!((strcmp(pa->time,check1)==0)||(strcmp(pa->time,check2)==0)||(strcmp(pa->time,check3)==0)||(strcmp(pa->time,check4)==0)||(strcmp(pa->time,check5)==0)||(strcmp(pa->time,check6)==0)))
	   {
	   	//printf("%s",pa->time);
	   	
	   		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "节次信息输入不合法",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
        } 
	    InsertArrangeNode(gp_head,pa);
	    goto FLAG;
	   	break;
	   case 10:goto FLAG;
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
}
BOOL Del_arrange_info()
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室编号：", "学期：","周次:","星期:","节次:","课程名:","人数:","所属学院:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 20;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 9;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+3, rcPop.Top+14},
					{rcPop.Left+3, rcPop.Top+16},
					{rcPop.Left+7, rcPop.Top+18}}; 
					
    labels.pLoc = aLoc;

    areas.num = 10;
    SMALL_RECT aArea[] = {{rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 2,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 2},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + +15+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 10},
                           {rcPop.Left + 5+strlen(pCh[2]), rcPop.Top + 12,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 12},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 14,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 14},
                           {rcPop.Left + 7+strlen(pCh[2]), rcPop.Top + 16,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 16},
	                       {rcPop.Left +7, rcPop.Top + 18,
                           rcPop.Left + 10, rcPop.Top + 18},
                           {rcPop.Left + 15, rcPop.Top + 18,
                           rcPop.Left + 18, rcPop.Top + 18}};
    char aSort[] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 13, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 14, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att,12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 14;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 7+strlen(pCh[2]);
    pos.Y=rcPop.Top + 16;
    FillConsoleOutputAttribute(gh_std_out, att,10, pos, &ul);
    struct arrange_node *pa;
    pa=(ARRANGE_NODE*)malloc(sizeof(ARRANGE_NODE)); 
   
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pa->class_No,aArea[0]);
	   	break;
	   case 2:readtext(pa->curr_name,aArea[1]);
	   	break;
	   case 3:readnum(&(pa->day),aArea[2]);
	   	break;
	   case 4:readtext(pa->school,aArea[3]);
	   	break;
	   case 5:readtext(pa->semester,aArea[4]);
	   	break;
	   case 6:readnum(&(pa->Stu_No),aArea[5]);
	    break;
	   case 7:readtext(pa->time,aArea[6]);
	    break;
	   case 8:readtext(pa->week_No,aArea[7]);
	   	break;
	   case 9:DelArrangeNode(gp_head,pa); 
	   goto FLAG;
	   	break;
	   case 10:goto FLAG;
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
	
}

BOOL Modify_arrange_info(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室编号：", "学期：","周次:","星期:","节次:","课程名:","人数:","所属学院:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 20;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 9;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+3, rcPop.Top+14},
					{rcPop.Left+3, rcPop.Top+16},
					{rcPop.Left+7, rcPop.Top+18}}; 
					
    labels.pLoc = aLoc;

    areas.num = 10;
    SMALL_RECT aArea[] = {{rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 2,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 2},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + +15+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 10},
                           {rcPop.Left + 5+strlen(pCh[2]), rcPop.Top + 12,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 12},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 14,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 14},
                           {rcPop.Left + 7+strlen(pCh[2]), rcPop.Top + 16,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 16},
	                       {rcPop.Left +7, rcPop.Top + 18,
                           rcPop.Left + 10, rcPop.Top + 18},
                           {rcPop.Left + 15, rcPop.Top + 18,
                           rcPop.Left + 18, rcPop.Top + 18}};
    char aSort[] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 13, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 14, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att,12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 14;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 7+strlen(pCh[2]);
    pos.Y=rcPop.Top + 16;
    FillConsoleOutputAttribute(gh_std_out, att,10, pos, &ul);
    struct arrange_node *pa;
    pa=(ARRANGE_NODE*)malloc(sizeof(ARRANGE_NODE)); 
   
	char check1[100] = "1-2";
	char check2[100] = "3-4";
	char check3[100] = "5-6";
	char check4[100] = "7-8";
	char check5[100] = "9-10";
	char check6[100] = "11-12";

	while (DealInput(&areas, &iHot) == 13)
	{
		switch (iHot)
		{
		case 1:readtext(pa->class_No, aArea[0]);
			break;
		case 2:readtext(pa->curr_name, aArea[1]);
			break;
		case 3:readnum(&(pa->day), aArea[2]);
			break;
		case 4:readtext(pa->school, aArea[3]);
			break;
		case 5:readtext(pa->semester, aArea[4]);
			break;
		case 6:readnum(&(pa->Stu_No), aArea[5]);
			break;
		case 7:readtext(pa->time, aArea[6]);
			break;
		case 8:readtext(pa->week_No, aArea[7]);
			break;
		case 9:
			if (SeekClassNode(gp_head, pa->class_No) == NULL)
			{
				PopOff();
				GetConsoleCursorInfo(gh_std_out, &lpCur);
				lpCur.bVisible = FALSE;
				SetConsoleCursorInfo(gh_std_out, &lpCur);
				char *plabel_name[] = {
								   "无法找到该教室",
								   "确认"
				};

				ShowModule(plabel_name, 2);
				PopOff();
				return FALSE;
			}
			if (SeekArrangeNode(gp_head, pa) == NULL)
			{
				PopOff();
				GetConsoleCursorInfo(gh_std_out, &lpCur);
				lpCur.bVisible = FALSE;
				SetConsoleCursorInfo(gh_std_out, &lpCur);
				char *plabel_name[] = {
								   "无法找到该排课信息",
								   "确认"
				};

				ShowModule(plabel_name, 2);
				PopOff();
				return FALSE;
			}
			if (!((strcmp(pa->time, check1) == 0) || (strcmp(pa->time, check2) == 0) || (strcmp(pa->time, check3) == 0) || (strcmp(pa->time, check4) == 0) || (strcmp(pa->time, check5) == 0) || (strcmp(pa->time, check6) == 0)))
			{


				PopOff();
				GetConsoleCursorInfo(gh_std_out, &lpCur);
				lpCur.bVisible = FALSE;
				SetConsoleCursorInfo(gh_std_out, &lpCur);
				char *plabel_name[] = {
					"节次信息输入不合法",
					"确认"
				};
				ShowModule(plabel_name, 2);
				PopOff();
				return FALSE;
			}
			Modify_Arrange_node(pa);
			goto FLAG;
			break;
		case 10:goto FLAG;
			break;
		}
	}
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
	
}
BOOL ModifyArrangeNode(BUILDING_NODE *hd,ARRANGE_NODE *painfo,ARRANGE_NODE *pARRANGE_NODE)
{
	CONSOLE_CURSOR_INFO lpCur;
	ARRANGE_NODE *pARRANGE_NODE_temp;
	ARRANGE_NODE *pARRANGE_NODE_next;
	pARRANGE_NODE_temp=SeekArrangeNode(hd,painfo);
	if(pARRANGE_NODE_temp!=NULL)
	{
		pARRANGE_NODE_next=pARRANGE_NODE_temp->next;
		*pARRANGE_NODE_temp=*painfo;
		pARRANGE_NODE_temp->next=pARRANGE_NODE_next;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
 } 
BOOL show_building_info(BUILDING_NODE *pb)
{
	PopOff();
	CONSOLE_CURSOR_INFO lpCur;
	struct building_node *pbuilding;
	pbuilding=(struct building_node*)malloc(sizeof(BUILDING_NODE));
	if(SeekBuildingNode(gp_head,pb->building_No)==NULL)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
    ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;	
	}
	pbuilding=SeekBuildingNode(gp_head,pb->building_No);
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼编号：", "教学楼名称：","管理员电话：","教室间数:","容纳学生数:","教室总面积:","确定    取消"};
    int iHot = 1;
    pos.X = 27;
    pos.Y = 16;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 7;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+7, rcPop.Top+14}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {
	                       {rcPop.Left + 7, rcPop.Top + 14,
                           rcPop.Left + 10, rcPop.Top + 14},
                           {rcPop.Left + 15, rcPop.Top + 14,
                           rcPop.Left + 18, rcPop.Top + 14}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pbuilding->building_No);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pbuilding->name);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pbuilding->telphone);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%d",pbuilding->class_amount);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%d",pbuilding->stu_amount);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 12;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%f",pbuilding->total_area);
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1: 
	    goto FLAG; 
	    break;
	   case 2:
	    goto FLAG;	
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	return TRUE;
	PopOff();
}
BOOL Query_by_minfo(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入待查教学楼名称信息", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top + 6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = {1, 0, 0};
    char aTag[] = {1, 2, 3}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
    char *nam;
	nam=(char *)malloc(sizeof(char)*20); 
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(nam,aArea[0]);
	   break;
	   case 2:show_building_minfo(gp_head,nam);
	   goto FLAG;
	   case 3: 
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL show_building_minfo(BUILDING_NODE *hd,char *tmp)
{
    PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼相关信息统计","教学楼  教学楼编号 管理员电话 教室数 容纳学生数 教室总面积" ,"确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 65;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    int n;
    n=statbuildingno(gp_head);
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN| FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    int i=0;
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	struct building_node *p;
    for(p=hd;p!=NULL;p=p->next)
    {
    	if(strstr(p->name,tmp)!=NULL)
    	{
		    pos.X=rcPop.Left + 9;
            pos.Y = rcPop.Top + 5+i;
            SetConsoleCursorPosition(gh_std_out,pos);
             att =  FOREGROUND_GREEN| FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    	    printf("%-9s%-11s%-9s%-11d%-6d%-2f",p->name,p->building_No,p->telphone,p->class_amount,p->stu_amount,p->total_area);
    	i++;
        }
	}
	if(i==0)
	{
	    PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;	
	}
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        goto FLAG;
    }
    FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL RestoreData(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入待导入文件名", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{rcPop.Left + 8, rcPop.Top + 4,
                           rcPop.Left + 23, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top + 6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = {1, 0, 0};
    char aTag[] = {1, 2, 3}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 8;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 16, pos, &ul);
    char *txt_nam;
	txt_nam=(char*)malloc(15*sizeof(char)); 
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(txt_nam,aArea[0]);
	   break;
	   case 2:RestoreSysData(&gp_head,txt_nam);
	   goto FLAG;
	   case 3: 
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
 } 
BOOL show_class_info(CLASS_NODE *p)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室编号：", "教学楼编号：","楼层：","教室面积:","容纳学生数:","是否是多媒体教室:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 16;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 7;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+7, rcPop.Top+14}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {
	                       {rcPop.Left + 7, rcPop.Top + 14,
                           rcPop.Left + 10, rcPop.Top + 14},
                           {rcPop.Left + 15, rcPop.Top + 14,
                           rcPop.Left + 18, rcPop.Top + 14}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    
    struct class_node *pc;
	pc=SeekClassNode(gp_head,p->class_No);
	if(pc==NULL)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教室",
                           "确认"
                          };
         
    ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;
	}
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pc->class_No);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pc->building_No);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%d",pc->floor);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%f",pc->class_area);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%d",pc->contain_Num);
    pos.X=rcPop.Left + 15+strlen(pCh[2]);
    pos.Y=rcPop.Top + 12;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",pc->type);
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:goto FLAG;
	   	break;
	   case 2:goto FLAG;
	   	break;
	   
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	return TRUE;
	PopOff();
}
BOOL Queryclass_by_minfo()
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼编号：","楼层：","容纳学生数：   至", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 10;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
                    {rcPop.Left+3, rcPop.Top+6},
                    {rcPop.Left+7, rcPop.Top+8},
					                          };
    labels.pLoc = aLoc;

    areas.num = 6;
    SMALL_RECT aArea[] = { {rcPop.Left + 15, rcPop.Top + 2,
                           rcPop.Left + 19, rcPop.Top + 2},
                           {rcPop.Left + 9, rcPop.Top + 4,
                           rcPop.Left + 13, rcPop.Top + 4},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 17, rcPop.Top + 6}, 
                            {rcPop.Left + 20, rcPop.Top + 6,
                           rcPop.Left + 22, rcPop.Top + 6},
                           {rcPop.Left + 7, rcPop.Top + 8,
                           rcPop.Left + 10, rcPop.Top +8},
                           {rcPop.Left + 15, rcPop.Top + 8,
                           rcPop.Left + 18, rcPop.Top + 8}
                           };
    char aSort[] = {1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3,  4, 5, 6}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 15;
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
     pos.X=rcPop.Left + 9;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
     pos.X=rcPop.Left + 15;
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 3, pos, &ul);
     pos.X=rcPop.Left + 20;
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 3, pos, &ul); 
    struct class_node *pc;
    pc=(CLASS_NODE*)malloc(sizeof(CLASS_NODE));
	int min=0; 
	int max=0; 
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pc->building_No,aArea[0]);
	   break;
	   case 2:readnum(&(pc->floor),aArea[1]);
	   break;
	   case 3:readnum(&min,aArea[2]);
	   break;
	   case 4:readnum(&max,aArea[3]);
	   break;
	   case 5:show_class_minfo(pc,min,max);
	   goto FLAG; 
	   break;
	   case 6:goto FLAG; 
	   break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL show_class_minfo(CLASS_NODE *pc,int min,int max)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室相关信息","教室编号  教学楼编号 楼层 教室面积 容纳学生数 是否为多媒体教室" ,"确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 70;
    pos.Y = 16;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    

    struct class_node *pclass;
	struct building_node *pb;
	pb=SeekBuildingNode(gp_head,pc->building_No);
	if(pb==NULL)
	{   PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;
		
	 } 
	int i=0;
	for(pclass=pb->cnext;pclass!=NULL;pclass=pclass->next)
	{
		if((pclass->floor==pc->floor)&&((pclass->contain_Num<=max)&&(pclass->contain_Num>=min)))
		{
			GetConsoleCursorInfo(gh_std_out, &lpCur);
            lpCur.bVisible = FALSE;
	        SetConsoleCursorInfo(gh_std_out, &lpCur);
			pos.X=rcPop.Left + 8;
            pos.Y =rcPop.Top + 5+i;
            SetConsoleCursorPosition(gh_std_out,pos);
    	    printf("%-9s%-11s%-5d%-9.2f%-11d%-10s",pclass->class_No,pclass->building_No,pclass->floor,pclass->class_area,pclass->contain_Num,pclass->type);
    	    i++;
		}
		 
	}
	if(i==0)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到相应教室",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	 } 
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
       goto FLAG;
    }
    
    FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Queryarrange_by_info1()
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼编号","教室编号","学期","课程名:","星期:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 14;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+7, rcPop.Top+12}};
    labels.pLoc = aLoc;

    areas.num = 7;
    SMALL_RECT aArea[] = {{rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 2,
                           rcPop.Left + 8+strlen(pCh[0]), rcPop.Top + 2},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + 10+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 8+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 14+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 8+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 10+strlen(pCh[1]), rcPop.Top + 10},
	                       {rcPop.Left + 7, rcPop.Top + 12,
                           rcPop.Left + 10, rcPop.Top + 12},
                           {rcPop.Left + 15, rcPop.Top + 12,
                           rcPop.Left + 18, rcPop.Top + 12}};
    char aSort[] = {1, 1, 1, 1, 1, 0,  0};
    char aTag[] = {1, 2, 3, 4, 5, 6, 7};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 8, pos, &ul);
    pos.X=rcPop.Left + 8+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 7, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 6, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 8, pos, &ul);
    CLASS_NODE *pc;
    pc=(CLASS_NODE*)malloc(sizeof(CLASS_NODE)); 
    ARRANGE_NODE *pa;
    pa=(ARRANGE_NODE*)malloc(sizeof(ARRANGE_NODE));
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pc->building_No,aArea[0]);
	   	break;
	   case 2:readtext(pc->class_No,aArea[1]);
	   	break;
	   case 3:readtext(pa->semester,aArea[2]);
	    break;
	   case 4:readtext(pa->curr_name,aArea[3]);
	   	break;
	   case 5:readnum(&(pa->day),aArea[4]);
	   	break;
	   case 6:show_arrange_minfo1(pa,pc);
	   goto FLAG; 
	    break;
	   case 7: 
	    goto FLAG;
	   break;
	
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE; 
}
BOOL show_arrange_minfo1(ARRANGE_NODE *pa,CLASS_NODE *pc)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"排课相关信息","教室编号 学期 周次 星期 节次 课程名 学生人数 所属学院" ,"确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 65;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    struct class_node *pclass;
	pclass=SeekClassNode(gp_head,pc->class_No);
	if(pclass==NULL)
	{
	    PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到该教学楼",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;
		
	}
	struct arrange_node *parrange;
	int i=0;
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	for(parrange=pclass->anext;parrange!=NULL;parrange=parrange->next) 
	{
		if((strcmp(parrange->semester,pa->semester)==0)&&(strcmp(parrange->curr_name,pa->curr_name)==0)&&(parrange->day==pa->day))
		{
			
			pos.X=rcPop.Left + 8;
            pos.Y = rcPop.Top + 5+i;
            SetConsoleCursorPosition(gh_std_out,pos);
    	    printf("%-8s%-6s%-4s%-5d%-8s%-5s%-8d%-6s%",parrange->class_No,parrange->semester,parrange->week_No,parrange->day,parrange->time,parrange->curr_name,parrange->Stu_No,parrange->school);
	        i++;
		}
	}
	if(i==0)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无法找到相关排课信息",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;
	}
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        bRet = TRUE;
    }
    else
    {
        bRet = TRUE;
    } 
	
	PopOff();
    return TRUE;
}
BOOL Queryarrange_by_info2()
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"学期：","所属学院：","课程名：", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 10;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
                    {rcPop.Left+3, rcPop.Top+6},
                    {rcPop.Left+7, rcPop.Top+8},
					                          };
    labels.pLoc = aLoc;

    areas.num = 5;
    SMALL_RECT aArea[] = { {rcPop.Left + 9, rcPop.Top + 2,
                           rcPop.Left + 20, rcPop.Top + 2},
                           {rcPop.Left + 13, rcPop.Top + 4,
                           rcPop.Left + 20, rcPop.Top + 4},
                           {rcPop.Left + 11, rcPop.Top + 6,
                           rcPop.Left + 20, rcPop.Top + 6},
                           {rcPop.Left + 7, rcPop.Top + 8,
                           rcPop.Left + 10, rcPop.Top +8},
                           {rcPop.Left + 15, rcPop.Top + 8,
                           rcPop.Left + 18, rcPop.Top + 8}
                           
                           };
    char aSort[] = {1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 9;
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
     pos.X=rcPop.Left + 13;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 8, pos, &ul);
     pos.X=rcPop.Left + 11;
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 10, pos, &ul); 
	ARRANGE_NODE *pa;
	pa=(ARRANGE_NODE*)malloc(sizeof(ARRANGE_NODE)); 
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pa->semester,aArea[0]); 
	   break;
	   case 2:readtext(pa->curr_name,aArea[1]);
	   break;
	   case 3:readtext(pa->school,aArea[2]);
	   break;
	   case 4:show_arrange_minfo2(pa,gp_head);
	   goto FLAG; 
	   break;
	   case 5:goto FLAG; 
	   break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL show_arrange_minfo2(ARRANGE_NODE *pa,BUILDING_NODE *hd)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"排课相关信息","教室编号 学期 周次 星期 节次 课程名 学生人数 所属学院" ,"确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 65;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    
    struct building_node *pbuilding;
    struct class_node *pclass;
	struct arrange_node *parrange;
	int i=0;
	for(pbuilding=hd;pbuilding!=NULL;pbuilding=pbuilding->next)
	{
		for(pclass=pbuilding->cnext;pclass!=NULL;pclass=pclass->next)
		{
			for(parrange=pclass->anext;parrange!=NULL;parrange=parrange->next)
			{
				if((strcmp(parrange->semester,pa->semester)==0)&&(strcmp(parrange->curr_name,pa->curr_name)==0)&&(strcmp(parrange->school,pa->school)==0))
				{
					pos.X=rcPop.Left + 8;
                    pos.Y = rcPop.Top + 5+i;
                    SetConsoleCursorPosition(gh_std_out,pos);
    	            printf("%-8s%-6s%-4s%-5d%-8s%-5s%-8d%-6s",parrange->class_No,parrange->semester,parrange->week_No,parrange->day,parrange->time,parrange->curr_name,parrange->Stu_No,parrange->school);
	                i++;
	                GetConsoleCursorInfo(gh_std_out, &lpCur);
                    lpCur.bVisible = FALSE;
	                SetConsoleCursorInfo(gh_std_out, &lpCur);
				}
			}
		}
	}
	if(i==0)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "无对应排课信息",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE; 
	 } 
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        goto FLAG;
    }
    FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL StatBuildingClassQ()
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入待查询学期：", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top + 6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = {1, 0, 0};
    char aTag[] = {1, 2, 3}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
    char *tmp;
    tmp=(char*)malloc(20*sizeof(char));
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(tmp,aArea[0]);
	   break;
	   case 2:Showbuildingarrangeinfo(tmp);
	   goto FLAG;
	   case 3: 
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}

BOOL Showbuildingarrangeinfo(char *tmp)
{
    PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼开课信息统计","教学楼 开课门数 开课学时数 听课学生人次数 使用单位数","确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 65;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top +3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    
    BUILDING_NODE* pb;
	CLASS_NODE *pc;
	ARRANGE_NODE *pa;
	int subj_num=0;
	int total_hour=0;
	int stu_num=0;
	int school_num=0;
	CURR_NUM *pcn1;
	CURR_NUM *pcn2;
	SCHOOL_NUM *psn1;
	SCHOOL_NUM *psn2;
	int flag1 = 1;
	int flag2 = 1;
	char week_type1[]="1-12";
	char week_type2[]="14";
	char week_type3[]="16-18";
	int i=0;
	int signal=0;
	
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	
	for(pb=gp_head;pb!=NULL;pb=pb->next) 
	{
		subj_num=0;
	    total_hour=0;
	    stu_num=0;
	    school_num=0;
		for(pc=pb->cnext;pc!=NULL;pc=pc->next)
		{
			for(pa=pc->anext;pa!=NULL;pa=pa->next)
			{
				if(strcmp(pa->semester,tmp)==0)
				{
					signal=1;
					for(pcn1=PCN;pcn1!=NULL;pcn1=pcn1->next)
					{
						if(strcmp(pcn1->currnam,pa->curr_name)==0)
						{
							flag1=0;
						}
					}
					if(flag1)
					{
						pcn2=(CURR_NUM*)malloc(sizeof(CURR_NUM));
						strcpy(pcn2->currnam,pa->curr_name);
						pcn2->next=PCN;
						PCN=pcn2;
					}
					flag1=1;
					for(psn1=PSN;psn1!=NULL;psn1=psn1->next)
					{
						if(strcmp(psn1->schoolnam,pa->school)==0)
						{
							flag2=0;
						}
					}
					if(flag2)
					{
						psn2=(SCHOOL_NUM*)malloc(sizeof(SCHOOL_NUM));
						strcpy(psn2->schoolnam,pa->school);
						psn2->next=PSN;
						PSN=psn2;
					}
					flag2 = 1;
					if(strstr(pa->week_No,week_type1)!=NULL)
					{
						total_hour+=12*2;
					}
					if(strstr(pa->week_No,week_type2)!=NULL)
					{
						total_hour+=2;
					}
					if(strstr(pa->week_No,week_type3)!=NULL)
					{
						total_hour+=3*2;
					}
					stu_num+=pa->Stu_No;
				}
			}
		}
		if(signal==1)
		{
		for(pcn1=PCN;pcn1!=NULL;pcn1=pcn1->next)
		{
			subj_num++;
	    }
		for(psn1=PSN;psn1!=NULL;psn1=psn1->next)
		{
			school_num++;
		}
		pos.X=rcPop.Left + 10;
        pos.Y = rcPop.Top + 5+i;
        SetConsoleCursorPosition(gh_std_out,pos);
    	printf("%-10s%-10d%-10d%-10d%-10d",pb->name,subj_num,stu_num, stu_num,school_num);
    	i++;
    	signal=0;
    	PCN=NULL;
    	PSN=NULL;
        } 
	}
	if(i==0)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {"没有符合要求的教学楼",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	}
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        goto FLAG;
    }
    
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL StatClassArrangeQ()
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼:","学期:","周次:","星期:", "确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 5;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
                    {rcPop.Left+3, rcPop.Top+6},
                    {rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+7, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 6;
    SMALL_RECT aArea[] = { {rcPop.Left +10, rcPop.Top + 2,
                           rcPop.Left + 20, rcPop.Top + 2},
                           {rcPop.Left + 13, rcPop.Top + 4,
                           rcPop.Left + 20, rcPop.Top + 4},
                           {rcPop.Left + 11, rcPop.Top + 6,
                           rcPop.Left + 20, rcPop.Top + 6},
                           {rcPop.Left + 11, rcPop.Top + 8,
                           rcPop.Left + 20, rcPop.Top + 8},
                           {rcPop.Left + 7, rcPop.Top + 10,
                           rcPop.Left + 10, rcPop.Top +10},
                           {rcPop.Left + 15, rcPop.Top + 10,
                           rcPop.Left + 18, rcPop.Top + 10}
                           };
    char aSort[] = {1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 11, pos, &ul);
     pos.X=rcPop.Left + 13;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 8, pos, &ul);
     pos.X=rcPop.Left + 11;
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 10, pos, &ul); 
    pos.X=rcPop.Left + 11;
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 10, pos, &ul);
    char *building;
    building=(char*)malloc(20*sizeof(char));
    struct arrange_node *pa;
    pa=(struct arrange_node*)malloc(sizeof(ARRANGE_NODE));
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(building,aArea[0]);
	   break;
	   case 2:readtext(pa->semester,aArea[1]);
	   break;
	   case 3:readtext(pa->week_No,aArea[2]);
	   break;
	   case 4:readnum(&(pa->day),aArea[3]);
	   break;
	   case 5:Showclassarrangeinfo(building,pa); 
	   goto FLAG;
	   break;
	   case 6:goto FLAG; 
	   break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Showclassarrangeinfo(char *tmp,ARRANGE_NODE *parrange)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教学楼开课信息统计","教室 1-2节 3-4节 5-6节 7-8节 9-10节 11-12节","确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 65;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top +3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    BUILDING_NODE* pb;
	CLASS_NODE *pc;
	ARRANGE_NODE *pa;
	pb=SeekBuildingNode(gp_head,tmp);
	if(pb==NULL)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "找不到教学楼",
                           "确认"
                          };
         
    ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	 } 
	char time1[100]="1-2";
	char time2[100]="3-4";
	char time3[100]="5-6";
	char time4[100]="7-8";
	char time5[100]="9-10";
	char time6[100]="11-12";
	int i=0;
	int flag;
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	for(pc=pb->cnext;pc!=NULL;pc=pc->next)
	{
		flag=0;
		for(pa=pc->anext;pa!=NULL;pa=pa->next)
		{
			
			if((strcmp(pa->semester,parrange->semester)==0)&&(pa->day==parrange->day)&&(strstr(pa->week_No,parrange->week_No)!=NULL))
			{
				if(flag==0)
				{
				pos.X=rcPop.Left + 7;
                pos.Y = rcPop.Top + 5+i;
                SetConsoleCursorPosition(gh_std_out,pos);
    	        printf("%s",pc->class_No);
    	        flag=1;
    	        }
    	        if(strcmp(pa->time,time1)==0)
    	        {
				pos.X=rcPop.Left + 12;
                pos.Y = rcPop.Top + 5+i;
                SetConsoleCursorPosition(gh_std_out,pos);
    	        printf("%s",pa->curr_name);
    	        }
    	        if(strcmp(pa->time,time2)==0)
    	        {
				pos.X=rcPop.Left + 18;
                pos.Y = rcPop.Top + 5+i;
                SetConsoleCursorPosition(gh_std_out,pos);
    	        printf("%s",pa->curr_name);
    	        }
    	        if(strcmp(pa->time,time3)==0)
    	        {
    	        	pos.X=rcPop.Left + 24;
                    pos.Y = rcPop.Top + 5+i;
                    SetConsoleCursorPosition(gh_std_out,pos);
    	        	printf("%s",pa->curr_name);
				}
    	        if(strcmp(pa->time,time4)==0)
    	        {
    	        	pos.X=rcPop.Left + 100;
                    pos.Y = rcPop.Top + 5+i;
                    SetConsoleCursorPosition(gh_std_out,pos);
    	        	printf("%s",pa->curr_name);
				}
    	        if(strcmp(pa->time,time5)==0)
    	        {
    	        	pos.X=rcPop.Left + 36;
                    pos.Y = rcPop.Top + 5+i;
                    SetConsoleCursorPosition(gh_std_out,pos);
    	        	printf("%s",pa->curr_name);
				}
    	        if(strcmp(pa->time,time6)==0)
    	         {
    	        	pos.X=rcPop.Left + 43;
                    pos.Y = rcPop.Top + 5+i;
                    SetConsoleCursorPosition(gh_std_out,pos);
    	        	printf("%s",pa->curr_name);
				}
				
			}
		}
	    i++;
	}
	if(i==0)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "找不到教室",
                           "确认"
                          };
         
    ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	}
    if (DealInput(&areas, &iHot) == 13 )
    {
        bRet = TRUE;
    }
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL StatCurrArrangeQ()
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"学期:","学院:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = { {rcPop.Left +10, rcPop.Top + 2,
                           rcPop.Left + 20, rcPop.Top + 2},
                           {rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 20, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top +6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = {1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 2;
    FillConsoleOutputAttribute(gh_std_out, att, 11, pos, &ul);
     pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
     FillConsoleOutputAttribute(gh_std_out, att, 11, pos, &ul);
     
     char *semester;
     char *school;
     semester=(char*)malloc(100*sizeof(char));
     school=(char *)malloc(100*sizeof(char));
     
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(semester,aArea[0]);
	   break;
	   case 2:readtext(school,aArea[1]);
	   break;
	   case 3:Showcurrarrangeinfo(semester,school);
	   goto FLAG;
	   break;
	   case 4:
	   	goto FLAG;
	   break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Showcurrarrangeinfo(char* tmp1,char *tmp2)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"课程相关信息统计","课程名  教学楼 教室编号 周次 星期 节次" ,"确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 55;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    
    
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+20, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+24, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 24, rcPop.Top + 10,
                           rcPop.Left + 27, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    BUILDING_NODE *pb;
	CLASS_NODE *pc;
	ARRANGE_NODE *pa;
	int i=0;
	for(pb=gp_head;pb!=NULL;pb=pb->next)
	{
		for(pc=pb->cnext;pc!=NULL;pc=pc->next)
	    {
		    for(pa=pc->anext;pa!=NULL;pa=pa->next)
		     {
		     	if((strcmp(pa->semester,tmp1)==0)&&(strcmp(pa->school,tmp2)==0))
		     	{
		     		pos.X=rcPop.Left + 10;
                    pos.Y = rcPop.Top + 5+i;
                    SetConsoleCursorPosition(gh_std_out,pos);
                    printf("%-5s%-7s%-10s%-5s%-5d%-3s",pa->curr_name,pb->name,pc->class_No,pa->week_No,pa->day,pa->time);
                    i++;
                    GetConsoleCursorInfo(gh_std_out, &lpCur);
                    lpCur.bVisible = FALSE;
	                SetConsoleCursorInfo(gh_std_out, &lpCur);
				}
			 }
	    }
	}
    if(i==0)
    {
    	PopOff();
    	GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "找不到相应排课信息",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return FALSE;
	}
			
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        goto FLAG;
    }
    
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Query_class_by_No(void)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"请输入待查询教室编号：","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    
                    {rcPop.Left+7, rcPop.Top+6},
					                          };
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{rcPop.Left + 10, rcPop.Top + 4,
                           rcPop.Left + 14, rcPop.Top + 4},
                           {rcPop.Left + 7, rcPop.Top + 6,
                           rcPop.Left + 10, rcPop.Top + 6},
                           {rcPop.Left + 15, rcPop.Top + 6,
                           rcPop.Left + 18, rcPop.Top + 6}
                           };
    char aSort[] = { 1, 0, 0};
    char aTag[] = {1, 2, 3}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 10;
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 5, pos, &ul);
    CLASS_NODE* pc;
	pc=(struct class_node*)malloc(sizeof(CLASS_NODE)); 
	
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pc->class_No,aArea[0]);
	   break;
	   case 2:show_class_info(pc);
	   goto FLAG;
	   case 3:
	   goto FLAG;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Modify_Arrange_node(ARRANGE_NODE *painfo)
{
	PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室编号：", "学期：","周次:","星期:","节次:","课程名:","人数:","所属学院:","确定    取消"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 27;
    pos.Y = 20;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 9;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+3, rcPop.Top+4},
					{rcPop.Left+3, rcPop.Top+6},
					{rcPop.Left+3, rcPop.Top+8},
                    {rcPop.Left+3, rcPop.Top+10},
					{rcPop.Left+3, rcPop.Top+12},
					{rcPop.Left+3, rcPop.Top+14},
					{rcPop.Left+3, rcPop.Top+16},
					{rcPop.Left+7, rcPop.Top+18}}; 
					
    labels.pLoc = aLoc;

    areas.num = 9;
    SMALL_RECT aArea[] = {/*{rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 2,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 2},*/ 
                           {rcPop.Left + 3+strlen(pCh[1]), rcPop.Top + 4,
                           rcPop.Left + +15+strlen(pCh[1]), rcPop.Top + 4},
                           {rcPop.Left + 3+strlen(pCh[2]), rcPop.Top + 6,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 6},
                           {rcPop.Left + 3+strlen(pCh[0]), rcPop.Top + 8,
                           rcPop.Left + 11+strlen(pCh[0]), rcPop.Top + 8},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 10,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 10},
                           {rcPop.Left + 5+strlen(pCh[2]), rcPop.Top + 12,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 12},
                           {rcPop.Left + 4+strlen(pCh[1]), rcPop.Top + 14,
                           rcPop.Left + 15+strlen(pCh[1]), rcPop.Top + 14},
                           {rcPop.Left + 7+strlen(pCh[2]), rcPop.Top + 16,
                           rcPop.Left + 16+strlen(pCh[2]), rcPop.Top + 16},
	                       {rcPop.Left +7, rcPop.Top + 18,
                           rcPop.Left + 10, rcPop.Top + 18},
                           {rcPop.Left + 15, rcPop.Top + 18,
                           rcPop.Left + 18, rcPop.Top + 18}};
    char aSort[] = { 1, 1, 1, 1, 1, 1, 1, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 2;
    SetConsoleCursorPosition(gh_std_out,pos);
    printf("%s",painfo->class_No);
    pos.X=rcPop.Left + 3+strlen(pCh[1]);
    pos.Y=rcPop.Top + 4;
    FillConsoleOutputAttribute(gh_std_out, att, 13, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[2]);
    pos.Y=rcPop.Top + 6;
    FillConsoleOutputAttribute(gh_std_out, att, 14, pos, &ul);
    pos.X=rcPop.Left + 3+strlen(pCh[0]);
    pos.Y=rcPop.Top + 8;
    FillConsoleOutputAttribute(gh_std_out, att, 9, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 10;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 12;
    FillConsoleOutputAttribute(gh_std_out, att,12, pos, &ul);
    pos.X=rcPop.Left + 4+strlen(pCh[1]);
    pos.Y=rcPop.Top + 14;
    FillConsoleOutputAttribute(gh_std_out, att, 12, pos, &ul);
    pos.X=rcPop.Left + 7+strlen(pCh[2]);
    pos.Y=rcPop.Top + 16;
    FillConsoleOutputAttribute(gh_std_out, att,10, pos, &ul);
    struct arrange_node *pa;
    pa=(ARRANGE_NODE*)malloc(sizeof(ARRANGE_NODE)); 
    strcpy(pa->class_No,painfo->class_No);
    while(DealInput(&areas, &iHot) == 13)
	{
	   switch(iHot)
	   {
	   case 1:readtext(pa->curr_name,aArea[0]);
	   	break;
	   case 2:readnum(&(pa->day),aArea[1]);
	   	break;
	   case 3:readtext(pa->school,aArea[2]);
	   	break;
	   case 4:readtext(pa->semester,aArea[3]);
	   	break;
	   case 5:readnum(&(pa->Stu_No),aArea[4]);
	    break;
	   case 6:readtext(pa->time,aArea[5]);
	    break;
	   case 7:readtext(pa->week_No,aArea[6]);
	   	break;
	   case 8:ModifyArrangeNode(gp_head,painfo,pa);
	   goto FLAG;
	   	break;
	   case 9:goto FLAG;
	   	break;
	  }
	} 
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
int statclassno(BUILDING_NODE *hd)
{
	struct building_node *p;
	struct class_node *pc;
	int num=0;
    for(p=hd;p!=NULL;p=p->next)
    {
    	for(pc=p->cnext;pc!=NULL;pc=pc->next)
    	{
    		num++;
		}
	}
	return num;
}
BOOL Statclassinfo()
{
    PopOff();
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"教室相关信息统计","教学楼编号 教室编号 楼层 教室面积    容纳学生数" ,"确定"};
    int iHot = 1;
    CONSOLE_CURSOR_INFO lpCur;
    pos.X = 65;
    pos.Y = 12;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE |BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+23, rcPop.Top+2},
                    {rcPop.Left+7, rcPop.Top+4}, 
                    {rcPop.Left+29, rcPop.Top+10},
					                          };
    labels.pLoc = aLoc;
    areas.num = 1;
    SMALL_RECT aArea[] = {
                           {rcPop.Left + 29, rcPop.Top + 10,
                           rcPop.Left + 32, rcPop.Top + 10},
                           };
    char aSort[] = {0};
    char aTag[] = {1}; 
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top +3;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    att =  FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
    
    int n=0;
	n=statclassno(gp_head); 
	if(n==0)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
	    SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
                           "没有教室信息",
                           "确认"
                          };
         
        ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;
	 } 
	int i=0;
	struct building_node *pb;
    struct class_node *pc;
    for(pb=gp_head;pb!=NULL;pb=pb->next)
	{
		for(pc=pb->cnext;pc!=NULL;pc=pc->next)
		{
		pos.X=rcPop.Left + 10;
        pos.Y=rcPop.Top + 5+i;
        SetConsoleCursorPosition(gh_std_out,pos);
    	printf("%-8s%-9s%-5d%-12f%-6d",pc->building_No,pc->class_No,pc->floor,pc->class_area,pc->contain_Num);
	    i++;
	    }
    }
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        goto FLAG;
    }
	FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
    return TRUE;
}
BOOL Statarrangeinfo()
{
	PopOff();
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	char *pCh[] = { "排课相关信息统计","教室编号   学期     周次 星期        课程名" ,"确定" };
	int iHot = 1;
	CONSOLE_CURSOR_INFO lpCur;
	pos.X = 65;
	pos.Y = 12;
	rcPop.Left = (SCR_COL - pos.X) / 2;
	rcPop.Right = rcPop.Left + pos.X - 1;
	rcPop.Top = (SCR_ROW - pos.Y) / 2;
	rcPop.Bottom = rcPop.Top + pos.Y - 1;
	att = BACKGROUND_BLUE | BACKGROUND_GREEN; /*| BACKGROUND_RED;*/  /*白底黑字*/
	labels.num = 3;
	labels.ppLabel = pCh;
	COORD aLoc[] = { { rcPop.Left + 23, rcPop.Top + 2 },
	{ rcPop.Left + 7, rcPop.Top + 4 },
	{ rcPop.Left + 29, rcPop.Top + 10 },
	};
	labels.pLoc = aLoc;
	areas.num = 1;
	SMALL_RECT aArea[] = {
		{ rcPop.Left + 29, rcPop.Top + 10,
		rcPop.Left + 32, rcPop.Top + 10 },
	};
	char aSort[] = { 0 };
	char aTag[] = { 1 };
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&rcPop, att, &labels, &areas);
	pos.X = rcPop.Left + 1;
	pos.Y = rcPop.Top + 3;
	FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 1, pos, &ul);
	att = FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;

	int n = 0;
	n = statarrangeno(gp_head);
	if (n == 0)
	{
		PopOff();
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = FALSE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
		char *plabel_name[] = {
			"没有排课信息",
			"确认"
		};

		ShowModule(plabel_name, 2);
		PopOff();
		return TRUE;
	}
	int i = 0;
	struct building_node *pb;
	struct class_node *pc;
	struct arrange_node *pa;
	for (pb = gp_head; pb != NULL; pb = pb->next)
	{
		for (pc = pb->cnext; pc != NULL; pc = pc->next)
		{
			for (pa = pc->anext; pa != NULL; pa = pa->next)
			{
				pos.X = rcPop.Left + 10;
				pos.Y = rcPop.Top + 5 + i;
				SetConsoleCursorPosition(gh_std_out, pos);
				printf("%-8s%-9s%-5s%-12d%-6s", pa->class_No, pa->semester,pa->week_No, pa->day, pa->curr_name);
				i++;
			}
		}
	}
	if (DealInput(&areas, &iHot) == 13 && iHot == 1)
	{
		goto FLAG;
	}
    FLAG:
	GetConsoleCursorInfo(gh_std_out, &lpCur);
	lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);
	PopOff();
	return TRUE;
}

int statarrangeno(BUILDING_NODE *hd)
{
	struct building_node *p;
	struct class_node *pc;
	struct arrange_node *pa;
	int num = 0;
	for (p = hd; p != NULL; p = p->next)
	{
		for (pc = p->cnext; pc != NULL; pc = pc->next)
		{
			for (pa = pc->anext; pa != NULL; pa = pa->next)
			{
				num++;
			}
		}
	}
	return num;
}
/**
* 函数名称: InitInterface
* 函数功能: 初始化界面.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*
* 调用说明:
*/
void InitInterface()
{
	WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
		| BACKGROUND_BLUE;  /*黄色前景和蓝色背景*/

	SetConsoleTextAttribute(gh_std_out, att);  /*设置控制台屏幕缓冲区字符属性*/

	ClearScreen();  /* 清屏*/

					/*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
	gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*屏幕字符属性*/
	gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
	gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/
	gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/
	gp_top_layer->rcArea.Top = 0;
	gp_top_layer->rcArea.Right = SCR_COL - 1;
	gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
	gp_top_layer->pContent = NULL;
	gp_top_layer->pScrAtt = gp_scr_att;
	gp_top_layer->next = NULL;

	ShowMenu();     /*显示菜单栏*/
	//ShowState();    /*显示状态栏*/

	return;
}
/**
* 函数名称: ClearScreen
* 函数功能: 清除屏幕信息.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*
* 调用说明:
*/
void ClearScreen(void)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home = { 0, 0 };
	unsigned long size;

	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);/*取屏幕缓冲区信息*/
	size = bInfo.dwSize.X * bInfo.dwSize.Y; /*计算屏幕缓冲区字符单元数*/

											/*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
	FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);

	/*将屏幕缓冲区所有单元填充为空格字符*/
	FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);

	return;
}
/**
* 函数名称: ShowMenu
* 函数功能: 在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*
* 调用说明:
*/
void ShowMenu()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	CONSOLE_CURSOR_INFO lpCur;
	COORD size;
	COORD pos = { 0, 0 };
	int i, j;
	int PosA = 2, PosB;
	char ch;

	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
	size.X = bInfo.dwSize.X;
	size.Y = 1;
	SetConsoleCursorPosition(gh_std_out, pos);
	for (i = 0; i < 5; i++) /*在窗口第一行第一列处输出主菜单项*/
	{
		printf("  %s  ", ga_main_menu[i]);
	}

	GetConsoleCursorInfo(gh_std_out, &lpCur);
	lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);  /*隐藏光标*/

											   /*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/
	gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	SMALL_RECT rcMenu = { 0, 0, size.X - 1, 0 };

	/*将窗口第一行的内容读入到存放菜单条屏幕区字符信息的缓冲区中*/
	ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

	/*将这一行中英文字母置为红色，其他字符单元置为白底黑字*/
	for (i = 0; i<size.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
			| BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}

	/*修改后的菜单条字符信息回写到窗口的第一行*/
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	COORD endPos = { 0, 1 };
	SetConsoleCursorPosition(gh_std_out, endPos);  /*将光标位置设置在第2行第1列*/

												   /*将菜单项置为热区，热区编号为菜单项号，热区类型为0(按钮型)*/
	i = 0;
	do
	{
		PosB = PosA + strlen(ga_main_menu[i]);  /*定位第i+1号菜单项的起止位置*/
		for (j = PosA; j<PosB; j++)
		{
			gp_scr_att[j] |= (i + 1) << 2; /*设置菜单项所在字符单元的属性值*/
		}
		PosA = PosB + 4;
		i++;
	} while (i<5);

	TagMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/

	return;
}
/**
* 函数名称: PopUp
* 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.
* 输入参数: pRc 弹出窗口位置数据存放的地址
*           att 弹出窗口区域字符属性
*           pLabel 弹出窗口中标签束信息存放的地址
pHotArea 弹出窗口中热区信息存放的地址
* 输出参数: 无
* 返 回 值: 无
*
* 调用说明:
*/
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
	LAYER_NODE *nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char *pCh;
	int i, j, row;

	/*弹出窗口所在位置字符单元信息入栈*/
	size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
	size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/
											/*申请存放弹出窗口相关信息的动态存储区*/
	nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
	nextLayer->next = gp_top_layer;
	nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
	nextLayer->rcArea = *pRc;
	nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y * sizeof(CHAR_INFO));
	nextLayer->pScrAtt = (char *)malloc(size.X*size.Y * sizeof(char));
	pCh = nextLayer->pScrAtt;
	/*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
	ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
		for (j = pRc->Left; j <= pRc->Right; j++)
		{
			*pCh = gp_scr_att[i*SCR_COL + j];
			pCh++;
		}
	}
	gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/
							   /*设置弹出窗口区域字符的新属性*/
	pos.X = pRc->Left;
	pos.Y = pRc->Top;
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{
		FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
		pos.Y++;
	}
	/*将标签束中的标签字符串在设定的位置输出*/
	for (i = 0; i<pLabel->num; i++)
	{
		pCh = pLabel->ppLabel[i];
		if (strlen(pCh) != 0)
		{
			WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
				pLabel->pLoc[i], &ul);
		}
	}
	/*设置弹出窗口区域字符单元的新属性*/
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*此二重循环设置字符单元的层号*/
		for (j = pRc->Left; j <= pRc->Right; j++)
		{
			gp_scr_att[i*SCR_COL + j] = gp_top_layer->LayerNo;
		}
	}

	for (i = 0; i<pHotArea->num; i++)
	{   /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
		row = pHotArea->pArea[i].Top;
		for (j = pHotArea->pArea[i].Left; j <= pHotArea->pArea[i].Right; j++)
		{
			gp_scr_att[row*SCR_COL + j] |= (pHotArea->pSort[i] << 6)
				| (pHotArea->pTag[i] << 2);
		}
	}
	return;
}

/**
* 函数名称: PopOff
* 函数功能: 关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*
* 调用说明:
*/
void PopOff(void)
{
	LAYER_NODE *nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char *pCh;
	int i, j;

	if ((gp_top_layer->next == NULL) || (gp_top_layer->pContent == NULL))
	{   /*栈底存放的主界面屏幕信息，不用关闭*/
		return;
	}
	nextLayer = gp_top_layer->next;
	/*恢复弹出窗口区域原外观*/
	size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
	size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
	WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
	/*恢复字符单元原属性*/
	pCh = gp_top_layer->pScrAtt;
	for (i = gp_top_layer->rcArea.Top; i <= gp_top_layer->rcArea.Bottom; i++)
	{
		for (j = gp_top_layer->rcArea.Left; j <= gp_top_layer->rcArea.Right; j++)
		{
			gp_scr_att[i*SCR_COL + j] = *pCh;
			pCh++;
		}
	}
	free(gp_top_layer->pContent);    /*释放动态存储区*/
	free(gp_top_layer->pScrAtt);
	free(gp_top_layer);
	gp_top_layer = nextLayer;
	gi_sel_sub_menu = 0;
	return;
}

/**
* 函数名称: DrawBox
* 函数功能: 在指定区域画边框.
* 输入参数: pRc 存放区域位置信息的地址
* 输出参数: 无
* 返 回 值: 无
*
* 调用说明:
*/
void DrawBox(SMALL_RECT *pRc)
{
	char chBox[] = { '+','-','|' };  /*画框用的字符*/
	COORD pos = { pRc->Left, pRc->Top };  /*定位在区域的左上角*/

	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左上角*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
	{   /*此循环画上边框横线*/
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
	}
	pos.X = pRc->Right;
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右上角*/
	for (pos.Y = pRc->Top + 1; pos.Y < pRc->Bottom; pos.Y++)
	{   /*此循环画边框左边线和右边线*/
		pos.X = pRc->Left;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
		pos.X = pRc->Right;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
	}
	pos.X = pRc->Left;
	pos.Y = pRc->Bottom;
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左下角*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
	{   /*画下边框横线*/
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
	}
	pos.X = pRc->Right;
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右下角*/
	return;
}
/**
* 函数名称: DealInput
* 函数功能: 在弹出窗口区域设置热区, 等待并相应用户输入.
* 输入参数: pHotArea
*           piHot 焦点热区编号的存放地址, 即指向焦点热区编号的指针
* 输出参数: piHot 用鼠标单击、按回车或空格时返回当前热区编号
* 返 回 值:
*
* 调用说明:
*/
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	int num, arrow, iRet = 0;
	int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
	char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/

	SetHotPoint(pHotArea, *piHot);
	while (TRUE)
	{    /*循环*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);
		if ((inRec.EventType == MOUSE_EVENT) &&
			(inRec.Event.MouseEvent.dwButtonState
				== FROM_LEFT_1ST_BUTTON_PRESSED))
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
			cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
			cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;

			if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
			{
				*piHot = cTag;
				SetHotPoint(pHotArea, *piHot);
				if (cSort == 0)
				{
					iRet = 13;
					break;
				}
			}
		}
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
			if (asc == 0)
			{
				arrow = 0;
				switch (vkc)
				{  /*方向键(左、上、右、下)的处理*/
				case 37: arrow = 1; break;
				case 38: arrow = 2; break;
				case 39: arrow = 3; break;
				case 40: arrow = 4; break;
				}
				if (arrow > 0)
				{
					num = *piHot;
					while (TRUE)
					{
						if (arrow < 3)
						{
							num--;
						}
						else
						{
							num++;
						}
						if ((num < 1) || (num > pHotArea->num) ||
							((arrow % 2) && (pHotArea->pArea[num - 1].Top
								== pHotArea->pArea[*piHot - 1].Top)) || ((!(arrow % 2))
									&& (pHotArea->pArea[num - 1].Top
										!= pHotArea->pArea[*piHot - 1].Top)))
						{
							break;
						}
					}
					if (num > 0 && num <= pHotArea->num)
					{
						*piHot = num;
						SetHotPoint(pHotArea, *piHot);
					}
				}
			}
			else if (vkc == 27)
			{  /*ESC键*/
				iRet = 27;
				break;
			}
			else if (vkc == 13 || vkc == 32)
			{  /*回车键或空格表示按下当前按钮*/
				iRet = 13;
				break;
			}
		}
	}
	return iRet;
}
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
	CONSOLE_CURSOR_INFO lpCur;
	COORD pos = { 0, 0 };
	WORD att1, att2;
	int i, width;

	att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
	att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
	for (i = 0; i<pHotArea->num; i++)
	{  /*将按钮类热区置为白底黑字*/
		pos.X = pHotArea->pArea[i].Left;
		pos.Y = pHotArea->pArea[i].Top;
		width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
		if (pHotArea->pSort[i] == 0)
		{  /*热区是按钮类*/
			FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
		}
	}

	pos.X = pHotArea->pArea[iHot - 1].Left;
	pos.Y = pHotArea->pArea[iHot - 1].Top;
	width = pHotArea->pArea[iHot - 1].Right - pHotArea->pArea[iHot - 1].Left + 1;
	if (pHotArea->pSort[iHot - 1] == 0)
	{  /*被激活热区是按钮类*/
		FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
	}
	else if (pHotArea->pSort[iHot - 1] == 1)
	{  /*被激活热区是文本框类*/
		SetConsoleCursorPosition(gh_std_out, pos);
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = TRUE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
	}
}
/**
* 函数名称: TagSubMenu
* 函数功能: 在指定子菜单项上做选中标记.
* 输入参数: num 选中的子菜单项号
* 输出参数: 无
* 返 回 值: 无
*
* 调用说明:
*/
void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	int width;

	LocSubMenu(gi_sel_menu, &rcPop);  /*计算弹出子菜单的区域位置, 存放在rcPop中*/
	if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom - rcPop.Top - 1))
	{   /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
		return;
	}

	pos.X = rcPop.Left + 2;
	width = rcPop.Right - rcPop.Left - 3;
	if (gi_sel_sub_menu != 0) /*首先取消原选中子菜单项上的标记*/
	{
		pos.Y = rcPop.Top + gi_sel_sub_menu;
		att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
		FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
		pos.X += 1;
		att |= FOREGROUND_RED;/*白底红字*/
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
	}
	/*在制定子菜单项上做选中标记*/
	pos.X = rcPop.Left + 2;
	pos.Y = rcPop.Top + num;
	att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
	FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
	gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
	return;
}

/**
* 函数名称: LocSubMenu
* 函数功能: 计算弹出子菜单区域左上角和右下角的位置.
* 输入参数: num 选中的主菜单项号
* 输出参数: rc 存放区域位置信息的地址
* 返 回 值: 无
*
* 调用说明:
*/
void LocSubMenu(int num, SMALL_RECT *rc)
{
	int i, len, loc = 0;

	rc->Top = 1; /*区域的上边定在第2行，行号为1*/
	rc->Left = 1;
	for (i = 1; i<num; i++)
	{   /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
		rc->Left += strlen(ga_main_menu[i - 1]) + 4;
		loc += ga_sub_menu_count[i - 1];
	}
	rc->Right = strlen(ga_sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/
	for (i = 1; i<ga_sub_menu_count[num - 1]; i++)
	{   /*查找最长子菜单字符串，将其长度存放在rc->Right*/
		len = strlen(ga_sub_menu[loc + i]);
		if (rc->Right < len)
		{
			rc->Right = len;
		}
	}
	rc->Right += rc->Left + 3;  /*计算区域的右边界*/
	rc->Bottom = rc->Top + ga_sub_menu_count[num - 1] + 1;/*计算区域下边的行号*/
	if (rc->Right >= SCR_COL)  /*右边界越界的处理*/
	{
		len = rc->Right - SCR_COL + 1;
		rc->Left -= len;
		rc->Right = SCR_COL - 1;
	}
	return;
}
void RunSys(BUILDING_NODE **phead)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	BOOL bRet = TRUE;
	int i, loc, num;
	int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
	char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/

	while (bRet)
	{
		/*从控制台输入缓冲区中读一条记录*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);

		if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/
			cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
			if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
			{
				/* cAtt > 0 表明该位置处于热区(主菜单项字符单元)
				* cAtt != gi_sel_menu 表明该位置的主菜单项未被选中
				* gp_top_layer->LayerNo > 0 表明当前有子菜单弹出
				*/
				if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
					PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
				}
			}
			else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
			{
				TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
			}

			if (inRec.Event.MouseEvent.dwButtonState
				== FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左边第一键*/
			{
				if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
				{
					if (cAtt > 0) /*如果该位置处于热区(主菜单项字符单元)*/
					{
						PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
					}
					/*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/
					else if (gp_top_layer->LayerNo > 0)
					{
						PopOff();            /*关闭弹出的子菜单*/
						gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
					}
				}
				else /*层号不为0，表明该位置被弹出子菜单覆盖*/
				{
					if (cAtt > 0) /*如果该位置处于热区(子菜单项字符单元)*/
					{
						PopOff(); /*关闭弹出的子菜单*/
						gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/

											 /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
						bRet = ExeFunction(gi_sel_menu, cAtt);
					}
				}
			}
			else if (inRec.Event.MouseEvent.dwButtonState
				== RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
			{
				if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
				}
			}
		}
		else if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/
			&& inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
			asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/

														/*系统快捷键的处理*/
			if (vkc == 112) /*如果按下F1键*/
			{
				if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
				}
				bRet = ExeFunction(5, 1);  /*运行帮助主题功能函数*/
			}
			else if (inRec.Event.KeyEvent.dwControlKeyState
				& (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
			{ /*如果按下左或右Alt键*/
				switch (vkc)  /*判断组合键Alt+字母*/
				{
				case 88:  /*Alt+X 退出*/
					if (gp_top_layer->LayerNo != 0)
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					bRet = ExeFunction(1, 4);
					break;
				case 70:  /*Alt+F*/
					PopMenu(1);
					break;
				case 77: /*Alt+M*/
					PopMenu(2);
					break;
				case 81: /*Alt+Q*/
					PopMenu(3);
					break;
				case 83: /*Alt+S*/
					PopMenu(4);
					break;
				case 72: /*Alt+H*/
					PopMenu(5);
					break;
				}
			}
			else if (asc == 0) /*其他控制键的处理*/
			{
				if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
				{
					switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
					{
					case 37:
						gi_sel_menu--;
						if (gi_sel_menu == 0)
						{
							gi_sel_menu = 5;
						}
						TagMainMenu(gi_sel_menu);
						break;
					case 39:
						gi_sel_menu++;
						if (gi_sel_menu == 6)
						{
							gi_sel_menu = 1;
						}
						TagMainMenu(gi_sel_menu);
						break;
					case 40:
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else  /*已弹出子菜单时*/
				{
					for (loc = 0, i = 1; i<gi_sel_menu; i++)
					{
						loc += ga_sub_menu_count[i - 1];
					}  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
					switch (vkc) /*方向键(左、右、上、下)的处理*/
					{
					case 37:
						gi_sel_menu--;
						if (gi_sel_menu < 1)
						{
							gi_sel_menu = 5;
						}
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 38:
						num = gi_sel_sub_menu - 1;
						if (num < 1)
						{
							num = ga_sub_menu_count[gi_sel_menu - 1];
						}
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)
						{
							num--;
						}
						TagSubMenu(num);
						break;
					case 39:
						gi_sel_menu++;
						if (gi_sel_menu > 5)
						{
							gi_sel_menu = 1;
						}
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 40:
						num = gi_sel_sub_menu + 1;
						if (num > ga_sub_menu_count[gi_sel_menu - 1])
						{
							num = 1;
						}
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)
						{
							num++;
						}
						TagSubMenu(num);
						break;
					}
				}
			}
			else if ((asc - vkc == 0) || (asc - vkc == 32)) {  /*按下普通键*/
				if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
				{
					switch (vkc)
					{
					case 70: /*f或F*/
						PopMenu(1);
						break;
					case 77: /*m或M*/
						PopMenu(2);
						break;
					case 81: /*q或Q*/
						PopMenu(3);
						break;
					case 83: /*s或S*/
						PopMenu(4);
						break;
					case 72: /*h或H*/
						PopMenu(5);
						break;
					case 13: /*回车*/
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else /*已弹出子菜单时的键盘输入处理*/
				{
					if (vkc == 27) /*如果按下ESC键*/
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					else if (vkc == 13) /*如果按下回车键*/
					{
						num = gi_sel_sub_menu;
						PopOff();
						gi_sel_sub_menu = 0;
						bRet = ExeFunction(gi_sel_menu, num);
					}
					else /*其他普通键的处理*/
					{
						/*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
						for (loc = 0, i = 1; i<gi_sel_menu; i++)
						{
							loc += ga_sub_menu_count[i - 1];
						}

						/*依次与当前子菜单中每一项的代表字符进行比较*/
						for (i = loc; i<loc + ga_sub_menu_count[gi_sel_menu - 1]; i++)
						{
							if (strlen(ga_sub_menu[i])>0 && vkc == ga_sub_menu[i][1])
							{ /*如果匹配成功*/
								PopOff();
								gi_sel_sub_menu = 0;
								bRet = ExeFunction(gi_sel_menu, i - loc + 1);
							}
						}
					}
				}
			}
		}
	}
}
void CloseSys(BUILDING_NODE *hd)
{
	BUILDING_NODE *pBuildingNode1 = hd, *pBuildingNode2;
	CLASS_NODE *pClassNode1, *pClassNode2;
	ARRANGE_NODE *pArrangeNode1, *pArrangeNode2;

	while (pBuildingNode1 != NULL) /*释放十字交叉链表的动态存储区*/
	{
		pBuildingNode2 = pBuildingNode1->next;
		pClassNode1 = pBuildingNode1->cnext;
		while (pClassNode1 != NULL) /*释放学生基本信息支链的动态存储区*/
		{
			pClassNode2 = pClassNode1->next;
			pArrangeNode1 = pClassNode1->anext;
			while (pArrangeNode1 != NULL) /*释放缴费信息支链的动态存储区*/
			{
				pArrangeNode2 = pArrangeNode1->next;
				free(pArrangeNode1);
				pArrangeNode1 = pArrangeNode2;
			}
			free(pClassNode1);
			pClassNode1 = pClassNode2;
		}
		free(pBuildingNode1);  /*释放主链结点的动态存储区*/
		pBuildingNode1 = pBuildingNode2;
	}

	ClearScreen();        /*清屏*/

						  /*释放存放菜单条、状态条、性别代码和学生类别代码等信息动态存储区*/
	free(gp_buff_menubar_info);
	free(gp_buff_stateBar_info);

	/*关闭标准输入和输出设备句柄*/
	CloseHandle(gh_std_out);
	CloseHandle(gh_std_in);

	/*将窗口标题栏置为运行结束*/
	SetConsoleTitle("运行结束");

	return;
}
/**
 * 函数名称: TagMainMenu
 * 函数功能: 在指定主菜单项上置选中标志.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int PosA = 2, PosB;
    char ch;
    int i;

    if (num == 0) /*num为0时，将会去除主菜单项选中标记*/
    {
        PosA = 0;
        PosB = 0;
    }
    else  /*否则，定位选中主菜单项的起止位置: PosA为起始位置, PosB为截止位置*/
    {
        for (i=1; i<num; i++)
        {
            PosA += strlen(ga_main_menu[i-1]) + 4;
        }
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;

    /*去除选中菜单项前面的菜单项选中标记*/
    for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*在选中菜单项上做标记，黑底白字*/
    for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                               | FOREGROUND_RED;
    }

    /*去除选中菜单项后面的菜单项选中标记*/
    for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*将做好标记的菜单条信息写到窗口第一行*/
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

    return;
}
/**
 * 函数名称: PopMenu
 * 函数功能: 弹出指定主菜单项对应的子菜单.
 * 输入参数: num 指定的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;

    if (num != gi_sel_menu)       /*如果指定主菜单不是已选中菜单*/
    {
        if (gp_top_layer->LayerNo != 0) /*如果此前已有子菜单弹出*/
        {
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*若已弹出该子菜单，则返回*/
    {
        return;
    }

    gi_sel_menu = num;    /*将选中主菜单项置为指定的主菜单项*/
    TagMainMenu(gi_sel_menu); /*在选中的主菜单项上做标记*/
    LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/

    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];
    }
    /*将该组子菜单项项名存入标签束结构变量*/
    labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*标签束中标签字符串的个数*/
    COORD aLoc[100];/*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/
    for (i=0; i<labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
    /*设置热区信息*/
    areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/
    SMALL_RECT aArea[100];                    /*定义数组存放所有热区位置*/
    char aSort[100];                      /*定义数组存放所有热区对应类别*/
    char aTag[100];                         /*定义数组存放每个热区的编号*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*给弹出窗口画边框*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/
        pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pCh)==0) /*串长为0，表明为空串*/
        {   /*首先画横线*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &ul);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*取消该区域字符单元的热区属性*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*按位与的结果保留了低两位*/
            }
        }

    }
    /*将子菜单项的功能键设为白底红字*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {
            continue;  /*跳过空串*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}
