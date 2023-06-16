#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#define MAX_FILENAME_LENGTH 256
#define OUTPUT_FILE "test.txt"

int is_file_exists(const char *filename) {
    int file = open(filename, O_RDONLY);
    if (file == -1) {
        return 0;
    }
    close(file);
    return 1;
}

void *play_sound(void *filename) {
	char command[256];
	sprintf(command, "aplay %s > /dev/null 2>&1", (char *)filename);
	system(command);
	return NULL;
}

void change_sound_file(char *sound_filename) {
    while (1) {
        printw("\nEnter the new filename: ");
        getnstr(sound_filename, MAX_FILENAME_LENGTH);

        if (is_file_exists(sound_filename)) {
        	printw("Done.\n");
            break;
        } else {
            printw("File does not exist. Please try again.\n");
        }
    }
}




int main() {
	bool score = false;
	FILE *outputFile;

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
//	noecho();

	printw("Press: 'q' to quit.\n");
	printw("Press: 'c' to change sound.\n");
	printw("Press: 'm' to save score.\n");
	refresh();

	char sound_filenames[9][MAX_FILENAME_LENGTH] = {
        "RISER.wav",
        "DOUBLEKICK.wav",
        "KICK.wav",
        "SNARE.wav",
        "1.wav",
        "2.wav",
        "3.wav",
        "4.wav",
        "5.wav"
	};

	pthread_t threads[9];
	char keys[9] = {'a', 's', 'd', 'f', 'j', 'i', 'o', 'k', 'l'};
	clock_t start, end;
    double cpu_time_used;

	printw("you can use 'asfdjiol' for playing.\n");


	while (1) {


		int ch = getch();
		start = clock();

		if (score) {
			fputc(ch, outputFile);
		}

        if (ch=='a'){
            pthread_create(&threads[0], NULL, play_sound, sound_filenames[0]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
        }
        else if (ch=='s'){
            pthread_create(&threads[1], NULL, play_sound, sound_filenames[1]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
        }
        else if (ch=='d'){
            pthread_create(&threads[2], NULL, play_sound, sound_filenames[2]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
        }
        else if (ch=='f'){
            pthread_create(&threads[3], NULL, play_sound, sound_filenames[3]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
        }
		else if (ch == 'j') {
			pthread_create(&threads[4], NULL, play_sound, sound_filenames[4]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
		}
		else if (ch == 'i') {
			pthread_create(&threads[5], NULL, play_sound, sound_filenames[5]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
		}
		else if (ch == 'o') {
			pthread_create(&threads[6], NULL, play_sound, sound_filenames[6]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
		}
		else if (ch == 'l') {
			pthread_create(&threads[7], NULL, play_sound, sound_filenames[7]);
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printw("\nExecution time: %f seconds\n", cpu_time_used);
        } else if (ch == 'k') {
            pthread_create(&threads[8], NULL, play_sound, sound_filenames[8]);
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            printw("\nExecution time: %f seconds\n", cpu_time_used);
        }
        else if (ch=='q'){
            break;
        }
        else if(ch=='c'){
            printw("\nWhich key do you want to change?\nPress 'c' to exit.\n");
			int ch2 = getch();
			for (int i = 0; i < 4; i++) {
				if (ch2 == keys[i]) {
					change_sound_file(sound_filenames[i]);
					break;
				}
				else if (ch2 == 'c') {
					printw("\nChange canceled.\n");
					break;
				}
			}
			printw("invalid key\n");
        }
        else if(ch=='m'){
            if (!score) {
				outputFile = fopen(OUTPUT_FILE, "w");
				if (outputFile == NULL) {
					printf("Error opening output file\n");
					return 1;
				}
				score = true;
			}
			
        }


		
	}


	if(score){
	fclose(outputFile);
	}
    

    endwin();
    return 0;
}
