// Q. Wriet a smart pointer class.

#include <iostream>
#include <cstdlib>
using namespace std;

template <typename T>
class SmartPointer{
public:
	// constructor
    SmartPointer(T* ptr){
        ref = ptr;
		// allocated the ref_counter in the heap since it will be shared by pointers
        ref_count = (unsigned*)malloc(sizeof(unsigned));	
        *ref_count = 1;
    }
	
	// this constructor creates a new smart pointer that points to an existing object.
	// We will need to first set obj and ref_count to pointer to sptr's obj and ref_count.
	// Then, because we crreated a new reference to obj, we need to increment ref_count
    SmartPointer(SmartPointer<T> &sptr){
        ref = sptr.ref;
        ref_count = sptr.ref_count;
        ++*ref_count;
    }
	
	// create by Assignment operator
	// Decrement its reference count, then copy the pointers to obj and ref_count over. 
	// Finally, seince we created a new reference, we need to increment ref_count
    SmartPointer<T>& operator=(SmartPointer<T> &sptr){
        if (this != &sptr) {
            if (--*ref_count == 0){
                clear();
                cout<<"operator= clear"<<endl;
            }
            
            ref = sptr.ref;
            ref_count = sptr.ref_count;
            ++*ref_count;
        }
        return *this;
    }
	
	// Destructor: destory a reference to the object. 
	// Decrement ref_count. if ref_count is 0, then free the memory created by the integer and
	// destory the object
    ~SmartPointer(){
        if (--*ref_count == 0){
            clear(); 
			cout<<"destructor clear"<<endl; 
		} 
	}
	
    T getValue() { return *ref; }
    
private:
    void clear(){
        delete ref;
        free(ref_count);
        ref = NULL; // 避免它成为迷途指针
        ref_count = NULL;
    }
   
protected:
	// The smart pointer class needs pointers to both the object itself and to the ref counter.
	// These must be pointers, rather than the actual object or ref count value since the goal of 
	// a smart pointer is that the reference count is tracked across multiple smart pointers 
	// to one object.
    T *ref;
    unsigned *ref_count;
};

int main(){
    int *ip1 = new int();
    *ip1 = 11111;
    int *ip2 = new int();
    *ip2 = 22222;
    SmartPointer<int> sp1(ip1), sp2(ip2);
    SmartPointer<int> spa = sp1;
    sp2 = spa;
    return 0;
}
