#include <string.h>
#include <stdlib.h>
#ifndef _ORAUTIL_
#define _ORAUTIL_
#define RET_OK 0
#define RET_NOT_OK 1

class TUtil{
public:
    static void trimspace(char *str);  /*去掉首尾空格*/
    static void trimleft(char *str);   /*去掉首空格*/
    static void trimright(char *str);  /*去掉尾空格*/
    static int strupper(char *dst,char *src);  /*把src变成大写字母复制到dst */
};

class ORAutil{
public:
      /*vcbnbcpy - copy buf to VARCHAR, stop at first blank.
       * SYNOPSIS
       * int vcbnbcpy(char *v, char *s, int n)
       * DESCRIPTION
       * (vcbnbcpy = VarChar Buf Number Blank CoPY)
       * Arguments: v1.arr, buf, len
       * Copy at most n characters from a buffer to a string.
       * The first blank will stop the copying.
       * Vcbnbcpy is normally called via the macro VCBBCPY or via
       * VCBNBCPY. 
       * RETURN VALUE
       *	The number of copied characters are returned. At most n.
       * SEE ALSO                          
       *	VCBBCPY(macro), VCBNBCPY(macro)
       */
    static int vcbnbcpy(char *v, char *s, int n);
    /*--------------------------------------------------- vcbnrbcpy
     * NAME                                                        
     *	vcbnrbcpy - copy buf to VARCHAR, ignor trailing blanks	   if
     * SYNOPSIS                                                    
     *	int vcbnrbcpy(char *v, char *s, int n)                     
     * DESCRIPTION                                                 
     *	(vcbnrbcpy = VarChar Buf Number Rear Blank CoPY)           
     *	Arguments: v1.arr, buf, len                                
     *	Copy at most n characters from a buffer to a VARCHAR.      
     *	Trailing blanks will be ignored.                           
     *	Vcbnrbcpy is normally called via the macro VCBRBCPY or via 
     *	VCBNRBCPY.                                                 
     * RETURN VALUE                                                
     *	The number of copied characters are returned. At most n.   
     * SEE ALSO                                                    
     *	VCBRBCPY(macro), VCBNRBCPY(macro)                          
     */                                                            
    static int vcbnrbcpy(char *v, char *s, int n); 
    /*--------------------------------------------------- vccmp                                                  
     * NAME                                                                    
     *	vccmp - VARCHAR compare                                                
     * SYNOPSIS                                                                
     *	int vccmp(char *s1, int l1, char *s2, int l2)                          
     * DESCRIPTION                                                             
     *	Compare VARCHAR variables.                                             
     *	Vccmp compares its arguments and returns an integer less than,         
     *	equal to, or greater than 0, according as s1 is lexicographically      
     *	less than, equal to, or greater than s2.                               
     *	Comparisons are done as if the characters were unsigned.               
     *	Normally called via macro VCCMP().                                     
     * SEE ALSO                                                                
     *	VCCMP(macro)                                                           
     */                                                                        
    static int vccmp(char *s1, int l1, char *s2, int l2);
    /*--------------------------------------------------- vcscmp                                                                    
     * NAME                                                                
     *	vcscmp - VARCHAR, string compare                                   
     * SYNOPSIS                                                            
     *	int vcscmp(char *s1, char *s2, int l)                              
     * DESCRIPTION                                                         
     *	Arguments V1.arr, str, V1.len.                                     
     *	Compare one VARCHAR variable with a string.                        
     *	Vcscmp compares its arguments and returns an integer less than,    
     *	equal to, or greater than 0, according as s1 is lexicographically  
     *	less than, equal to, or greater than s2.                           
     *	Comparisons are done as if the characters were unsigned.           
     *	Normally called via macro VCSCMP().                                
     * SEE ALSO                                                            
     *	VCSCMP(macro)                                                      
     */                                                                    
    static int vcscmp(char *s1, char *s2, int l);                                  
    /*--------------------------------------------------- vcscpy                
     * NAME                                                                                                                         
     *	vcscpy - copy string to VARCHAR	                                        
     * SYNOPSIS                                                                 
     *	int vcscpy(char *v, char *s, int n)                                     
     * DESCRIPTION                                                              
     *	Arguments: v1.arr, str, v2.len                                          
     *	Copy a string to a VARCHAR.                                             
     *	Vcscpy must be called via macro VCSCPY.                                 
     * RETURN VALUE                                                             
     *	The least value of v1.len and the length of the string is returned.     
     * SEE ALSO                                                                 
     *	VCSCPY(macro)                                                           
     */                                                                         
    static int vcscpy(char *v, char *s, int n); 
    /*--------------------------------------------------- vccpy 
     * NAME                                                     
     *	vccpy - copy between two VARCHAR variables              
     * SYNOPSIS                                                 
     *	int vccpy(char *s1, char *s2, int l)                    
     * DESCRIPTION                                              
     *	Arguments: v1.arr, v2.arr, v2.len                       
     *	Vccpy is a copy command for VARCHAR variables.          
     *	Vccpy must be called via macro VCCPY.                   
     * RETURN VALUE                                             
     *	The length l is returned	                            
     * SEE ALSO                                                 
     *	VCCPY(macro)                                            
     */                                                         
    static int vccpy(char *s1, char *s2, int l); 

    /*--------------------------------------------------- svccpy                                                      
     * NAME                                                            
     *	svccpy - copy VARCHAR to string                                
     * SYNOPSIS                                                        
     *	void svccpy(char *s, char *v, int l)                           
     * DESCRIPTION                                                     
     *	Arguments: str, v1.arr, v2.len                                 
     *	Copy a VARCHAR to a string.                                    
     *	Normally called via macro SVCCPY.                              
     * SEE ALSO                                                        
     *	SVCCPY(macro)                                                  
     */                                                                
    static void svccpy(char *s, char *v, int l); 
    
}; /* end class ORAutil */
   
/*
 * Length of VARCHAR 
 */
#define VCLEN(v)       ((int)v.len)

/*
 * Pointer to VARCHAR string
 */
#define VCSTR(v)       ((char *)v.arr)
/*                                                                                                        
 * NAME                                                   
 *	CVCCPY - copy VARCHAR to char                         
 * SYNOPSIS                                               
 *	void CVCCPY(char c, VARCHAR v)                        
 * DESCRIPTION                                            
 *	Copy VARCHAR v t ochar c.                             
 *	CVCCPY is a macro.                                    
 */                                                       
#define CVCCPY(c, v) { if ((v).len == 0) (c) = '\0'; else (c) = (v).arr[0]; } 
/*
 * NAME
 *	SVCCPY - copy VARCHAR to string
 * SYNOPSIS
 *	void SVCCPY(char *s, VARCHAR v)
 * DESCRIPTION                                      
 *	Copy a VARCHAR to a string. SVCCPY doesn't check the length of the
 *	string!                                         
 */                                                 
#define SVCCPY(s, v)   ORAutil::svccpy(s, VCSTR(v), VCLEN(v))    
/* NAME
 *	VCBNBCPY - copy buf to VARCHAR, stop at first blank
 * SYNOPSIS
 *	int VCBNBCPY(char *v, char *s, int n)
 * DESCRIPTION
 *	(VCBNBCPY = VarChar Buf Number Blank CoPY)
 *	Copy at most n characters to a VARCHAR.
 *	Stop at first blank.
 * RETURN VALUE
 *	The number of copied characters are returned.
 * SEE ALSO
 *	VCBBCPY(macro)
 */
#define VCBNBCPY(v, s, n)   (v.len = ORAutil::vcbnbcpy(VCSTR(v), (char *)s, \
			             sizeof((v).arr)) < n ?                 \
				     sizeof((v).arr) : n))
/* NAME
 *	VCBNRBCPY - copy buf to VARCHAR, ignor trailing blanks	
 * SYNOPSIS
 *	int VCBNRBCPY(char *v, char *s, int n)
 * DESCRIPTION
 *	(VCBNRBCPY = VarChar Buf Number Rear Blank CoPY)
 *	Copy at most n characters to a VARCHAR.
 *	Trailing blanks will be ignored.
 * RETURN VALUE
 *	The number of copied characters are returned.
 * SEE ALSO
 *	VCBRBCPY(macro)
 */
#define VCBNRBCPY(v, s, n)   (v.len = ORAutil::vcbnrbcpy(VCSTR(v), (char *)s, \
			              sizeof((v).arr) < n ?                  \
				      sizeof((v).arr) : n))
/* NAME                                     
 *	VCBRBCPY - copy buf to VARCHAR, ignor trailing blanks	
 * SYNOPSIS
 *	int VCBRBCPY(char *v, char *s)
 * DESCRIPTION
 *	(VCBRBCPY = VarChar Buf Rear Blank CoPY)
 *	Copy at most sizeof(s) characters to a VARCHAR.
 *	Trailing blanks will be ignored.
 * RETURN VALUE
 *	The number of copied characters are returned.
 * SEE ALSO
 *	VCBNRBCPY(macro)
 */
#define VCBRBCPY(v, s)	(v.len = ORAutil::vcbnrbcpy(VCSTR(v).arr, (char *)s, \
			         sizeof((v).arr) < sizeof(s) ?          \
				 sizeof((v).arr) : sizeof(s)))
/*
 * NAME
 *	VCCCPY - copy char to VARCHAR	
 * SYNOPSIS
 *	void VCCCPY(VARCHAR v, char c)
 * DESCRIPTION
 *	Copy char c to VARCHAR v.
 *	VCCCPY is a macro.
 */
#define VCCCPY(v, c)   { if ((c) == '\0') { (v).len = 0; } else { (v).arr[0] = (c); (v).len = 1; } }
/*
 * NAME
 *	VCCMP - compare two VARCHAR's
 * SYNOPSIS
 *	int VCCMP(VARCHAR v1, VARCHAR v2)
 * DESCRIPTION
 *	Compare VARCHAR variables.
 *	VCCMP compares its arguments and returns an integer less than,
 *	equal to, or greater than 0, according as v1 is lexicographically
 *	less than, equal to, or greater than v2.
 *	Comparisons are done as if the characters were unsigned.
 *	VCCMP is a macro.
 */
#define VCCMP(v1, v2)  ORAutil::vccmp(VCSTR(v1), VCLEN(v1), VCSTR(v2), VCLEN(v2))
/*
 * NAME
 *	VCCPY - copy VARCHAR to VARCHAR
 * SYNOPSIS
 *	int VCCPY(VARCHAR v1, VARCHAR v2)
 * DESCRIPTION
 *	VCCPY is a copy command for VARCHAR variables.
 *	VCCPY is a macro.
 * RETURN VALUE
 *	The length of the copied VARCHAR is returned.
 */
#define VCCPY(v1, v2)  (v1.len = ORAutil::vccpy(VCSTR(v1), VCSTR(v2), VCLEN(v2)))
/*
 * NAME
 *	VCSCMP - compare VARCHAR with string
 * SYNOPSIS
 *	int VCSCMP(VARCHAR v, char *s)
 * DESCRIPTION
 *	Compare one VARCHAR variable with a string.
 *	VCSCMP compares its arguments and returns an integer less than,
 *	equal to, or greater than 0, according as v is lexicographically
 *	less than, equal to, or greater than s.
 *	Comparisons are done as if the characters were unsigned.
 *	VCSCMP is a macro.
 */
#define VCSCMP(v, s)   ORAutil::vcscmp(VCSTR(v), s, VCLEN(v))
/*
 * NAME
 *	VCSCPY - copy string to VARCHAR
 * SYNOPSIS
 *	int VCSCPY(VARCHAR v, char *s)
 * DESCRIPTION
 *	Copy a string to a VARCHAR. If the string is to long, a substring
 *	suited for the VARCHAR is copied.
 * RETURN VALUE
 *	The length of the copied string is returned.
 */
#define VCSCPY(v, s)   (v.len = ORAutil::vcscpy((char *) (v).arr, (char *)(s), sizeof((v).arr)))				 
				       
/*! \class OraError
 *  oracle出错异常类,当WHENEVER SQLERR 时抛出
 *
 */

class OraError {
public:

        /*! \brief 构造函数.
            @param c  错误号
            @param i  错误信息
        */
        OraError(long c, char* i=(char *)"") { 
		sqlcode=c; 	
		if( NULL != i )
		{
			strncpy(errmessage,i,512);	
			errmessage[512]='\0';	
		}
	}

        /** \brief 获取错误号 */
        long get_error() const { return sqlcode; };

        /** \brief 获取错误信息, */
        const char* get_info() const { return errmessage; };

protected:
        /** \brief 错误号*/
        long sqlcode;
        /** \brief 错误信息*/
        char errmessage[512+1];
};

void SqlErroR() throw (OraError);
extern "C" {
	int ChkSqlErr(char* outstr, int outstrlen);
};
				   
#endif				   

