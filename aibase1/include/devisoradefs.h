/* $Header: /cvsdata/bossroot/02code/09jishu/SRBC/include/devisoradefs.h,v 1.8 2007/03/06 09:52:34 hzw Exp $
 */
/************************************************************************
               
Title       : devisoradefs
Description : Oracle Pro*C INCLUDE's and WHENEVER's.
File        : devisoradefs.h
Creator     : 
Date        : 
Comments : 


************************************************************************/
#ifndef _DEVISORADEFS_
#define _DEVISORADEFS_

#define  ORASTFANY 3
EXEC SQL INCLUDE sqlca;

//#ifndef __linux__
EXEC SQL INCLUDE oraca;
//#endif

//oraca.orastxtf = ORASTFANY;

/* Just a dummy begin-end to avoid error when compiling programs
 * without Oracle variables (occurs when begin-end missing, Pro*C bug).
 */
EXEC SQL BEGIN DECLARE SECTION;
EXEC SQL END DECLARE SECTION;

#define COMMIT 1
#define ROLLBACK 2

#define RET_OK 0
#define RET_NOT_OK 1
#define RET_NOT_FOUND 2

#define FOUND (sqlca.sqlcode == 0)
#define NOTFOUND (sqlca.sqlcode == 1403)
#define ENDOFFETCH (sqlca.sqlcode == 1403)
#define ROWS_FETCHED (sqlca.sqlerrd[2])
#define ROWS_UPDATED (sqlca.sqlerrd[2])
#define ROWS_DELETED (sqlca.sqlerrd[2])
#define ROWS_DONE (sqlca.sqlerrd[2])
#define _SQLCODE     (sqlca.sqlcode)
//#define SQLCODE     (sqlca.sqlcode)

extern "C"
{
   extern int sqlgls(char*,size_t *,size_t *);
}
//EXEC SQL WHENEVER SQLERROR do SqlErroR() ;
#endif
