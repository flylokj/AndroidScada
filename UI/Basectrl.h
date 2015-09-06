#ifndef BASECTRL
#define BASECTRL
#include <QtCore/QRect>
#include <QRgb>
#include <QtCore/QStringList>

//#define PARMSIZE            4352//磨耗参数和面板参数大小和;
//#define MOLDSIZE            4096//模号文件大小;
//#define PANELSIZE           255//面板参数大小;

#define PARMSIZE            12288//磨耗参数和面板参数大小和;
#define MOLDSIZE            10240//模号文件大小;
#define PANELSIZE           2048//面板参数大小;

#define D_COIL_OFFSET       0//D字软元件地址偏移;
#define M_COIL_OFFSET       4096//M位软元件地址偏移;
#define X_COIL_OFFSET       6144//X位软元件地址偏移;
#define Y_COIL_OFFSET       8192//Y位软元件地址偏移;



#define WINQT
#define WINLINUX
//#define MYANDROID
#ifdef MYANDROID
#define HEXFILE				"/data/data/org.qtproject.scada/files/fuck.hex"
#define OPTIONGFILE         "/data/data/org.qtproject.scada/files/Option.ini"
#define WARNINIFILE			"/data/data/org.qtproject.scada/files/warning.ini"
#define SCRIPTDIR           "/data/data/org.qtproject.scada/files/" //脚本文件目录;
#define MOLDDIR             "/data/data/org.qtproject.scada/files/" //模号参数文件目录;
#define LAGNUAGEFILE        "/data/data/org.qtproject.scada/files/cn.qm"
//#elif WINLINUX
//#define HEXFILE				"./fuck.hex"
//#define OPTIONGFILE         "./Option.ini"
//#define WARNINIFILE			"./res/warning.ini"
//#define SCRIPTDIR           "./script/" //脚本文件目录;
//#define MOLDDIR             "./mold/" //模号参数文件目录;
#else
#define HEXFILE				"E:\\WORKSPACE\\vc++\\KJL\\Drawer\\MyScada\\Debug\\project\\OutPut\\fuck.hex"
#define OPTIONGFILE         "E:\\WORKSPACE\\vc++\\KJL\\Drawer\\MyScada\\Debug\\project\\OutPut\\Option.ini"
#define WARNINIFILE			".\\res\\warning.ini"
#define SCRIPTDIR           ".\\script\\" //脚本文件目录;
#define MOLDDIR             ".\\mold\\" //模号参数文件目录;
#define LAGNUAGEFILE        "cn.qm"
#endif

//控件类型;
enum KJLCtrlType
{
  KJLBtnType,//按钮;
  KJLEditBoxType,//编辑框;
  KJLComboBoxType,//组合框;
  KJLLineType,//线条;
  KJLGroupBoxType,//分组框;
  KJLStaticType,//静态;
  KJLStatusType,//状态控件;
  KJLProgressType,//进度条控件;
  KJLSubWinType,//子窗口控件
};

//动作类型;
enum ActType{SINGLESET=0,SET,LINKPAGE,RUNTASK,LINKSUBWIN, RUNSCRIPT};

//状态控件类型;
enum StatType{ TextStat=0, ImgStat, DateStat};

//系统状态;
enum SysStatus{PAGECHANGE, SYSPERMISSION, RUNTIME, MOLDNAME};


typedef struct tagLOGPEN { /* lgpn */
uint lopnStyle;
QPoint lopnWidth;
QRgb lopnColor;
} LOGPEN;

typedef struct tagLOGBRUSH
{
uint lbStyle;
QRgb lbColor;
long lbHatch;
} LOGBRUSH;

//控件数据头;
typedef struct _tagCtrlHead
{
    int ctrlType;//控件类型;
    int ctrlSize;//控件大小;
}CtrlHead, *LPCtrlHead;



//控件共同的属性;
typedef struct _tagBaseCtrl
{
    QRect rect;
    bool bPen;
    bool bBrush;
    LOGPEN logpen;
    LOGBRUSH logbrush;
    QRgb forwardColor;
    bool bVar;
    int nTextID;
    bool bDynamic;
    int nDynamicID;
    int nDynamicCondition;
    int nPermission;//控件的权限0:监控员, 1:操作员, 2:系统管理员;
}BaseCtrl, *LPBaseCtrl;


//动作;
typedef struct _tagActDeal{
    int nOption;
    int nVar;
    int nVal;
}ActDeal, *LPActDeal;


//控件私有属性;
//按钮私有属性;
typedef struct _tagBtnPri
{
    // 正常背景图片ID;
    int m_nNormalPicID;
    // 获得焦点的背景图片ID;
    int m_nHoverPicID;
    // 被按下的背景图片ID;
    int m_nPressPicID;
    // 按下时候是否执行动作;
    bool m_bPressAct;
    // 是否执行弹起动作;
    bool m_bReleaseAct;
    // 是否执行焦点动作;
    bool m_bHoverAct;
    // 按钮形态:0普通按；1工具按钮;
    int m_nBtnType;

    //按下动作;
    ActDeal m_pressAct;
    //弹起动作;
    ActDeal m_releaseAct;
    //焦点动作;
    ActDeal m_focusAct;

}BtnPri, *LPBtnPri;


//编辑框控件私有属性;
typedef struct _tagEditPri
{
    // 是否显示标题;
    bool bText;
    //是否自动保存;
    bool bAutoSave;
    //是否显示密码;
    bool bDispPwd;
    // 是否执行确认动作;
    bool bIndexChangeAct;
    // 是否执行焦点动作;
    bool bFocusAct;
    // 标题字符串ID;
    int nTitleTextID;
    // 标题方向;
    int nTitleOrientation;
    //确认动作;
    ActDeal m_indexChangeAct;
    //焦点动作;
    ActDeal m_focusAct;
}EditPri, *LPEditPri;

//状态控件私有属性;
typedef struct _tagStatusPri
{
    //类型:0-2：文字，图片，日期;
    int m_nType;
    // 水平对齐方式:0-2左、右、中;
    int m_nHorizantalAlign;
    //状态条目数;
    int m_nStatCnt;
}StatusPri, *LPStatusPri;

//静态控件私有属性;
typedef struct _tagStaticPri
{
    // 水平对齐方式:0-2左、右、中;
    int m_nHorizantalAlign;
}StaticPri, *LPStaticPri;

//进度条控件私有属性;
typedef struct _tagProgressPri
{
    bool bSysOpt;
    int nSysOpt;
    int nOreintation;
    int colorNormal;
    int colorUpAlarm;
    int colorDwnAlarm;
}ProgressPri, *LPProgressPri;

//子窗口控件私有属性;
typedef struct _tagSubWinPri
{
    int id;
    int nSubPageCnt;//子画面数量;
}SubWinPri, *LPSubWinPri;


/* @教导记录的描述,该结构体是传送给主机的数据;
 * nStep:步序号;
 * nType:教导记录类型{或者说是动作类型比较好理解};
 * pData:描述教导记录的数据;
*/
typedef struct _tagTutorRecord
{
    int nStep;
    int nType;
    void *pData;
}TutorRecord, *LPTutorRecord;

/* @教导记录的显示描述,该结构体主要用于解析和显示教导记录的数据到教导画面上;
 * @strList:显示在列表框上的一条教导记录字符串;
 * @record:教导记录数据;
 *
*/
typedef struct _tagTutorRecordStr
{
    QStringList strList;
    TutorRecord record;

}TutorRecordStr, *LPTutorRecordStr;

//记录的类型和子类型;
#define AXIS_ACT_TUTOR_RECORD_TYPE          0//轴动作;
#define LAYER_ACT_TUTOR_RECORD_TYPE         1//堆叠;
#define ZTOOL_ACT_TUTOR_RECORD_TYPE         2//治具;
#define SIGNAL_ACT_TUTOR_RECORD_TYPE        3//塑机信号;
#define EXTRA_DEV_TUTOR_RECORD_TYPE         4//辅助设备;
#define PRESERVE_TUTOR_RECORD_TYPE          5//预留;
#define CHECK_TUTOR_RECORD_TYPE             6//检测;
#define WAIT_TUTOR_RECORD_TYPE              7//等待;
#define OTHER_TUTOR_RECORD_TYPE             8//其他;
#define MOLD_END_TUTOR_RECORD_TYPE			9//模组结束;




#define SYS_ADDR_PAGE 0x03






#endif // BASECTRL

