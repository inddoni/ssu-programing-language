#include<iostream>
#include<fcntl.h>
#include<string>
#include<stack>
#include<vector>
#include<queue>
#include<cctype>
using namespace std;

#define MAX_LINES 100

class Function {
public:

	
	string name;
	vector<string> param;
	queue<string> funCalc;

	Function() {
		name = "";
		//cout << "function ��ü ����" << endl;
	}

	Function(string name) {
		this->name = name;
	}

	void setParam(vector<string> pList) {
		for (int i = 0; i < pList.size(); i++) {
			param.push_back(pList[i]);
		}
	}

	void setFunCalc(queue<string> fc) {
		int size = fc.size();
		for (int i = 0; i < size; i++) {
			string value = fc.front();
			funCalc.push(value);			
			fc.pop();
		}
	}
	queue<string> getFunCalc() {
		return funCalc;
	}
	int getParamSize() {
		return param.size();
	}
	string getName() {
		return name;
	}

};

class Lexeme_unit {
public:
	string lexemes[300];
	string token[300];

	//token ���� : integer, var, IF_op, MINUS_op, parentheses

	~Lexeme_unit() {
		delete[] lexemes;
	}
};

class ToyInterpreter {
public:

	static int count; //Toy language ���� �Լ��� ���� 

	stack<string> parsetrees;
	stack<int> imCode; // �߰��ڵ� (����� ����ϴµ� ���)

	string lineRead[MAX_LINES];
	int lineNum = 0;
	int result[MAX_LINES]; // �� ������ ������� ������ ����

	Function* funclist;
	vector<string> funs;


	int fileLoad(char* filename);

	void defineFunc(string defineFun);
	void FunctionList(); // function �迭 ����
	void printDEFUN();
	bool isMyFunc(string s);
	int searchFunc(string name);
	queue<string> convertFunc(string name, int v1);
	queue<string> convertFunc(string name, int v1, int v2);
	queue<string> convertFunc(string name, int v1, int v2, int v3);

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


