#ifndef RESMAN_H
#define RESMAN_H

//Desc:ResMan负责资源管理;

#include <QtCore/QObject>
#include <QColor>
#include <QtCore/QVector>
#include <QPixmap>
#include <QTranslator>
#include <QApplication>
#include "SimulateMan.h"
#include "UI/Basectrl.h"



typedef struct _tagProjectParm{
    // 启动画面;
    int m_nFirstPage;
    //宽度;
    int m_nWidth;
    //高度;
    int m_nHeight;
    // 是否画面轮显;
    bool m_bPageCirculation;
    // 轮显周期;
    int m_nPeriod;
    //地址偏移;
    int m_nAddrOff;
    //地址范围;
    int m_nAddrRange;
    //语言;
    int m_nLanguage;

}ProjectParm, *LPProjectParm;

//面板参数;
typedef struct _tagPanelParm
{
    int widith;//宽;
    int height;//高;
    int BkMode;//0单色；1渐变；2；背景图片
    QRgb bkColor;//背景颜色;
    QRgb topColor;//顶部颜色;
    QRgb bottomColor;//底部颜色;
    int nBkPic;//背景图片ID;
}PanelParm, *LPPanelParm;


//页面头，用于描述一个页面
typedef struct _tagPageHead
{
    int nPageSize;
    int nTitleID;
    int nCtrlNum;
}PageHead, *LPPageHead;

typedef struct _tagSubPageListHead
{
    int nSubPageListSize;//子画面列表大小;
    int nSubPageCnt;//子画面数目;
}SubPageListHead, *LPSubPageListHead;

//包含了所有的页面数据;
typedef struct _tagPagesStruct
{
    int nPageNum;//页面数目;
   QVector<LPPageHead> pageVector;//所有的页面指针;
   //QVector<LPPageHead> *pSubPageVector;//子画面矩阵;
   QVector< QVector<LPPageHead> >pSubPageVector;//子画面矩阵;

}PagesStruct, *LPPagesStruct;





class ResManager : public QObject
{
    Q_OBJECT
public:
    explicit ResManager(QObject *parent = 0);
    ~ResManager();

private:
//data:
    //资源;
    QVector<QPixmap*> m_imgVec;//图片资源;
    QVector<QString *>m_strVec;//字符串资源;
    QVector<QString *>m_warnVec;//警告信息资源;
    QString m_nullString;//空字符串;
    void *pHex;//读取的Hex文件缓冲区;
    int m_nLanguageSel;//语言选择:0-中文, 1-英文;
    int m_nMaxLanguage;//最多支持的语言数目;
    int m_nAdminPwd;//管理员密码;
    int m_nSeniorAdminPwd;//高级管理员密码;
	QTranslator *m_pTranslator;
	

//methods:
    void setupSubPageVector(const LPSubPageListHead pSub, int nId);
    void setupTaskVector(int *p, int nId);
	void loadString(void);//加载字符串;

public:
//data:
    char *m_pSimulateVar;//模拟量描述参数;
    LPProjectParm m_pProjectParm;//工程参数;
    LPPanelParm m_pPanelParm;//面板参数;
    PagesStruct m_pageStruct;//所有的页面参数;
    //QVector<LPActDeal> *m_pTaskVector;//子画面矩阵;
	QVector< QVector<LPActDeal> >m_pTaskVector;//子画面矩阵;


//methods:
    //加载资源;
    void loadHex(void);//加载编译文件;
    void loadConfig(void);//加载配置选项;
    void loadImg(void);//加载图片资源;
    void loadWarnText(void);//加载警告信息;
	void loadStr(void);//加载字符串;
    void releaseStrRes();//释放字符串资源;
    void releaseImgRes();//释放图片资源;
    void releaseWarnRes();//释放警告信息;
    void changeLanguage(int nLanguage, bool bInit=false);//切换语言;
    void changePwd(int who, int oldPwd, int newPwd);
    QPixmap *getPixmap(const int id);
    QPixmap *getPanelBkPixmap();
    QString *getString(const int id);
    const QString &getWarningText(const int id);


signals:
    void updateDisp();

};


extern  ResManager *g_pResManModule;//资源管理模块;

#endif // RESMAN_H
