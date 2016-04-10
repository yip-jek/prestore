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
 * @brief Я��תƷ����
 *
 * @file TBrand.h
 * @brief Я��תƷ����ͷ�ļ�
 *
 * 
 */

#ifdef SHM_HASH_KEY_LEN
#undef SHM_HASH_KEY_LEN
#endif

#define SHM_HASH_KEY_LEN 12

#include "TShmHash.h"
#include "TSem.h"

#define BRAND_NUM 8 /* һ������ӵ�е�����¼�� */
#define BRAND_SHM_KEY 0x20001001
#define BRAND_SEG_KEY 0x20001001
#define MIN_BRAND_USER 1000000
#define MAX_BRAND_USER 100000000
#define BLOCK_NUM 64

#define FB_YEA 0
#define FB_NA  1
#define FB_ERROR 2

#define BV_INVALID 0
#define BV_VALID 1		/* ʹ���� flag */

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

typedef int brand_time_t; /* �� 2037 ���, Ҫ��Ϊ long long :) */
typedef char path_v[512];

/* �û����ڴ��еĶ�����¼�ṹ */
struct brand_val {
	brand_time_t bv_stime; 		/* ��ʼʱ�� */
	brand_time_t bv_etime; 		/* ����ʱ�� */
	char bv_switch_flag[3]; 	/* �� */
	char bv_district[3];		/* ������ */
	char bv_usr_type[3]; 		/* �û����� */
	char bv_brand; 				/* �û�Ʒ�� */
	char bv_rec_type[3];		/* ��¼���� */
	char bv_use_flag;			/* ʹ�ñ�־ */
};

typedef struct brand_struct {
	char switch_flag[3]; 	/* �� */
	char district[3]; 		/* ������ */
	char user_type[3]; 		/* �û����� */
	char brand[2]; 			/* Ʒ�� */
} brand_t;

/* һ���û����ڴ���ֻռ��������һ���ṹ */
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

	int m_fd, m_rec;	/* ɾ�����ڼ�¼ʱʹ�� */
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
	 * @brief ����Ԥ�Ƶ��û���, Ĭ��Ϊ MIN_BRAND_USER
	 * @param count Ԥ�Ƶ��û���
	 */ 
	void set_brand_count(int count);

	/**
	 * @brief ����ʹ�õ�Ʒ�Ʊ���, Ĭ��Ϊ sysp0_inf_brand
	 * @param table Ʒ�Ʊ���
	 */
	void settable(const char *table);

	/** 
	 * @brief ����ʹ�õĹ����ڴ�/�ź�����ƫ�ƣ�һ��ֻ�ǲ�����
	 *		��ʼΪBRAND_SHM_KEY/BRAND_SEG_KEY
	 * @param key ƫ��
	 */
	void set_key(int key);

	/**
	 * @brief �õ�ʹ�õ�Ʒ�Ʊ���
	 */
	const char *gettable();

	/**
	 * @brief �����ݿ�����ݶ��������ڴ�
	 * @param force
	 * 		����ڴ��Ѿ�����:
	 *     		a. ��� force Ϊ��, ���Ȱ��ڴ�ɾ��
	 *     		b. ��� force Ϊ��, ��ֱ�ӷ���
	 * @param file ΪNULLʱ,�����ݿ�load(Ĭ��); ������ļ�file.
	 * @return �ɹ�����0�� ʧ��-1
	 */
	int load(bool force = false, const char * file = NULL);

	/**
	 * @brief ���ӵ������ڴ�
	 * @param can_load
	 * 		��� can_load Ϊ��, ��, ���û������ڴ�Ļ��ᴴ����,
	 * 		Ĭ�� can_load Ϊ��(ֻ��).
	 * @return �ɹ�����0�� ʧ��-1
	 */
	int init(bool can_load = false);

	/**
	 * @brief ����һ��subno��¼��һ������ֻ��һ��INF_BRAND
	 * @param subno 11λ�ֻ�����+'\0'
	 * @param INF_BRAND 
	 * @return �ɹ�����0�� ʧ��-1
	 */
	int insert(const char *subno, const INF_BRAND *ib);

	/**
	 * @brief ɾ�������¼�����У�
	 * @param subno 11λ�ֻ�����+'\0'
	 * @return �ɹ�����0�� ʧ��-1
	 */
	int remove(const char *subno);

	/**
	 * @brief ���Һ����¼(ƥ����롢ʱ��)
	 * @param subno 11λ�ֻ�����+'\0'
	 * @param time  14 λ��ʱ�䴮,�磺 20070527120102
	 * @param user_type �����û�����
	 * @param brief ����Ʒ��
	 * @return:
	 * 		FB_YEA 	�ҵ�
	 * 		FB_NA	�Ҳ���
	 * 		FB_ERROR ����
	 */
	int find(const char *subno, const char *time, char *usr_type, char *brand);

	/**
	 * @brief ���Һ����¼(ƥ����롢ʱ��)
	 * @param subno 11λ�ֻ�����+'\0'
	 * @param time  14 λ��ʱ�䴮,�磺 20070527120102
	 * @param brand_t   ����brand_t
	 * @return:
	 * 		FB_YEA 	�ҵ�
	 * 		FB_NA	�Ҳ���
	 * 		FB_ERROR ����
	 */
	int find(const char *subno, const char *time, brand_t *bt);

	/**
	 * @brief ���Һ����¼(ƥ�����)
	 * @param subno 11λ�ֻ�����+'\0'
	 * @param INF_BRAND ����INF_BRAND
	 * @return:
	 * 		FB_YEA 	�ҵ�
	 * 		FB_NA	�Ҳ���
	 * 		FB_ERROR ����
	 */
	int find(const char *subno, INF_BRAND &ib);
	void print_bv(const brand_val *bv);

	/** 
	 * @brief ���ڴ��¼���ı���ʽ������ļ�
	 * @param file_name_head �����ļ���Ϊ file_name_head_10%d  10%dΪ���ڿ�
	 * @return �ɹ�����0��ʧ��-1
	 */
	int Traversal2files(const char *file_name_head);

	/**
	 * @brief ɾ�����ڼ�¼������ɾ���ļ�¼д�ļ�
	 * @param prefname ������ļ���ǰ׺
	 * @param path 		�ļ�����ı���Ŀ¼
	 * @param linkpath symlink�ļ���Ŀ¼����
	 * @param linkcount symlink��Ŀ¼��
	 * @param max_rec_total �ļ��е�����¼��
	 * @param overday ������������������Ļᱻɾ��������90��365��Ĭ��90��
	 * @return �ɹ�����0�� ʧ��-1
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

