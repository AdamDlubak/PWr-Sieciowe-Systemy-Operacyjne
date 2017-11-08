#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){ 
	int **pipes;			/* Tablica potoków */
	char **params;			/* Tablica parametrów funkcji */
	char* function;			/* Nazwa (ścieżka do) funkcji */
	int subprocess = 0;		/* Liczba podprocesów do wykonania */
	int instruction = 0;	/* Liczba instrukcji do wykonania */
	int name = 0;			/* Czy podany parametr jest nazwą funkcji, czy parametrem */
	int funcAmt = 0;		/* Liczba parametrów */
	int j = 1;
	int i, p, f, k, result, status, param;
	
	/* Zliczanie liczby funkcji do wywołania */
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], ",") == 0) subprocess++;
	}
	subprocess++;
	
	/* Tworzenie potoków - o 1 mniej niż funkcji do wywołania */
	pipes = malloc((subprocess-1)*sizeof(int*)); 
	for(i = 0; i < subprocess - 1; i++) {
		 pipes[i] = malloc(2*sizeof(int));
		 p = pipe(pipes[i]);
		 if(p == -1) { perror("Error pipe()"); exit(0); } 
	} 


	/* Parsowanie poszczególnych parametrów polecenia */
	for(i = 1; i < argc; i++) {
		/* Sprawdzanie czy jest to nazwa (scieżka do) funkcji, czy parametr */
		if(name == 0){
			param = 0;
			/* Zliczanie parametrów funkcji i tworzenie dla nich tablicy */
			for(; j < argc; j++) {
				if(strcmp(argv[j], ",") == 0) break;
				else param++;
			}
			params = malloc((param+1)*sizeof(char*));
			params[param] = NULL;

			function = argv[i];
			name = 1;
			instruction++;
		}
		else { name = 2; }
		
		/* Odczytanie ostatniego parametru i/lub wykonanie operacji*/
		if ((strcmp(argv[i], ",") == 0) || (i+1) == argc) {
			if((i+1) == argc) {
				params[funcAmt] = argv[i];
				funcAmt++;	
			}

			name = 0;
			funcAmt = 0;
	
			/* Scenariusz 1/3: Pierwsza funkcja */
			if(instruction == 1){
				switch(f = fork()) {
					case -1:
						perror("Error fork()"); exit(0);
					break;

					case 0:
					for(k = 0; k < subprocess - 1; k++) {
						if(k != (instruction-1)){
							close(pipes[k][0]);		
							close(pipes[k][1]);	
						}	
					}
					if(instruction == subprocess){
						execv(function, params); 	
					} else {
						close(pipes[instruction-1][0]);
						
						result = dup2(pipes[instruction-1][1], 1);
						if(result == -1) { perror("Error dup2()"); exit(0); }
						execv(function, params); 
					}
						exit(0); 
					break;
				}
			/* Scenariusz 2/3: Ostatnia funkcja */	
			} else if (instruction == subprocess) {
				switch(f = fork()) {
					case -1:
						perror("Error fork()"); exit(0);
					break;

					case 0:
					for(k = 0; k < subprocess - 1; k++) {
						if(k != (instruction-2)){
							close(pipes[k][0]);		
							close(pipes[k][1]);	
						}	
					}
					close(pipes[instruction-2][1]);
					
						result = dup2(pipes[instruction-2][0], 0);
						
						if(result == -1) { perror("Error dup2()"); exit(0); }
						execv(function, params);
						exit(0);
					break;
				} 
			/* Scenariusz 3/3: Funkcje pośrednie */
			} else {
				switch(f = fork()) {
					case -1:
						perror("Error fork()"); exit(0);
					break;

					case 0:
					for(k = 0; k < subprocess - 1; k++) {
						if(k != (instruction-1) && k != (instruction-2)){
							close(pipes[k][0]);		
							close(pipes[k][1]);	
						}
					}
					close(pipes[instruction-2][1]);
					close(pipes[instruction-1][0]);
					
						result = dup2(pipes[instruction-2][0], 0);			
						if(result == -1) { perror("Error dup21()"); exit(0); }
					
					
						result = dup2(pipes[instruction-1][1], 1);
						if(result == -1) { perror("Error dup22()"); exit(0); }
	
						execv(function, params);
						exit(0);
					break;
				} 
			}
		/* Parsowanie parametru */
		} else if (name == 2) {
			params[funcAmt] = argv[i];
			funcAmt++;	
		}
			
	}

		/* Zamykanie strumieni */
		for(i = 0; i < subprocess - 1; i++){
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
			
		/* Zwalnianie pamięci */
		for(i = 0; i < subprocess - 1; i++){
			free(pipes[i]);				
		}
		free(pipes);


/* Kończenie procesów */	 
	p = wait(&status);
	if(p == -1) { perror("Error wait()"); exit(0); }
	exit(0);

	return 1;
} 