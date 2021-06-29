#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qpainter.h>
#include <qtextedit.h>
#include <qtextcursor.h>
#include <QTextBlock>

class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor(QWidget *parent = nullptr);

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();// 更新行号部分的宽度，并且设置视口部件的margin

protected:
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();//光标位置变更的信号
	void updateLineNumberArea(const QRect &rect, int dy);

private:
	QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget
{
public:
	LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
	{}

	QSize sizeHint() const override
	{
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	// 控件的重绘工作，放到了codeEditor里面
	// 主要是因为需要取得文本块的序号和视口的大小，写在codeEditor里，参数调用会更加方便
	void paintEvent(QPaintEvent *event) override
	{
		codeEditor->lineNumberAreaPaintEvent(event);
	}

private:
	CodeEditor *codeEditor;
};