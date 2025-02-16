
#include "H02_polyAdd.h"
extern int allocSize;	   // size of the allocated memory for each test
extern int allocSizeMax;  // max allocated mem size among those of all tests

void readPolyAB(int* pnA, polyPtr* pA, int* pnB, polyPtr* pB) {
	if (scanf("%d", pnA) == EOF)		//다항식 A nonzero term 개수 받고 오류가 있다면 종료
		WRONG_INPUT	
	if (*pnA != NULL) {  //nonzero term이 0이 아닐 때
		MALLOC(*pA, polyPtr, (*pnA)*sizeof(poly)); //A를 가리키는 포인터 동적할당
		for (int i = 0; i < *pnA; i++) {
			if (scanf("%d %d", &(((*pA) + i)->coef), &(((*pA) + i)->expon))==EOF) //coef와 expon 입력 받고 오류가 있다면 종료
				WRONG_INPUT
		}
	}
	else
		pA = NULL; //nonzero term이 0일 때 zero polynomial 만들기

	if (scanf("%d", pnB) == EOF)	//다항식 B에 대한 함수로 위와 동일	
		WRONG_INPUT	
	if (*pnB != NULL) {
		MALLOC(*pB, polyPtr, (*pnB)*sizeof(poly));
		for (int m = 0; m < *pnB; m++) {
			if (scanf("%d %d", &(((*pB) + m)->coef), &(((*pB) + m)->expon))==EOF)
				WRONG_INPUT
		}
	}
	else
		pB = NULL;
		
	return;
}

int allocPolyD(int nA, polyPtr A, int nB, polyPtr B, polyPtr* pD) { //상수항 고려
	int nD = 0;
	int LEA = -2, LEB = -2; //차수
	int Amax=-1, Bmax=-1; //중복 방지를 위한 이전 loop 차수 저장
	int a = 0, b = 0; //index 저장
	int num = 0; //사용한 총 항의 개수

	if ((nA != 0) && (nB != 0)) {

		do {
			if (LEA < Amax) { //이전 LOOP에서 LEA에 저장된 차수가 D term 수에 반영이 됐을 때 (LEA>=LEB였을 시)
				for (int p = 0; p < nA; p++) { //term 수만큼 반복
					if (((A + p)->expon) >= LEA) { //LEA에 저장된 값(현재의 최고차항)보다 큰지 확인
						if (Amax < 0) //루프 첫 실행 시
						{
							LEA = (A + p)->expon; //최고차항의 차수 저장
							a = p; //최고차항일 때 INDEX 저장
							Amax = LEA; //이번 최고차항이 다음 LOOP에서 또 최고차항으로 선정되지 않기 위해 저장
						}
						else if (((A + p)->expon) < Amax) { //이전 LOOP의 차수보다 작을 때 (중복 방지)
							LEA = (A + p)->expon; //차수 저장
							a = p; //INDEX 저장
							Amax = LEA; //이번 차수가 다음에도 선정되지 않기 위해 저장 
						}
					}
				}
			}

			if (LEB < Bmax) {  //B에 관한 함수로 위와 동일
				for (int q = 0; q < nB; q++) {
					if (((B + q)->expon) >= LEB) {
						if (Bmax < 0) {
							LEB = (B + q)->expon;
							b = q;
							Bmax = LEB;
						}
						else if (((B + q)->expon) < Bmax) {
							LEB = (B + q)->expon;
							b = q;
							Bmax = LEB;
						}
					}
				}
			}

			switch COMPARE(LEA, LEB) { //차수 비교
			case -1 : //B의 차수가 더 클 때
				LEB = 0; //해당 차수값을 사용했음을 알리기 위해 리셋
				nD++; //D term 수 증가
				num++; //사용한 항 증가
				break;
			case 0 : //같을 때
				if (((A + a)->coef) + ((B + b)->coef) != 0) //계수의 합이 0이 아닐 때
					nD++; //D term 수 증가
				LEA = 0; 
				LEB = 0;
				num++; //A 사용한 항 계산
				num++; // B 사용한 항 계산
				break;
			case 1: //A의 차수가 더 클 때
				LEA = 0;
				nD++;
				num++;
				break;
			}
		} while (num<(nA+nB)); //사용한 총 항의 개수가 (A 항의 개수 + B 항의 개수) 이상이 될 때까지 반복
		MALLOC(*pD, polyPtr, nD * sizeof(poly)) 
	}
	else if ((nA == 0) && (nB != 0)) { //A가 zero polynomial일 때
		MALLOC(*pD, polyPtr, nB * sizeof(poly)); //B만큼 memory alloc
		nD = nB; //B term 수 복사
	}
	else if ((nA != 0) && (nB == 0)) { //B가 zero polynomial일 때
		MALLOC(*pD, polyPtr, nA * sizeof(poly));
		nD = nA;
	}
	else { //둘 다 zero polynomial일 때
		*pD = NULL; //D도 zero polynomial
		nD = 0; //term 수=0
	}

	allocSize += nA * sizeof(poly); //dynamic allocation한 메모리 크기 저장
	allocSize += nB * sizeof(poly);
	allocSize += nD * sizeof(poly);

	return nD;
}

void addPoly(int nA, polyPtr A, int nB, polyPtr B, int nD, polyPtr D) {
	int LEA2 = -2, LEB2 = -2; //차수
	int Amax2 = -1, Bmax2 = -1; //이전 LOOP에 쓰인 차수
	int count = 0; 
	int an=0, bn=0; //INDEX 저장
	int num2 = 0; //사용한 항의 개수

	if ((nA != 0) && (nB != 0)) { //둘 다 zero polynomial이 아닐 때

		do {
			if (LEA2 < Amax2) { //이전 loop에서 LEA2>=LEB2였을 시
				for (int u = 0; u < nA; u++) { 
					if (an == nA - 1) //A의 항을 모두 다 사용하였을 때
						LEA2 = -1; //차수 비교 시  이미 사용된 차수 사용 방지
					if (((A + u)->expon) >= LEA2) { //현재 저장된 차수보다 클 때
						if (Amax2 < 0) { //첫 실행 시
							LEA2 = (A + u)->expon; //최고차수 저장
							an = u; //INDEX 저장
							Amax2 = LEA2; //현재 차수 저장
						}
						else if (((A + u)->expon) < Amax2) //이미 사용된 차수와 중복 방지
						{
							LEA2 = (A + u)->expon; //차수 저장
							an = u; //INDEX 저장
							Amax2 = LEA2; //현재 차수 저장
						}
					}
				}
			}

			if (LEB2 < Bmax2) { //위와 동일
				for (int r = 0; r < nB; r++) {
					if (bn == nB - 1)
						LEB2 = -1;
					if (((B + r)->expon) >= LEB2) {
						if (Bmax2 < 0) {
							LEB2 = (B + r)->expon;
							bn = r;
							Bmax2 = LEB2;
						}
						else if (((B + r)->expon) < Bmax2) {
							LEB2 = (B + r)->expon;
							bn = r;
							Bmax2 = LEB2;
						}
					}
				}
			}

			switch COMPARE(LEA2, LEB2) { //차수 비교
			case -1 : //B가 더 클 때
				(D + count)->coef = (B + bn)->coef; //계수 저장
				(D + count)->expon = LEB2; //차수 저장
				LEB2 = 0; //해당 차수는 사용했음을 알림
				num2++; //사용한 항 개수 증가
				count++; //D의 INDEX 증가
				break;
			case 0 : //같을 때
				if (((A + an)->coef) + ((B + bn)->coef) != 0){ //계수의 합이 0이 아닐 시
					(D + count)->coef = ((A + an)->coef) + ((B + bn)->coef);
					(D + count)->expon = LEA2;
					count++;
				}
				LEA2 = 0, LEB2 = 0;
				num2+= 2;
				break;
			case 1 : //A가 더 클 때
				(D + count)->coef = (A + an)->coef;
				(D + count)->expon = LEA2;
				LEA2 = 0;
				num2++;
				count++;
				break;
			}
			
		} while (num2<(nA+nB)); //사용한 항의 개수가 전체 항의 개수 이상이 될 때까지 반복
	}
	else if ((nA == 0) && (nB != 0)) { //A가 ZERO POLYNOMIAL일 때
		for (int z = 0; z < nB; z++) {//B term 수만큼 반복
			(D + z)->coef = (B + z)->coef; //B 계수 저장
			(D + z)->expon = (B + z)->expon; //B 차수 저장
		}
	}
		
	else if ((nA != 0) && (nB == 0)) { //위와 동일
		for (int y = 0; y < nA; y++) {
			(D + y)->coef = (A + y)->coef;
			(D + y)->expon = (A + y)->expon;
		}
	}
	else  // 둘 다 ZERO POLYNOMIAL일 때
		D = NULL; //D도 ZERO POLYNOMIAL


	return;
}



