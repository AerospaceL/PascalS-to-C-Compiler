#include "compiler_design_qt.h"
#include "head.h"

extern ifstream in;  //源代码输入流
extern ofstream out; //目标代码输出流
extern ofstream log_file; //日志输出流


extern vector<word>word_stream;//记号流
extern vector<string>code_stream;//生成代码流
extern vector<error_item>error_stream;//错误流


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
				QMessageBox::information(this, "Success", QString::fromLocal8Bit("保存成功"), QMessageBox::Ok);
			else
				QMessageBox::critical(this, "Error", QString::fromLocal8Bit("保存失败"), QMessageBox::Ok);
			renew();
		}
		else
			QMessageBox::critical(this, "Error", QString::fromLocal8Bit("保存失败"), QMessageBox::Ok);
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
				QMessageBox::information(this, "Success", QString::fromLocal8Bit("保存成功"), QMessageBox::Ok);
			else
				QMessageBox::critical(this, "Error", QString::fromLocal8Bit("保存失败"), QMessageBox::Ok);
			renew();
		}
		else
			QMessageBox::critical(this, "Error", QString::fromLocal8Bit("保存失败"), QMessageBox::Ok);
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
	savePascal();//将目标文件中的内容保存

	//询问是否要保存现场
	int answer = QMessageBox::question(this, tr("Quit"), QString::fromLocal8Bit("是否保存现场"), QMessageBox::Yes, QMessageBox::No);
	if (answer == QMessageBox::Yes) 
	{
		saveNotice();
		saveRoute();
		pascal_route = "Scene/pascalProgram.pas";
		c_route = "Scene/cProgram.c";//保存现场的两个文件
		saveC();
		savePascal();//将文件同时保存到保留现场的文件中
	}
	ofstream clear_file;
	clear_file.open("Scene/pascalBoard.pas", ios::trunc);
	clear_file << "";
	clear_file.close();
	clear_file.open("Scene/cBoard.c", ios::trunc);
	clear_file << "//You are using the BLACKBOARD system produced, which will be cleared when closed." << endl;
	clear_file << "//If you want to save your edit, please choose a vaild .pas source file and a .c target file." << endl;
	clear_file.close();//清除白板

}
/****************************窗口所需*******************************/

settings compiler_design_qt::preProcess()//开始前设置params
{
	settings params;
	params.scr_path = pascal_route.toStdString();
	params.dst_path = c_route.toStdString();
	//params.log_path = "log.txt";
	params.file_output = true;

	return params;
}

void compiler_design_qt::showInit()//初始屏幕中的数据内容
{
	ui->notice->setPlainText(">>>");
	ui->inputLine->setText("Blackboard");
	ui->outputLine->setText("Blackboard"); //初始不显示路径 假装是没有选择文件也可以运行
	getProgramC();
	getProgramPascal();
}

void compiler_design_qt::recover()//恢复现场
{
	int answer = QMessageBox::question(this, tr("Recover"), QString::fromLocal8Bit("是否确定要还原上次保存的现场？"), QMessageBox::Yes, QMessageBox::No);

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

void compiler_design_qt::renew()//刷新
{
	ui->inputLine->setText(pascal_route);
	ui->outputLine->setText(c_route);
	getProgramPascal();
}

void compiler_design_qt::alterMarkPascal()
{
	if (pascal_altered)
		ui->label->setText(QString::fromLocal8Bit("Pascal源程序*"));
	else //第一次会在导入文件的时候触发 此时alter为0
		pascal_altered=1;
}

void compiler_design_qt::alterMarkC()
{
	if (c_altered)
		ui->label_2->setText(QString::fromLocal8Bit("C语言程序*"));
	else//第一次会在导入文件的时候触发 此时alter为0
		c_altered = 1;
}

/****************************流->窗口*******************************/

void compiler_design_qt::cProgramGenerate()//从代码流中获取生成的c语言程序到屏幕上
{
	string show_c = "";
	for (int i = 0; i < (int)code_stream.size(); i++)
		show_c += code_stream[i] + "\n";

	ui->cProgram->setPlainText(QString::fromStdString(show_c));
}

int compiler_design_qt::errorGenerate()//从错误流中获取错误信息并输出到屏幕上 并返回是否有错误信息产生
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

/****************************窗口->文件*******************************/

int compiler_design_qt::saveC()//保存c代码到当前所选择的文件中
{
	QString c_program = ui->cProgram->toPlainText();//获取文本框中的pascal程序
	ui->label_2->setText(QString::fromLocal8Bit("C语言程序"));
	if (c_program == NULL)//如果输入框为空
		return 2;
	else
	{
		ofstream c_file;
		c_file.open(c_route.toStdString(), ios::trunc);
		if (c_file.is_open())
			c_file << c_program.toStdString();//将c程序更新到文件中
		else
			return 0;
	}
	return 1;
}

int compiler_design_qt::savePascal()//保存pascal代码到当前所选择的文件中
{
	QString pascal_program = ui->pascalProgram->toPlainText();//获取文本框中的pascal程序
	ui->label->setText(QString::fromLocal8Bit("Pascal源程序"));
	if (pascal_program == NULL)//如果输入框为空
		return 2;
	else
	{
		ofstream pascal_file;
		pascal_file.open(pascal_route.toStdString(), ios::trunc);
		if (pascal_file.is_open())
			pascal_file << pascal_program.toStdString();//将pascal程序更新到文件中
		else
			return 0;
	}
	return 1;
}

void compiler_design_qt::saveNotice()
{
	QString notice = ui->notice->toPlainText();//获取文本框中的pascal程序
	if (notice != NULL)//如果输入框不为空
	{
		ofstream notice_file;
		notice_file.open("Scene/notices.txt", ios::trunc);
		notice_file << notice.toStdString();//将pascal程序更新到文件中
	}
}

void compiler_design_qt::saveRoute()
{
	string routes;
	routes = pascal_route.toStdString() + "\n" + c_route.toStdString();
	ofstream route_file;
	route_file.open("Scene/routes.txt", ios::trunc);
	route_file << routes ;//将pascal程序更新到文件中
}

/****************************文件->窗口*******************************/

void compiler_design_qt::getError()//从文件中获取错误信息
{
	ifstream my_error;
	my_error.open("Scene/error.txt", ios::in);
	stringstream ss;
	string error;
	ss << my_error.rdbuf();
	error = ss.str();

	catNotice(error);

	my_error.close();             //关闭文件输入流 
}
void compiler_design_qt::getProgramC()//从文件中获取C语言程序
{
	ifstream c_program;
	c_program.open(c_route.toStdString(), ios::in);
	stringstream ss;
	string program;
	ss << c_program.rdbuf();
	program = ss.str();

	ui->label_2->setText(QString::fromLocal8Bit("C语言程序"));
	c_altered = 0;
	ui->cProgram->setPlainText(QString::fromStdString(program));

	c_program.close();             //关闭文件输入流 

}

void compiler_design_qt::getProgramPascal()//从文件中获取Pascal程序
{
	ifstream pascal_program;
	pascal_program.open(pascal_route.toStdString(), ios::in);
	stringstream ss;
	ss << pascal_program.rdbuf();

	ui->label->setText(QString::fromLocal8Bit("Pascal源程序"));
	pascal_altered = 0;
	ui->pascalProgram->setPlainText(QString::fromStdString(ss.str()));

	pascal_program.close();             //关闭文件输入流 

}

void compiler_design_qt::getNotice()//从文件中获取信息数据
{
	ifstream notice;
	notice.open("Scene/notices.txt", ios::in);
	stringstream ss;
	ss << notice.rdbuf();

	ui->notice->setPlainText(QString::fromStdString(ss.str()));

	notice.close();             //关闭文件输入流 
}

void compiler_design_qt::getRoute()//从文件中获取路径
{
	ifstream routes;
	routes.open("Scene/routes.txt", ios::in);
	string str;
	getline(routes, str);
	pascal_route = QString::fromStdString(str);
	getline(routes, str);
	c_route = QString::fromStdString(str);
}

/****************************流->文件*******************************/

void compiler_design_qt::outputWord()
{
	ofstream words;
	words.open("Scene/word.txt", ios::trunc);

	words << left << setw(10) << "记号id";
	words << left << setw(15) << "记号标志";
	words << left << setw(15) << "记号值";
	words << left << setw(15) << "所在行数" << endl;
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


	words.close();             //关闭文件输入流 
}

/****************************生成/编译/运行*******************************/

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

void compiler_design_qt::generateC()//生成c语言代码到代码流中
{
	if (confirm())
	{
		init();//开始编译前一定要初始化
	
		if (!savePascal())//更新pascal文件
			QMessageBox::critical(this, "Error", "Empty input", QMessageBox::Ok);
		else
		{
			catNotice("Generating...");
			settings params = preProcess();
			if (input(params))  //源代码读入
			{
				lexical_analysis(); //词法分析
				grammatical_analysis();//语法制导翻译
				output(params);  //目标代码和日志输出

				if (!errorGenerate())//生成错误信息
				{
					cProgramGenerate();//在屏幕上生成c语言程序
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

void compiler_design_qt::complieProgramC()//编译c程序
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
			system("rm cProgram.exe");//删除之前的可执行文件
			system(compile_order.c_str());//编译当前可执行文件
			getCompileError();//获取报错信息
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

void compiler_design_qt::runProgramC()//运行c程序
{
	if (ui->cProgram->toPlainText() == NULL)
		QMessageBox::critical(this, "Error", "Empty program", QMessageBox::Ok);
	else
	{
		ifstream my_exe;
		my_exe.open("cProgram.exe", ios::binary);
		if (my_exe.is_open())
		{
			system("cProgram > Scene/output.txt");//程序输出重定向到文件
			getOutputC();//获取文件输出结果
		}
		else
			catNotice("No executable file...");

	}
}

void compiler_design_qt::getOutputC()//获取C语言程序的运行结果
{
	ifstream c_output;
	c_output.open("Scene/output.txt", ios::out);
	stringstream ss;
	string result;
	ss << c_output.rdbuf();
	result = ss.str();
	catNotice(result);			//将输出置入文本框
	c_output.close();             //关闭文件输入流 
}

void compiler_design_qt::getCompileError()//从文件中获取编译时的报错
{
	ifstream com_error;
	com_error.open("Scene/compileError.txt", ios::out);
	stringstream ss;
	string result;
	ss << com_error.rdbuf();
	result = ss.str();
	if(result.length() > 0)
		catNotice(result);			//将输出置入文本框
	com_error.close();             //关闭文件输入流 
}

/****************************路径选择*******************************/

QString compiler_design_qt::chooseFile(QString filter)//选择文件函数 具体选择什么文件再调用
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
		if (filenames[0].indexOf(help) == -1)//如果选择了scene文件夹下的内容是不允许的
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

QString compiler_design_qt::chooseSaveRoute(QString type)//选择保存路径
{
	QString kind = type + QString::fromLocal8Bit("文件(*") + type + ")";
	QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::currentPath(), kind);

	if(filename.indexOf(type) == -1)//如果不是以.c结尾的
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
		else//如果选择了scene文件夹下的内容是不允许的
		{
			QMessageBox::critical(this, "Error", "Donnot save/choose file in folder '/Scene'", QMessageBox::Ok);
			return "ERROR";
		}
	}
	else
		return "ERROR";
}

void compiler_design_qt::chooseInputFile()//选择输入文件 即pascal程序
{
	QString input = chooseFile("*.pas");
	if (input != "ERROR")//如果选择文件成功
	{
		pascal_route = input;//将路径存储在private数据中
		ui->inputLine->setText(pascal_route);
		getProgramPascal();
	}
	else
		QMessageBox::critical(this, "Error", "Choose file error", QMessageBox::Ok);
}

void compiler_design_qt::chooseOutputFile()//选取输出的C语言程序路径
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


/****************************窗口命令行输出*******************************/

void compiler_design_qt::catNotice(string str)//设置notice文本框
{
	QString notice = ui->notice->toPlainText();
	str = str + "\n>>>";
	notice = notice + QString::fromStdString(str);
	ui->notice->setPlainText(notice);
	QScrollBar *scrollbar = ui->notice->verticalScrollBar();
	scrollbar->setSliderPosition(scrollbar->maximum());
}

void compiler_design_qt::clearNotice()//清空提示窗口
{
	ui->notice->setPlainText(">>>");
}
