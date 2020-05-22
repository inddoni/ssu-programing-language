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
		//cout << "function 객체 생성" << endl;
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

	//token 종류 : integer, var, IF_op, MINUS_op, parentheses

	~Lexeme_unit() {
		delete[] lexemes;
	}
};

class ToyInterpreter {
public:

	static int count; //Toy language 내의 함수의 개수 

	stack<string> parsetrees;
	stack<int> imCode; // 중간코드 (결과값 계산하는데 사용)

	string lineRead[MAX_LINES];
	int lineNum = 0;
	int result[MAX_LINES]; // 각 라인의 결과값을 가지고 있음

	Function* funclist;
	vector<string> funs;


	int fileLoad(char* filename);

	void defineFunc(string defineFun);
	void FunctionList(); // function 배열 선언
	void printDEFUN();
	bool isMyFunc(string s);
	int searchFunc(string name);
	queue<string> convertFunc(string name, int v1);
	queue<string> convertFunc(string name, int v1, int v2);
	queue<string> convertFunc(string name, int v1, int v2, int v3);

	bool isNum(char str); //한 개의 character가 숫자인지 확인
	bool isLetter(char str); //한 개의 character가 문자인지 확인
	bool isInteger(string s); //한 문자열이 전부 숫자인지 확인
	bool isnegaInt(char str); // 음수 확인
	int stointeger(string s); //문자열을 int형으로 변환
	void setLine(string s);

	Lexeme_unit* lexicalAnalyzer();
	int syntaxAnalyzer(Lexeme_unit* lex);
	void intermediateCode();
	void printResult();

	~ToyInterpreter(); //소멸자

};


