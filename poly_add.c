
#include "H02_polyAdd.h"
extern int allocSize;	   // size of the allocated memory for each test
extern int allocSizeMax;  // max allocated mem size among those of all tests

void readPolyAB(int* pnA, polyPtr* pA, int* pnB, polyPtr* pB) {
	if (scanf("%d", pnA) == EOF)		//���׽� A nonzero term ���� �ް� ������ �ִٸ� ����
		WRONG_INPUT	
	if (*pnA != NULL) {  //nonzero term�� 0�� �ƴ� ��
		MALLOC(*pA, polyPtr, (*pnA)*sizeof(poly)); //A�� ����Ű�� ������ �����Ҵ�
		for (int i = 0; i < *pnA; i++) {
			if (scanf("%d %d", &(((*pA) + i)->coef), &(((*pA) + i)->expon))==EOF) //coef�� expon �Է� �ް� ������ �ִٸ� ����
				WRONG_INPUT
		}
	}
	else
		pA = NULL; //nonzero term�� 0�� �� zero polynomial �����

	if (scanf("%d", pnB) == EOF)	//���׽� B�� ���� �Լ��� ���� ����	
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

int allocPolyD(int nA, polyPtr A, int nB, polyPtr B, polyPtr* pD) { //����� ���
	int nD = 0;
	int LEA = -2, LEB = -2; //����
	int Amax=-1, Bmax=-1; //�ߺ� ������ ���� ���� loop ���� ����
	int a = 0, b = 0; //index ����
	int num = 0; //����� �� ���� ����

	if ((nA != 0) && (nB != 0)) {

		do {
			if (LEA < Amax) { //���� LOOP���� LEA�� ����� ������ D term ���� �ݿ��� ���� �� (LEA>=LEB���� ��)
				for (int p = 0; p < nA; p++) { //term ����ŭ �ݺ�
					if (((A + p)->expon) >= LEA) { //LEA�� ����� ��(������ �ְ�����)���� ū�� Ȯ��
						if (Amax < 0) //���� ù ���� ��
						{
							LEA = (A + p)->expon; //�ְ������� ���� ����
							a = p; //�ְ������� �� INDEX ����
							Amax = LEA; //�̹� �ְ������� ���� LOOP���� �� �ְ��������� �������� �ʱ� ���� ����
						}
						else if (((A + p)->expon) < Amax) { //���� LOOP�� �������� ���� �� (�ߺ� ����)
							LEA = (A + p)->expon; //���� ����
							a = p; //INDEX ����
							Amax = LEA; //�̹� ������ �������� �������� �ʱ� ���� ���� 
						}
					}
				}
			}

			if (LEB < Bmax) {  //B�� ���� �Լ��� ���� ����
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

			switch COMPARE(LEA, LEB) { //���� ��
			case -1 : //B�� ������ �� Ŭ ��
				LEB = 0; //�ش� �������� ��������� �˸��� ���� ����
				nD++; //D term �� ����
				num++; //����� �� ����
				break;
			case 0 : //���� ��
				if (((A + a)->coef) + ((B + b)->coef) != 0) //����� ���� 0�� �ƴ� ��
					nD++; //D term �� ����
				LEA = 0; 
				LEB = 0;
				num++; //A ����� �� ���
				num++; // B ����� �� ���
				break;
			case 1: //A�� ������ �� Ŭ ��
				LEA = 0;
				nD++;
				num++;
				break;
			}
		} while (num<(nA+nB)); //����� �� ���� ������ (A ���� ���� + B ���� ����) �̻��� �� ������ �ݺ�
		MALLOC(*pD, polyPtr, nD * sizeof(poly)) 
	}
	else if ((nA == 0) && (nB != 0)) { //A�� zero polynomial�� ��
		MALLOC(*pD, polyPtr, nB * sizeof(poly)); //B��ŭ memory alloc
		nD = nB; //B term �� ����
	}
	else if ((nA != 0) && (nB == 0)) { //B�� zero polynomial�� ��
		MALLOC(*pD, polyPtr, nA * sizeof(poly));
		nD = nA;
	}
	else { //�� �� zero polynomial�� ��
		*pD = NULL; //D�� zero polynomial
		nD = 0; //term ��=0
	}

	allocSize += nA * sizeof(poly); //dynamic allocation�� �޸� ũ�� ����
	allocSize += nB * sizeof(poly);
	allocSize += nD * sizeof(poly);

	return nD;
}

void addPoly(int nA, polyPtr A, int nB, polyPtr B, int nD, polyPtr D) {
	int LEA2 = -2, LEB2 = -2; //����
	int Amax2 = -1, Bmax2 = -1; //���� LOOP�� ���� ����
	int count = 0; 
	int an=0, bn=0; //INDEX ����
	int num2 = 0; //����� ���� ����

	if ((nA != 0) && (nB != 0)) { //�� �� zero polynomial�� �ƴ� ��

		do {
			if (LEA2 < Amax2) { //���� loop���� LEA2>=LEB2���� ��
				for (int u = 0; u < nA; u++) { 
					if (an == nA - 1) //A�� ���� ��� �� ����Ͽ��� ��
						LEA2 = -1; //���� �� ��  �̹� ���� ���� ��� ����
					if (((A + u)->expon) >= LEA2) { //���� ����� �������� Ŭ ��
						if (Amax2 < 0) { //ù ���� ��
							LEA2 = (A + u)->expon; //�ְ����� ����
							an = u; //INDEX ����
							Amax2 = LEA2; //���� ���� ����
						}
						else if (((A + u)->expon) < Amax2) //�̹� ���� ������ �ߺ� ����
						{
							LEA2 = (A + u)->expon; //���� ����
							an = u; //INDEX ����
							Amax2 = LEA2; //���� ���� ����
						}
					}
				}
			}

			if (LEB2 < Bmax2) { //���� ����
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

			switch COMPARE(LEA2, LEB2) { //���� ��
			case -1 : //B�� �� Ŭ ��
				(D + count)->coef = (B + bn)->coef; //��� ����
				(D + count)->expon = LEB2; //���� ����
				LEB2 = 0; //�ش� ������ ��������� �˸�
				num2++; //����� �� ���� ����
				count++; //D�� INDEX ����
				break;
			case 0 : //���� ��
				if (((A + an)->coef) + ((B + bn)->coef) != 0){ //����� ���� 0�� �ƴ� ��
					(D + count)->coef = ((A + an)->coef) + ((B + bn)->coef);
					(D + count)->expon = LEA2;
					count++;
				}
				LEA2 = 0, LEB2 = 0;
				num2+= 2;
				break;
			case 1 : //A�� �� Ŭ ��
				(D + count)->coef = (A + an)->coef;
				(D + count)->expon = LEA2;
				LEA2 = 0;
				num2++;
				count++;
				break;
			}
			
		} while (num2<(nA+nB)); //����� ���� ������ ��ü ���� ���� �̻��� �� ������ �ݺ�
	}
	else if ((nA == 0) && (nB != 0)) { //A�� ZERO POLYNOMIAL�� ��
		for (int z = 0; z < nB; z++) {//B term ����ŭ �ݺ�
			(D + z)->coef = (B + z)->coef; //B ��� ����
			(D + z)->expon = (B + z)->expon; //B ���� ����
		}
	}
		
	else if ((nA != 0) && (nB == 0)) { //���� ����
		for (int y = 0; y < nA; y++) {
			(D + y)->coef = (A + y)->coef;
			(D + y)->expon = (A + y)->expon;
		}
	}
	else  // �� �� ZERO POLYNOMIAL�� ��
		D = NULL; //D�� ZERO POLYNOMIAL


	return;
}



