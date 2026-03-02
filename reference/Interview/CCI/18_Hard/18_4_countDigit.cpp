// Q. Write a method to count the number of 2s between 0 and n
#include <iostream>
using namespace std;

// Bruteforce
int count2s(int n);   			// counts the number of '2' digits in a signle number
int count2sInRange(int n) {
	int cnt=0;
	for(int i=2; i<=n; i++) {	// Might as well start at 2
		cnt +=count2s(i);
	}
	return cnt;
}

int count2s(int n){
    int count = 0;
    while(n > 0){
        if(n%10 == 2)
            ++count;
        n /= 10;
    }
    return count;
}

/* Improved: roughly one tenth of the time, the "last" digit will be a 2
	0	1	2 	3	4	5	6	7	8	9	
	10	11	12	13	14	15	16	17	18	19
	20	21	22	23	24	25	26	27	28	29
	...
	110	111	112	113	114	115	116	117	118	119

	We say "roughly" because there are (very common) boundary conditions. For example,
	between 1 and 100, the 10s digit is a 2 exactly 1/10th of the time. However, between 1
	and 37, the 10s digit is a 2 much more than 1/10th time.

	We can work out what exactly the ratio is by looking at the three cases individually:
	case digit<2, case digit=2, case digit>2

	1. case digit<2
		Consider the value x = 61523 and d=3, and observe that x[d]=1 (that is, the dth digit 
		of x is 1). There are 2s at the 3rd digit in the ranges 2000-2999, 12000-12999, ...
		52000-52999. We will not yet have hit the range 62000-62999, so there are 6000 2s totol
		in the 3rd digit.
		This is same amount as if we were just counting all the 2s in the 3rd digit bewteen
		1 and 60000
		In other words, we can round down to the nearest 10^(d+1), and then divide by 10, to
		compute the number of 2s in the dth digit.
		
		if x[d] < 2: 
			y = round down to nearest 10^(d+1)
			return y/10

	2. case digit>2
		Apply almost the exact same logic of case 1. 
		if x = 63525, then instead of range 0-63252, use the range of 0-70000.

		if x[d] < 2: 
			y = round up to nearest 10^(d+1)
			return y/10

	2. case digit==2
		Apply almost the exact same logic of case 1. 
		if x = 62523, then use case 1 (0-52000) and count the remaining.
		The remaining is 62000-62523, that is 524

		if x[d] == 2: 
			y = round down to nearest 10^(d+1)
			z = the right side of x
			return y/10 + (z+1)
*/

int count2s_v2(int n){
    int count = 0;
    int factor = 1;
    int low = 0, cur = 0, high = 0;

    while(n/factor != 0){
        low = n - (n/factor) * factor;	//	round down to nearest 10^(d+1)	 
        cur = (n/factor) % 10;			//  the current digit
        high = n / (factor*10);			//  round up to nearest 10^(d+1)	 higher digit

        if(cur<2)    
			count += high * factor;
        else if(cur==2)
            count += high * factor + low + 1;
        else // cur>2
            count += (high + 1) * factor;
        

        factor *= 10;
    }

    return count;
}

// Generalization of counting Xs
int countis(int n, int i){
    if(i<1 || i>9) return -1;//i只能是1到9

    int count = 0;
    int factor = 1;
    int low = 0, cur = 0, high = 0;

    while(n/factor != 0){
        low = n - (n/factor) * factor;//低位数字
        cur = (n/factor) % 10;//当前位数字
        high = n / (factor*10);//高位数字

        if(cur < i)
            count += high * factor;
        else if(cur == i)
            count += high * factor + low + 1;
        else
            count += (high + 1) * factor;

        factor *= 10;
    }

    return count;
}
int main(){
    for(int i=1; i<=1000; ++i)
        cout<<"i= " << i << " " << count2sInRange(i)<<" "<<count2s_v2(i)<<" "<<countis(i, 2)<<endl;
    // int n = 999;
    // for(int i=1; i<10; ++i)
    //     cout<<Countis(n, i)<<endl;
    return 0;
}
