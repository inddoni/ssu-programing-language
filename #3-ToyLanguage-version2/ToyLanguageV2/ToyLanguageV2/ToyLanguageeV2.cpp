#include<iostream>
#include<fcntl.h>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include<stack>
#include<queue>
#include<cctype>
#include "ToyLanguage.h"
using namespace std;



bool ToyInterpreter::isNum(char str) {
	bool result = false;

	if (str >= 48 && str <= 57)
		result = true;

	return result;
}


bool ToyInterpreter::isLetter(char str) {
	bool result = false;

	if ((str >= 65 && str <= 90) || (str >= 97 && str <= 122) || str == 45)
		result = true;
	else if (str == '/')
		result = true;

	return result;
}


bool ToyInterpreter::isInteger(string s) {

	bool result = false;

	for (int i = 0; i < s.length(); i++) {
		if (s[i] >= 48 && s[i] <= 57) {
			result = true;
			continue;
		}
		else if (s[i] == 45) {
			result = true;
		}
		else {
			result = false;
			break;
		}
	}

	return result;
}

bool ToyInterpreter::isnegaInt(char str) {
	bool result = false;

	if (str == 45) { //'-'�̸� true
		result = true;
	}

	return result;
}

int ToyInterpreter::stointeger(string s) {
	int result = 0;

	char change[15] = { 0, };
	strcpy(change, s.c_str());
	result = atoi(change);

	return result;
}

void ToyInterpreter::setLine(string s) {
	lineNum = 1;
	lineRead[0] = s;

}

/* ===================================================================================
* Explain : File Load �޴� �����Ͽ� ������ �о����, �о�� ������ ���� ����(term)��
*			��� �� �� term�� ��������� ������ִ� �Լ�. (1. File Load ����)
* return : �����ڵ�
* parameter : �������ϴ� ������ �̸�
*  ====================================================================================
*/
int ToyInterpreter::fileLoad(char* filename) {

	FILE* file;
	file = fopen(filename, "r"); //�б���� ����
	char dataRead[120] = { 0, };

	// ���ϸ� ���� ����ó��
	while (file == NULL) {
		char refilename[30] = { 0, };
		cout << "���ϸ��� �ٽ� �Է����ּ��� >> ";
		cin >> refilename;
		//cin.ignore(256, '\n');
		file = fopen(refilename, "r");
	}

	// ���� ��ü�� �� �پ� �о�ͼ� lineRead �迭�� ���
	while (!feof(file)) {
		fgets(dataRead, 120, file);

		lineRead[lineNum] = dataRead;
		lineNum++;
	}

	cout << endl << "���� ������" << endl;
	cout << "----------------------------------------------" << endl;
	for (int i = 0; i < lineNum; i++) {
		cout << lineRead[i];
	}
	cout << endl << "----------------------------------------------" << endl;
	cout << "�Դϴ�." << endl << endl;
	fclose(file);

	return 0;
}


//Function class�� ���� ������� �ʱ�ȭ
int ToyInterpreter::count = 2; //IF�� MINUS�� ���´ϱ� 2�� �ʱ�ȭ ���ش�.

/* ===================================================================================
* Explain : ���ο� �Լ��� �����ϴ� �Լ�. Defun.txt���Ͽ� �ش� �Լ��� �ִ��� Ȯ�� ��
			������ ���ο� �Լ��� �����Ѵ�.
* return : (�����ϴ� �Լ��� �̹� �ְų�, �Ű������� �ϳ��� ������ ����)
* parameter :  ������ �Լ� ��ü (�Լ��̸�, parameter, �ش翬�� ������ ���ڿ�)
*  ===================================================================================
*/
void ToyInterpreter::FunctionList() {
	funclist = new Function[15];
	//ó���� IF�� MINUS�� �̸� ����
	//MINUS�� IF�� �Ķ����, ���� ���� �̸��� �־��ش�.
	funclist[0].name = "MINUS";
	funclist[1].name = "IF";
}

/* ===================================================================================
* Explain : ���ο� �Լ��� �����ϴ� �Լ�. Defun.txt���Ͽ� �ش� �Լ��� �ִ��� Ȯ�� ��
            ������ ���ο� �Լ��� �����Ѵ�.
* return : (�����ϴ� �Լ��� �̹� �ְų�, �Ű������� �ϳ��� ������ ����)
* parameter :  ������ �Լ� ��ü (�Լ��̸�, parameter, �ش翬�� ������ ���ڿ�)
*  ===================================================================================
*/
void ToyInterpreter::defineFunc(string defineFun) {

	string newFunStr = defineFun;
	string temp = "";
	int cnt = 0;
	vector<string> p;
	queue<string> qContent;
	bool isParam = true;
	bool isName = false;

	funs.push_back(newFunStr);
	int error = -1;

	
	//cout << newFunStr << endl;

	for (int i = 0; i < newFunStr.size(); i++) {
		//��ū ����
		if (newFunStr[i] == ' ' && temp != " " && temp != "") {
			if (isParam) {
				if (temp == "DEFUN") {
					temp = "";
					continue;
				}
				else if(isName) {
					int j = 0;
					while (funclist[j].getName() != "") {
						if (temp == funclist[j].getName()) {
							//parmeter ������ ��������
							if (p.size() == 0) {
								error = 0;
								cout << "�Լ��� parameter�� �������� �ʽ��ϴ�." << endl;
								break;
							}
							else {
								cnt++;
								isParam = false;
								break;
							}
						}
						j++;
					}
					if (temp != "" && isParam) {
						//parmeter ������ ��������
						if (p.size() == 0 && temp != "x") {
							error = 0;
							cout << "�Լ��� parameter�� �������� �ʽ��ϴ�." << endl;
							break;
						}
						else if (p.size() != 0 || temp == "x") {
							p.push_back(temp);
							temp = "";
							continue;
						}
						else {
							cnt++;
							isParam = false;
							break;
						}
					}
				}
			}
			//��ū �� �׸� ���� ����
			if (cnt == 0 && error != 0) {
				//����üũ : ������ �̸��� �Լ��� �ִ��� Ȯ��
				int k = 0;
				while (funclist[k].getName() != "") {
					if (temp == funclist[k].getName()) {
						cout << temp << " �Լ��� �̹� ���ǵǾ��ֽ��ϴ�." << endl;
						error = 0;
						break;
					}
					k++;
				}
				if (error != 0) {
					funclist[count].name = temp;
					isName = true;
					cnt++;
				}
				else {
					break;
				}
			}
			else if (cnt == 1 && error != 0) {
				p.push_back(temp);
			}
			else if (error == 0) {
				break;
			}
			else {
				qContent.push(temp);
			}

			temp = "";
		}
		else if(newFunStr[i] != '(' && newFunStr[i] != ')' && newFunStr[i] != ' ')
			temp += newFunStr[i];
		
		else if(error == 0) {
			break;
		}
		//������ ��ū ���
		//else if (i+1 == newFunStr.size()) {
		//	temp += newFunStr[i];
		//}

	}
	
	if (error != 0) {
		// ��������ū�߰�
		qContent.push(temp);

		funclist[count].setParam(p);
		funclist[count].setFunCalc(qContent);
		count++;

		ofstream output("defun.txt", ios::app);
		output << newFunStr.substr(6) << endl;
		output.close();

		cout << "���ο� �Լ� �߰� �Ϸ� - " << funclist[count - 1].getName() << endl;
	}
	else {
		funclist[count].name = "";
	}
	

}
void ToyInterpreter::printDEFUN() {
	FILE* file = fopen("defun.txt", "r"); //�б���� ����
	char dataRead[100] = { 0, };
	string printStr[20];
	int idx = 0;
	
	// ���� ��ü�� �� �پ� �о�ͼ� printStr �迭�� ���
	while (!feof(file)) {
		fgets(dataRead, 100, file);

		printStr[idx] = dataRead;
		idx++;
	}
	//cout << endl << "���� ������" << endl;
	//cout << "----------------------------------------------" << endl;
	for (int i = 0; i < idx-1; i++) {
		cout << printStr[i];
	}
	//cout << endl << "----------------------------------------------" << endl;
	//cout << "�Դϴ�." << endl << endl;
	fclose(file);
}

//ã�� �Լ��� �ε��� ��ȯ (������ -1 ��ȯ)
int ToyInterpreter::searchFunc(string name) {
	int i = 0;
	bool find = false;
	while (funclist[i].getName() != "") {
		if (name == funclist[i].getName()) {
			find = true;
			break;
		}
		i++;
	}
	if (find)
		return i;
	else
		return -1;

}
queue<string> ToyInterpreter::convertFunc(string name, int v1) {
	queue<string> newfunC;

	//�Լ��̸��� �´�(�ε����� �´�) ť�� ������
	int idx = searchFunc(name);
	queue<string> funC = funclist[idx].getFunCalc();

	//ť�� �ϳ��� ���ϸ鼭 x = v1 �����ؼ�
	//���ο� ť�� ����
	while (!funC.empty()) {
		string s;
		s = funC.front();
		funC.pop();
		if (s == "x")
			newfunC.push(to_string(v1));
		else
			newfunC.push(s);
	}

	return newfunC;
}
queue<string> ToyInterpreter::convertFunc(string name, int v1, int v2) {
	queue<string> newfunC;

	//�Լ��̸��� �´�(�ε����� �´�) ť�� ������
	int idx = searchFunc(name);
	queue<string> funC = funclist[idx].getFunCalc();

	//ť�� �ϳ��� ���ϸ鼭 x = v1, y = v2 �����ؼ�
	//���ο� ť�� ����
	while (!funC.empty()) {
		string s;
		s = funC.front();
		funC.pop();
		if (s == "x")
			newfunC.push(to_string(v1));
		else if (s == "y")
			newfunC.push(to_string(v2));
		else
			newfunC.push(s);
	}

	return newfunC;
}
queue<string> ToyInterpreter::convertFunc(string name, int v1, int v2, int v3) {
	queue<string> newfunC;

	//�Լ��̸��� �´�(�ε����� �´�) ť�� ������
	int idx = searchFunc(name);
	queue<string> funC = funclist[idx].getFunCalc();

	//ť�� �ϳ��� ���ϸ鼭 x = v1, y = v2 �����ؼ�
	//���ο� ť�� ����
	while (!funC.empty()) {
		string s;
		s = funC.front();
		funC.pop();
		if (s == "x")
			newfunC.push(to_string(v1));
		else if (s == "y")
			newfunC.push(to_string(v2));
		else if (s == "z")
			newfunC.push(to_string(v3));
		else
			newfunC.push(s);
	}

	return newfunC;
}
bool ToyInterpreter::isMyFunc(string s) {
	int i = 0;
	bool check = false;
	while (funclist[i].getName() != "") {
		if (s == funclist[i].getName()) {
			check = true;
			break;
		}
		i++;
	}

	return check;
}
/* ===================================================================================
* Explain : lexical analyzer. ������ lexeme ������ �߶� lexeme_unit�� ����
* return : �ۼ��� lexeme_unit
* parameter :  ����
*  ===================================================================================
*/
Lexeme_unit* ToyInterpreter::lexicalAnalyzer(void) {

	Lexeme_unit* lex;
	lex = new Lexeme_unit[lineNum];
	// �� Ŭ������ ã�Ƴ� lexeme�� �� ������ �����Ѵ�.


	string temp = "";

	for (int i = 0; i < lineNum; i++) {

		string test = lineRead[i]; //�ϳ��� ������ �о�´�.

		int k = 0;
		int count = -1; //���γ��� lexeme ����

		//�������� Ȯ��
		if (isNum(test[k])) {

			temp.append(1, test[k]);
			k++;

			while (test[k] != '\n' && test[k] != NULL) {
				if (isNum(test[k])) {
					temp.append(1, test[k]);
					k++;
				}
				else {
					cout << "[error] ������ �Է��� �����մϴ�." << endl;
					return NULL;
				}
			}
			count = 0;
		}

		//�������� Ȯ��
		else if (isnegaInt(test[k]) && test[k] != NULL) {
			temp.append(1, test[k]);
			k++;
			if (isnegaInt(test[k]))
				cout << "'-'�� �ʹ� ���� �Է��߽��ϴ�." << endl;

			while (test[k] != '\n' && !isnegaInt(test[k])) {
				if (isNum(test[k])) {
					temp.append(1, test[k]);
					k++;
				}
				else {
					cout << "[error] ���� �Է� ����" << endl;
					return NULL;
				}
			}
			count = 0;
		}

		//�������� Ȯ�� (���ڸ� ����!)
		else if (isLetter(test[k])) {

			temp.append(1, test[k]);
			k++;

			while (test[k] != '\n' && test[k] != NULL) {
				if (isLetter(test[k])) {
					temp.append(1, test[k]);
					k++;
				}
				else {
					cout << "[error] ���� �Է� ���� (���ĺ��� '-'�� �Է��� �����մϴ�.)" << endl;
					return NULL;
				}
			}
			count = 0;
		}

		//��ȣ���� �������� Ȯ��
		else if (test[k] == 40) {
			count++;
			lex[i].lexemes[count].append(1, test[k]); // '(' ����
			k++;

			string op = "";

			while (test[k] != '\n' && test[k] != NULL) {
				//��ȣ ���� ������ ���ڰ� ������ ������ ���ڸ� �ٿ��� �����
				//�Լ���ȿ� �ִ��� Ȯ���Ѵ�.

				if (isLetter(test[k]) && test[k] != '-') {
					int ercode = -1;
					while (test[k] != ' ') {
						if (isLetter(test[k])) {
							op.append(1, test[k]);
							k++;
						}
						else {
							cout << "�����Է¿���" << endl;
							ercode = 0;
							break;
						}
					}
					int n = 0;
					while (funclist[n].getName() != "" && ercode == -1) {
						if (op == funclist[n].getName()) {
							count++;
							lex[i].lexemes[count] = op;
							op = "";
							break;
						}
						n++;
					}
					if (op != "") {
						//���ڿ� ���ڰ� ���� �͵� �ƴϰ�, �Լ��� �ƴϸ� undefined ����
						cout << "undefined" << endl;
					
					}
				}

				//else if (test[k] == 'I') {
				//	while (test[k] != ' ') { //�����϶�����
				//		op.append(1, test[k]);
				//		k++;
				//	}
				//	if (op.compare("IF") == 0) {
				//		count++;
				//		lex[i].lexemes[count] = op;
				//		op = "";
				//	}
				//	else {
				//		cout << "undefined" << endl;
				//	}
				//}

				else if (isNum(test[k])) {
					temp.append(1, test[k]);
					k++;
					while (test[k] != ' ') { //���� ���� ������ ������ ����
						if (isNum(test[k])) { //���� �͵� �������� Ȯ��
							temp.append(1, test[k]);
							k++;
						}
						else {
							break;
						}
					}
					count++;
					lex[i].lexemes[count] = temp;
					temp = "";
				}

				//�������� Ȯ��
				else if (isnegaInt(test[k])) {
					temp.append(1, test[k]);
					k++;
					while (test[k] != ' ' && !isnegaInt(test[k])) {
						if (isNum(test[k])) {
							temp.append(1, test[k]);
							k++;
						}
						else {
							break;
						}
					}
					if (isnegaInt(test[k])) {
						cout << "-�� ������ �ԷµǾ����ϴ�." << endl;
						continue;
					}
					else {
						count++;
						lex[i].lexemes[count] = temp;
					}
					temp = "";

				}

				//else if (isLetter(test[k])) {
				//	temp.append(1, test[k]);
				//	k++;
				//	while (test[k] != ' ') { //���� ���� ������ ������ ����
				//		if (isLetter(test[k])) { //���� �͵� �������� Ȯ��
				//			temp.append(1, test[k]);
				//			k++;
				//		}
				//		else {
				//			cout << "���� �Է� ����" << endl;
				//			break;
				//		}
				//	}
				//	count++;
				//	lex[i].lexemes[count] = temp;
				//	temp = "";
				//}

				else if (test[k] == ')') {
					count++;
					lex[i].lexemes[count].append(1, test[k]);
					k++;
				}

				else if (test[k] == '(') {
					count++;
					lex[i].lexemes[count].append(1, test[k]);
					k++;
				}

				else if (test[k] == ' ') { // ������ �ѱ��
					k++;
					continue;
				}

				else {
					cout << "������ ���� syntax error" << endl;
				}
			}

			continue;
		}

		else if (test[i] == ' ') {
			k++;
			continue;
		}

		else {
			cout << "lexeme �ν� �Ұ�" << endl;
		}

		lex[i].lexemes[count] = temp;
		temp = ""; //temp�� ������ lexeme ������ �ʱ�ȭ

	}

	

	return lex;
}
/* ===================================================================================
* Explain : syntax analyzer. lexeme_unit�� ����� lexeme���� token�� �з��ϴ� �Լ�
			����, ����, ������(IF, MINUS), ��ȣ�� �з��ȴ�. lex�� ��Ȯ�� ������ �Ǵ�
			parse tree ���� (postorder)
* return : �����ڵ�
* parameter : lexeme_unit (lexeme�� ������ ��ü)
*  ===================================================================================
*/
int ToyInterpreter::syntaxAnalyzer(Lexeme_unit* lex) {

	int iserror = 1;

	// 1. token �ľ�
	for (int i = 0; i < lineNum; i++) {
		int k = 0;
		while (lex[i].lexemes[k] != "") { //�ش� ������ lexeme ���� ����.

			if (isInteger(lex[i].lexemes[k])) {
				lex[i].token[k] = "integer";
				k++;
			}

			else if (lex[i].lexemes[k].compare("(") == 0) {
				lex[i].token[k] = "parenthesesO";
				k++;
			}

			else if (lex[i].lexemes[k].compare(")") == 0) {
				lex[i].token[k] = "parenthesesC";
				k++;
			}

			else if (isMyFunc(lex[i].lexemes[k])) {
				lex[i].token[k] = "FUNC";
				k++;
			}

			/*else if (lex[i].lexemes[k].compare("IF") == 0) {
				lex[i].token[k] = "IF_op";
				k++;
			}*/

			else {
				lex[i].token[k] = "var";
				k++;
			}
		}
	}


	// 2. parse trees ���ÿ� �־��ֱ� (postorder)
	// �ϳ��� ���ؿ� ���� ��ü������ �־��ְ� ���� ������ ���� 99�� �Ѵ�.
	for (int i = 0; i < lineNum; i++) {
		int k = 0;
		while (lex[i].token[k] != "") {
			if (lex[i].token[k] == "integer") {
				parsetrees.push(lex[i].lexemes[k]);
				k++;
			}
			else if (lex[i].token[k] == "var") {
				parsetrees.push("undefined");
				k++;
			}
			else if (lex[i].token[k] == "parenthesesO") {
				int pCount = 0;
				while (lex[i].token[k] != "") {

					if (lex[i].token[k] == "parenthesesO") {
						pCount++; //���� ��ȣ count
						k++;

						if (lex[i].token[k] == "FUNC") {
							parsetrees.push(lex[i].lexemes[k]);
							k++;
							if (lex[i].token[k] == "(") {
								continue;
							}
							else if (lex[i].token[k] == "integer") {
								parsetrees.push(lex[i].lexemes[k]);
								k++;
							}
						}
						else {
							cout << "�����ڸ� �߸��Է��ϼ̽��ϴ�." << endl;
							iserror = -1;
						}

					}

					else if (lex[i].token[k] == "integer") { //������ �ι�° integer�� ���⼭ �����ϰ� �ȴ�.
						parsetrees.push(lex[i].lexemes[k]);
						k++;
					}
					else if (lex[i].token[k] == "parenthesesC") {
						pCount--;
						k++;
					}

					else {
						cout << "undefined" << endl;
						return NULL;
					}

				}

				if (pCount > 0) {
					cout << "')'�� �����ϴ�." << endl;
					return NULL;
				}
				else if (pCount < 0) {
					cout << "'('�� �����ϴ�." << endl;
					return NULL;
				}

			}
			else {
				cout << "parsetrees push error" << endl;
				k++;
				iserror = -1;
			}
		}
		parsetrees.push("999"); //���α׷��� ��(�ش� ������ ��) 999�� ǥ��
	}

	return iserror;
}

/* ===================================================================================
* Explain : syntax analyzer���� ������ parse tree ���ÿ��� ���� �ϳ��� ������
			������� ����Ͽ� �����Ѵ�. 9999�� undefined�� ����ϰ� ���� ���
* return : ����
* parameter : ����
*  ===================================================================================
*/
void ToyInterpreter::intermediateCode() {
	string current;
	int num; //string to int �� ������ ����
	int t1;
	int t2;
	int t3;
	int calc;

	while (!parsetrees.empty()) {

		current = parsetrees.top();

		while (!parsetrees.empty() && current != "999") {

			current = parsetrees.top();
			parsetrees.pop();

			if (current == "MINUS") {

				t1 = imCode.top();
				imCode.pop();
				t2 = imCode.top();
				imCode.pop();

				if (t1 != 9999 && t2 != 9999) {
					calc = t1 - t2;
					imCode.push(calc);
				}
				else
					cout << "�߰��ڵ� ���Ұ� : �Ŀ� var�� ����" << endl;

			}
			else if (current == "IF") {
				t1 = imCode.top();
				imCode.pop();
				t2 = imCode.top();
				imCode.pop();

				if (t1 != 9999 && t2 != 9999) {
					if (t1 > 0)
						calc = t2;
					else
						calc = 0;
					imCode.push(calc);
				}
				else
					cout << "�߰��ڵ� ���Ұ� : �Ŀ� var�� ����" << endl;
			}
			//IF�� MINUS�� �ƴ� �Լ����̸�
			else if (isMyFunc(current)) {
				//prameter ������ ���� ����
				int idx = searchFunc(current);
				int paramSize = funclist[idx].getParamSize();
				queue<string> result;
				if (!imCode.empty()) {
					if (paramSize == 1) {
						t1 = imCode.top();
						imCode.pop();
						result = convertFunc(current, t1);
					}
					else if (paramSize == 2) {
						t1 = imCode.top();
						imCode.pop();
						t2 = imCode.top();
						imCode.pop();
						if (current == "TIMES") {
							if (t1 == 0) {
								imCode.push(0);
								continue;
							}
						}
						result = convertFunc(current, t1, t2);
					}
					else {
						t1 = imCode.top();
						imCode.pop();
						t2 = imCode.top();
						imCode.pop();
						t3 = imCode.top();
						imCode.pop();
						result = convertFunc(current, t1, t2, t3);
					}
					while (!result.empty()) {
						string value = result.front();
						result.pop();
						parsetrees.push(value);
					}
				}
				
			}
			else if (current == "undefined") {
				imCode.push(9999);
			}
			else if (current != "999") { //����
				num = stointeger(current);
				imCode.push(num);
			}
			else {
				//error
			}
		}
		//while ������ ��������
		if (current == "999") { //���α׷� ������ ���
			imCode.push(8888); // ���߿� ����� �� ���� ���� �ϵ���
			if (parsetrees.top() == "999")
				parsetrees.pop();
		}
	}
}

/* ===================================================================================
* Explain :imCode ������ ���� pop�ϸ鼭 ȭ�鿡 ������ִ� �Լ�
* return : ����
* parameter : ����
*  ===================================================================================
*/
void ToyInterpreter::printResult() {
	int result;
	cout << "--------------------------------------------" << endl;

	while (!imCode.empty()) {
		result = imCode.top();
		imCode.pop();
		if (result != 8888) { //���α׷��� ���� �ƴϸ�
			if (result == 9999) {
				cout << "��� : " << "undefined" << endl;
				continue;
			}
			cout << "��� : " << result << endl;
		}
	}
	cout << "--------------------------------------------" << endl;
}

ToyInterpreter::~ToyInterpreter() {
	delete funclist;
	//delete[] lineRead;

}
