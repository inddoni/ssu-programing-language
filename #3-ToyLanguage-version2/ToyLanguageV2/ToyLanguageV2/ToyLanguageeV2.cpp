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

	if (str == 45) { //'-'이면 true
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
* Explain : File Load 메뉴 실행하여 파일을 읽어오고, 읽어온 파일의 내부 내용(term)을
*			출력 후 그 term의 결과값까지 출력해주는 함수. (1. File Load 구현)
* return : 에러코드
* parameter : 열고자하는 파일의 이름
*  ====================================================================================
*/
int ToyInterpreter::fileLoad(char* filename) {

	FILE* file;
	file = fopen(filename, "r"); //읽기모드로 열기
	char dataRead[120] = { 0, };

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
		fgets(dataRead, 120, file);

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


//Function class의 정적 멤버변수 초기화
int ToyInterpreter::count = 2; //IF랑 MINUS들어간 상태니까 2로 초기화 해준다.

/* ===================================================================================
* Explain : 새로운 함수를 정의하는 함수. Defun.txt파일에 해당 함수가 있는지 확인 후
			없으면 새로운 함수를 포함한다.
* return : (정의하는 함수가 이미 있거나, 매개변수가 하나도 없으면 에러)
* parameter :  정의한 함수 전체 (함수이름, parameter, 해당연산 포함한 문자열)
*  ===================================================================================
*/
void ToyInterpreter::FunctionList() {
	funclist = new Function[15];
	//처음에 IF랑 MINUS는 미리 선언
	//MINUS와 IF는 파라메터, 내용 없이 이름만 넣어준다.
	funclist[0].name = "MINUS";
	funclist[1].name = "IF";
}

/* ===================================================================================
* Explain : 새로운 함수를 정의하는 함수. Defun.txt파일에 해당 함수가 있는지 확인 후
            없으면 새로운 함수를 포함한다.
* return : (정의하는 함수가 이미 있거나, 매개변수가 하나도 없으면 에러)
* parameter :  정의한 함수 전체 (함수이름, parameter, 해당연산 포함한 문자열)
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
		//토큰 저장
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
							//parmeter 없으면 에러내기
							if (p.size() == 0) {
								error = 0;
								cout << "함수에 parameter가 존재하지 않습니다." << endl;
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
						//parmeter 없으면 에러내기
						if (p.size() == 0 && temp != "x") {
							error = 0;
							cout << "함수에 parameter가 존재하지 않습니다." << endl;
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
			//토큰 각 항목에 나눠 저장
			if (cnt == 0 && error != 0) {
				//에러체크 : 동일한 이름의 함수가 있는지 확인
				int k = 0;
				while (funclist[k].getName() != "") {
					if (temp == funclist[k].getName()) {
						cout << temp << " 함수는 이미 정의되어있습니다." << endl;
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
		//마지막 토큰 출력
		//else if (i+1 == newFunStr.size()) {
		//	temp += newFunStr[i];
		//}

	}
	
	if (error != 0) {
		// 마지막토큰추가
		qContent.push(temp);

		funclist[count].setParam(p);
		funclist[count].setFunCalc(qContent);
		count++;

		ofstream output("defun.txt", ios::app);
		output << newFunStr.substr(6) << endl;
		output.close();

		cout << "새로운 함수 추가 완료 - " << funclist[count - 1].getName() << endl;
	}
	else {
		funclist[count].name = "";
	}
	

}
void ToyInterpreter::printDEFUN() {
	FILE* file = fopen("defun.txt", "r"); //읽기모드로 열기
	char dataRead[100] = { 0, };
	string printStr[20];
	int idx = 0;
	
	// 파일 전체를 한 줄씩 읽어와서 printStr 배열에 담기
	while (!feof(file)) {
		fgets(dataRead, 100, file);

		printStr[idx] = dataRead;
		idx++;
	}
	//cout << endl << "파일 내용은" << endl;
	//cout << "----------------------------------------------" << endl;
	for (int i = 0; i < idx-1; i++) {
		cout << printStr[i];
	}
	//cout << endl << "----------------------------------------------" << endl;
	//cout << "입니다." << endl << endl;
	fclose(file);
}

//찾는 함수의 인덱스 반환 (없으면 -1 반환)
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

	//함수이름에 맞는(인덱스에 맞는) 큐를 가져옴
	int idx = searchFunc(name);
	queue<string> funC = funclist[idx].getFunCalc();

	//큐를 하나씩 팝하면서 x = v1 대입해서
	//새로운 큐에 저장
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

	//함수이름에 맞는(인덱스에 맞는) 큐를 가져옴
	int idx = searchFunc(name);
	queue<string> funC = funclist[idx].getFunCalc();

	//큐를 하나씩 팝하면서 x = v1, y = v2 대입해서
	//새로운 큐에 저장
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

	//함수이름에 맞는(인덱스에 맞는) 큐를 가져옴
	int idx = searchFunc(name);
	queue<string> funC = funclist[idx].getFunCalc();

	//큐를 하나씩 팝하면서 x = v1, y = v2 대입해서
	//새로운 큐에 저장
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
					return NULL;
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
				//괄호 열기 다음에 문자가 나오면 끝까지 문자를 붙여서 만들고
				//함수명안에 있는지 확인한다.

				if (isLetter(test[k]) && test[k] != '-') {
					int ercode = -1;
					while (test[k] != ' ') {
						if (isLetter(test[k])) {
							op.append(1, test[k]);
							k++;
						}
						else {
							cout << "문자입력에러" << endl;
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
						//문자에 숫자가 섞인 것도 아니고, 함수명도 아니면 undefined 오류
						cout << "undefined" << endl;
					
					}
				}

				//else if (test[k] == 'I') {
				//	while (test[k] != ' ') { //공백일때까지
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

				//else if (isLetter(test[k])) {
				//	temp.append(1, test[k]);
				//	k++;
				//	while (test[k] != ' ') { //공백 문자 나오기 전까지 저장
				//		if (isLetter(test[k])) { //다음 것도 문자인지 확인
				//			temp.append(1, test[k]);
				//			k++;
				//		}
				//		else {
				//			cout << "문자 입력 에러" << endl;
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
		parsetrees.push("999"); //프로그램의 끝(해당 라인의 끝) 999로 표시
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
			//IF와 MINUS가 아닌 함수명이면
			else if (isMyFunc(current)) {
				//prameter 사이즈 보고 결정
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
			else if (current != "999") { //숫자
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
			if (parsetrees.top() == "999")
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
	delete funclist;
	//delete[] lineRead;

}
