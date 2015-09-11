/*!
 *  \file      ResManager.h
 *  \brief     Pretty nice class.
 *  \details   This class is used to demonstrate a number of section commands.
 *  \author    WJF
 *  \version   v0.0
 *  \date      2015/09/06
 *  \pre       First initialize the system.
 *  \bug       Not all memory is freed when deleting an object of this class.
 *  \warning   Improper use can crash your application
 *  \copyright GNU Public License.
 */
#ifndef RESMAN_H
#define RESMAN_H


#include <QtCore/QObject>
#include <QColor>
#include <QtCore/QVector>
#include <QPixmap>
#include <QTranslator>
#include <QApplication>
#include "SimulateMan.h"
#include "UI/Basectrl.h"


//!工程参数
/*! 描述MyScada组态软件工程的工程参数*/
typedef struct _tagProjectParm{
    int m_nFirstPage;           //!<启动画面id.
    int m_nWidth;               //!<目标屏幕宽度(pixel).
    int m_nHeight;              //!<目标屏幕高度(pixel).
    bool m_bPageCirculation;    //!<是否画面轮显.
    int m_nPeriod;              //!<轮显周期.
    int m_nAddrOff;             //!<地址偏移.
    int m_nAddrRange;           //!<地址范围.
    int m_nLanguage;            //!<语言.

}ProjectParm, *LPProjectParm;


//!面板显示参数
/*! 描述面板显示参数信息：\n
 * 参考面板宽度;\n
 * 参考面板高度;\n
 * 背景模式;\n
 * 纯背景颜色;\n
 * 顶部颜色(渐变);\n
 * 底部颜色(渐变);\n
 * 背景图片ID;\n
 */
typedef struct _tagPanelParm
{
    int widith;                 //!<宽(pixel).
    int height;                 //!<高(pixel).
    int BkMode;                 //!< 0单色；1渐变；2；背景图片
    QRgb bkColor;               //!<背景颜色;
    QRgb topColor;              //!<顶部颜色;
    QRgb bottomColor;           //!<底部颜色;
    int nBkPic;                 //!<背景图片ID;
}PanelParm, *LPPanelParm;


//!页面头
/*! 描述一个页面的信息:大小(该页面所占用的字节数)、页面标题ID、控件数目*/
typedef struct _tagPageHead
{
    int nPageSize;
    int nTitleID;
    int nCtrlNum;
}PageHead, *LPPageHead;


//!子页面列表头
/*! 描述子页面列表的信息:子画面列表大小(所占字节数)、子画面数目*/
typedef struct _tagSubPageListHead
{
    int nSubPageListSize;
    int nSubPageCnt;
}SubPageListHead, *LPSubPageListHead;


//!页面管理结构体
/*! 包含了所有的页面数据,所有父、子页面均可以在此找到*/
typedef struct _tagPagesStruct
{
   int nPageNum;//!<一级页面数目;
   QVector<LPPageHead> pageVector;//!<所有的一级页面指针;
   QVector< QVector<LPPageHead> >pSubPageVector;//!<子画面矩阵(所有的二级页面指针);

}PagesStruct, *LPPagesStruct;



//! 资源管理模块,对外提供接口:
/*!
 *  模拟量hex地址.\n
 *  工程参数.\n
 *  面板参数.\n
 *  所有页面的集合.\n
 *  任务列表.\n
 *  图片资源获取.\n
 *  字符串资源获取.\n
 *  警告信息资源获取.\n
 *  语言切换.\n
 *  返回横向伸缩比.\n
 *  返回纵向伸缩比.\n
 */
class ResManager : public QObject
{
    Q_OBJECT

public:
    explicit ResManager(QObject *parent = 0);
    ~ResManager();

private:
//data:
    QVector<QPixmap*> m_imgVec;         //!<图片资源;
    QVector<QString *>m_strVec;         //!<字符串资源;
    QVector<QString *>m_warnVec;        //!<警告信息资源;
    QString m_nullString;               //!<空字符串;
    char *pHex;                         //!<读取的Hex文件缓冲区;
    int m_nLanguageSel;                 //!<语言选择:0-中文, 1-英文;
    int m_nMaxLanguage;                 //!<最多支持的语言数目;
    QTranslator *m_pTranslator;         //!<翻译器;
    double m_dXfactor;                  //!<横向伸缩比.
    double m_dYfactor;                  //!<纵向伸缩比.
	
//methods:
    void setupSubPageVector(const LPSubPageListHead pSub, int nId); //建立子画面列表;
    void setupTaskVector(int *p, int nId);                          //建立人物列表;
    void loadString(void);                                          //加载字符串;
    void loadStr(void);                                             //根据语言选择加载字符串.
    void loadHex(void);                                             //加载16进制数据文件.
    void loadLanguageConfig(void);                                  //加载语言配置选项.
    void loadImg(void);                                             //加载图片资源.
    void loadWarnText(void);                                        //加载警告信息.
    void releaseStrRes();                                           //释放字符串资源.
    void releaseImgRes();                                           //释放图片资源.
    void releaseWarnRes();                                          //释放警告信息.

public:
//data:
    char *m_pSimulateVar;                                   //!<模拟量描述参数.
    LPProjectParm m_pProjectParm;                           //!<工程参数.
    LPPanelParm m_pPanelParm;                               //!<面板显示参数.
    PagesStruct m_pageStruct;                               //!<所有的页面参数.
    QVector< QVector<LPActDeal> >m_pTaskVector;             //!<任务列表参数.


//methods:
    QPixmap *getPixmap(const int id);                       //!<根据图片ID获取图片资源;
    QPixmap *getPanelBkPixmap();                            //!<获取面板背景图片;
    QString *getString(const int id);                       //!<根据字符串ID获取字符串资源;
    const QString &getWarningText(const int id);            //!<根据ID获取警告信息字符串资源;
    void changeLanguage(int nLanguage, bool bInit=false);   //!<语言切换;
    const double getdXfactor() const {return m_dXfactor;}   //!返回横向伸缩比.
    const double getdYfactor() const {return m_dYfactor;}   //!返回纵向伸缩比.



signals:
    void updateDisp();                                      //!<刷新当前页面信号;

};

//!资源管理模块全局对象
extern  ResManager *g_pResManModule;//资源管理模块;

#endif // RESMAN_H
