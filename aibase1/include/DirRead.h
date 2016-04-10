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

#define HASH_KEY_LEN 256		/* THash 用于保持路径 原32不够长 */
#include <THash.h>

#define DR_NA 0
#define DR_YEA 1
#define DR_MAX_DIR_TOTAL 512

struct dir_st {
	char path[256]; /* 目录路径 */
	int total; 		/* 当前得到的文件个数 */
	DIR *dir;
};

class DirRead {
	private:
		struct dir_st *dir_st;
		int dir_total; 			/* 目录的个数 */
		int max_file_total; 	/* 每个目录一次取的文件数 */
		char tmp_dir[256]; 		/* 临时目录 */
		int tmp_dir_done_flag;  /* =DR_YEA 表示临时目录没有文件 */
		char error[256]; 		/* 存放错误信息 */
		int curr_index; 		/* 当前的 dir_st 的索引, &dir_st[curr_index] */
		int recursive_flag; 	/* =DR_YEA 表示有递归目录 */
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
