// Q. Find the kth number such that the only prime factors are 3, 5, and 7

/*
Checks:
	n = 3^a * 5^b * 7^c
	Let's list some numbers
            	a	b	c
	1	-		1	0	0
	3	3		1	0	0
	5	5		0	1	0
	7 	7		0	0	1
	9	3*3		2	0	0
	15	3*5		1	1	0
	21	3*7		1	0	1
	25	5*5		0	2	0
	27	3*9		3	0	0
	35	5*7		0	1	1	
	45	5*9		2	1	0
	49	7*7		0	0	2
	63	3*21	2	0	1	

	Every number can be expressed as one of the followings:
		3 * (some previous numbers)
		5 * (some previous numbers)
		7 * (some previous numbers)
	
	1. Naive version:
		Multiply each number in the list by 3, 5, and 7 and find the smallest element that has not
		yet been added to the list. -> O(n^2)
	
	2. Better version:
		number is chosen in 3-queue -> multiply previous number by 3 in all the queue
		number is chosen in 5-quque -> multiply previous number by 5 in 5-queue and 7-queue
			-> becuase 5*3 is already done in 5-queue 
		number is chosen in 7-queue -> multiply previous number by 7 in only the 7-quque
			-> because 7*3 and 7*5 is already done in 7-queue
	
*/

#include <iostream>
#include <queue>
using namespace std;

int mini(int a, int b){
    return a < b ? a : b;
}
int mini(int a, int b, int c){
    return mini(mini(a, b), c);
}

void printQueue(queue<int> q) 
{
	while(!q.empty()) {
		cout << q.front() << " ";
		q.pop();
	}
}

int get_num(int k){
    if(k <= 0) return 0;
    int res = 1, cnt = 1;
	// initialize three queue for 3, 5, 7
    queue<int> q3, q5, q7;
	
	// Insert 1 for 3, 5, 7
    q3.push(3); q5.push(5); q7.push(7);
    for(; cnt<k; ++cnt){
        int v3 = q3.front();
        int v5 = q5.front();
        int v7 = q7.front();
		// find minimum x from the front of three queues
        res = mini(v3, v5, v7);
        if(res == v7){
            q7.pop();
        }
        else{
            if(res == v5){
                q5.pop();
            }
            else{
                if(res == v3){
                    q3.pop();
                    q3.push(3*res);
                }
            }
            q5.push(5*res);
        }
        q7.push(7*res);
		
		// print
		cout <<cnt << " -> " << "res= " << res << " q3= "; 
		printQueue(q3);
		cout << ", q5 = " ;
		printQueue(q5);
		cout << ", q7 = " ;
		printQueue(q7);
		cout << endl;

		// 3 -> appends x*3, x*5, and x*7
		// 5 -> appends x*5, x*7 sinec x*3 is already put in 3
		// 7 -> appends x*7, since x*3 and x*5 are already put in 3 and 5
    }
    return res;
}
int main(){
    //for(int i=1; i<20; ++i)
        cout<<get_num(30)<<endl;
    return 0;
}
