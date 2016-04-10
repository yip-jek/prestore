/*
 * Author: hace
 * Date: 2009.3.18
 */

#ifndef __DIRREAD_H
#define __DIRREAD_H

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define HASH_KEY_LEN 256		/* THash ���ڱ���·�� ԭ32������ */
#include <THash.h>

#define DR_NA 0
#define DR_YEA 1
#define DR_MAX_DIR_TOTAL 512

struct dir_st {
	char path[256]; /* Ŀ¼·�� */
	int total; 		/* ��ǰ�õ����ļ����� */
	DIR *dir;
};

class DirRead {
	private:
		struct dir_st *dir_st;
		int dir_total; 			/* Ŀ¼�ĸ��� */
		int max_file_total; 	/* ÿ��Ŀ¼һ��ȡ���ļ��� */
		char tmp_dir[256]; 		/* ��ʱĿ¼ */
		int tmp_dir_done_flag;  /* =DR_YEA ��ʾ��ʱĿ¼û���ļ� */
		char error[256]; 		/* ��Ŵ�����Ϣ */
		int curr_index; 		/* ��ǰ�� dir_st ������, &dir_st[curr_index] */
		int recursive_flag; 	/* =DR_YEA ��ʾ�еݹ�Ŀ¼ */
		THash <int> *hash;

	public:
		DirRead();
		~DirRead();
		int dr_init(const char **indir, int total, int max_file_total,
				const char *tmp_dir, int flag = DR_NA);
		int dr_read(char *file, int len);
		char *dr_error();
		int dr_done();

	private:
		int dr_read_tmp_dir(char *file, int len);
		int dr_add_dir(const char *path);
};

#endif
