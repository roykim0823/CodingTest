// Q. Design a parking lot using object-oriented principles.

/* 
Checks:
	1. Clarify the vague question with your interviewer
		What types of vehicles it can support
		Whether the parking lot has multiple levels, and so on
	2. Assumption there
		a. It has multiple levels. Each level has multiple rows of spots
		b. It can park motocycles, cars, and buses.
		c. There are motocycle spots, compact spots, and large sptos.
			- motocycle can park in any spot
			- a can can park in either a single compact spot or a single large spot
			- a bus can park in five large spots that are consecutive and within the same row.
			  a bus cannot park in small spots
*/
#include <iostream>
#include <vector>

using namespace std

enum VehicleType { Motorcycle, Compact, Large };

// Abstract class Vehicle from which Car, Bus, and Motocycle inherit. To handle the different parking 
// spot sizes, we have just one class ParkingSpot which has a member variable indicating the size.
class Vehicle {
protected:
	vector<ParkingSpot> parkingSpots;
	string licensePlate;
	int spotsNeeded;
	VehicleType type;

public:
	int getSpotsNeeded() { return spotsNeeded; }
	VehicleType getType() { return type; }

	// Park vehicle in this spot
	void parkInSpot(ParkingSpot s) { parkingSpots.push_back(s); }

	// Remoe car from spot, and notify spot that it's gone
	void clearSpots() { return ;};

	// Check if the spot is big enough for the vehicle
	virtual bool canFitInSpot(ParkingSpot spot) == 0;	// pure virtual function -> abstract class
	
};

class Bus: public Vehicle {
public:
	Bus() {
		spotsNeeded = 5;
		type = VehicleSize.Large;
	}

	// Check if the spot is a Compact or a Large
	bool canFitInSpot(ParkingSpot spot) { }
}

class Car: public Vehicle {
public:
	Car() {
		spotsNeeded = 1;
		type = VehicleSize.Compact;
	}

	// Check if the spot is a Compact or a Large
	bool canFitInSpot(ParkingSpot spot) { }
}


class Motorcycle: public Vehicle {
public:
	Motorcycle() {
		spotsNeeded = 1;
		type = VehicleSize.Motorcycle;
	}

	// Check if the spot is a Compact or a Large
	bool canFitInSpot(ParkingSpot spot) { }
}

/* The ParkingLot class is essentially a wrpper class for an array of Levels. By implementation it this
   way, we are able to separate out logic that deals with actually finding free spots and parking cars
   out from the broader actions of the ParkingLot. If we didn't do it this way, we would need to holding
   parking spots in some sort of double array (or hash table which maps from a level number to the list 
   of spots). It's cleaner to just separate ParkingLot from Level.
*/

class ParkingLot {
private:
	Level[] levels;
	int NUM_LEVEL;

public:
	ParkingLot(int num_level==5) {
		NUM_LEVEL=num_level;
	}

    // Park the vehicle in a spot (or spots)
	bool parkVehicle(Vehicle vehicle) { }
};

// Represent a level in a parking garage
class Level {
private:
	int floor;
	vector<ParkingSpot> spots;
	int availableSpots;
	int SPOTS_PER_ROW;

public:
	Level(int flr, int numberSpots) { 
		floor=flr;
		avaiableSpots=numberSpots;
	}

	int availableSpots() { return availableSports; }

	// Find a plcae to park this vehicle 
	bool parkVehicle(Vehicle vehicle) {}

	// Park a vehicle at the spot spotNumber
	bool parkStartingAtSpot(int num, Vehicle v) { }

	// Find a spot to park this vehicle. 
	int findAvailableSpots(Vehicle vehicle) {}

	// When a car was removed from the spot, or -1 on failure
	int findAvailableSpots(Vehicle vehicle) {}

	// When a car was removed from the spot, increment available Spots
	void spotFreed() { availableSpots++; }
};

/* The ParkingSpot is implemented by having just a variable which represents the size of the spot.  
   We could have implemented this by having classes for LargeSpot, CompactSpot, and MotocycleSpot 
   which inherit from ParkingSpot, but this is probably overkill. The spots probably do not have different
   behaviors, other than their sizes.
*/

class ParkingSpot {
private:
	Vehicle vehicle;
	VehicleType type;
	int row
	int spotNumber;
	Level level;

public:
	ParkingSpot(Level lvl, int r, int n, VehicleSize s) { }

	bool isAvailable() { return vehicle == null; }

	// Check if the spot is big enough and is available
	bool canFitVehicle(Vehicle vehicle) { }

	// Park Vehicle in this spot
	bool park(Vehicle v) {}

	int getRow() { return row; }
	int getSpotNumber() { return spotNumber; }

	// Remove vehicle from spot
	void removeVehicle() { }
};



