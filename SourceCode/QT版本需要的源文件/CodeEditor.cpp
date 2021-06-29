#include"CodeEditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{//在构造器中，将插槽连接到QPlain 文本编辑中的信号。创建编辑器时，必须计算行号面积宽度并突出显示第一行。
	lineNumberArea = new LineNumberArea(this);

	connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
	connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
	connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}


int CodeEditor::lineNumberAreaWidth()
{//该函数计算左侧行号列的宽度。以编辑器最后一行的数字数乘以数字的最大宽度。
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

	return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{//当更新行号区域的宽度时 只需调用QAbstractScrollArea::setViewportMargins(). 
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
//编辑器查看端口已滚动时，会调用此插槽。
//作为参数给出的QRect是编辑区域的一部分，该区域确实需要更新（重绘）。 dy为保持垂直滚动视图的像素数
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{//当编辑器的大小发生变化时，还需要调整行号区域的大小
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{//当光标位置发生变化时，我们突出显示当前行，即包含光标的行。
	QList<QTextEdit::ExtraSelection> extraSelections;
	
	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;
		//QPlainTextEdit提供了同时进行多行选择的可能性。我们可以设置这些选择的字符格式 (QTextCharFormat) 。
		//在设置新建QPlainTextEdit::ExtraSelection之前，清除鼠标选择 
		//否则，当用用鼠标选择多个行时，将突出显示多个选中的行。 
		QColor lineColor = QColor(Qt::yellow).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{//在接收到打印窗口信号的时候会调用这个函数 
	//开始绘制窗口背景 
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), Qt::lightGray);
	//遍历所有可见的行并绘制行号 
	//注意：在纯文本编辑中，每行将包含一个QTextBlock：但如果启用了wrapping，则行可能会跨越文本编辑视口中的几行。
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	//下面获取第一个文本块的顶部和底部 y 坐标，并根据循环中每个迭代中的当前文本块的高度调整这些值。
	int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
	int bottom = top + qRound(blockBoundingRect(block).height());
	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + qRound(blockBoundingRect(block).height());
		++blockNumber;
	}
}
