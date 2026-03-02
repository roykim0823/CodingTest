// Q. Explain the data structures and algorithms that you would use to design on in-memory file system.
//    Illustrate with an example in code where possible.

/* Don't panic! Just think through the components of a file system, and tackle this problem just like
   any other object-oriented design question.

Check:
	A file system, in its most simplistic version, consists of Files and Directories. Each Directory
	contains a set of Files and Directories. Since Files and Directories share so many characteristics,
	implement Files and Directories from the same class, Entry
*/


enum Type { F, D };	// F=File, D=Directory

class Entry {
protected:
	Directory parent;
	long created;
	long lastUpdated;
	long lastAccessed;
	string name;
	Type type;

public:
	Entry(string n, Directory p) {
		name = n;
		parent = p;
		created = System.currentTimeMillis();
		lastUpdated = System.currentTimeMillis();
		lastAccessed = System.currentTimeMillis();
	}

	bool delete() {
		if( parent == NULL ) return false;
		return parent.deleEntry(this);
	}

	int size() = 0;	// pure virual function -> make this class abstract class

	string getFullPath() {
		if(parent==NULL) return name;
		else return parent.getFullPath()+"/"+name;
	}

	// Getter and setters
	long getCreatioinTime() { return created; }
	long getLastUpdatedTime() { return lastUpdated; }
	long getLastAccessedTime() { return lastAccessed; }
	void changeName(string n) { name = n; }
	Type getType() { return type; }
	string getName() { return name; }
}

class File: public Entry {
private:
	string content;
	int size;

public:
	File(string n, Directory p, int sz) {
		super(n, p);
		size = sz;
		type=F;
	}

	int size() { return size; }
	string getContents() { return content; }
	void setContents(string c) { content = c; }
}

class Directory : public Entry {
protected:
	vector<Entry> contents;

public:
	Directory(string n, Directory p) {
		super(n, p);
		type=D;
	}

public:
	int size() { 
		int size = 0;
		for (int i=0; i<contents.size(); i++) {
			size+=contents[i].size();
		}
		return size;
	}

	int numberOfFiles() {
		int count=0;
		for (int i=0; i<contents.size(); i++) {
			if(contents[i].getType() == D) {
				count++;	// Directory counts as a file
				Directory d = (Directory) e;
				count += d.numberOfFiles();
			} else if (contents[i].getType()==F) {
				count++;
			}
		}

		return count;
	}

	bool deleteEntry(Entry entry) {
		return contents.remove(entry);
	}

	void addEntry(Entry entry) {
		contens.add(entry);
	}

	vectory<Entry>& getContents() { return coutents; }
}

		




