#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_compiler_design_qt.h"
#include "head.h"

#include<kernelspecs.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qscrollbar.h>

class compiler_design_qt : public QMainWindow
{
    Q_OBJECT

public:
    compiler_design_qt(QWidget *parent = Q_NULLPTR);
private slots:
	
protected:
	void closeEvent(QCloseEvent *event);
private:
    Ui::compiler_design_qtClass *ui;

	/****************************窗口所需*******************************/

	settings preProcess();//预处理
	void showInit();//初始屏幕显示数据
	void recover();//恢复现场
	void renew();//刷新
	void alterMarkPascal();
	void alterMarkC();

	/****************************流->窗口*******************************/

	void cProgramGenerate();//从代码流中获取生成的c语言程序到屏幕上
	int errorGenerate();//从错误流中获取错误信息并输出到屏幕上

	/****************************窗口->文件*******************************/

	int saveC();//保存c代码到当前所选择的文件中
	int savePascal();//保存pascal代码到当前所选择的文件中
	void saveNotice();//保存当前提示消息
	void saveRoute();//保存当前文件路径

	/****************************文件->窗口*******************************/

	void getError();//从文件中获取错误信息
	void getProgramC();//从文件中获取C语言程序
	void getProgramPascal();//从文件中获取Pascal程序
	void getNotice();//从文件中获取提示信息
	void getRoute();//从文件中获取路径信息

	/****************************流->文件*******************************/

	void outputWord();

	/****************************生成/编译/运行*******************************/

	int confirm();//确认现在可以执行操作
	void generateC();//生成c语言代码到代码流中
	void complieProgramC();//编译c语言程序
	void runProgramC();//运行c语言程序
	void getOutputC();//从文件中获取C语言程序的运行输出
	void getCompileError();//从文件中获取编译时的报错

	/****************************路径选择*******************************/

	QString chooseFile(QString);//选取文件
	QString chooseSaveRoute(QString);//选择保存路径
	void chooseInputFile();//选取输入的Pascal程序路径
	void chooseOutputFile();//选取输出的C语言程序路径
	void chooseLogFile();//选取日志文件路径

	/****************************窗口命令行输出*******************************/

	void catNotice(string);//连接新的提示信息
	void clearNotice();//清楚提示信息



	/****************************私有变量*******************************/

	QString pascal_route;
	QString c_route;
	QString log_route;

	int c_altered;//c程序有变动
	int pascal_altered;//pascal程序有变动

};
