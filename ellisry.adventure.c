// TODO: header here...

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum roomType{ START_ROOM, END_ROOM, MID_ROOM};

// holds the name/type/outbound connections for a room
struct room{
	char name[9];    // 8 char length + null term
	enum roomType type;
	char outboundConnect[6][9];    // max connect of 6 + ea. entry 8 chars
	int outboundCount;    // helps keep count of number of connections	
};



// F(x) prototypes
void getDir(char* newestDir, size_t len);
void getFile(char* newestDirName, struct room roomList[], size_t len);

int main(){
	// Var for getting sub dir
	char* newestDirName = malloc(256 * sizeof(char));
	memset(newestDirName, '\0', sizeof(newestDirName));

	// Struct to hold room data
	struct room roomList[7]; // Holds the 7 rooms
	size_t listSize = (sizeof(roomList) / sizeof(*roomList));	

	/*
	// Vars for game
	int roomCount = 0; // Holds num of rooms player visits
	char* roomHistory[9]; // Holds history of rooms entered
	char currentRoom[9];
	char* userInput;	
	*/

	// Get most recent 'ellisry.rooms.' sub dir
	getDir(newestDirName, sizeof(newestDirName));

	// Populate random room list from files in sub dir
	
	 
	// Loop for game here...
	/*	
	while()
	*/

	// Clean up memory allocated
	free(newestDirName);

	return 0;
}

// Finds the most recent directory created with the 'ellisry.rooms.' prefix.
// Citation: Used Sect. 2.4 of class reading for reference.
// https://oregonstate.instructure.com/courses/1706555/pages/2-dot-4-manipulating-directories
void getDir(char *newestDirName, size_t len){
	// Local vars
	int newestDirTime = -1;    // holds the timestamp of curr dir analyzed
	char targetDirPrefix[32] = "ellisry.rooms.";  // prefix we want

	DIR* dirToCheck; // Holds the dir we're starting in
	struct dirent *fileInDir; // Hold current sudirectory of starting dir
	struct stat dirAttributes; // Holds data about sub dir

	dirToCheck = opendir("."); // Open up the directory prog is in

	if(dirToCheck > 0){ // Ensure cur dir can be opened
		while((fileInDir = readdir(dirToCheck)) != NULL){ // Check entry in dir
			if(strstr(fileInDir->d_name, targetDirPrefix) != NULL){
				stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry
				
				if((int)dirAttributes.st_mtime > newestDirTime){
					newestDirTime = (int)dirAttributes.st_mtime;
					memset(newestDirName, '\0', len);
					strcpy(newestDirName, fileInDir->d_name);
				}
			}
		}
	}
	
	closedir(dirToCheck);
}
