// TODO: header here...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// The rooms
const char* rooms[] = {"Ballroom", "Basement", "Attic", "Garage", "Hall", "Kitchen", "Library", "Lounge", "Study", "Bathroom"};

enum roomType{ START_ROOM, END_ROOM, MID_ROOM};

// holds the name/type/outbound connections for a room
struct room{
	char name[9];    // 8 char length + null term
	enum roomType type;
	char outboundConnect[6][9];    // max connect of 6 + ea. entry 8 chars
	int outboundCount;    // helps keep count of number of connections	
};

// function headers
int randomInt(int max, int min);
void randomRoomList(struct room roomList[], size_t len);

int main(){

	// Variable declaration
	int pid = 0;    // Holds process id
	char* directory = malloc(25 * sizeof(char));    // Holds directory name; should be plenty of room
	memset(directory, '\0', 25);    // Clearing string
	char* firstDir = malloc(15 * sizeof(char));    // First part of directory
	memset(firstDir, '\0', 15);
	strcpy(firstDir, "ellisry.rooms.");	
	char* strPid = malloc(11 * sizeof(char));    // Holds pid as string
	memset(strPid, '\0', 11);    // Clearing...
	int dirResult = 0;    // holds status of directory creation
	struct room roomList[7];    // Holds the 7 rooms
	

	// Set the starting point for rand()
	srand((unsigned int) time(NULL));

	// Get PID for process
	pid = getpid();
	// Testing
	printf("%d\n", pid);
	// Convert PID from int to string
	snprintf(strPid, 11, "%d", pid);
	// Testing
	printf("%s\n", strPid);
	
	// Make string for <username>.rooms.<pid> directory
	snprintf(directory, 25, "%s%s", firstDir, strPid);
	// Testing
	printf("%s\n", directory);

	// Make actual directory
	dirResult = mkdir(directory, 0755);
	// Check result of call
	if(dirResult != 0){
		perror("Error creating directory!");
		return(-1);
	}

	// Testing
	struct room test;
	strcpy(test.name, rooms[0]);    // Should be Ballroom
	test.type = START_ROOM;
	strcpy(test.outboundConnect[0], rooms[1]);    // Should be Basement
	test.outboundCount = 1;
	printf("%s, %d, %s, %d\n", test.name, test.type, test.outboundConnect[0], test.outboundCount);
	memcpy(&roomList[0], &test, sizeof(roomList[0]));
	printf("%s, %d, %s, %d\n", roomList[0].name, roomList[0].type, roomList[0].outboundConnect[0], roomList[0].outboundCount);	

	// Get initialize randomized list of 7 rooms
	randomRoomList(roomList, sizeof(roomList) / sizeof(*roomList));
	
	// Testing
	int i = 0;
	for(i; i < (sizeof(roomList)/sizeof(*roomList)); i++){
		printf("%s %d\n", roomList[i].name, roomList[i].type);
	}

	/*
	// Create all connections in graph
	while (IsGraphFull() == false){
		AddRandomConnection();
	}
	*/

	// Cleanup memory allocation
	free(directory);
	free(firstDir);
	free(strPid);

	return 0;
}

// Returns a random integer
int randomInt(int min, int max){
	int randomInt = 0;
	randomInt = (rand() % (max - min)) + min;
	return randomInt;
}

// Initializes the list of random rooms with names and a type
void randomRoomList(struct room roomList[], size_t len){
	// local vars
	int i = 0;    // traverse thru random room list
	int randInt = 0;    // holds random num
	// keeps track of which rooms have been assigned; 0 = not, 1 = assigned
	int takenList[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// loop to move thru room list and assign out rooms
	for(i; i < len; i++){
		randInt = randomInt(0, 10);
		while(takenList[randInt] != 0){
			randInt = randomInt(0, 10);
		}

		// update taken list and copy
		takenList[randInt] = 1;	
		strcpy(roomList[i].name, rooms[randInt]);
	
		// assign out type
		if(i == 0){
			roomList[i].type = START_ROOM;
		}
		else if(i == (len - 1)){
			roomList[i].type = END_ROOM;
		}
		else{
			roomList[i].type = MID_ROOM;
		}
	}
}

/*
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
}*/
