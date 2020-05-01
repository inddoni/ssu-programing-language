/*
 * main.cpp
 *
 *  Created on: 2020. 4. 9.
 *      Author: InJeong
 */

#include <iostream>
#include <cstdlib>

using namespace std;

class PickingStone{

public:
	int numofstone; //처음 시작할때 전체 개수
	int takenstones; //처음 시작할때 가지고간 개수
	bool Player; // true == maxPlayer, false == minPlayer
	int lastValue;
	int currTotal;
	int* stonelist;


	void startValue(int numofstone, int takenstones, int* list);
	bool isPrime(int value); //소수인지 아닌지 체크
	int multiple(int value); //배수의 개수 체크
	int prime(int value); //소수의 개수 체크
	int* candidate(int value, int num); //값의 배수와 약수를 배열로 반환한다.
	double evaluation(int value);
	int bestMove();
	void calculate();
	void printEvaluation();
	~PickingStone();

};

void PickingStone::startValue(int numofstone, int takenstones, int* list){

	this->numofstone = numofstone;
	this->takenstones = takenstones;
	currTotal = numofstone;

	cout << "Game Start" << endl;

	stonelist = new int[currTotal+1];
	for (int i = 1; i <= currTotal; i++){ //n만큼의 stone list 채우기
		stonelist[i] = i;
	}

	//어떤 플레이어 차례인지 확인
	if(takenstones >= 0){
		if(takenstones == 0){

			Player = true;
		}
		else if(takenstones % 2 == 1)
			Player = false;
		else
			Player = true;
	}
	else{
		cout << "taken stone : 0 이상의 숫자를 입력해야합니다." << endl;
	}

	if(takenstones > 0){
		for (int i = 0; i < takenstones; i++){ //잡은 돌은 stone list에서 0으로 처리
			int pickValue = list[i];
			stonelist[pickValue] = 0;
			currTotal--;
		}

		lastValue = list[takenstones-1];
		cout << "last stone : " << lastValue << endl;
		cout << "current total stone : " << currTotal << endl << endl;
	}



}
//소수인지 아닌지 판별하는 함수
bool PickingStone::isPrime(int value){
	bool result = true;

	if(value < 0)
		return false;

	if(value == 1)
		result = true;
	else {
		for(int i = 2; i <= value; i++){
			if(value % i == 0){
				result = false;
				break;
			}
			else
				result = true;
		}
	}
	return result;
}

// value의 배수가 몇개있는지 확인하는 함수
int PickingStone::multiple(int value){
	int result = 0;
	int i = 2;
	int mult = value*i;

	while(mult < numofstone){
//		if(stonelist[mult] != 0){

		result++;
		i++;
		mult = value*i;

//		}
	}

	return result;
}

// value의 소수가 몇개있는지 확인하는 함수
int PickingStone::prime(int value){ //3
	int result = 0;

	for(int i = 1; i <= value; i++){ //8 1
		if(value % i == 0){
			//소수체크에 true가 나오면 개수세기
			result++;
		}

	}

	return result;
}


int* PickingStone::candidate(int value, int num){

	// 1 , 나의 배수, 나의 약수 list


	int* returnArray = new int[num];
//	cout << value << "의 약수와 배수 총 개수 : " << num << endl;

	int count = 0;
	for(int i = 1; i <= value; i++){ //소수 구하기
		if(value % i == 0){
			//소수체크에 true가 나오면 개수세기
			returnArray[count] = i;
			count++;
		}

	}

	int j = 2;
	int mult = value * j;
	while(mult < numofstone){
		returnArray[count] = mult;
		count++;
		j++;
		mult = value * j;
	}

//	cout << value << "의 후보 돌 출력  : " ;

//	for(int i = 0; i < num; i++){
////		cout << returnArray[i] << " " ;
//	}
//	cout << endl;

	return returnArray;
}

double PickingStone::evaluation(int value){



	lastValue = value;


	double result = 0.0;

	if(stonelist[1] == 1){ //1번 돌을 가지고 가지 않은 경우
		result = 0.0;
	}
	else if(lastValue == 1){
		if(currTotal % 2 == 1)
			result += 0.5;
		else
			result -= 0.5;
	}
	else if(isPrime(lastValue)){ //소수이면
		if(multiple(lastValue) % 2 == 1)
			result += 0.7;
		else
			result -= 0.7;
	}
	else{
		if(prime(lastValue) % 2 == 1)
			result += 0.6;
		else
			result -= 0.6;
	}


	if(Player) {//MaxPlayer Turn
		return result;
	}
	else { //minPlayer turn
		result -= (result * 2.0);
		return result;
	}


}

int PickingStone::bestMove(){
	// 내가선택할수 있는 값 loop
	//내가 선택할수 있는 값 = 1, 나의 배수, 나의 약수

	int num = multiple(lastValue) + prime(lastValue);
	int* candidateList = candidate(lastValue, num);
	int test = num;

	for(int i = 0; i < num; i++){
		int testValue = candidateList[i];
		if(stonelist[testValue] == 0){
			candidateList[i] = 0;
			test --;
		}
	}

	double* bestValue = new double[test];
	// last value가 선택할수 있는 모든 후보 중에 이미 잡은 돌은 빼고 평가함수를 돌릴 배열을 만듬
	int j = 0;
	for(int i = 0; i < num; i++){
		if(candidateList[i] != 0){
			bestValue[j] = candidateList[i];
//			cout << "bv" << bestValue[j] << endl;
			j++;
		}
	}

	double maxValue = 0.0;
	double minValue = 0.0;
	double resultValue;
	int maxindex = 0;
	int minindex = 0;

	for (int i = 0; i < test; i++){
		resultValue = evaluation(bestValue[i]);
//		cout << "bestValue = " << bestValue[i] << " - 평가값 : " << resultValue << endl;
		if(resultValue >= maxValue){
			maxValue = resultValue;
			maxindex = bestValue[i];
//			cout<<"max : " << maxindex << endl;
		}
		else if(resultValue <= minValue){
			minValue = resultValue;
			minindex = bestValue[i];
//			cout<<"min : "<< minindex << endl;
		}
		else
			continue;
	}

	if (Player && maxindex == 0 && minindex != 0){
		maxindex = minindex;
	}


	//max player 인지 min player인지 보고 값 선택하기
	if(Player){ //MaxPlayer Turn
		if(maxindex != 0) {
			cout << "Maxplayer - bestValue : " << maxindex << "번 돌을 선택하세요!" << endl;
			Player = false;
			return maxindex;
		}
		else if(maxindex == 0){
//			cout << "max 0번" << endl;
			Player = false;
			return 0;
		}
		else{
			cout << "Maxplayer - bestValue : " << minindex << "번 돌을 선택하세요!" << endl;
			Player = false;
			return minindex;
		}
	}
	else { //minPlayer turn
		if(minindex != 0){
			cout << "Minplayer - bestValue : " << minindex << "번 돌을 선택하세요!" << endl;
			Player = true;
			return minindex;
		}
		else if(maxindex == 0){
//			cout << "min 0번" << endl;
			Player = true;
			return 0;
		}
		else{
			cout << "Minplayer - bestValue : " << maxindex << "번 돌을 선택하세요!" << endl;
			Player = true;
			return maxindex;
		}

	}

	delete[] candidateList;
	delete[] bestValue;


}

void PickingStone::calculate(){
	//더이상 선택할 수 없을때까지 best move를 돌리고 (후보 돌이 다 0이면 선택할 수 없는 상황)

	int num = multiple(lastValue) + prime(lastValue); //4
	int* array = candidate(lastValue, num); //6
	int count = 0;

	for(int i = 0; i < num; i++){
		int arrayIndex = array[i];
		if(stonelist[arrayIndex] == 0){
			count++;
		}
	}

	int result;
	int depth = 0;
	int nodes = 0;
	double calc = 0.0;

	while(true){
		if(count == num){
			cout << "더 이상 계산할 값이 없습니다. 1" << endl;
			if(Player) //다음플레이어가 맥스면 현재플레이어는 민이다.
				calc = -1.0;
			else
				calc = 1.0;

			cout.precision(5);
			cout << "Calculated Value : " << calc << endl;
			cout << "Number of Visited Nodes : " << num << endl;
			cout << "MaxDepth : " << currTotal << endl;
			break;
		}
		else{
			result = bestMove();
//			cout << "result : " << result <<endl;
			lastValue = result;
//			cout << "lastValue : " << lastValue << endl;
			stonelist[result] = 0;
			currTotal++;
			nodes++;

			if(result == 0){
				cout << endl << "더 이상 계산할 값이 없습니다." << endl << endl;
				if(Player)
					calc = 1.0;
				else
					calc = -1.0;

				cout.precision(5);
				cout << "Calculated Value : " << calc << endl;
				cout << "Number of Visited Nodes : " << nodes << endl;
				cout << "MaxDepth : " << depth << endl;
				break;
			}


			depth++;

		}
	}


}

void PickingStone::printEvaluation(){

	cout << "pinrtEvaluation" << endl;

	int* printEval = new int[numofstone+1];
	for(int i = 1; i < numofstone; i++){
		if(stonelist[i] == 0){
			printEval[i] = -111;
		}
		else{
			printEval[i] = evaluation(stonelist[i]);
		}
		cout << i << "\t";
	}

	cout << "다음돌에 따른 평가값 출력" <<endl;

	for(int i = 1; i < numofstone; i++){
		cout << printEval << " ";
	}

	cout<<endl;
	delete[] printEval;

}


PickingStone::~PickingStone(){
	delete[] stonelist;
}

int main(void){

	// play <num_of stones> <num_of_taken_stones> <list_of_taken_stones>
	int total = 0;
	int taken = 0;
	int* list;

	cout << "play " << endl;
	cin >> total;
	cin >> taken;
	list = new int[taken];

	for(int i = 0; i < taken; i++)
		cin >> list[i];

	PickingStone ps;

	if(taken == 0){
		int mid = total / 2;

		int randNum = (rand() % mid) + 1;
		if (randNum % 2 == 0)
			randNum--;
		list[0] = randNum;

		ps.startValue(total, 1, list);

	}
	else {
		ps.startValue(total, taken, list);
	}

	ps.calculate();


	delete[] list;

	return 0;
}

