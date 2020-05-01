#include<iostream>
#include<fcntl.h>
#include<string>
#include<stack>
#include<cctype>
using namespace std;

#define MAX_LINES 100

class Lexeme_unit {
public:
	string lexemes[100];
	string token[100];

	//token ���� : integer, var, IF_op, MINUS_op, parentheses

	~Lexeme_unit() {
		delete[] lexemes;
	}
};

class ToyInterpreter {
public:
	stack<string> parsetrees; 
	stack<int> imCode; // �߰��ڵ� (����� ����ϴµ� ���)

	string lineRead[MAX_LINES];
	int lineNum = 0;
	int result[MAX_LINES]; // �� ������ ������� ������ ����

	int fileLoad(char* filename);

	bool isNum(char str); //�� ���� character�� �������� Ȯ��
	bool isLetter(char str); //�� ���� character�� �������� Ȯ��
	bool isInteger(string s); //�� ���ڿ��� ���� �������� Ȯ��
	bool isnegaInt(char str); // ���� Ȯ��
	int stointeger(string s); //���ڿ��� int������ ��ȯ
	void setLine(string s);

	Lexeme_unit* lexicalAnalyzer(); 
	int syntaxAnalyzer(Lexeme_unit* lex);
	void intermediateCode();
	void printResult();

	~ToyInterpreter(); //�Ҹ���

};




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
		result = true ;
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
	char dataRead[50] = { 0, };

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
		fgets(dataRead, 50, file);

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

				if (test[k] == 'M') {
					while (test[k] != ' ') {
						op.append(1, test[k]);
						k++;
					}
					if (op.compare("MINUS") == 0) {
						count++;
						lex[i].lexemes[count] = op;
						op = "";
					}
					else {
						cout << "undefined" << endl;
					}
				}

				else if (test[k] == 'I') {
					while(test[k] !=  ' ') { //�����϶�����
						op.append(1, test[k]);
						k++;
					}
					if (op.compare("IF") == 0) {
						count++;
						lex[i].lexemes[count] = op;
						op = "";
					}
					else {
						cout << "undefined" << endl;
					}
				}

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

				else if (isLetter(test[k])) {
					temp.append(1, test[k]);
					k++;
					while (test[k] != ' ') { //���� ���� ������ ������ ����
						if (isLetter(test[k])) { //���� �͵� �������� Ȯ��
							temp.append(1, test[k]);
							 k++;
						}
						else {
							cout << "���� �Է� ����" << endl;
							break;
						}
					}
					count++;
					lex[i].lexemes[count] = temp;
					temp = "";
				}

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
			
			else if (lex[i].lexemes[k].compare("MINUS") == 0) {
				lex[i].token[k] = "MINUS_op";
				k++;
			}

			else if (lex[i].lexemes[k].compare("IF") == 0) {
				lex[i].token[k] = "IF_op";
				k++;
			}

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

						if (lex[i].token[k] == "MINUS_op" || lex[i].token[k] == "IF_op") {
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
		parsetrees.push("999"); //���α׷��� �� 999�� ǥ��
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
	int calc;

	while(!parsetrees.empty()) {

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
			else if (current == "undefined") {
				imCode.push(9999);
			}
			else if (current != "999"){ //����
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
			if(parsetrees.top() == "999")
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

	//delete[] lineRead;

}

int main(void) {

	
	bool loop = true;
	

	while (loop) {

		ToyInterpreter tInter;
		Lexeme_unit* lexe = new Lexeme_unit;

		int choose;
		char fileName[30] = { 0, };
		int flag = 0;
		

		cout << "============================================" << endl;
		cout << "1. File Load" << endl;
		cout << "2. Interactive Mode" << endl;
		cout << "3. Exit" << endl;
		cout << "============================================" << endl;
		cout << "�޴��� �����ϼ��� >> ";
		cin >> choose;
		cin.ignore(256,'\n');

		if (choose == 1) {

			cout << "���ϸ��� �Է��ϼ��� >> ";
			cin >> fileName;
			cin.ignore(256, '\n');
			tInter.fileLoad(fileName); //fileName
			lexe = tInter.lexicalAnalyzer();
			flag = tInter.syntaxAnalyzer(lexe);
			if (flag != -1) {
				tInter.intermediateCode();
				tInter.printResult();
			}
			cout << endl;
		}
		else if (choose == 2) {
			string inter;
			cout << "������ �Է��ϼ���. " << endl << "--------------------------------------------" << endl;
			getline(cin, inter);
			//cin.ignore(256, '\n');
			tInter.setLine(inter);
			lexe = tInter.lexicalAnalyzer();
			if (lexe == NULL) {
				cout << "�� ������ ���� ���α׷� ����" << endl;
			}
			else {
				flag = tInter.syntaxAnalyzer(lexe);
				if (flag != -1 && flag != NULL) {
					tInter.intermediateCode();
					tInter.printResult();
				}
			}
			cout << endl;
		}
		else if (choose == 3) {
			cout << "���α׷��� �����մϴ�." << endl;
			loop = false;
		}
		else {
			cout << "���� �޴��� �����߽��ϴ�. �ٽ� �������ּ���" << endl;
		}

	}

	return 0;

}