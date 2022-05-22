//Noam lahmani 318868312
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/wait.h>

//submit this
int main(int argc,char* argv[]) {
	
	if (argc !=2){
		exit(-1);
	}
	
	DIR *dip;
	struct dirent *dit;
	int fdin;
    pid_t pid;
    DIR *dir2;
	fdin = open(argv[1],O_RDONLY);
	if (fdin < 0) {                
		perror("Error in:Open");
		exit(-1);
	}
	
	char buff[500];
    char path[151] = {0}, input[151] = {0}, output[151] = {0};
   	char *token;
	int sz1 = read(fdin, buff,sizeof(buff));
	if(sz1==-1){
		perror("Error in: read");
		exit(-1);
	}

  	token = strtok(buff,"\n");
  	strcpy(path, token);
  	token = strtok(NULL, "\n");
  	strcpy(input, token);
  	token = strtok(NULL, "\n");
  	strcpy(output, token);
  	

	int csvFile = open("results.csv", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	
	if (csvFile < 0) {                
		perror("Error in: open");
		exit(-1);
	}


  	if((dip = opendir(path)) == NULL){
		exit(-1);
	}

	if(access(input, F_OK) == -1) {
	perror("Input file not exists");
		exit(-1);
	}
	if(access(output, F_OK) == -1) {
	perror("Output file not exists");
		exit(-1);
	}
	int error_file = open("errors.txt", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (error_file < 0){
		perror("Error in: open");
		exit(-1);
	}
	if(dup2(error_file,2) < 0){
		perror("Error in: dup");
		exit(-1);
	}
	
	while ((dit = readdir(dip)) != NULL) {

		int has_file = 0;
		int status = 0;
		int check = 0;
		struct dirent *dit2;
		if(strcmp(dit->d_name,".") == 0 || strcmp(dit->d_name,"..") == 0){
			continue;
		}
		if (dit->d_type == DT_DIR){
			char newPath[350] = {0};
			strcat(newPath,path);
			strcat(newPath,"/");
			strcat(newPath,dit->d_name);
			struct dirent *dit2;
			if((dir2 = opendir(newPath)) == NULL){
				perror("Error in: opendir");  
				continue;
			}

		while ((dit2 = readdir(dir2)) != NULL) {
			int length = strlen(dit2->d_name);
			if(dit2->d_name[length-1] == 'c'  && dit2->d_name[length-2] == '.') {
				has_file = 1;
				if ((pid = fork()) < 0){
					perror("Error in: fork");   
					exit(-1);
				}
				else if (pid == 0){
					char PathToC[500] = {0};
					strcat(PathToC,newPath);
					strcat(PathToC,"/");
					strcat(PathToC,dit2->d_name);
					if(access(PathToC, F_OK) == -1){
					perror("Error in: access");
					}
					char *args[] = {"gcc", PathToC, NULL};
					if (execvp(args[0], args) < 0) {  
						perror("Error in: execvp");
					}
				}
				else {
					waitpid(pid, &status, 0);
					if (WIFEXITED(check)){
						if(WEXITSTATUS(check) != 0)
							check = -1;
				}
				if(access("a.out", F_OK) == -1) {
					check = -1;
				}
				
				else{
					int open_input = open(input, O_RDONLY);
					if(open_input == -1){

						perror("Error in: open");
						exit(-1);
					}
					
					int open_output = open("out.txt", O_CREAT|O_TRUNC|O_WRONLY,0777);
					

					if(open_output < 0){
						perror("Error in: open");
						exit(-1);
					}
					if ((pid = fork()) < 0){
						perror("Error in: fork");
						exit(-1);
					}
				
					//if child
					else if(pid == 0){
						if(dup2(open_input, 0) < 0){
							perror("Error in: dup2");
							exit(-1);
						}
						
						if(dup2(open_output, 1) < 0){
							perror("Error in: dup2");
							exit(-1);
						}
						
						char* run[] = { "./a.out", input, NULL };
						if(execvp(run[0], run) == -1) {
								perror("Error in: execvp");
						}
							
					}
					waitpid(pid, NULL, 0);
					
					close(open_input);
					close(open_output);
				}
				
					if ((pid = fork()) < 0){
						perror("Error in: fork");
						exit(-1);
					}
				
					//if child
					else if(pid == 0){
						char* ex21_args[] = { "./comp.out", "out.txt", output, NULL };
						if(execvp(ex21_args[0], ex21_args) == -1) {
								perror("Error in: execvp");
						}	
					}
					
					else {
						waitpid(pid, &status, 0);
						if (WIFEXITED(status)) {
							check = WEXITSTATUS(status);        
						}
					}
					if (remove("a.out") < 0) {
						perror("Error in: remove");
					}
				}
			}
		}
        }

		if(write(csvFile,dit->d_name,strlen(dit->d_name)) == -1){return 0;};	//write= no error
		if(!has_file) {
			if(write(csvFile,",0,NO_C_FILE\n",strlen(",0,NO_C_FILE\n") == -1)){return 0;}
		}
		
		else if(check == -1) {
			if(write(csvFile,",10,COMPILATION_ERROR\n",strlen(",10,COMPILATION_ERROR\n") == -1)){return 0;} 
		}
		
		else if(check == 2) {
			if(write(csvFile,",50,WRONG\n",strlen(",50,WRONG\n") == -1)){return 0;}
		}
		else if(check == 3) {
			if(write(csvFile,",75,SIMILAR\n",strlen(",75,SIMILAR\n") == -1)){return 0;}
		}	
		else if(check == 1) {
			if(write(csvFile,",100,EXCELLENT\n",strlen(",100,EXCELLENT\n") == -1)){return 0;}
		}
	}
	
	closedir(dip);
	close(fdin);
	close(error_file);
	close(csvFile);	
	if (remove("out.txt") < 0) {
        perror("Error in: remove");
    }

	}
	
	

	
	
	

