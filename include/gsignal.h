#ifndef _G_SIGNAL_H_
#define _G_SIGNAL_H_


typedef void sgl_fun(int);

class GSignal
{
public:
	static bool Init();
	static bool IsRunning();

private:
	static sgl_fun* SetSignal(int sig_no, sgl_fun* f);
	static void SglFuncQuit(int sig);

private:
	static bool s_bQuit;
};

#endif	// _G_SIGNAL_H_

