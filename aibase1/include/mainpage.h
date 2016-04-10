/** @mainpage ������(aibase1-library)����ʹ���ֲ�
 
@section intro_sec ����
 
<pre>

1. ��ʷ

  �ɻ����� aibase1 (����)(������ά��).
  �»����� aibase2 . 

2. ���ܼ����

2.1 ����


2.2 ���

3. ���뼰ʹ��

3.1 ����

  ���ڴ˿�ĳ�����˵, ����ʱֻ��ʹ������ ab1-config ����, �������ж������,
����:

--cflags  ���ͷ�ļ���
--oralibs64    ���������


���, һ���ʹ�÷���������� Makefile ����:

### �õ�ϰ���ǣ�main������Ӧ�ô�ӡ������İ汾������ʱ�䣬�Լ�ʹ�õĻ�����İ汾
int main(int argc, char * argv[])
{
	...
    LOG("test version: [%s], %s %s\n", _VERSION, __DATE__, __TIME__);
	LOG("ab1_version: [%s] \n", _AB1_VERSION);
	...
}

���н����
test version: [232], Mar  6 2012 16:55:22
ab1_version: [aibase1-library: 0.1 rc8 (r153644)]

##############################  begin Makefile
## �����������������������ͷ�ļ��Լ�64λ��
inc += $(shell ab1-config --cflags)
libs += $(shell ab1-config --oralibs64 aibase1)

## ��������������main�����д�ӡ�汾�ģ�����Ҫ���������VERSION����ļ�
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


###�����ʹ��ab1-config --rule-mk�����--rule-mk�Ѿ�����ã�
cpp ��aix��xlC -q64
��������Makefile��inc��libs��version��ab1_version��cppflag��
��cat `ab1-config --rule-mk` �ο�
ʹ�ú�Makefile����������Ȼmain����Ҫ��ӡ�汾���ο����棩

############################# using ab1-config --rule-mk  begin Makefile

root = .# ����ģ���е����·�����ұ������治���пո�,�����VERSIONӦ�������Ŀ¼��
include $(shell ab1-config --rule-mk) #��������õ�rule-mk

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
