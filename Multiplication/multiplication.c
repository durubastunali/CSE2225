#include <stdio.h>
#include <stdlib.h>

struct number { //Struct
    int digit; //A single digit of the number
    struct number *next; //Pointer to the next digit
};

typedef struct number Number; //Typedef

//Function assignments
void readInputFile (Number *multiplicand, Number *multiplier, int baseArray[], FILE *input);
void baseNtoDecimal(Number *baseNNum, Number *base10Num, int numBase);
Number* reverseNumber(Number *num);
void multiplication(Number *multiplicand, Number *multiplier, Number *result, int base);
void printConsole(Number *num);
void initializeResult(Number *result, Number *multiplicand, Number *multiplier);
void initializeNumber(Number *num, Number *reference);

int main(int argc, char *argv[]) { //main function
    FILE *input = fopen("sample_input.txt", "r");

    //Declaring the heads of the multiplicand, multiplier and the result
    Number *multiplicand = (Number *)malloc(sizeof(Number));
    Number *multiplier = (Number *)malloc(sizeof(Number));
    Number *result = (Number *)malloc(sizeof(Number));

    int baseInput[2]; //The array that will hold the read base number from the input
    int baseNum = 0; //Proper int value of the base

    readInputFile(multiplicand, multiplier, baseInput, input); //Invoking function to read the input file

    if (baseInput[0] == 1 && baseInput[1] == 0) //If the number is decimal
        baseNum = 10;
    else if ((baseInput[0] >= 0 && baseInput[1] >= 0) || baseInput[0] <= 1) { //If the base is less than 2, greater than 10
        printf("Base must be between 2 and 10.");
        exit(0);
    }
    else if (baseInput[0] > 1) //If the base is between 2 and 10, as desired
        baseNum = baseInput[0];

    initializeResult(result, multiplicand, multiplier); //Initializing all the digits of the result to zero
    printf("Multiplicand: ");
    printConsole(multiplicand); //Displaying the given multiplicand
    printf("Multiplier: ");
    printConsole(multiplier); //Displaying the given multiplier

    multiplicand = reverseNumber(multiplicand); //Reverse the multiplicand linked list for multiplication
    multiplier = reverseNumber(multiplier); //Reverse the multiplier linked list for multiplication

    multiplication(multiplicand, multiplier, result, baseNum); //Invoke the multiplication function
    result = reverseNumber(result); //Reverse the result to print it properly
    printf("Result: ");
    printConsole(result); //Print the result on console

    if (baseNum != 10) { //If the base isn't 10, do the proper calculations to find the decimal form of the base N numbers
        Number *base10Multiplicand = (Number *) malloc(sizeof(Number)); //Base 10 form of the base N multiplicand
        initializeNumber(base10Multiplicand, multiplicand);
        baseNtoDecimal(multiplicand, base10Multiplicand, baseNum);
        printf("Multiplicand (10): ");
        printConsole( reverseNumber(base10Multiplicand));

        Number *base10Multiplier = (Number *) malloc(sizeof(Number)); //Base 10 form of the base N multiplier
        initializeNumber(base10Multiplier, multiplier);
        baseNtoDecimal(multiplier, base10Multiplier, baseNum);
        printf("Multiplier (10): ");
        printConsole(reverseNumber(base10Multiplier));

        Number *base10Result = (Number *) malloc(sizeof(Number)); //Base 10 form of the base N result
        initializeNumber(base10Result, result);
        baseNtoDecimal( reverseNumber(result), base10Result, baseNum);
        printf("Result (10): ");
        printConsole(reverseNumber(base10Result));
    }
    else { //If the base is already 10, no need to do the additional calculations to find the decimal forms. Directly print the decimal forms.
        printConsole(reverseNumber(multiplicand));
        printConsole(reverseNumber(multiplier));
        printConsole(result);
    }
    fclose(input); //Close the input file
}

void readInputFile (Number *multiplicand, Number *multiplier, int baseArray[], FILE *input) { //Read the input file
    Number *digitMultiplicand = multiplicand; //Multiplicand
    Number *lastDigitMultiplicand = NULL; //Last digit of the multiplicand
    char multiplicandDigit = getc(input); //The read char
    while (multiplicandDigit != '\n') { //Read the line until it reaches end
        Number *nextDigit = (Number *) malloc(sizeof(Number)); //Assign malloc to the next node which's going to be added
        digitMultiplicand->digit = ((int) (multiplicandDigit)) - 48; //Find the integer value of the ASCII coded char and assign to the node
        digitMultiplicand->next = nextDigit;
        lastDigitMultiplicand = digitMultiplicand;
        digitMultiplicand = nextDigit;
        multiplicandDigit = getc(input); //Update the read value
    } lastDigitMultiplicand->next = NULL;

    Number *digitMultiplier = multiplier; //Multiplier
    Number *lastDigitMultiplier = NULL; //Last digit of the multiplier
    char multiplierDigit = getc(input); //The read char
    while (multiplierDigit != '\n') { //Read the line until it reaches end
        Number *nextDigit = NULL;
        nextDigit = (Number *) malloc(sizeof(Number)); //Assign malloc to the new node which's going to be added
        digitMultiplier->digit = ((int) (multiplierDigit)) - 48; //Find the integer value of the ASCII coded char and assign to the node
        digitMultiplier->next = nextDigit;
        lastDigitMultiplier = digitMultiplier;
        digitMultiplier = nextDigit;
        multiplierDigit = getc(input); //Update the read value
    } lastDigitMultiplier->next = NULL;

    baseArray[0] = (int)getc(input) - 48; //Declare the  base
    baseArray[1] = (int)getc(input) - 48;
}

void multiplication(Number *multiplicand, Number *multiplier, Number *result, int base) { //Multiplication
    Number *numMultiplicand = multiplicand; //Multiplicand
    Number *numMultiplier = multiplier; //Multiplier
    Number *numResult = result; //Result
    Number *nextDigit = result; //Next updated head of the result

    int value = 0; //Total value of the multiplication and addition of digits
    int digit = 0; //The number to be written on result
    int carry = 0; //The number to be carried to the next calculation

    while (numMultiplier != NULL) { //Traverse the whole multiplier
        while (numMultiplicand != NULL) { //Traverse the whole multiplicand
            value = numMultiplicand->digit * numMultiplier->digit + carry + numResult->digit;
            digit = value % base; //Remainder of the total value to base is to be written on the result's current digit
            carry = value / base; //The division of the total value to base is to be written on the result's current digit
            numResult->digit = digit; //Assign the digit to the node
            if (numMultiplicand->next == NULL && carry > 0) { //When It's unable to continue the calculation but there's a carry left
                numResult->next->digit = carry;
                break;
            }
            numResult = numResult->next; //Continue to write on the next digits of the result
            numMultiplicand = numMultiplicand->next; //Continue on the next digit of the multiplicand
        }
        carry = 0; //Reset the carry
        numMultiplier = numMultiplier->next; //Continue on the next digit of the multiplier
        nextDigit = nextDigit->next; //Update the new head for the result
        numResult = nextDigit; //Assign the new head to the result
        numMultiplicand = multiplicand; //Renew the multiplicand, so it can be traversed again by the next multiplier digit
    }
}

void baseNtoDecimal(Number *baseNNum, Number *base10Num, int numBase) { //Base N to decimal conversion
    Number *num = baseNNum; //Number in base N form
    Number *base = (Number *) malloc(sizeof(Number)); //The base linked list
    initializeNumber(base, baseNNum); //Initialize the base linked list to zero
    base->digit = 1; //The base linked list always starts with the value of base^0 = 1
    Number *headBase = base; //Keep the head of the base linked list
    Number *result = base10Num; //Number in base 10 form

    int value = 0; //Total value of the multiplication and addition of digits
    int digit = 0; //The number to be written on result
    int carry = 0; //The number to be carried to the next calculation

    while (num != NULL) { //Traverse the whole num
        while (base != NULL) { //Traverse the whole base
            value = num->digit * base->digit + carry + result->digit;
            digit = value % 10; //Remainder of the total value to base is to be written on the result's current digit
            carry = value / 10; //The division of the total value to base is to be written on the result's current digit
            result->digit = digit; //Assign the digit to the node
            if (base->next == NULL && carry > 0) { //When It's unable to continue the calculation but there's a carry left
                result->next->digit = carry;
                break;
            }
            result = result->next; //Continue to write on the next digit of the result
            base = base->next; ////Continue on the next digit of the base
        }
        base = headBase; //Renew the base
        carry = 0; //Reset the carry
        while (base != NULL) { //Traverse the whole base and multiply the whole linked list with the int value of the original base number
            value = base->digit * numBase + carry;
            digit = value % 10; //Remainder of the total value to base is to be written on the base's current digit
            carry = value / 10; //The division of the total value to base is to be written on the base's current digit
            base->digit = digit; //Assign the digit to the node
            if (base->next == NULL && carry > 0) { ////When It's unable to continue the calculation but there's a carry left
                base->next->digit = carry;
                break;
            }
            base = base->next; //Continue to write on the next digit of the base
        }
        carry = 0; //Reset the carry
        num = num->next; //Continue on the next digit of the num
        result = base10Num; //Declare the result as the base 10 form of the original number
        base = headBase; //Renew the base
    }
}

void initializeResult(Number *result, Number *multiplicand, Number *multiplier) { //Initialize the beginning result to the data values of zero
    Number *digitsReference = multiplicand; //Assign first reference to the multiplicand
    Number *currentDigit = result; //Result
    Number *lastDigit = NULL;
    int i;
    for (i = 0; i < 2; i++) { //Traverse number of digits of multiplicand + number of digits of multiplier times
        while (digitsReference != NULL) {
            Number *nextDigit = (Number *) malloc(sizeof(Number)); //Memory allocate the next digit
            currentDigit->digit = 0; //Assign 0 to the data value
            currentDigit->next = nextDigit; //Assign a next node
            lastDigit = currentDigit;
            currentDigit = nextDigit;
            digitsReference = digitsReference->next; //Continue on the next digit
        }
        digitsReference = multiplier; //Update the reference as multiplier
    } lastDigit->next = NULL; //Finish the linked list
}

void initializeNumber (Number *num, Number *reference) { //Initialize the number to the data values of zero (used for base and base 10 formed number)
    Number *digitsReference = reference;
    Number *currentDigit = num;
    Number *lastDigit  =NULL;
    while (digitsReference != NULL) {
        Number *nextDigit = (Number *) malloc(sizeof(Number));
        currentDigit->digit = 0;
        currentDigit->next = nextDigit;
        lastDigit = currentDigit;
        currentDigit = nextDigit;
        digitsReference = digitsReference->next;
    } lastDigit->next = NULL;
}

Number* reverseNumber(Number *num) { //Reverse the linked list
    Number *changedDigit = NULL;
    Number *temporary = NULL;
    Number *currentDigit = num;

    while (currentDigit != NULL) { // A -> B
        temporary = currentDigit->next; //Temporarily assigning the current's next node to a node
        currentDigit->next = changedDigit; //Assigning the changed digit as the next digit of the current digit
        changedDigit = currentDigit; //Declaring the changed digit as the current digit
        currentDigit = temporary; //Declaring the current digit as the node it was temporarily kept
    }
    currentDigit = changedDigit; // B -> A
    return currentDigit; //Returning the reversed linked list
}

void printConsole(Number *num) { //Print on the console
    while (num->digit == 0) { //Skip the additional zeros in front of the number
        if (num->next == NULL) { //If the number is fully zero, which means any of the numbers are zero
            break;
        } num = num->next;
    }
    while (num != NULL) { //Traverse the whole number starting from the nonzero digit
        printf("%d", num->digit); //Print the digits in order
        num = num->next; //Continue on the next digit
    } printf("\n"); //Next line
}