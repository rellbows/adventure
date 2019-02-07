// TODO: header here...

#include <stdio>

int main(){

	// Get PID for process
	
	// Make <username>.rooms.<pid> directory

	// Create all connections in graph
	while (IsGraphFull() == false){
		AddRandomConnection();
	}

	return 0;
}

// Returns true if all rooms have 3 to 6 outbound connections
bool IsGraphFull(){
	// TODO: need to start
}

void AddRandomConnection(){
	Room A;	// Maybe a struct, maybe global arrays of ints
	Room B;

	while(true){
		A = GetRandomRoom();

		if(CanAddConnectionFrom(A) == true){
			break;
		}
	}

	do{
		B = GetRandomRoom();
	}
	while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

	ConnectRoom(A, B);	// TODO: Add this connection to the real
	ConnectRoom(B, A);	// variable b/c this A and B will be destoryed
}

// Returns a random Room, does NOT validate if connection can be added
Room GetRandomRoom(){
	// TODO: complete....
}

// Returns true if a connection from Foom x to Room y already exists, false
// otherwise
bool ConnectionAlreadyExists(x, y){
	// TODO: complete...
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(Room x, Room y){
	// TODO: complete...
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(Room x, Room y){
	// TODO: complete...
}
