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

	//token 종류 : integer, var, IF_op, MINUS_op, parentheses

	~Lexeme_unit() {
		delete[] lexemes;
	}
};

class ToyInterpreter {
public:
	stack<string> parsetrees; 
	stack<int> imCode; // 중간코드 (결과값 계산하는데 사용)

	string lineRead[MAX_LINES];
	int lineNum = 0;
	int result[MAX_LINES]; // 각 라인의 결과값을 가지고 있음

	int fileLoad(char* filename);

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

	if (str == 45) { //'-'이면 true
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
* Explain : File Load 메뉴 실행하여 파일을 읽어오고, 읽어온 파일의 내부 내용(term)을 
*			출력 후 그 term의 결과값까지 출력해주는 함수. (1. File Load 구현)
* return : 에러코드
* parameter : 열고자하는 파일의 이름
*  ====================================================================================
*/
int ToyInterpreter::fileLoad(char* filename) {

	FILE* file;
	file = fopen(filename, "r"); //읽기모드로 열기
	char dataRead[50] = { 0, };

	// 파일명 오류 예외처리
	while (file == NULL) {  
		char refilename[30] = { 0, };
		cout << "파일명을 다시 입력해주세요 >> ";
		cin >> refilename;
		//cin.ignore(256, '\n');
		file = fopen(refilename, "r");
	}

	// 파일 전체를 한 줄씩 읽어와서 lineRead 배열에 담기
	while (!feof(file)) {
		fgets(dataRead, 50, file);

		lineRead[lineNum] = dataRead;
		lineNum++;
	}

	cout << endl << "파일 내용은" << endl;
	cout << "----------------------------------------------" << endl;
	for (int i = 0; i < lineNum; i++) {
		cout << lineRead[i];
	}
	cout << endl << "----------------------------------------------" << endl;
	cout << "입니다." << endl << endl;
	fclose(file);

	return 0;
}
/* ===================================================================================
* Explain : lexical analyzer. 라인을 lexeme 단위로 잘라서 lexeme_unit에 저장
* return : 작성한 lexeme_unit
* parameter :  없음
*  ===================================================================================
*/
Lexeme_unit* ToyInterpreter::lexicalAnalyzer(void) {

	Lexeme_unit* lex;
	lex = new Lexeme_unit[lineNum]; 
	// 이 클래스에 찾아낸 lexeme과 그 갯수를 저장한다.

	
	string temp = "";

	for (int i = 0; i < lineNum; i++) {
		
		string test = lineRead[i]; //하나의 라인을 읽어온다.

		int k = 0; 
		int count = -1; //라인내에 lexeme 개수

		//숫자인지 확인
		if (isNum(test[k])) { 
			
			temp.append(1, test[k]);
			k++;

			while (test[k] != '\n' && test[k] != NULL) {
				if (isNum(test[k])) {
					temp.append(1, test[k]);
					k++;
				}
				else {
					cout << "[error] 정수만 입력이 가능합니다." << endl;
					return NULL;
				}
			}
			count = 0;
		}

		//음수인지 확인
		else if (isnegaInt(test[k]) && test[k] != NULL) {
			temp.append(1, test[k]);
			k++;
			if (isnegaInt(test[k]))
				cout << "'-'를 너무 많이 입력했습니다." << endl;

			while (test[k] != '\n' && !isnegaInt(test[k])) {
				if (isNum(test[k])) {
					temp.append(1, test[k]);
					k++;
				}
				else {
					cout << "[error] 음수 입력 오류" << endl;
				}
			}
			count = 0;
		}

		//문자인지 확인 (문자면 변수!)
		else if (isLetter(test[k])) { 
			
			temp.append(1, test[k]);
			k++; 

			while (test[k] != '\n' && test[k] != NULL) {
				if (isLetter(test[k])) {
					temp.append(1, test[k]);
					k++;
				}
				else {
					cout << "[error] 문자 입력 오류 (알파벳과 '-'만 입력이 가능합니다.)" << endl;
					return NULL;
				}
			}
			count = 0;
		}
			
		//괄호열기 문자인지 확인
		else if (test[k] == 40) {
			count++;
			lex[i].lexemes[count].append(1, test[k]); // '(' 저장
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
					while(test[k] !=  ' ') { //공백일때까지
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
					while (test[k] != ' ') { //공백 문자 나오기 전까지 저장
						if (isNum(test[k])) { //다음 것도 숫자인지 확인
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

				//음수인지 확인
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
						cout << "-가 여러번 입력되었습니다." << endl;
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
					while (test[k] != ' ') { //공백 문자 나오기 전까지 저장
						if (isLetter(test[k])) { //다음 것도 숫자인지 확인
							temp.append(1, test[k]);
							 k++;
						}
						else {
							cout << "문자 입력 에러" << endl;
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

				else if (test[k] == ' ') { // 공백은 넘기기
					k++;
					continue;
				}

				else {
					cout << "연산자 구문 syntax error" << endl;
				}
			}

			continue;
		}

		else if (test[i] == ' ') {
			k++;
			continue;
		}

		else {
			cout << "lexeme 인식 불가" << endl;
		}

		lex[i].lexemes[count] = temp;
		temp = ""; //temp에 저장한 lexeme 넣으면 초기화
				
	}

	return lex;
}
/* ===================================================================================
* Explain : syntax analyzer. lexeme_unit에 저장된 lexeme들의 token을 분류하는 함수
            숫자, 변수, 연산자(IF, MINUS), 괄호로 분류된다. lex가 정확한 값인지 판단
			parse tree 생성 (postorder)
* return : 에러코드
* parameter : lexeme_unit (lexeme을 저장한 객체)
*  ===================================================================================
*/
int ToyInterpreter::syntaxAnalyzer(Lexeme_unit* lex) {

	int iserror = 1;

	// 1. token 파악
	for (int i = 0; i < lineNum; i++) {
		int k = 0;
		while (lex[i].lexemes[k] != "") { //해당 라인의 lexeme 들을 돈다.
			
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


	// 2. parse trees 스택에 넣어주기 (postorder)
	// 하나의 스텍에 파일 전체내용을 넣어주고 라인 구분은 숫자 99로 한다.
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
						pCount++; //열린 괄호 count
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
							cout << "연산자를 잘못입력하셨습니다." << endl;
							iserror = -1;
						}
						
					}

					else if (lex[i].token[k] == "integer") { //연산의 두번째 integer은 여기서 저장하게 된다.
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
					cout << "')'가 없습니다." << endl;
					return NULL;
				}
				else if (pCount < 0) {
					cout << "'('가 없습니다." << endl;
					return NULL;
				}
				
			}
			else {
				cout << "parsetrees push error" << endl;
				k++;
				iserror = -1;
			}
		}
		parsetrees.push("999"); //프로그램의 끝 999로 표시
	}

	return iserror;
}

/* ===================================================================================
* Explain : syntax analyzer에서 생성한 parse tree 스택에서 값을 하나씩 꺼내서 
            결과값을 계산하여 저장한다. 9999면 undefined를 출력하고 싶을 경우 
* return : 없음
* parameter : 없음
*  ===================================================================================
*/
void ToyInterpreter::intermediateCode() {
	string current;
	int num; //string to int 후 저장할 변수
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
					cout << "중간코드 계산불가 : 식에 var가 있음" << endl;

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
					cout << "중간코드 계산불가 : 식에 var가 있음" << endl;
			}
			else if (current == "undefined") {
				imCode.push(9999);
			}
			else if (current != "999"){ //숫자
				num = stointeger(current);
				imCode.push(num);
			}
			else {
				//error
			}
		}
		//while 밖으로 빠져나옴
		if (current == "999") { //프로그램 끝인지 물어봄
			imCode.push(8888); // 나중에 출력할 때 라인 구분 하도록
			if(parsetrees.top() == "999")
				parsetrees.pop();
		}
	}
}

/* ===================================================================================
* Explain :imCode 스택의 값을 pop하면서 화면에 출력해주는 함수
* return : 없음
* parameter : 없음
*  ===================================================================================
*/
void ToyInterpreter::printResult() {
	int result;
	cout << "--------------------------------------------" << endl;

	while (!imCode.empty()) {
		result = imCode.top();
		imCode.pop();
		if (result != 8888) { //프로그램의 끝이 아니면
			if (result == 9999) {
				cout << "결과 : " << "undefined" << endl;
				continue;
			}
			cout << "결과 : " << result << endl;
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
		cout << "메뉴를 선택하세요 >> ";
		cin >> choose;
		cin.ignore(256,'\n');

		if (choose == 1) {

			cout << "파일명을 입력하세요 >> ";
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
			cout << "문장을 입력하세요. " << endl << "--------------------------------------------" << endl;
			getline(cin, inter);
			//cin.ignore(256, '\n');
			tInter.setLine(inter);
			lexe = tInter.lexicalAnalyzer();
			if (lexe == NULL) {
				cout << "위 에러로 인한 프로그램 종료" << endl;
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
			cout << "프로그램을 종료합니다." << endl;
			loop = false;
		}
		else {
			cout << "없는 메뉴를 선택했습니다. 다시 선택해주세요" << endl;
		}

	}

	return 0;

}