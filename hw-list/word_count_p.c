/*
 * Implementation of the word_count interface using Pintos lists and pthreads.
 *
 * You may modify this file, and are expected to modify it.
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

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_lp.c"
#endif

#ifndef PTHREADS
#error "PTHREADS must be #define'd when compiling word_count_lp.c"
#endif

#include "word_count.h"
#include "pthread.h"

void init_words(word_count_list_t* wclist) { /* TODO */
  list_init(&wclist->lst);
}

size_t len_words(word_count_list_t* wclist) {
  /* TODO */
  return list_size(&wclist->lst);
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  struct list_elem *e;
  for(e=list_begin(&wclist->lst);e!=list_end(&wclist->lst);e=list_next(e)){
    struct word_count *f=list_entry(e, struct word_count, elem);
    if(strcmp(f->word, word)==0){
      return f;
    }
  }
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  pthread_mutex_lock(&wclist->lock);
  struct word_count *wc1 = (struct word_count*)malloc(sizeof(struct word_count));
  struct list_elem *e;
  for(e=list_begin(&wclist->lst); e!=list_end(&wclist->lst); e=list_next(e)){
    struct word_count *f = list_entry(e, struct word_count, elem);
    if(strcmp(f->word, word)==0){
      f->count+=1;
      pthread_mutex_unlock(&wclist->lock);
      return f;
    }
  }
  wc1->count=1;
  wc1->word=(char*)malloc(64);
  strcpy(wc1->word,word);
  list_push_front(&wclist->lst,&wc1->elem);
  pthread_mutex_unlock(&wclist->lock);
  return wc1;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { /* TODO */
  struct list_elem *e;
  for(e=list_begin(&wclist->lst); e!=list_end(&wclist->lst); e=list_next(e)){
    struct word_count *f = list_entry(e, struct word_count, elem);
    fprintf(outfile, "%i\t%s\n", f->count, f->word);
  }
}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  struct word_count *wc1 = list_entry(ewc1, struct word_count, elem);
  struct word_count *wc2 = list_entry(ewc2, struct word_count, elem);
  if(strcmp(wc1->word, wc2->word)<0){
    return true;
  }
  else{
    return false;
  }
}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  /* TODO */
  list_sort(&wclist->lst, less_list, less);
}
