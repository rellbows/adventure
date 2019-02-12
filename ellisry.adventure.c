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
void loadData(char* fullPath, struct room roomList[], size_t len, int fileIndex);

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
	getFile(newestDirName, roomList, listSize);	
	
	// Testing
	int i = 0;
	for(i; i < listSize; i++){
		printf("%s\n", roomList[i].name);
		int j = 0;
		for(j; j < roomList[i].outboundCount; j++){
			printf("%s\n", roomList[i].outboundConnect[j]);
		}
		printf("%d\n\n", roomList[i].type);
	}
 
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

// Traverses thru the newest 'ellisry.rooms.' sub dir and calls on the 
// another function to load the room data into the list
void getFile(char* newestDirName, struct room roomList[], size_t len){
	// Local vars
	char currentFile[14]; // 8 max room name + '_room' + 1
	char fullPath[273]; // max sub dir + max filename + 2X '/' + '.'+ 1
	int fileIndex = 0; // keep track of number of files
	
	DIR* dirToCheck = opendir(newestDirName); // Sub dir we want to check
	struct dirent *fileInDir;
	struct stat dirAttributes; // Holds data about the sub dir

	if(dirToCheck > 0){
		while((fileInDir = readdir(dirToCheck)) != NULL){
			if(strstr(fileInDir->d_name, "_room") != NULL){
				stat(fileInDir->d_name, &dirAttributes); // Get the data
			
				memset(currentFile, '\0', sizeof(currentFile));
				strcpy(currentFile, fileInDir->d_name);
			
				// Testing
				printf("%s\n", currentFile);

				// Get full path in prep for passing to
				// our function that will load the data
				snprintf(fullPath, sizeof(fullPath), "./%s/%s", newestDirName, currentFile);

				// Testing
				printf("%s\n", fullPath);

				// Call our function to pull the data
				// from file and load into room list
				loadData(fullPath, roomList, len, fileIndex);

				// Increase file Index
				fileIndex++;
			}	
		}
	}
}

// Load data from specified file into the room list struct
void loadData(char* fullPath, struct room roomList[], size_t len, int fileIndex){
	// Local vars
	FILE* inputFile = fopen(fullPath, "r");
	char buffer[100]; // Should leave plenty of room for each line
	int count = 0; // Keep track of # of connections
	int i = 0; // Keep track line index

	// Check to ensure file opened sucessfully
	if(inputFile == NULL){
		perror("Error opening file");
		exit(1);
	}

	while(fscanf(inputFile, "%*[^:]: %s", buffer) == 1){
		if(i == 0){
			strcpy(roomList[fileIndex].name, buffer);
		}
		else{
			if(strcmp(buffer, "START_ROOM") == 0){
				roomList[fileIndex].type = 0;
			}
			else if(strcmp(buffer, "MID_ROOM") == 0){
				roomList[fileIndex].type = 2;
			}
			else if(strcmp(buffer, "END_ROOM") == 0){
				roomList[fileIndex].type = 1;
			}
			else{
				strcpy(roomList[fileIndex].outboundConnect[count], buffer);
				roomList[fileIndex].outboundCount = ++count;
			}
		}
		
		// Increase line index
		i++;
	}
	
	// Close out stream
	fclose(inputFile);
}
