#include "compiler_design_qt.h"
#include "head.h"

extern ifstream in;  //Դ����������
extern ofstream out; //Ŀ����������
extern ofstream log_file; //��־�����


extern vector<word>word_stream;//�Ǻ���
extern vector<string>code_stream;//���ɴ�����
extern vector<error_item>error_stream;//������


extern int a, b[10][10];

compiler_design_qt::compiler_design_qt(QWidget *parent)
    : QMainWindow(parent)
{
	ui = new Ui::compiler_design_qtClass;
    ui->setupUi(this);
	pascal_route = "Scene/pascalBoard.pas";
	c_route = "Scene/cBoard.c";
	QObject::connect(ui->generateButton, &QPushButton::clicked, this, &compiler_design_qt::generateC);
	QObject::connect(ui->compileButton, &QPushButton::clicked, this, &compiler_design_qt::complieProgramC);
	QObject::connect(ui->runButton, &QPushButton::clicked, this, &compiler_design_qt::runProgramC);
	QObject::connect(ui->clearButton, &QPushButton::clicked, this, &compiler_design_qt::clearNotice);
	QObject::connect(ui->chooseOutput, &QPushButton::clicked, this, &compiler_design_qt::chooseOutputFile);

	QObject::connect(ui->loadPascal, &QAction::triggered, this, &compiler_design_qt::chooseInputFile);
	QObject::connect(ui->savePascal, &QAction::triggered, this, &compiler_design_qt::savePascal);
	QObject::connect(ui->savePlacePascal, &QAction::triggered, this, [=]()
	{
		QString filename = chooseSaveRoute(".pas");
		if (filename != "ERROR")
		{
			if (filename.indexOf(QDir::currentPath()) == -1)
				pascal_route = filename;
			else
				pascal_route =  filename.mid(QDir::currentPath().length() + 1);

			if (savePascal())
				QMessageBox::information(this, "Success", QString::fromLocal8Bit("����ɹ�"), QMessageBox::Ok);
			else
				QMessageBox::critical(this, "Error", QString::fromLocal8Bit("����ʧ��"), QMessageBox::Ok);
			renew();
		}
		else
			QMessageBox::critical(this, "Error", QString::fromLocal8Bit("����ʧ��"), QMessageBox::Ok);
	});
	QObject::connect(ui->saveC, &QAction::triggered, this, &compiler_design_qt::saveC);
	QObject::connect(ui->savePlaceC, &QAction::triggered, this, [=]()
	{
		QString filename = chooseSaveRoute(".c");
		if (filename != "ERROR")
		{
			if (filename.indexOf(QDir::currentPath()) == -1)
				c_route = filename;
			else
				c_route = filename.mid(QDir::currentPath().length() + 1);

			if(saveC())
				QMessageBox::information(this, "Success", QString::fromLocal8Bit("����ɹ�"), QMessageBox::Ok);
			else
				QMessageBox::critical(this, "Error", QString::fromLocal8Bit("����ʧ��"), QMessageBox::Ok);
			renew();
		}
		else
			QMessageBox::critical(this, "Error", QString::fromLocal8Bit("����ʧ��"), QMessageBox::Ok);
	});
	QObject::connect(ui->recover, &QAction::triggered, this, &compiler_design_qt::recover);
	QObject::connect(ui->openWordResult, &QAction::triggered, this, [=]() {system("start Scene/word.txt"); });

	showInit();
	QObject::connect(ui->pascalProgram, &QPlainTextEdit::textChanged, this, &compiler_design_qt::alterMarkPascal);
	QObject::connect(ui->cProgram, &QPlainTextEdit::textChanged, this, &compiler_design_qt::alterMarkC);

}

void compiler_design_qt::closeEvent(QCloseEvent * e)
{
	saveC();
	savePascal();//��Ŀ���ļ��е����ݱ���

	//ѯ���Ƿ�Ҫ�����ֳ�
	int answer = QMessageBox::question(this, tr("Quit"), QString::fromLocal8Bit("�Ƿ񱣴��ֳ�"), QMessageBox::Yes, QMessageBox::No);
	if (answer == QMessageBox::Yes) 
	{
		saveNotice();
		saveRoute();
		pascal_route = "Scene/pascalProgram.pas";
		c_route = "Scene/cProgram.c";//�����ֳ��������ļ�
		saveC();
		savePascal();//���ļ�ͬʱ���浽�����ֳ����ļ���
	}
	ofstream clear_file;
	clear_file.open("Scene/pascalBoard.pas", ios::trunc);
	clear_file << "";
	clear_file.close();
	clear_file.open("Scene/cBoard.c", ios::trunc);
	clear_file << "//You are using the BLACKBOARD system produced, which will be cleared when closed." << endl;
	clear_file << "//If you want to save your edit, please choose a vaild .pas source file and a .c target file." << endl;
	clear_file.close();//����װ�

}
/****************************��������*******************************/

settings compiler_design_qt::preProcess()//��ʼǰ����params
{
	settings params;
	params.scr_path = pascal_route.toStdString();
	params.dst_path = c_route.toStdString();
	//params.log_path = "log.txt";
	params.file_output = true;

	return params;
}

void compiler_design_qt::showInit()//��ʼ��Ļ�е���������
{
	ui->notice->setPlainText(">>>");
	ui->inputLine->setText("Blackboard");
	ui->outputLine->setText("Blackboard"); //��ʼ����ʾ·�� ��װ��û��ѡ���ļ�Ҳ��������
	getProgramC();
	getProgramPascal();
}

void compiler_design_qt::recover()//�ָ��ֳ�
{
	int answer = QMessageBox::question(this, tr("Recover"), QString::fromLocal8Bit("�Ƿ�ȷ��Ҫ��ԭ�ϴα�����ֳ���"), QMessageBox::Yes, QMessageBox::No);

	if (answer == QMessageBox::Yes)
	{
		pascal_route = "Scene/pascalProgram.pas";
		c_route = "Scene/cProgram.c";
		getProgramC();
		getProgramPascal();
		getNotice();

		getRoute();
		ui->inputLine->setText(pascal_route);
		ui->outputLine->setText(c_route);
	}
	
}

void compiler_design_qt::renew()//ˢ��
{
	ui->inputLine->setText(pascal_route);
	ui->outputLine->setText(c_route);
	getProgramPascal();
}

void compiler_design_qt::alterMarkPascal()
{
	if (pascal_altered)
		ui->label->setText(QString::fromLocal8Bit("PascalԴ����*"));
	else //��һ�λ��ڵ����ļ���ʱ�򴥷� ��ʱalterΪ0
		pascal_altered=1;
}

void compiler_design_qt::alterMarkC()
{
	if (c_altered)
		ui->label_2->setText(QString::fromLocal8Bit("C���Գ���*"));
	else//��һ�λ��ڵ����ļ���ʱ�򴥷� ��ʱalterΪ0
		c_altered = 1;
}

/****************************��->����*******************************/

void compiler_design_qt::cProgramGenerate()//�Ӵ������л�ȡ���ɵ�c���Գ�����Ļ��
{
	string show_c = "";
	for (int i = 0; i < (int)code_stream.size(); i++)
		show_c += code_stream[i] + "\n";

	ui->cProgram->setPlainText(QString::fromStdString(show_c));
}

int compiler_design_qt::errorGenerate()//�Ӵ������л�ȡ������Ϣ���������Ļ�� �������Ƿ��д�����Ϣ����
{
	string show_error = "Generate failure...\n";
	string line = "";
	int flag = 1;
	for (int i = 0; i < (int)error_stream.size(); i++) 
	{
		if (error_stream[i].stage == GRAMMAR && flag) 
		{
			flag = 0;
			show_error += "[Error type " + to_string(error_stream[i].stage) + "] line " +
				to_string(error_stream[i].row) + " : " + error_stream[i].content + "\n";
		}
		else
		{
			show_error += "[Error type " + to_string(error_stream[i].stage) + "] line " +
				to_string(error_stream[i].row) + " : " + error_stream[i].content +" " + error_stream[i].error_code + "\n";
		}
	}
	//for (int i = 0; i < (int)error_stream.size(); i++)
	//{

	//	line = "error "+ std::to_string(error_stream[i].stage) 
	//		+ "(line: " + std::to_string(error_stream[i].row) +
	//		", code: '" + error_stream[i].error_code + "'): msg \"" + error_stream[i].content + "\"";
	//	show_error += line + "\n";
	//}

	if ((int)error_stream.size() == 0)
		return 0;
	else
	{
		catNotice(show_error);
		return 1;
	}
}

/****************************����->�ļ�*******************************/

int compiler_design_qt::saveC()//����c���뵽��ǰ��ѡ����ļ���
{
	QString c_program = ui->cProgram->toPlainText();//��ȡ�ı����е�pascal����
	ui->label_2->setText(QString::fromLocal8Bit("C���Գ���"));
	if (c_program == NULL)//��������Ϊ��
		return 2;
	else
	{
		ofstream c_file;
		c_file.open(c_route.toStdString(), ios::trunc);
		if (c_file.is_open())
			c_file << c_program.toStdString();//��c������µ��ļ���
		else
			return 0;
	}
	return 1;
}

int compiler_design_qt::savePascal()//����pascal���뵽��ǰ��ѡ����ļ���
{
	QString pascal_program = ui->pascalProgram->toPlainText();//��ȡ�ı����е�pascal����
	ui->label->setText(QString::fromLocal8Bit("PascalԴ����"));
	if (pascal_program == NULL)//��������Ϊ��
		return 2;
	else
	{
		ofstream pascal_file;
		pascal_file.open(pascal_route.toStdString(), ios::trunc);
		if (pascal_file.is_open())
			pascal_file << pascal_program.toStdString();//��pascal������µ��ļ���
		else
			return 0;
	}
	return 1;
}

void compiler_design_qt::saveNotice()
{
	QString notice = ui->notice->toPlainText();//��ȡ�ı����е�pascal����
	if (notice != NULL)//��������Ϊ��
	{
		ofstream notice_file;
		notice_file.open("Scene/notices.txt", ios::trunc);
		notice_file << notice.toStdString();//��pascal������µ��ļ���
	}
}

void compiler_design_qt::saveRoute()
{
	string routes;
	routes = pascal_route.toStdString() + "\n" + c_route.toStdString();
	ofstream route_file;
	route_file.open("Scene/routes.txt", ios::trunc);
	route_file << routes ;//��pascal������µ��ļ���
}

/****************************�ļ�->����*******************************/

void compiler_design_qt::getError()//���ļ��л�ȡ������Ϣ
{
	ifstream my_error;
	my_error.open("Scene/error.txt", ios::in);
	stringstream ss;
	string error;
	ss << my_error.rdbuf();
	error = ss.str();

	catNotice(error);

	my_error.close();             //�ر��ļ������� 
}
void compiler_design_qt::getProgramC()//���ļ��л�ȡC���Գ���
{
	ifstream c_program;
	c_program.open(c_route.toStdString(), ios::in);
	stringstream ss;
	string program;
	ss << c_program.rdbuf();
	program = ss.str();

	ui->label_2->setText(QString::fromLocal8Bit("C���Գ���"));
	c_altered = 0;
	ui->cProgram->setPlainText(QString::fromStdString(program));

	c_program.close();             //�ر��ļ������� 

}

void compiler_design_qt::getProgramPascal()//���ļ��л�ȡPascal����
{
	ifstream pascal_program;
	pascal_program.open(pascal_route.toStdString(), ios::in);
	stringstream ss;
	ss << pascal_program.rdbuf();

	ui->label->setText(QString::fromLocal8Bit("PascalԴ����"));
	pascal_altered = 0;
	ui->pascalProgram->setPlainText(QString::fromStdString(ss.str()));

	pascal_program.close();             //�ر��ļ������� 

}

void compiler_design_qt::getNotice()//���ļ��л�ȡ��Ϣ����
{
	ifstream notice;
	notice.open("Scene/notices.txt", ios::in);
	stringstream ss;
	ss << notice.rdbuf();

	ui->notice->setPlainText(QString::fromStdString(ss.str()));

	notice.close();             //�ر��ļ������� 
}

void compiler_design_qt::getRoute()//���ļ��л�ȡ·��
{
	ifstream routes;
	routes.open("Scene/routes.txt", ios::in);
	string str;
	getline(routes, str);
	pascal_route = QString::fromStdString(str);
	getline(routes, str);
	c_route = QString::fromStdString(str);
}

/****************************��->�ļ�*******************************/

void compiler_design_qt::outputWord()
{
	ofstream words;
	words.open("Scene/word.txt", ios::trunc);

	words << left << setw(10) << "�Ǻ�id";
	words << left << setw(15) << "�Ǻű�־";
	words << left << setw(15) << "�Ǻ�ֵ";
	words << left << setw(15) << "��������" << endl;
	for (int i = 0; i < (int)word_stream.size(); i++)
	{
		if (word_stream[i].id != -1)
		{
			words << left << setw(10) << word_stream[i].id;
			words << left << setw(15) << word_stream[i].symbol;
			words << left << setw(15) << word_stream[i].value;
			words << left << setw(15) << word_stream[i].row << endl;
		}
	}


	words.close();             //�ر��ļ������� 
}

/****************************����/����/����*******************************/

int compiler_design_qt::confirm()
{
	if (pascal_route != "" && c_route != "")
	{
		ifstream f1(pascal_route.toStdString()), f2(c_route.toStdString());
		if (f1.good() && f2.good())
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void compiler_design_qt::generateC()//����c���Դ��뵽��������
{
	if (confirm())
	{
		init();//��ʼ����ǰһ��Ҫ��ʼ��
	
		if (!savePascal())//����pascal�ļ�
			QMessageBox::critical(this, "Error", "Empty input", QMessageBox::Ok);
		else
		{
			catNotice("Generating...");
			settings params = preProcess();
			if (input(params))  //Դ�������
			{
				lexical_analysis(); //�ʷ�����
				grammatical_analysis();//�﷨�Ƶ�����
				output(params);  //Ŀ��������־���

				if (!errorGenerate())//���ɴ�����Ϣ
				{
					cProgramGenerate();//����Ļ������c���Գ���
					catNotice("C program generated!");
				}

				outputWord();
				in.close();
			}
		}
	}
	else
		QMessageBox::critical(this, "Error", "Please choose vaild files(C/pascal).", QMessageBox::Ok);
}

void compiler_design_qt::complieProgramC()//����c����
{
	if (confirm())
	{
		if (ui->cProgram->toPlainText() == NULL)
			QMessageBox::critical(this, "Error", "Empty program", QMessageBox::Ok);
		else
		{
			ifstream my_exe;
			string compile_order;
			compile_order = "g++ -o cProgram " + c_route.toStdString() + " 2> Scene/compileError.txt";
			saveC();
			system("rm cProgram.exe");//ɾ��֮ǰ�Ŀ�ִ���ļ�
			system(compile_order.c_str());//���뵱ǰ��ִ���ļ�
			getCompileError();//��ȡ������Ϣ
			my_exe.open("cProgram.exe", ios::binary);
			if (my_exe.is_open())
				catNotice("Successfully compiled!");
			else
				catNotice("Compiled failure...");
		}
	}
	else
		QMessageBox::critical(this, "Error", "Please choose vaild files(C/pascal).", QMessageBox::Ok);
}

void compiler_design_qt::runProgramC()//����c����
{
	if (ui->cProgram->toPlainText() == NULL)
		QMessageBox::critical(this, "Error", "Empty program", QMessageBox::Ok);
	else
	{
		ifstream my_exe;
		my_exe.open("cProgram.exe", ios::binary);
		if (my_exe.is_open())
		{
			system("cProgram > Scene/output.txt");//��������ض����ļ�
			getOutputC();//��ȡ�ļ�������
		}
		else
			catNotice("No executable file...");

	}
}

void compiler_design_qt::getOutputC()//��ȡC���Գ�������н��
{
	ifstream c_output;
	c_output.open("Scene/output.txt", ios::out);
	stringstream ss;
	string result;
	ss << c_output.rdbuf();
	result = ss.str();
	catNotice(result);			//����������ı���
	c_output.close();             //�ر��ļ������� 
}

void compiler_design_qt::getCompileError()//���ļ��л�ȡ����ʱ�ı���
{
	ifstream com_error;
	com_error.open("Scene/compileError.txt", ios::out);
	stringstream ss;
	string result;
	ss << com_error.rdbuf();
	result = ss.str();
	if(result.length() > 0)
		catNotice(result);			//����������ı���
	com_error.close();             //�ر��ļ������� 
}

/****************************·��ѡ��*******************************/

QString compiler_design_qt::chooseFile(QString filter)//ѡ���ļ����� ����ѡ��ʲô�ļ��ٵ���
{
	QFileDialog *my_file = new QFileDialog(this);
	my_file->setWindowTitle("Please choose your file");
	my_file->setDirectory(QDir::currentPath());
	my_file->setNameFilter(filter);

	QStringList filenames;

	if (my_file->exec())
	{
		filenames = my_file->selectedFiles();
		QString help = QDir::currentPath() + "/Scene";
		if (filenames[0].indexOf(help) == -1)//���ѡ����scene�ļ����µ������ǲ������
		{
			if (filenames[0].indexOf(QDir::currentPath()) == -1)
				return filenames[0];
			else
				return filenames[0].mid(QDir::currentPath().length()+1);
		}
		else
		{
			QMessageBox::critical(this, "Error", "Donnot save/choose file in folder '/Scene'", QMessageBox::Ok);
			return "ERROR";
		}
	}
	else
		return "ERROR";
}

QString compiler_design_qt::chooseSaveRoute(QString type)//ѡ�񱣴�·��
{
	QString kind = type + QString::fromLocal8Bit("�ļ�(*") + type + ")";
	QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::currentPath(), kind);

	if(filename.indexOf(type) == -1)//���������.c��β��
		filename += type;

	if (filename != type)
	{
		if (filename.indexOf(QDir::currentPath() + "/Scene") == -1)
		{
			QFile file(filename);
			QString ret;
			file.open(QFile::WriteOnly);
			if (file.exists() && file.isOpen())
				ret = filename;
			else
				ret = "ERROR";
			file.close();

			return ret;
		}
		else//���ѡ����scene�ļ����µ������ǲ������
		{
			QMessageBox::critical(this, "Error", "Donnot save/choose file in folder '/Scene'", QMessageBox::Ok);
			return "ERROR";
		}
	}
	else
		return "ERROR";
}

void compiler_design_qt::chooseInputFile()//ѡ�������ļ� ��pascal����
{
	QString input = chooseFile("*.pas");
	if (input != "ERROR")//���ѡ���ļ��ɹ�
	{
		pascal_route = input;//��·���洢��private������
		ui->inputLine->setText(pascal_route);
		getProgramPascal();
	}
	else
		QMessageBox::critical(this, "Error", "Choose file error", QMessageBox::Ok);
}

void compiler_design_qt::chooseOutputFile()//ѡȡ�����C���Գ���·��
{
	QString output = chooseFile("*.c");
	if (output != "ERROR")
	{
		c_route = output;
		ui->outputLine->setText(output);
		getProgramC();
	}
	else
		QMessageBox::critical(this, "Error", "Choose file error", QMessageBox::Ok);
}


/****************************�������������*******************************/

void compiler_design_qt::catNotice(string str)//����notice�ı���
{
	QString notice = ui->notice->toPlainText();
	str = str + "\n>>>";
	notice = notice + QString::fromStdString(str);
	ui->notice->setPlainText(notice);
	QScrollBar *scrollbar = ui->notice->verticalScrollBar();
	scrollbar->setSliderPosition(scrollbar->maximum());
}

void compiler_design_qt::clearNotice()//�����ʾ����
{
	ui->notice->setPlainText(">>>");
}
