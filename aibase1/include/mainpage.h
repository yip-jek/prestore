/** @mainpage 基础库(aibase1-library)在线使用手册
 
@section intro_sec 介绍
 
<pre>

1. 历史

  旧基础库 aibase1 (本库)(基本不维护).
  新基础库 aibase2 . 

2. 功能及组成

2.1 功能


2.2 组成

3. 编译及使用

3.1 编译

  对于此库的程序来说, 编译时只需使用命令 ab1-config 即可, 此命令有多个参数,
如下:

--cflags  输出头文件项
--oralibs64    输出链接项


因此, 一般的使用方法如下面的 Makefile 例子:

### 好的习惯是：main函数中应该打印本程序的版本及编译时间，以及使用的基础库的版本
int main(int argc, char * argv[])
{
	...
    LOG("test version: [%s], %s %s\n", _VERSION, __DATE__, __TIME__);
	LOG("ab1_version: [%s] \n", _AB1_VERSION);
	...
}

运行结果：
test version: [232], Mar  6 2012 16:55:22
ab1_version: [aibase1-library: 0.1 rc8 (r153644)]

##############################  begin Makefile
## 下面两个变量包含基础库的头文件以及64位库
inc += $(shell ab1-config --cflags)
libs += $(shell ab1-config --oralibs64 aibase1)

## 下面三个是用于main函数中打印版本的，其中要求程序中有VERSION这个文件
version = $(shell cat VERSION | perl -pechomp)
ab1_version = $(shell ab1-config --version | perl -pechomp)

cppflag += -D_VERSION="\"\$(version)\\"" -D_AB1_VERSION="\"aibase1-library: \$(ab1_version)\\""

%.o:%.cpp
	g++ \$(cppflag) \$(inc) -o $@ -c $<

test:test.o
	g++ \$(cppflag) -o $@ $^ \$(libs) 

clean:
	rm -f test *.o
##############################  end Makefile


###你可以使用ab1-config --rule-mk，这个--rule-mk已经定义好：
cpp 如aix是xlC -q64
包含上面Makefile的inc、libs、version、ab1_version、cppflag等
可cat `ab1-config --rule-mk` 参考
使用后Makefile是这样，当然main函数要打印版本（参考上面）

############################# using ab1-config --rule-mk  begin Makefile

root = .# 在你模块中的相对路径并且变量后面不能有空格,程序的VERSION应该在这个目录下
include $(shell ab1-config --rule-mk) #包含定义好的rule-mk

%.o:%.cpp
	\$(cpp) \$(cppflag) \$(inc) -o $@ -c $<

test: test.o
	\$(cpp) \$(cppflag) -o $@ $^ \$(libs)

clean:
    rm -f test *.o

############################# using ab1-config --rule-mk  begin Makefile
</pre>

@author 
 
*/
