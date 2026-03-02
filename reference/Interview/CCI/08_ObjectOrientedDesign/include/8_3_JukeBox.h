// Q. Design a musical jukebox using object-oriented principles.

/* 
Checks:
	1. Clarify design constraints.
		Is this jukebox playing CDs? Records? MP3s?
		Is it a simulation on a computer? or is it supposed to represent a physical jukebox?
		Does it take money, or is it free? (what current if it takes money? it delivers change?)
	2. Assumptions Here:
		The jukebox is a computer simulation that closely minors physical jukeboxes.
		It is free.
	3. Outline the basic system components:
		JukeBox, CD, Song, Artist, Playlist, Display (displays details on the screen)
	4. Possible Actions:
		Playlist creation (includes add, delete, and shuffle)
		CD selector, Song selector, Queueing up a song, Get next song from playlist
	5. User interface: Adding, Deleting, Credit information

	Each of the main system components translates roughly to an object, and each action translates
	to a method. 
*/

// Jukebox class represents the body of the problem. Many of the interactions between the components
// of the system, or between the system and the user, are chaneled through here
class Jukebox {
private:
	CDPlayer cdPlayer;
	User user;
	Set<CD> cdCollection;
	SongSelector ts;

public:
	Jukebox(CDPlayer cdPlayer, User user, Set<CD> cdCollection, SongSelector ts) {...}
	Song getCurrentSong() { return ts.getCurrentSong(); }
	void setUser(User u) { this.user = u; }
}

// Like a real CD player, the CDPlayer class supports storing just one CD at a time. The CDs that are not
// in play are stored in the jukebox.

class CDPlayer {
private:
	Playlist p;
	CD c;

	// Constructors
	CDPlayer(CD c, Playlist p) {...}
	CDPlayer(Playlist p) { this.p = p; }
	CDPlayer(CD c) { this.c= c; }
	
	// Play a song
	void playSong(Song s) {...}

	// Getters and setters
	Playlist getPlaylist() { return p; }
	void setPlaylist(Playlist p) { this.p = p; }

	CD getCD() { return c; }
	void setCD() { this.c = c; }
}

// The Playlist manages the current and next songs to play. It is essentially a warpper class for
// a queue and offers some additional methods for convenience.
class Playlist {
private:
	Song song;
	Queue<Song> queue;
public:
	Playlist(Song song, Queue<Song> queue) {...}
	Song getNextSToPlay() {
		return queue.peek();
	}

	void queueUpSong(Song s) { queue.add(s); }
}

// The classes for CD, Song, and User are all fairly straightforward. They consist mainly of member
// variables and getters and setters
class CD { // data for id, artist, songs, etc 
}

class Song { // data for id, CD (could be null), title, length, etc
}

class User {
private:
	string name;
	long ID;
public:
	User(string name, long iD) {...}

	string getName() { return name; }
	void setName(string name) { this.name=name; }
	long getID() { return ID; }
	void setID(long iD) { ID = iD; }
	User getUser() { return this; }
	static User addUser(string name, long iD) {...}
}

	


