#include"CodeEditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{//�ڹ������У���������ӵ�QPlain �ı��༭�е��źš������༭��ʱ����������к������Ȳ�ͻ����ʾ��һ�С�
	lineNumberArea = new LineNumberArea(this);

	connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
	connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
	connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}


int CodeEditor::lineNumberAreaWidth()
{//�ú�����������к��еĿ�ȡ��Ա༭�����һ�е��������������ֵ�����ȡ�
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
{//�������к�����Ŀ��ʱ ֻ�����QAbstractScrollArea::setViewportMargins(). 
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
//�༭���鿴�˿��ѹ���ʱ������ô˲�ۡ�
//��Ϊ����������QRect�Ǳ༭�����һ���֣�������ȷʵ��Ҫ���£��ػ棩�� dyΪ���ִ�ֱ������ͼ��������
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{//���༭���Ĵ�С�����仯ʱ������Ҫ�����к�����Ĵ�С
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{//�����λ�÷����仯ʱ������ͻ����ʾ��ǰ�У������������С�
	QList<QTextEdit::ExtraSelection> extraSelections;
	
	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;
		//QPlainTextEdit�ṩ��ͬʱ���ж���ѡ��Ŀ����ԡ����ǿ���������Щѡ����ַ���ʽ (QTextCharFormat) ��
		//�������½�QPlainTextEdit::ExtraSelection֮ǰ��������ѡ�� 
		//���򣬵��������ѡ������ʱ����ͻ����ʾ���ѡ�е��С� 
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
{//�ڽ��յ���ӡ�����źŵ�ʱ������������� 
	//��ʼ���ƴ��ڱ��� 
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), Qt::lightGray);
	//�������пɼ����в������к� 
	//ע�⣺�ڴ��ı��༭�У�ÿ�н�����һ��QTextBlock�������������wrapping�����п��ܻ��Խ�ı��༭�ӿ��еļ��С�
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	//�����ȡ��һ���ı���Ķ����͵ײ� y ���꣬������ѭ����ÿ�������еĵ�ǰ�ı���ĸ߶ȵ�����Щֵ��
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
