#include<iostream>
#include<fcntl.h>
#include<string>
#include<stack>
#include<queue>
#include<cctype>
#include<fstream>
#include"ToyLanguage.h"
using namespace std;


int main(void) {


	bool loop = true;

	//defun.txt �ʱ�ȭ (����)
	std::ofstream ofs;
	ofs.open("defun.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();

	vector<string> funcList;
	funcList.push_back("MINUS");
	funcList.push_back("IF");

	ToyInterpreter tInter;
	tInter.FunctionList(); //�Լ��迭����
	while (loop) {

		
		Lexeme_unit* lexe = new Lexeme_unit;
		

		int choose;
		char fileName[30] = { 0, };
		
		int flag = 0;

		
		cout << "============================================" << endl;
		cout << "1. Define DEFUN" << endl;
		cout << "2. Print DEFUN" << endl;
		cout << "3. Interpreter" << endl;
		cout << "4. Exit" << endl;
		cout << "============================================" << endl;
		cout << "�޴��� �����ϼ��� >> ";
		cin >> choose;
		cin.ignore(256, '\n');

		if (choose == 1) {
			string defineFun;
			cout << "�Լ��� �����ϼ��� >> ";
			getline(cin, defineFun);
			tInter.defineFunc(defineFun); 
			cout << endl;
		}
		else if (choose == 2) {
			tInter.printDEFUN();
		}
		else if (choose == 3) {
			cout << "���ϸ��� �Է��ϼ��� >> ";
			cin >> fileName;
			cin.ignore(256, '\n');
			tInter.fileLoad(fileName); //fileName
			lexe = tInter.lexicalAnalyzer();
			if (lexe != NULL) {
				flag = tInter.syntaxAnalyzer(lexe);
				if (flag != -1) {
					tInter.intermediateCode();
					tInter.printResult();
				}
			}
			cout << endl;
		}
		else if (choose == 4) {
			cout << "���α׷��� �����մϴ�." << endl;
			loop = false;
		}
		else {
			cout << "���� �޴��� �����߽��ϴ�. �ٽ� �������ּ���" << endl;
		}
		//cin.ignore(256, '\n');
	}

	return 0;

}