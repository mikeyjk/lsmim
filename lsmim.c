#include <dirent.h> // defines file descriptor
#include <stdio.h> // defines printf etc
#include <stdlib.h> // defines malloc
#include <string.h> // defines strncpy, strcat etc
#include <sys/stat.h> // defines ST_MODE file permissions
#include <time.h> // I nearly lost my mind over a seg fault issue, for some reason, if you use ctime() it compiles even without this header but just segs
#include <pwd.h> // used to retrieve the user and group name - using st_uid and st_guid from the struct stat
#include <grp.h> // used to store information about a group 
#include <limits.h> // used to allocate memory for char* arrays

#define MAXBUF CHAR_MAX // maximum size of a char

/**
 * ICT310
 * Assignment 1 - Question 6
 * 'myls'
 * Date: 09/09/13
 * Author: Michael J. Kiernan
 * Student Number: 31008429
 * 
 * */

const char *defaultPath = "./"; // default directory -> current working directory
const char *slash = "/"; // used a fair bit

void printFileInformation(char * file); // print file information in the way requested by q6
void printSimpleInformation(char * file); // print file information mimicing 'ls -a -l'
void printPermissions(mode_t *file); // prints out permissions of a file
void userPermission(mode_t *user);// prints out user permissions
void groupPermission(mode_t *group); // prints out group permissions
void otherPermission(mode_t *other); // prints out other permissions
void printTime(time_t* amcTime); // print the time
int containsSlash(char * check); // checks if a char* contains a '/' in it at all
void storeDirectory(char * string, char  * directory, size_t size); // formats a string so it points to the directory correctly
void storeFile(char* string, char* directory, size_t size); // formats a string so it points to the directory and the file correctly
void regularFile(mode_t *file); // prints d if a directory or - if a file
int isDirByName(char * file); // returns 1 if a directory 0 if not

void printSeperator(); // print " | "
void printNewLine(); // print "\n"
void printSpace(); // print ' '
void printPipe(); // print "|"

/**
* Mimics the ls output of -ls -a -l
* I had the most annoying bug for soo long where the filenames were garbage.
* I knew it was a GIGO error but it took me a while to realise I had to not only allocate memory for 
* a char ** fileNames, but each pointer that fileNames points to. which makes sense now, but didn't then.
*/
void simpleLS(char* location)
{
	DIR *d; // directory pointer

	struct dirent *dir; // file pointer

	char *userPath; // temporary string that will get filled with the path name
	char *filePath; // combination of the user supplied path and the filename
	
	char** fileNames; // array of char*'s with file names, this is used so that the output order can be controlled to mimic ls
	
	int i = 0; // used to iterate through file names
	int cwd = -1; // index of current working directory in fileNames[]
	int pwd = -1;  // index of parent directory in fileNames[]
	int files = 0; // counter for printing information
	
	int deb = 0;

	userPath = malloc(MAXBUF * sizeof(char *)); // set aside memory for path name
	fileNames = malloc(2 * (MAXBUF * sizeof(char *))); // set aside memory for file names, ** so multiplied by two

	storeDirectory(userPath, location, (size_t)(MAXBUF * sizeof(char *))); // append a '/' if necessary
		
	d = opendir(userPath); // open file stream

	if(d) // if d is a valid pointer - aka a valid file location
	{ 
		while((dir = readdir(d))) // while there is another file pointer to be passed on
		{
			fileNames[files] = malloc(MAXBUF * sizeof(char)); // set aside memory for file name pointer

			if(strcmp(dir->d_name, ".") == 0) // check for file "." / current working directory, this is done to mimic the ls output
			{
				cwd = files;
			}

			if(strcmp(dir->d_name, "..") == 0) // check for file ".." / parent directory, this is done to mimic the ls output
			{	
				pwd = files;
			}
			
			strcpy(fileNames[files], dir->d_name);
			
			files++;
		}
		
		// total, not implemented successfully
		printf("shallow total %d", files);

		// print '.' and '..' first
		if(cwd != -1) // '.'
		{
			filePath = (char*) calloc(MAXBUF, sizeof(char *)); // set aside memory for path name, clearing the memory also
			
			strncpy(filePath, userPath, (MAXBUF * sizeof(char *))); // copy the path to filePath " " + "./" = "./"

			strcat(filePath, fileNames[cwd]); // append the filename to the path name "./" + "test.c" = "./test.c"
			
			printSimpleInformation(filePath);	
			printSpace();
			printf("%s", fileNames[cwd]);
			
			printSpace();
			
			free(filePath); // clear the path name because the file name gets appended to it
		}
		
		if(pwd != -1) // '..'
		{
			filePath = (char*) calloc(MAXBUF, sizeof(char *)); // set aside memory for path name, clearing the memory also
			
			strncpy(filePath, userPath, (MAXBUF * sizeof(char *))); // copy the path to filePath " " + "./" = "./"
			
			strcat(filePath, fileNames[pwd]); // append the filename to the path name "./" + "test.c" = "./test.c"

			printSimpleInformation(filePath);
			printSpace();
			printf("%s", fileNames[pwd]);
			
			free(filePath); // clear the path name because the file name gets appended to it
		}
		
		// ls is alphabetical but I can't really be bothered implementing this
		for(i = 0; i < files; i++) // iterate through all files
		{
			if(i != cwd && i != pwd) // so long as it is not the cwd or pwd as we have printed them all ready
			{
				filePath = (char*) calloc(MAXBUF, sizeof(char *)); // set aside memory for path name, clearing the memory also
			
				strncpy(filePath, userPath, (MAXBUF * sizeof(char *))); // copy the path to filePath " " + "./" = "./"
			
				strcat(filePath, fileNames[i]); // append the filename to the path name "./" + "test.c" = "./test.c"

				printSimpleInformation(filePath);
				printSpace();
				printf("%s", fileNames[i]);			

				free(filePath); // clear the path name because the file name gets appended to it
			}
		}
	} 
	else
	{
		printf("\nEither a file or invalid directory.\n");
	}
	
	printNewLine();
	
	free(userPath);
	free(fileNames);
	
	closedir(d); // close file stream
}

// q6 answer
void expandedLS(char* location)
{
	DIR *d; // directory pointer

	struct dirent *dir; // file pointer

	char *userPath; // temporary string that will get filled with the path name
	char *filePath; // combination of the user supplied path and the filename

	userPath = malloc(MAXBUF * sizeof(char *)); // set aside memory for path name

	storeDirectory(userPath, location, (size_t)(MAXBUF * sizeof(char *))); // append a '/' to the path name so opendir can find it
		
	d = opendir(userPath); // open file stream

	if(d) // if d is a valid pointer - aka a valid file location
	{ 
		while((dir = readdir(d))) // while there is another file pointer to be passed on
		{
			filePath = (char*) calloc(MAXBUF, sizeof(char *)); // set aside memory for path name, clearing the memory also
			
			strncpy(filePath, userPath, (MAXBUF * sizeof(char *))); // copy the path to filePath " " + "./" = "./"
			
			strcat(filePath, dir->d_name); // append the filename to the path name "./" + "test.c" = "./test.c"
			
			printNewLine(); printPipe(); printSpace();
			
			printf("%s", dir->d_name); // do this here as opposed to printFileInformation so it doesn't include path like ls
			
			printSeperator();
			
			printFileInformation(filePath); // print info about the file
			
			free(filePath); // clear the path name because the file name gets appended to it
		}
	} 
	else
	{
		printf("\nEither a file or invalid directory.\n");
	}
	
	closedir(d); // close file stream
}

/**
* Checks for cmd line arguments.
* If there is one and it is -s or -S it prints the default directory in the simplified format.
* If there is 2 and the second is -s or -S it prints the supplied directory in the simplified format.
* If there is one it prints the requested format from q6 using the supplied directory.
*/
int main(int argc, char ** argv)
{
	if(argc > 1)  // command line arguments provided
	{	
		if( (strcmp(argv[1], "-s") == 0) || (strcmp(argv[1], "-S") == 0) ) // presents simple 'ls' format, a bit like ls but stripped down, for debugging/testing success accessing dirs and files
		{
			simpleLS((char*)defaultPath);
		}
		else
		{
			if(argv[2] != NULL)
			{ 
				if( (strcmp(argv[2], "-s") == 0) || (strcmp(argv[2], "-S") == 0) ) // directory provided and simple format requested
				{
					simpleLS(argv[1]);
				}
			}
			else
			{
				expandedLS(argv[1]); // meets the requirements of q6
			}
		}
	}
	else 
	{
		expandedLS((char*)defaultPath);
	}
	return(0);
}

/**
 * Prints out the time in 24 hour format.
 * Explicitly defined output using strftime.
 * Prints it out like: '14:45 - 01/01/13'.
 * 
 * */
void printTime(time_t* amcTime)
{
	// used to output the time/format the time
	
	char buffer[MAXBUF]; // used to format the date string
	
	struct tm * timeinfo; // used to store formatted date string
	time_t tempTime = 0;
	
	// access date, modification date, status date
	
	timeinfo = localtime(amcTime);
		
	strftime(buffer, 80, "%H:%M - %x", timeinfo);
		
	printf("%s", buffer);
}

void printNewLine()
{
	printf("\n");
}

/**
 * Prints out information regarding a file.
 * */
void printSimpleInformation(char * file)
{
	char * tempString = malloc(MAXBUF * sizeof(char *));
	
	DIR *subd; // file descriptor pointer

	int error = 0; // 1 = error
	int fileCount = 0; // amount of files

	struct dirent *subdir; // struct to fill with i-node number and name etc
	
	struct stat fileInfo; // information about a file
	
	struct passwd *user_name; // user id struct
	struct group *grp; // group struct

	error = lstat(file, &fileInfo); // store data about a file (files[i]) in &fileInfo
	
	if(error == 0)  // filled stat structure successfully
	{
		printNewLine();

		regularFile(&fileInfo.st_mode);
		printPermissions(&fileInfo.st_mode);	

		printSpace();
		printf("%d", (int)fileInfo.st_nlink);

		user_name = getpwuid(fileInfo.st_uid);
		grp = getgrgid(fileInfo.st_gid);
			
		printSpace();

		printf("%s", user_name->pw_name);

		printSpace();

		printf("%s", grp->gr_name);

		printSpace();

		printf("%ld", (long)fileInfo.st_size);

		printSpace();

		printTime(&fileInfo.st_atime);
				
	}
	else
	{
		printf("\nfile: %s", file);
		printf("\nInvalid directory. Eg; '../', '.', 'test/'.");
	}
	free(tempString);

	return;
}

/**
 * Prints out information regarding a file.
 * */
void printFileInformation(char * file)
{
	char * tempString = malloc(MAXBUF * sizeof(char *));
	
	DIR *subd; // file descriptor pointer

	int error = 0; // 1 = error
	int fileCount = 0; // amount of files

	struct dirent *subdir; // struct to fill with i-node number and name etc
	
	struct stat fileInfo; // information about a file
	
	struct group *grp; // group struct
	struct passwd *user_name; // user id struct

	error = lstat(file, &fileInfo); // store data about a file (files[i]) in &fileInfo
	
	if(error == 0)  // filled stat structure successfully
	{


		printNewLine();
		printPipe();
		printSpace();
		regularFile(&fileInfo.st_mode);
		printPermissions(&fileInfo.st_mode);	

		printSeperator();
		printf("%d", (int)fileInfo.st_nlink);

		user_name = getpwuid(fileInfo.st_uid);
		grp = getgrgid(fileInfo.st_gid);
			
		printSeperator();

		printf("%s", user_name->pw_name);

		printSeperator();

		printf("%s", grp->gr_name);

		printSeperator();

		printf("Size: %ld", (long)fileInfo.st_size);

		printSeperator();

		printf("FSDevice: %lld, %lld", (long long)(major(fileInfo.st_dev)), (long long)(major(fileInfo.st_dev)));
		printSeperator();
		printf("Inode: %ld", (long)fileInfo.st_ino);
		
		printSeperator(); printNewLine(); printPipe(); printSpace();
		
		
		printf("DeviceNum: %lld, %lld", (long long)(major(fileInfo.st_rdev)), (long long)(major(fileInfo.st_rdev)));
		
		printSeperator();
		
		printf("BS: %ld", (long)fileInfo.st_blksize);
		
		printSeperator();
		
		printf("512B: %ld", (long)fileInfo.st_blocks);
		
		printSeperator();
		printf("Access: ");
		printTime(&fileInfo.st_atime);
		
		printSeperator();				
		printf("Modified: ");
		printTime(&fileInfo.st_mtime);
		
		printSeperator();
		printf("Status: ");
		printTime(&fileInfo.st_ctime);	
		
		printSeperator();
		
		printNewLine();

		if(isDirByName(file)) // if the file is a directory
		{ 
			subd = opendir(file);
			
			if(subd != NULL) // I thought the while would have caught this, but it doesn't! and I was segfaulting on root directory
			{
				while((subdir = readdir(subd)) != NULL) 
				{
					printf("---->");; printSpace();
					printf("%d: inode: %d - name: %s\n", fileCount + 1, (int)subdir->d_ino, subdir->d_name);
					fileCount++;
				}
			}

			closedir(subd);
			printf("| (dir containing %d files) |", fileCount);
			printNewLine();
		}

	}
	else
		printf("\nInvalid directory. Eg; '../', '.', 'test/'.");
		
	free(tempString);

	return;
}

/**
Calls sub-routines to print out the permissions.
Eg; -rwxrwxrwx etc.
*/
void printPermissions(mode_t *file)
{
	userPermission(file);
	groupPermission(file);
	otherPermission(file);
}

/**
 * Checks if a string contains a slash.
 * If it does, it returns 1, otherwise it returns 0.
 * */
int containsSlash(char * check)
{
	int found = 0;
	int i = 0;

	for(i = 0; i < strlen(check); i++) // for the length of the string, if a slash is detected return 1
	{
		if(!(strcmp(&check[i], slash)))
			found = 1;
	}

	return(found); 
}
	
/**
 * Append a directory to a string.
 * Eg; "folder/" and wish to add another directory "folder2/" that is within folder.
 * This function would create a string: "folder/folder2/".
 * If the initial string is "folder" and does not contain a slash, one is appended, so: "folder/".
 * If the secondary string does not contain a slash it is also appended, eg: "folder/" + "folder2" would become "folder/folder2/".
 * */
void storeDirectory(char* changed, char* directory, size_t size) // formats a string so it points to the directory correctly
{
	if(!containsSlash(changed)) // check if the pre existing directory contains a slash
		strcat(changed, slash); // append a slash - this is required
	
	strncpy(changed, directory, size); // copy the user provided directory to tempString

	if(!containsSlash(directory)) // check if the directory all ready contains a slash
		strcat(changed, slash); // append a slash - this is useful 

	return;
}

/**
 * Append a file to a string.
 * Eg; "folder/" and wish to add a file eg "test.c".
 * It would then create a string called "folder/test.c".
 * Very similar to 'storeDirectory' but omits the last slash which denotes a directory.
 * */
void storeFile(char* changed, char* file, size_t size) // formats a string so it points to the directory correctly
{
	if(!containsSlash(changed)) // check if the pre existing directory contains a slash
		strcat(changed, slash); // append a slash - this is required
	
	strncpy(changed, file, size); // copy the user provided directory to tempString

	return;
}

/** 
Prints out whether the file is considered a directory or a file.
*/
void regularFile(mode_t *file)
{
	if(S_ISDIR(*file) != 0)
		printf("d");
	
	if(S_ISLNK(*file) != 0)
		printf("l");
		
	if(S_ISBLK(*file) != 0)
		printf("b");
	
	if(S_ISFIFO(*file) != 0)
		printf("p");
	
	if(S_ISCHR(*file) != 0)
		printf("c");
	
	if(S_ISSOCK(*file) != 0)
		printf("s");
					
	if(S_ISREG(*file) != 0)
		printf("-");

	return;
}

int isDirByName(char * file)
{
	int isDir = 0;

	struct stat fileInfo; // information about a file

	lstat(file, &fileInfo); // store data about a file (files[i]) in &fileInfo

	if(fileInfo.st_mode & S_IFDIR)
		isDir = 1;

	return(isDir);
}

int isDir(mode_t *file)
{
	int isDir = 0;

	if(*file & S_IFDIR)
		isDir = 1;
	else
		isDir = 0;

	return(isDir);
}

/** 
Prints out the permissions for user group 'user'.
*/
void userPermission(mode_t *user)
{
	if(*user & S_IRUSR)
		printf("r");
	else
		printf("-");

	if(*user & S_IWUSR)	
		printf("w");
	else
		printf("-");

	if(*user & S_IXUSR) 
		printf("x");
	else
		printf("-");

	return;
}

/** 
Prints out the permissions for user group 'group'.
*/
void groupPermission(mode_t *group)
{
	if(*group & S_IRGRP)
		printf("r");
	else
		printf("-");

	if(*group & S_IWGRP)
		printf("w");
	else
		printf("-");

	if(*group & S_IXGRP)
		printf("x");
	else
		printf("-");

	return;
}

/** 
Prints out the permissions for user group 'other'.
*/
void otherPermission(mode_t *other)
{
	if(*other & S_IROTH)
		printf("r");
	else
		printf("-");

	if(*other & S_IWOTH)
		printf("w");	
	else
		printf("-");

	if(*other & S_IXOTH)
		printf("x");
	else
		printf("-");

	return;
}

void printSpace()
{
	printf(" ");
}

void printPipe()
{
	printf("|");
}

void printSeperator()
{
	printf(" | ");
}
