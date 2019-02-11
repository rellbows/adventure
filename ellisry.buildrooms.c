// TODO: header here...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// The rooms
const char* rooms[] = {"Ballroom", "Basement", "Attic", "Garage", "Hall", "Kitchen", "Library", "Lounge", "Study", "Bathroom"};

enum roomType{ START_ROOM, END_ROOM, MID_ROOM};
typedef enum { false, true } bool;

// holds the name/type/outbound connections for a room
struct room{
	char name[9];    // 8 char length + null term
	enum roomType type;
	char outboundConnect[6][9];    // max connect of 6 + ea. entry 8 chars
	int outboundCount;    // helps keep count of number of connections	
};

// function prototypes 
int randomInt(int max, int min);
void randomRoomList(struct room roomList[], size_t len);
bool IsGraphFull(struct room roomList[], size_t len);
void AddRandomConnection(struct room roomList[], size_t len);
struct room GetRandomRoom(struct room roomList[], size_t len);
bool CanAddConnectionFrom(struct room x);
bool ConnectionAlreadyExists(struct room x, struct room y);
void ConnectRoom(struct room roomList[], int roomX, int roomY);

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

	// Get initialize randomized list of 7 rooms
	randomRoomList(roomList, sizeof(roomList) / sizeof(*roomList));
	
	// Testing
	int i = 0;
	for(i; i < (sizeof(roomList)/sizeof(*roomList)); i++){
		printf("%s %d\n", roomList[i].name, roomList[i].type);
	}

	// Create all connections in graph
	while(IsGraphFull(roomList, sizeof(roomList) / sizeof(*roomList)) == false){
		AddRandomConnection(roomList, sizeof(roomList) / sizeof(*roomList));
	}

	// Testing
	int j = 0;
	for(j; j < (sizeof(roomList)/sizeof(*roomList)); j++){
		printf("Room: %s Type: %d\n", roomList[j].name, roomList[j].type);
		printf("Room Connections:\n");
		int k = 0;
		for(k; k < roomList[j].outboundCount; k++){
			printf("%s\n", roomList[j].outboundConnect[k]);
		}
		printf("\n\n");
	}



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
	
		// set connection count
		roomList[i].outboundCount = 0;

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

// Returns true if all rooms have 3 to 6 outbound connections
bool IsGraphFull(struct room roomList[], size_t len){
	// local var(s)
	int i = 0;    // iterator thru room list

	// Testing
	printf("IsGraphFull\n");
	
	// iterate thru room list
	for(i; i < len; i++){
		if(roomList[i].outboundCount < 3){
			return false;
		}
	}
	return true;
}

void AddRandomConnection(struct room roomList[], size_t len){
	
	int roomA;    // hold index of two rooms
	int roomB;

	// Testing
	printf("AddRandomConnection\n");

	while(true){
		roomA = randomInt(0, 7); 

		if(CanAddConnectionFrom(roomList[roomA]) == true){
			break;
		}
	}

	do{
		roomB = randomInt(0, 7); 
	}
	while(CanAddConnectionFrom(roomList[roomB]) == false || roomA == roomB || ConnectionAlreadyExists(roomList[roomA], roomList[roomB]) == true);

	ConnectRoom(roomList, roomA, roomB);	// TODO: Add this connection to the real
	ConnectRoom(roomList, roomB, roomA);	// variable b/c this A and B will be destoryed
}

// Returns true if a connection can be added from room x (< 6 connections) false otherwise
bool CanAddConnectionFrom(struct room x){
	
	// Testing
	printf("CanAddConnectionFrom\n");

	if(x.outboundCount < 6){
		return true;
	}
	return false;
}

// Returns true if a connection from Foom x to Room y already exists, false
// otherwise
bool ConnectionAlreadyExists(struct room x, struct room y){
	// local var
	int i = 0;

	// Testing
	printf("ConnectionAlreadyExists\n");
	
	// iterate thru connection list of room x for room y
	// only need to do this 1 direction because connections
	// are made both ways
	for(i; i < x.outboundCount; i++){
		if(strcmp(x.outboundConnect[i], y.name) == 0){
			return true;
		}
	}
	return false;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct room roomList[], int roomX, int roomY){
	// local var
	int currentConnects = roomList[roomX].outboundCount;
	char* roomYName;
	memset(roomYName, '\0', sizeof(roomList[roomY].name));
	
	// Testing
	printf("ConnectRoom\n");

	// Get room Y name
	strcpy(roomYName, roomList[roomY].name);

	// Add room Y name to room X connection list
	strcpy(roomList[roomX].outboundConnect[currentConnects], roomYName);

	// Increment count for that room
	roomList[roomX].outboundCount = currentConnects + 1;
}
