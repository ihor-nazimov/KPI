/*	Рекурсивні алгоритми та структури даних
	Лабораторна робота №1
	Варіант 9. Дано послідовність, що містить від 2 до 30 слів,
    у кожному з яких від 2 до 10 рядкових букв;
    між сусідніми словами - кома, за останнім словом - крапка.
    Надрукувати всі слова послідовності, які відмінні від останнього слова,
    попередньо перетворивши кожне з них за наступним правилом:
    -	видалити зі слова останню букву

	Назімов І.П. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1000

/* Ітераційна функція
 * Друк першого слова вхідного рядку за умовами завдання.
 * in - вхідний рядок (вхід)
 * last - останнє слово (вихід)
 * return = 0 - успіх
 * return = -1 - помилка */
int printWithoutLast(char* in, char** last) {
    char* first = 0;
    //int length = 0;
    for (int i = 0; 1; i++) {
        switch ( in[i] ) {
            case ' ':
                fprintf(stderr, "ERROR: Incorrect input. Please avoid space.\n");
                return -1;
            case '\0':
                fprintf(stderr, "ERROR: Unexpected end of input.\n");
                return -1;
            case '.':
                if (first) {
                    if (i > 1) {
                        in[i-1] = '\0';
                        *last = first;
                        return 0; //завершення 
                    } else {
                        fprintf(stderr, "ERROR: Word length is lower then 2 letters.\n");
                        return -1;
                    }
                } else {
                    fprintf(stderr, "ERROR: Last word not found.\n");
                    return -1;
                }
            case ',':
                if ( first != 0 ) { //end
                    if (i < 2) {
                        fprintf(stderr, "ERROR: Word length is lower then 2 letters.\n");
                        return -1;
                    }
                    in[i - 1] = '\0';
                    if (printWithoutLast(in + i + 1, last) < 0) {
                        //fprintf(stderr, "ERROR: Word length is lower then 2 letters");
                        return -1;
                    }
                    if ( strcmp(first, *last) != 0 ) printf("%s\n", first);
                    return 0;
                    
                }
            default:
			    if ( !first ) first = in+i;
				break;   
        }

    }
}

int main() {
    char buf [MAX_LENGTH];
    char* last;

    buf[MAX_LENGTH-1] = 0; //попередження виходу за межі масиву

    printf("Hello!\nPlease enter 2 to 30 words with 2 to 10 low-case chars each.\nWords are spaced by comma and last word is followed by dot.\nThen press ENTER:\n");
    scanf("%s", &buf);

    if ( printWithoutLast(buf, &last) < 0 ) {
        fprintf(stderr, "Please try again\n");
        return -1;
    };
	
	printf("Press ENTER to exit");
	getchar();
	
    return 0;
}
