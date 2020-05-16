//Ottenere l'elenco dei file di una directory: fare riferimento a questo esempio:
//https://github.com/marcotessarotto/exOpSys/tree/master/023listFiles
//
//parte 1 - trova il file regolare più grande cercandolo all'interno di una directory
//parte 2 - trova il file regolare più grande cercandolo all'interno di una directory e ricorsivamente in tutte le sue
//			sotto-directory
//
//scrivere la seguente funzione:
//char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, int * largest_file_size);
//
//la funzione restituisce il percorso completo del file regolare più grande cercato nella directory specificata da
//directory_name.
//se explore_subdirectories_recursively != 0, allora cerca ricorsivamente in tutte le sue sotto-directory.
//(a parità di dimensioni, considera il primo trovato).
//
//la dimensione del file regolare più grande viene scritta nella variabile il cui indirizzo è dato da largest_file_size.
//se non ci sono file regolare, la funzione restituisce NULL (e largest_file_size non viene utilizzato).
//
//
//provare a fare girare il programma a partire da queste directory:
///home/utente

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, int * largest_file_size);

int main(int argc, char *argv[]) {
	char * fileName = "/home/utente";

	int largest_file_size = 0;
	char * largest_file = find_largest_file(fileName, 0, &largest_file_size);

	printf("\nThe largest file in the directory is: \n");
	printf("%-10s %s  -->  size: %d bytes", "regular", largest_file, largest_file_size);

	return EXIT_SUCCESS;
}

char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, int * largest_file_size){
	DIR * dir_stream_ptr;
	struct dirent *ep;
	char * largest = "NULL";
	errno = 0;

	dir_stream_ptr = opendir(directory_name);
	if (dir_stream_ptr == NULL) {
		printf("cannot open directory %s! bye", directory_name);
		exit(EXIT_FAILURE);
	}

	char* file_name;

	while ((ep = readdir(dir_stream_ptr)) != NULL) {
		if(directory_name[strlen(directory_name)-1] != '/'){
			file_name = malloc(strlen(directory_name) + strlen(ep->d_name) + 2);
		}
		else{
			file_name = malloc(strlen(directory_name) + strlen(ep->d_name) + 1);
		}
		file_name[0] = '\0';
		strcat(file_name, directory_name);
		if(directory_name[strlen(directory_name)-1] != '/')
			strcat(file_name, "/");
		strcat(file_name, ep->d_name);


		struct stat *statbuf;
		stat(directory_name, &statbuf);
		if((ep->d_type == DT_REG)&&((statbuf->st_size)>*largest_file_size)){
			*largest_file_size = statbuf->st_size;
			largest = file_name;
		}
		if((ep->d_type == DT_DIR)&&(explore_subdirectories_recursively != 0)){
			int size = 0;
			char * sub_largest = find_largest_file(file_name, 1, &size);
			if((sub_largest != NULL) && (size > *largest_file_size)){
				largest = sub_largest;
				*largest_file_size = size;
			}
		}
	}

	if (errno) {
		perror("readdir() error");
	}

	closedir(dir_stream_ptr);
	return largest;
}
