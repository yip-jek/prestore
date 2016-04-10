/*
 * Author: hace <qianshanhai At hotmail Dot com>
 * Date:   2007.5.29
 * Update: 2012.5.14
 */


#ifndef _T_BRAND_H
#define _T_BRAND_H

/** addtogroup brand @{ */

/** 
 * @class TBrand
 * @brief 携号转品牌类
 *
 * @file TBrand.h
 * @brief 携号转品牌类头文件
 *
 * 
 */

#ifdef SHM_HASH_KEY_LEN
#undef SHM_HASH_KEY_LEN
#endif

#define SHM_HASH_KEY_LEN 12

#include "TShmHash.h"
#include "TSem.h"

#define BRAND_NUM 8 /* 一个号码拥有的最大记录数 */
#define BRAND_SHM_KEY 0x20001001
#define BRAND_SEG_KEY 0x20001001
#define MIN_BRAND_USER 1000000
#define MAX_BRAND_USER 100000000
#define BLOCK_NUM 64

#define FB_YEA 0
#define FB_NA  1
#define FB_ERROR 2

#define BV_INVALID 0
#define BV_VALID 1		/* 使用中 flag */

#ifdef error1
#undef error1
#endif

#ifdef error2
#undef error2
#endif

#define error1(x) DL((ERROR, "%s:%d" x ".\n", __FILE__, __LINE__))
#define error2(x) DL((ERROR, "%s:%d %s: errno: %d, %s\n", __FILE__, __LINE__, \
		x, errno, strerror(errno)))

#define BRAND_TABLE "sysp0_inf_brand"

#ifndef SRBC_VERSION
#define SRBC_VERSION 2
#endif

typedef int brand_time_t; /* 在 2037 年后, 要换为 long long :) */
typedef char path_v[512];

/* 用户在内存中的多条记录结构 */
struct brand_val {
	brand_time_t bv_stime; 		/* 开始时间 */
	brand_time_t bv_etime; 		/* 结束时间 */
	char bv_switch_flag[3]; 	/* 局 */
	char bv_district[3];		/* 行政区 */
	char bv_usr_type[3]; 		/* 用户类型 */
	char bv_brand; 				/* 用户品牌 */
	char bv_rec_type[3];		/* 记录类型 */
	char bv_use_flag;			/* 使用标志 */
};

typedef struct brand_struct {
	char switch_flag[3]; 	/* 局 */
	char district[3]; 		/* 行政区 */
	char user_type[3]; 		/* 用户类型 */
	char brand[2]; 			/* 品牌 */
} brand_t;

/* 一个用户在内存中只占有这样的一个结构 */
typedef struct inf_brand {
	struct brand_val bv[BRAND_NUM];
} INF_BRAND;

class TBrand {

private:
	bool m_link_shm;
	int m_brand_count;
	char m_brand_table[128];
	TShmHash <INF_BRAND> * m_pShmhash;
	TSem * m_pSem[BLOCK_NUM];
	INF_BRAND m_remove;
	key_t m_brand_shm_key, m_brand_seg_key;

	int m_fd, m_rec;	/* 删除过期记录时使用 */
private:
	int init_sem();
	brand_val *find(const char *subno, const char *time);
	brand_val *find(const char *subno, brand_time_t time);
	int brand_count_auto(const char * file, int * brand_count);
	int load_from_db();
	int load_from_file(const char * file);
	int add_one_file(const char * line);
	int addone(const char *vc_subno, const struct brand_val *bv);
	int getBlockNum(const char *vc_subno);
	unsigned int make_hash( unsigned char *s,int len,unsigned int hash );
	int rollback();
	int Traversalone(int iblock, int fd);
	int del_overdue_one(int iblock, int overday, const char * prefname,
			const char * path, const path_v * linkpath, int linkcount, int max_rec_total);
	int overdue_rename(const char * prefname, const char * path, 
			const path_v * linkpath, int linkcount);
	int overdue_write_rec(const char * subno, brand_val * p, const char * prefname,
			const char * path, const path_v * linkpath, int linkcount, int max_rec_total);

public:
	TBrand();
	~TBrand();

	/**
	 * @brief 设置预计的用户数, 默认为 MIN_BRAND_USER
	 * @param count 预计的用户数
	 */ 
	void set_brand_count(int count);

	/**
	 * @brief 设置使用的品牌表名, 默认为 sysp0_inf_brand
	 * @param table 品牌表名
	 */
	void settable(const char *table);

	/** 
	 * @brief 设置使用的共享内存/信号量的偏移，一般只是测试用
	 *		初始为BRAND_SHM_KEY/BRAND_SEG_KEY
	 * @param key 偏移
	 */
	void set_key(int key);

	/**
	 * @brief 得到使用的品牌表名
	 */
	const char *gettable();

	/**
	 * @brief 从数据库把数据读到共享内存
	 * @param force
	 * 		如果内存已经存在:
	 *     		a. 如果 force 为真, 则先把内存删除
	 *     		b. 如果 force 为假, 则直接返回
	 * @param file 为NULL时,从数据库load(默认); 否则从文件file.
	 * @return 成功返回0， 失败-1
	 */
	int load(bool force = false, const char * file = NULL);

	/**
	 * @brief 连接到共享内存
	 * @param can_load
	 * 		如果 can_load 为真, 则, 如果没有这块内存的话会创建它,
	 * 		默认 can_load 为假(只读).
	 * @return 成功返回0， 失败-1
	 */
	int init(bool can_load = false);

	/**
	 * @brief 插入一个subno记录，一个号码只有一个INF_BRAND
	 * @param subno 11位手机号码+'\0'
	 * @param INF_BRAND 
	 * @return 成功返回0， 失败-1
	 */
	int insert(const char *subno, const INF_BRAND *ib);

	/**
	 * @brief 删除号码记录（所有）
	 * @param subno 11位手机号码+'\0'
	 * @return 成功返回0， 失败-1
	 */
	int remove(const char *subno);

	/**
	 * @brief 查找号码记录(匹配号码、时间)
	 * @param subno 11位手机号码+'\0'
	 * @param time  14 位的时间串,如： 20070527120102
	 * @param user_type 返回用户类型
	 * @param brief 返回品牌
	 * @return:
	 * 		FB_YEA 	找到
	 * 		FB_NA	找不到
	 * 		FB_ERROR 错误
	 */
	int find(const char *subno, const char *time, char *usr_type, char *brand);

	/**
	 * @brief 查找号码记录(匹配号码、时间)
	 * @param subno 11位手机号码+'\0'
	 * @param time  14 位的时间串,如： 20070527120102
	 * @param brand_t   返回brand_t
	 * @return:
	 * 		FB_YEA 	找到
	 * 		FB_NA	找不到
	 * 		FB_ERROR 错误
	 */
	int find(const char *subno, const char *time, brand_t *bt);

	/**
	 * @brief 查找号码记录(匹配号码)
	 * @param subno 11位手机号码+'\0'
	 * @param INF_BRAND 返回INF_BRAND
	 * @return:
	 * 		FB_YEA 	找到
	 * 		FB_NA	找不到
	 * 		FB_ERROR 错误
	 */
	int find(const char *subno, INF_BRAND &ib);
	void print_bv(const brand_val *bv);

	/** 
	 * @brief 将内存记录按文本格式输出到文件
	 * @param file_name_head 最终文件名为 file_name_head_10%d  10%d为所在块
	 * @return 成功返回0，失败-1
	 */
	int Traversal2files(const char *file_name_head);

	/**
	 * @brief 删除过期记录，并将删除的记录写文件
	 * @param prefname 输出的文件名前缀
	 * @param path 		文件输出的备份目录
	 * @param linkpath symlink文件的目录数组
	 * @param linkcount symlink的目录数
	 * @param max_rec_total 文件中的最大记录数
	 * @param overday 过期天数，大于这个的会被删除，允许90～365，默认90天
	 * @return 成功返回0， 失败-1
	 */
	int del_overdue(const char * prefname, const char * path, 
			const path_v * linkpath, int linkcount, 
			int max_rec_total, int overday = 90);
};

unsigned short short_time(const char *time);
int short_time(unsigned short time);
brand_time_t str2time_t(const char *str);
char *time2str(brand_time_t t, char *buf);

/** addgroup brand @{ */

#endif

