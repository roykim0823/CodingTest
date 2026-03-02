// Q. Game: RGBY, guess GGRR -> one hit and one pseudo-hit

#include <iostream>
#include <cstring>
using namespace std;

struct Result{
    int hits;
    int pseudo_hits;
};

// Not Correct
Result Estimate(const char* solution, const char* guess){
    Result res;
    res.hits = 0;
    res.pseudo_hits = 0;
    int solution_mask = 0;	// Set mask bit for each color

    for(int i=0; i<4; ++i){
        if(guess[i] == solution[i])
            ++res.hits;  
		else
        	solution_mask |= 1 << (solution[i] - 'A');
    }

    for(int i=0; i<4; ++i){
        if(guess[i] == solution[i])
            ;//++res.hits;
        else if(solution_mask & ( 1<<(guess[i] - 'A')))
            ++res.pseudo_hits;
    }
    return res;
}

int Min(int a, int b){
    return a < b ? a : b;
}

Result Estimate1(const char* solution, const char* guess){
    Result res;
    res.hits = 0;
    res.pseudo_hits = 0;
    int num = 26 + 5;
    int solution_count[num];
    memset(solution_count, 0, sizeof(solution_count));

    for(int i=0; i<4; ++i){
        if(guess[i] == solution[i])
            ++res.hits;
		else
        	++solution_count[(int)(solution[i]-'A')];
    }
	
    for(int i=0; i<4; ++i){
        if(guess[i] == solution[i])
            ;//++res.hits;
		else if (solution_count[(int)(guess[i]-'A')]>0)
        {
			++res.pseudo_hits;
			--solution_count[(int)(guess[i]-'A')];
		}
    }

    return res;
}
int main(){
    //char solution[] = "RYGB";
    //char guess[] = "YRRR";
    char solution[] = "RGBY";
    char guess[] = "GGRR";
    Result res = Estimate(solution, guess);
    cout<<res.hits<<" "<<res.pseudo_hits<<endl;
    Result res1 = Estimate1(solution, guess);
    cout<<res1.hits<<" "<<res1.pseudo_hits<<endl;
    return 0;
}
