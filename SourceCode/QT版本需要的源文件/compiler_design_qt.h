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

	/****************************��������*******************************/

	settings preProcess();//Ԥ����
	void showInit();//��ʼ��Ļ��ʾ����
	void recover();//�ָ��ֳ�
	void renew();//ˢ��
	void alterMarkPascal();
	void alterMarkC();

	/****************************��->����*******************************/

	void cProgramGenerate();//�Ӵ������л�ȡ���ɵ�c���Գ�����Ļ��
	int errorGenerate();//�Ӵ������л�ȡ������Ϣ���������Ļ��

	/****************************����->�ļ�*******************************/

	int saveC();//����c���뵽��ǰ��ѡ����ļ���
	int savePascal();//����pascal���뵽��ǰ��ѡ����ļ���
	void saveNotice();//���浱ǰ��ʾ��Ϣ
	void saveRoute();//���浱ǰ�ļ�·��

	/****************************�ļ�->����*******************************/

	void getError();//���ļ��л�ȡ������Ϣ
	void getProgramC();//���ļ��л�ȡC���Գ���
	void getProgramPascal();//���ļ��л�ȡPascal����
	void getNotice();//���ļ��л�ȡ��ʾ��Ϣ
	void getRoute();//���ļ��л�ȡ·����Ϣ

	/****************************��->�ļ�*******************************/

	void outputWord();

	/****************************����/����/����*******************************/

	int confirm();//ȷ�����ڿ���ִ�в���
	void generateC();//����c���Դ��뵽��������
	void complieProgramC();//����c���Գ���
	void runProgramC();//����c���Գ���
	void getOutputC();//���ļ��л�ȡC���Գ�����������
	void getCompileError();//���ļ��л�ȡ����ʱ�ı���

	/****************************·��ѡ��*******************************/

	QString chooseFile(QString);//ѡȡ�ļ�
	QString chooseSaveRoute(QString);//ѡ�񱣴�·��
	void chooseInputFile();//ѡȡ�����Pascal����·��
	void chooseOutputFile();//ѡȡ�����C���Գ���·��
	void chooseLogFile();//ѡȡ��־�ļ�·��

	/****************************�������������*******************************/

	void catNotice(string);//�����µ���ʾ��Ϣ
	void clearNotice();//�����ʾ��Ϣ



	/****************************˽�б���*******************************/

	QString pascal_route;
	QString c_route;
	QString log_route;

	int c_altered;//c�����б䶯
	int pascal_altered;//pascal�����б䶯

};
