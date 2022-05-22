//Noam lahmani 318868312
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

//submit this
bool has_spaces(char buf[]){
    if(*buf == ' ' || *buf == '\n'){
        return true;
    }
    return false;
}

bool is_same_letter(char buf1[], char buf2[]){
    if('A'<=*buf1 && *buf1 <= 'Z'){
        if(*buf1 + 32 == *buf2){
            return true;
        }
    }
    if('a'<=*buf1 && *buf1 <= 'z'){
        if(*buf1 - 32 == *buf2){
            return true;
        }
    }
    return false;
}


int check_if_similar(int fd1, int fd2){
	char buff1[1], buff2[1];
	int sz1, sz2;
	if(lseek(fd1, 0, SEEK_SET) < 0){
        printf("Error in: lseek");
		exit(-1);
	}
	if(lseek(fd2, 0, SEEK_SET) < 0){
        printf("Error in: lseek");
		exit(-1);
	}
	while(true){
		sz1 = read(fd1, buff1,sizeof(buff1));
		if(sz1==-1){
			printf("Error in: read");
			exit(-1);
		}
		sz2 = read(fd2, buff2,sizeof(buff1));
		if(sz2==-1){
			printf("Error in: read");
			exit(-1);
		}
		while(has_spaces(buff1) && sz1!=0){
			sz1 = read(fd1, buff1,sizeof(buff1));
			if(sz1==-1){
				printf("Error in: read");
				exit(-1);
			}
		}
		while(has_spaces(buff2) && sz2!=0){
			sz2 = read(fd2, buff2,sizeof(buff2));
			if(sz2==-1){
				printf("Error in: read");
				exit(-1);
			}
		}
		if(sz1 == 0 && sz2 == 0){
			return 1;
		}
		if(buff1[0] != buff2[0]){
			if(!is_same_letter(buff1,buff2)){
				return 0;
			}
		}
	}
}

int check_if_equal(int fd1, int fd2){
	char buff1[150], buff2[150];
	int sz1, sz2;
	while(true){
		sz1 = read(fd1, buff1,sizeof(buff1));
		if(sz1==-1){
			printf("Error in: read");
			exit(-1);
		}

		sz2 = read(fd2, buff2,sizeof(buff1));
		if(sz2==-1){
			printf("Error in: read");
			exit(-1);
		}
		if(sz1 != sz2){
			return 0;
		}
		if(sz1 == 0){
			return 1;
		}
		int comp = strcmp(buff1,buff2);
		if(comp != 0){
			return 0;
		}
        }
}
int main(int argc, char** argv) {
    if (argc !=3){
       exit(-1);
    }
    bool is_identical = true;
    char buff1[1], buff2[1];
    int fd1, fd2, sz1, sz2;
    bool is_continue = true;

    fd1 = open(argv[1],O_RDONLY);
    if(fd1 == -1){
    	printf("Error in: open1");
        exit(-1);
    }

    fd2 = open(argv[2],O_RDONLY);
    if(fd2 == -1){
    	printf("Error in: open2");
        exit(-1);
    }

	if(check_if_equal(fd1, fd2)==1){
	close(fd1);
	close(fd2);
		return 1;
	};
	if(check_if_similar(fd1, fd2)==1){
	close(fd1);
	close(fd2);
		return 3;
	};
	close(fd1);
	close(fd2);
	return 2;

    while (!is_continue){
        //if the chars are identical - read the next char from the buff1 and buff2
        if(strcmp(buff1,buff2)==0){
            sz1 = read(fd1, buff1,sizeof(buff1));
            if(sz1 == -1){
            printf("Error in: read");
                exit(-1);
            }
            sz2 = read(fd2, buff2,sizeof(buff2));
            if(sz2 == -1){
              printf("Error in: read");
              exit(-1);
            }
        }
            //if the differnce is between lowercase and uppercase
        else if(is_same_letter(buff1,buff2)){
            is_identical = false;
            sz1 = read(fd1, buff1,sizeof(buff1));
            if(sz1 == -1){
              printf("Error in: read");
              exit(-1);
            }
            sz2 = read(fd2, buff2,sizeof(buff2));
            if(sz2 == -1){
              printf("Error in: read");
              exit(-1);
            }
        }

            //space in buf1 but no space in buf2 - then read the next char from buf1
        else if(has_spaces(buff1)){
            is_identical = false;
            sz1 = read(fd1, buff1,sizeof(buff1));
            if(sz1 == -1){
              printf("Error in: read");
              exit(-1);
            }
        }

            //space in buf2 but no space in buf1 - then read the next char from buf2
        else if(has_spaces(buff2)){
            is_identical = false;
            sz2 = read(fd2, buff2,sizeof(buff2));
            if(sz2 == -1){
              printf("Error in: read");
              exit(-1);
            }
        }
            //different
        else{
        close(fd1);
	close(fd2);
            return 2;
        }

        if(sz1 == -1 || sz2 == -1){
            is_continue = false;
        }

    }

    while(fd1 != -1){
        if(!has_spaces(buff1)){
        close(fd1);
            return 2;
        }
        sz1 = read(fd1, buff1,sizeof(buff1));
        if(sz1 == -1){
           printf("Error in: read");
           exit(-1);
        }
    }

    while (fd2 != -1){
        if(!has_spaces(buff2)){
	close(fd2);
            return 2;
        }
        sz2 = read(fd2, buff2,sizeof(buff2));
        if(sz2 == -1){
           printf("Error in: read");
           exit(-1);
        }
    }

    if(is_identical){
        close(fd1);
	close(fd2);
        return 1;
    }
    close(fd1);
    close(fd2);
    return 3;

}
