/*
 * Word count application with one thread per input file.
 *
 * You may modify this file in any way you like, and are expected to modify it.
 * Your solution must read each input file from a separate thread. We encourage
 * you to make as few changes as necessary.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>

#include "word_count.h"
#include "word_helpers.h"

/*
 * main - handle command line, spawning one thread per file.
 */
struct attribs{
      FILE *fp;
      word_count_list_t *word_tracker;
};

void *call_add_word(struct attribs *at){
  char str[100];
  while (fscanf(at->fp, "%s", str) == 1){
    add_word(at->word_tracker,str);
  }
  return 0;
}

int main(int argc, char* argv[]) {
  /* Create the empty data structure. */
  word_count_list_t word_counts;
  init_words(&word_counts);
  if (argc <= 1) {
    /* Process stdin in a single thread. */
    count_words(&word_counts, stdin);
  } 
  else {
    /* TODO */
    struct dirent *de;
    pthread_t tid;
    DIR *dr;
    dr=opendir(argv[1]);
    if (dr == NULL){
      printf("Could not open current directory" );
      return 0;
    }
    struct attribs *attr = (struct attribs*)malloc(sizeof(struct attribs));
    char buffer[300];
    int length;
    while ((de = readdir(dr)) != NULL){
      FILE *file;
      length=sprintf(buffer,"%s%c%s",argv[1],'/',de->d_name);
      //printf("%s\n",buffer);
      char files[length];
      strcpy(files,buffer);
      file=fopen(files,"r");
      if(file == NULL) {
        perror("Error opening file");
        return(-1);
      }
      attr->fp=file;
      attr->word_tracker=&word_counts;
      pthread_create(&tid, NULL,(void*) call_add_word, attr);
      fclose(file);
    }
    closedir(dr);
  }
  
  /* Output final result of all threads' work. */
  wordcount_sort(&word_counts, less_count);
  fprint_words(&word_counts, stdout);
  //pthread_exit(NULL);
  return 0;
}
