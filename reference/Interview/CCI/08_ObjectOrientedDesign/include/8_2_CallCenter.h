/* Q. Design a call center with three levels of employees: respondent, manager, and director.
      An incoming telephone call must be first allocated to a respondent who is free. If the respondent
	  can't handel the call, he or she must escalate the call to a manager. If the manager is not free
	  or not able to handle it, then the call should be escalated to a director. Design the classes and
	  data structures for this problem. Implement a method dispatchCall() which assigns a call to the
	  first available employee.
*/
/* 
Checks: 
	1. few things are common to all three ranks of employees like address, name, job title, and age.
	   These things can be kept in one class and can be extended or inherited by others.
	2. CallHandler call routes the calls to the correct person.
	3. Design for long-term code flexibility and maintenance.
*/

// CallHandler is implemented as a singleton class.
class CallHandler {
private:
	static CallHandler instance;	// Singleton class

	// 3 levels of employees: respondents, managers, directors
	const int LEVELS = 3;

	// Initialize 10 respondents, 4 mangers, and 2 directors
	const int NUM_RESPONDENT = 10;
	const int NUM_MANAGERS = 4;
	const int NUM_DIRECTORS = 2;

	// List of employees, by level
	list<list<Employee>> empolyeeLevels;

	// Queues for each call's rank
	list<list<Call>> callQueues;

public:
	CallHandler() {... }

	// get instance of singleton class
	static CallHandler getInstance() {
		if(instance==NULL) instance = new CallHanlder();
		return instance;
	}

	// get the first available employee who can handle this call
    Employee getHandlerForCall(Call call) {...}

	// Routes the call to an available employee, or saves in a queue if no empolyee available
	void dispatchCall(Call call) {
		Employee emp = getHandlerForCall(call);
		if (emp != NULL) {
			emp.receiveCall(call);
			call.setHandler(emp);
		} else {
			// Please the call into correspoinding call queue according to its rank
			call.reply("Please wait for free employee to reply");
			callQueues.get(call.getRank().getValue()).add(call);
		}
	}

	// An employee got free. Look for a waiting call that emp. can serve. 
	// Return true if we assigned a call, false otherwise.
	bool assignCall(Employee emp) {...}
};

// Call represents a call from a user. A call has a minimum rank and is assigned to the first
// employee who can handle it
class Call {
private:
	// Mimimum rank of employee who can handle this call
	Rank rank;

	// Person who is calling.
	Caller caller;

	// Employee who is handling call
	Employee handler;

public:
	Call(Caller C) {
		rank = Rank.Responder;
		caller = C;
	}

	// Set employee who is handling call
	void setHandler(Employee e) { handler = e; }

	void reply(string message) {...}
	Rank getRank() { return rank; }
	void setRank(Rank r) { rank = r; }
	Rank incrementRank() {...}
	void disconnect() {...}
};

// Employee is a super call for the Director, Manger, and Respondent classes. It is implemented as an
// abstract class since there should be no reason to instantiate an Employee type directly.
class Employee {
private:
	Call currentCall = NULL;
protected:
	Rank rank;
public:
	Empolyee() {...} 

	// Start the conversation
	void receiveCall(Call call) {...}

	// this issue is resolved finish the call
	void callComplete() {...}

	// The issue has not been resolved. Escalate the call, and assign a new call to the employee
	void escalateAndReassign() {...}

	// Assign a new call to an employee, if the employee is free
	bool assignNewCall() {...}

	// Return whether or not the employee is free
	bool isFree() { return currentCall = NULL; }

	Rank getRank() { return rank; }
}

// Respondent, Director, and Manger classes are now just simple extension of the Employee class
class Director: public Employee {
public:
	Director() { rank = Rank.Director; }
}

class Manager: public Employee {
public:
	Manager() { rank = Rank.Manager; }
}

class Respondent: public Employee {
public:
	Respondent() { rank = Rank.Respondent; }
}
