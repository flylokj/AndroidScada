// JScript source code
var g_nStatus = 0; 
function init()
{
    m_pbtnTest.clicked.connect(OnBtnTest);
	//print('Hello QtScript');  
}

function OnBtnTest()
{
   ShowMessage("QSA脚本测试123456789!"); // 这里将被调用显示
}

//print("fuck test");
