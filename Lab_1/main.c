#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintAtoF (int x) {
    if ((x >= 10) && (x <= 15)) {
        printf("%c", (char)((int)('A') + x - 10));
    }
    else printf("%d", x);
}

int ToNew (char x) {
    if ((x >= 'A') && (x <= 'Z')) return x - 'A' + 10;
    else return x - '0';
}

int Check(char* x, int OldBase, int NewBase) {
    int DeniedSymb; // DeniedSymb - показатель, существуют ли в исходном числе недопустимые символы для той системы счисления (1 - существуют, 0 - не существуют)
    for (int i = 0; i < strlen(x); i++) {
        if (x[i] == '.') continue;
        if ((OldBase < 11) && (x[i] - '0' >= OldBase)) {
            DeniedSymb = 1;
        }
        if ((OldBase > 10) && !(((x[i] >= 'A') && (x[i] <= 'Z')) | ((x[i] >= '0') && (x[i] <= '9')))) {
            DeniedSymb = 1;
        }
    }
    if ((OldBase > 16) | (OldBase < 2) | (NewBase > 16) | (NewBase < 2) | (DeniedSymb == 1)) {
        printf("Bad input");
        return 1;
    }
    else return 0;
}

int TransferInt(char* x, int OldBase) {
    int new, IntPart, DotNumber = -1;
    new = ToNew(x[0]);
    for (int i = 0; i < strlen(x) - 1; i++) { // перевод целой части числа в промежуточную систему счисления
        if (x[i + 1] == '.') {
            IntPart = new; // IntPart - целая часть числа
            DotNumber = i + 1; // DotNumber - номер позиции точки, если у числа существует дробная часть (равен -1, если дробной части нет)
            break;
        }
        else {
            new = new * OldBase + ToNew(x[i + 1]);
        }
    }
    if (DotNumber == -1) {
        IntPart = new;
    }
    return IntPart;
}

float TransferDec(char* x, int OldBase) {
    int DotNumber = -1;
    for (int i = 0; i < strlen(x) - 1; i++) { // перевод целой части числа в промежуточную систему счисления
        if (x[i + 1] == '.') {
            DotNumber = i + 1; // DotNumber - номер позиции точки, если у числа существует дробная часть (равен -1, если дробной части нет)
            break;
        }
    }
    float DecPart;
    if (DotNumber != -1) {
        float OldBase1 = OldBase, new1;
        new1 = ToNew(x[strlen(x) - 1]) / OldBase1;
        for (int i = strlen(x) - 1; i > (DotNumber + 1); i--) {
           new1 = (new1 + ToNew(x[i - 1])) / OldBase1;
        }
        DecPart = new1; // DecPart - дробная часть числа
        return DecPart;
    }
}

void PutToScreen(char* x, int IntPart, int NewBase, float DecPart) {
    int a[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int m = 0, NotDigit = 0, DotNumber = -1;;
    float NewBase1 = NewBase;
    while (IntPart / NewBase1 > 0) { // перевод целой части числа в новую систему счисления
        if (NewBase > 10 && IntPart % NewBase >= 10) {
            a[m] = IntPart % NewBase;
            NotDigit = 1; // NotDigit - показатель, существуют ли в записи числа "цифры", превосходящие 9 (1 - существуют, 0 - не существуют)
        } else {
            a[m] = IntPart % NewBase;
        }
        IntPart /= NewBase;
        m++;
    }
    for (int i = m - 1; i >= 0; i--) { // вывод целой части нового числа
        if (NotDigit == 0) {
            printf("%d", a[i]);
        }
        else PrintAtoF(a[i]);
    }
    for (int i = 0; i < strlen(x) - 1; i++) { // перевод целой части числа в промежуточную систему счисления
        if (x[i + 1] == '.') {
            DotNumber = i + 1; // DotNumber - номер позиции точки, если у числа существует дробная часть (равен -1, если дробной части нет)
            break;
        }
    }
    if (DotNumber != -1) { // вывод дробной части нового числа
        printf(".");
        for (int i = 0; i < 12; i++) {
            int m = (int)(DecPart * NewBase);
            if (NewBase <= 10) {
                printf("%d", m);
            }
            else PrintAtoF(m);
            DecPart = DecPart * NewBase - m;
        }
    }
}

int main(int argc, char *argv[]) {
    int OldBase = atoi(argv[1]), NewBase = atoi(argv[2]), IntPart;
    float DecPart;
    if (Check(argv[3], OldBase, NewBase) == 0) {
        IntPart = TransferInt(argv[3], OldBase);
        DecPart = TransferDec(argv[3], OldBase);
        PutToScreen(argv[3], IntPart, NewBase, DecPart);
    }
    return 0;
}