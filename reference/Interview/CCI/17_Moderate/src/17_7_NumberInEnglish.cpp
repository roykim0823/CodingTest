// Q. Given any integer, print an English phrase that describes the integer.
//    e.g., "One Thousand, Two Hundred Thirty Four"

/* 
Check:
Converting each of three 3-digit segments of the number, and inserting unit words (thousands or millions)
	convert(19,323,984) = convert(19) + " million " +
						  convert(323) + " thousand " +
						  couvert(984)
*/

#include <iostream>
using namespace std;

string digits[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
string teens[] = {"Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
string tens[] = {"Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };
string bigs[] = {"", "Thousand", "Million"};

string numToString100(int number);

string numToString(int number) {
	if(number==0) {
		return "Zero";
	} else if (number<0) {
		return "Negative " + numToString(-1*number);
	}

	int count=0;
	string str="";

	while(number>0) {
		if(number%1000 != 0) {
			str = numToString100(number % 1000) + bigs[count] + " " + str;
		}
		number/=1000;
		count++;
	}

	return str;
}

string numToString100(int number) {
	string str="";

	// Convert hundreds place
	if(number >=100) {
		str+= digits[number/100-1] + " Hundred ";
		number%=100;
	}

	// Convert tens place
	if (number >=11 && number <=19) {
		return str+ teens[number-11]+ " ";
	}else if(number == 10 || number >=20) {
		str += tens[number/10 -1] + " ";
		number %= 10;
	}

	// Convert ones place
	if(number >=1 && number <=9) {
		str+=digits[number-1] + " ";
	}
	return str;
}

int main() {
   cout << numToString(19323984) << endl;
   cout << numToString(1) << endl;
   cout << numToString(12) << endl;
   cout << numToString(123) << endl;
   cout << numToString(1234) << endl;
	
}
