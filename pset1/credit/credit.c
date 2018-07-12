#include <cs50.h>
#include <math.h>
#include <stdio.h>


int checkCardType(long long cNum);
int findDSum(long long num);
int find1Sum(long long cNum, int dNumMultipler);
int checkSum(long long cNum);


int main(void)
{
    long long cNum = get_long_long("Credit Card Number: ");

    int a = checkCardType(cNum);
    int b = checkSum(cNum);

    if (a == 1 && b == 1)
    {
        printf("VISA\n");
    }
    else if (a == 2 && b == 1)
    {
        printf("AMEX\n");
    }
    else if (a == 3 && b == 1)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

//2 = Check if AE: 15 digits, starts with 34, 37
//3 = Check if MC: 16 digits, starts with 51, 52, 53, 54, 55
//1 = Check if Visa: 13 or 16 digits, starts with 4

int checkCardType(long long cNum)
{
    int numOfDigits = 1 + log10(cNum);          // Find number of digits in card number

    int cardType1 = 0;
    switch (numOfDigits)                        // Match number of digits to card number
    {
        case 13:
            cardType1 = 1;       // Visa
            break;
        case 15:
            cardType1 = 2;       // American Express
            break;
        case 16:
            cardType1 = 3;       // MasterCard or Visa
            break;
    }

    int numBeginsWith1 = cNum / pow(10, numOfDigits - 1);
    int numBeginsWith2 = cNum / pow(10, numOfDigits - 2);
    numBeginsWith2 = numBeginsWith2 % 10;

    int cardType2 = 0;
    switch (numBeginsWith1)
    {
        case 4:
            cardType2 = 1;      // Visa
            break;
        case 3:
            switch (numBeginsWith2)
            {
                case 4:
                case 7:
                    cardType2 = 2;      // American Express
                    break;
            }
            break;
        case 5:
            switch (numBeginsWith2)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    cardType2 = 3;      // MasterCard
                    break;
            }
            break;
    }

    if ((cardType1 == 1 || cardType1 == 3) && cardType2 == 1)
    {
        return 1;           //Visa
    }
    else if (cardType1 == 2 && cardType2 == 2)
    {
        return 2;           //American Express
    }
    else if (cardType1 == 3 && cardType2 == 3)
    {
        return 3;           //MasterCard
    }
    else
    {
        return 0;
    }
}

//Checksum
int checkSum(long long cNum)
{
    int sum1 = find1Sum(cNum, 2);

    cNum = cNum * 10;                     // Shifts all numbers to one digit left
    int sum2 = find1Sum(cNum, 1);

    int sum = sum1 + sum2;

    switch (sum % 10)
    {
        case 0:
            return 1;
            break;
        default:
            return 0;
    }
}

// Find sum of every other (digit * 2)
int find1Sum(long long cNum, int dNumMultiplier)
{
    int sum = 0;
    int sumD = 0;
    int numC = 100;
    long long dNum = 0;

    while (cNum != 0)
    {
        dNum = cNum % numC;
        dNum = dNum / 10;
        sumD = findDSum(dNum * dNumMultiplier);
        sum = sum + sumD;
        cNum = cNum / numC;
    }

    return sum;
}

// Find sum of digits
int findDSum(long long num)
{
    long long r = 0;
    int sumD = 0;

    while (num != 0)
    {
        r = num % 10;
        sumD = sumD + r;
        num = num / 10;
    }

    return sumD;
}