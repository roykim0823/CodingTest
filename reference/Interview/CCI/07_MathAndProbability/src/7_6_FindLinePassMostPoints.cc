// Q. Given a two dimentional graph with points on it, find a line which passses 
//    the most number of points 

/* Check: How will be the line represented?
	1. as a pair of points: this pair of points can be any two distinct points on the line,
	   so the same line could be represented in an infinite number of ways.
	2. As a slope and a y-intersect: this has an advantage in that every line segment on the same
	   greater line will have identical slopes and y-intercept
	Choose 2 
	Then, we have a bunch of line segments, represented as a slope and y-intercept, and we want to
	find the most common slope and y-intercept pair. How can we find the most common one?

	This is really no different than the classic "find the most common nubmer in a list of numbers"
	problem. We just iterate through the lines segments and use a hash table to count the number 
	of times we've seen each line.

	3. Be careful about the calculation of the slope of a line. It can be completely vertical.
	   Also division is not exact. The difference is checked by the epsilon.
*/

#include <iostream>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

struct point{
    double x, y;
};
class line{
public:
    double epsilon, slope, intercept;
    bool bslope;
public:
    line(){}
    line(point p, point q){
        epsilon = 0.0001;
        if(abs(p.x - q.x) > epsilon){
            slope = (p.y-q.y) / (p.x-q.x);
            intercept = p.y - slope * p.x;
            bslope = true;
        }
        else{
            bslope = false; 	// vertical line=infinite slope
            intercept = p.x;
        }
    }
    int hashcode(){
        int sl = (int)(slope * 1000);
        int in = (int)(intercept * 1000);
        return sl*1000 +  in;
    }
    void print(){
        cout<<"y = "<<slope<<"x + "<<intercept<<endl;
    }
};

line find_best_line(point *p, int point_num){
    line bestline;
    bool first = true;
    map<int, int> mii;
    for(int i=0; i<point_num; ++i){
        for(int j=i+1; j<point_num; ++j){
            line l(p[i], p[j]);
            if(mii.find(l.hashcode()) == mii.end()){	// miss the hash table
                mii[l.hashcode()] = 0;
            }
            mii[l.hashcode()] = mii[l.hashcode()] + 1;
            if(first){
                bestline = l;
                first = false;
            }
            else{
				// find the max one
                if(mii[l.hashcode()] > mii[bestline.hashcode()])
                    bestline = l;	
            }
        }
    }
    // int a = mii[bestline.hashcode()];
    // cout<<mii[bestline.hashcode()]<<endl;
    // cout<<(1+sqrt(1+8*a))/2<<endl;
    return bestline;
}
int main(){
    srand((unsigned)time(0));
    int graph_size = 100;
    int point_num = 500;
    point *p = new point[point_num];
    for(int i=0; i<point_num; ++i){
        p[i].x = rand()/double(RAND_MAX) * graph_size;
        p[i].y = rand()/double(RAND_MAX) * graph_size;
        //cout<<p[i].x<<" "<<p[i].y<<endl;
    }
    line l = find_best_line(p, point_num);
    l.print();
    return 0;
}
