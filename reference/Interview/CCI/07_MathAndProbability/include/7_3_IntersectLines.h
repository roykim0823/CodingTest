// Q. give two lines on a Cartesian plane, determine whether thw two lines would intersect.

/*
Checks:
There are lots of unknowns in this problem: 
	What format are the lines?
	What if they are the same line?
	So, discuss these ambiguities with your interviewer.

In problem like these, be aware of the following:
	Ask questions. This question has a lot of unknows - ask questions to clarify them.
		Many interviewers intentionally ask vague questions to see if you'll clarify your
		assumptions.
	When possible, design and use data structures. It shows that you understand and care
		about object-oriented design.
	Think through which data structures you design to represent a line. There are a lot of
		options, with lots of trade-offs. Pick one, and explain your choice.
	Don't assume that the slope and y-intercept are integers.
	Understand limitations of floating point representation. 
	  -> Never check for equality with ==, instead, check if the difference is less than \
	  	 an epsilon value

Solutions:
	Ask questions to clarify the problems
	1. type of variable (int, float, double)
	2. Understand the limitations of floating point, never use "==" equality 
   	-> Use less than an epsilon value
	
	Assumption here
	a. if two lines are the same (same slope and y-intercept), then they are considered to 
	   intersect
	b. We get to decide the data structure for the line

	if two different lines are not parallel, then they intersect. Thus, to check if two lines
	intersect, just need to check if the slopes are different (of if the line are identical).
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>	// for reverse
#include <stdexcept> 	// for throw invalid_argument
#include <limits>

using namespace std;


class Line {
private:
	double slope;
	double yintercept;

public:
	Line(double s, double y) {
		slope=s;
		yintercept=y;
	}

	bool intersect(Line line2) {
		return abs(slope-line2.slope) > numeric_limits<double>::epsilon() ||
				abs(yintercept - line2.yintercept) < numeric_limits<double>::epsilon();
	}
};
