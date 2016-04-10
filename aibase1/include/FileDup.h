/*
 * Author: hace <qianshanhai At hotmail Dot com>
 * Date:   2008.11.25
 * Update: 2008.12.11
 */

#ifndef __FILEDUP_H
#define __FILEDUP_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>

#include <THash.h>

#define FDUP_SEMNUM 256 /* Warning: don't modify */

#define FDUP_SUCCESS 0
#define FDUP_EXIST -1
#define FDUP_NOTFOUND -2
#define FDUP_SYSERROR -3

#define FDUP_ADD_REC_TOTAL 10*10000
#define FDUP_INIT_REC_TOTAL 50*10000

#define FDUP_DESTORY_FLAG 0x1
#define FDUP_REALLOC_FLAG 0x2

typedef struct fshm_head_t {
	int total; 		/* record total */
	int hash;		/* hash seg size */
	int data; 		/* data seg size */
	int used; 		/* data seg used */
	int flag; 		/* destroy flag */
	size_t size; 	/* mmap size */
	time_t mtime; 	/* last change time */
} fshm_head_t;

#define _FDUP_KEY_LEN 12 /* Warning: don't modify */

typedef struct fshm_rec_t {
	char key[_FDUP_KEY_LEN]; /* key */
	unsigned int next; 		/* value and next */
} fshm_rec_t;

typedef struct fshm_mark_t {
	unsigned int crc32;
	unsigned int reverse_crc32;
	int len;
} fshm_mark_t;

#define FDUP_ACTIVED 0x1

typedef struct fshm_st {
	int index;
	fshm_mark_t key; 	/* mark key */
	char path[256]; 	/* shm path */
	char file[256]; 	/* dup file path */
	int status; 		/* */
	int fd; 			/* shm fd */
	int out_fd; 		/* dup file fd */
	void *start; 		/* mmap file address */
	size_t size; 		/* old shm fd size: mmap size */
	struct fshm_head_t *head; 	/* hash head */
	struct fshm_rec_t *rec; 	/* hash record start address */
} fshm_t;

#define FDUP_FREC_USED 0x0
#define FDUP_FREC_DELETED 0x1
/*
 * check duplicate file struct
 * sizeof(file_dup_t) = 20
 */
typedef struct file_dup_t {
	char key[_FDUP_KEY_LEN]; /* check duplicate key */
	unsigned short pad0;
	unsigned char flag; 	/* = FDUP_FREC_USED used, FDUP_FREC_DELETED deleted */
	unsigned char len; 		/* key length */
	unsigned int time; 		/* process time */
} file_dup_t;

class FileDup {
	private:
		char m_errmsg[256];
		char m_root_path[256];
		char m_module_path[256];
		char m_module_name[64];
		char m_mark_file[256];

		THash <int> *m_hash;
		key_t m_key;
		int m_semid;

		fshm_t m_shm[FDUP_SEMNUM];
		fshm_t *m_mark_shm;

		int destory_time; 			/* in second */
		time_t last_check_time;

		int m_init_total;
		int m_add_total;

	private:
		void init_fshm_t(fshm_t *p, int idx);
		int get_sem();
		int create_sem();

		int get_sem_indx(const char *mark);
		int sem_lock_op(int idx, short int op);
		int lock_by_index(int idx);
		int unlock_by_index(int idx);

		char *mark_file(const char *mark);
		int find_mark(const char *mark);
		int add_mark(const char *mark);
		int get_mark_index(fshm_t *f);
		int get_sem_index(const char *mark);

		fshm_mark_t *get_key_by_mark(fshm_mark_t *p, const char *mark);
		int get_fname_by_mark(char *fname, int len, const void *key);

		char *get_shm_file(char *buf, int len, int index);
		void print_shm(const fshm_t *f);

		fshm_t *get_fshm_t(const char *mark, int idx = -1);
		int check_shm();
		int init_dup_file(fshm_t *f, bool init);
		int init_shm(fshm_t *f, const char *file, int total);
		int init_all_shm();
		int shm_find(fshm_t *f, const void *key, int size, fshm_rec_t **ret,
					unsigned int mask, unsigned int mov);
		int shm_add(fshm_t *f, const void *key, int size,
					unsigned int mask, unsigned int mov, unsigned int value);
		int shm_delete(fshm_t *f, const void *key, int size,
					unsigned int mask, unsigned int mov);

		int realloc_shm(fshm_t *f, int inc_rec_total);
		int get_shm(fshm_t *f, const char *path, size_t size);
		int delete_shm(fshm_t *f);

		int delete_record_from_shm(const char *mark, const void *key, int size);
		int delete_record_from_file(const char *mark, const void *key, int size);

	public:
		FileDup();
		~FileDup();

		unsigned int crc32(const void *s, int size);

		int init(const char *path, const char *module, key_t key);
		void set_destory_time(int second);
		void set_init_total(int total, int add = FDUP_ADD_REC_TOTAL);

		int lock(const char *mark);
		int unlock(const char *mark);

		int find(const char *mark, const void *key, int size);
		int add(const char *mark, const void *key, int size, int idx = -1);
		int check(const char *mark, const void *key, int size);

		int delete_record(const char *mark, const void *key, int size);
		void list_shm_file(void (*func)(const fshm_t *f) = NULL);

		char *errmsg();
};

#endif
