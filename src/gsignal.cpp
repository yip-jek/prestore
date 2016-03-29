#include "gsignal.h"
#include <signal.h>
#include "log.h"

bool GSignal::s_bQuit = false;

bool GSignal::Init()
{
	if ( SetSignal(SIGTERM, SglFuncQuit) == SIG_ERR )
	{
		Log::Instance()->Output("Signal SIGTERM error!");
		return false;
	}
	if ( SetSignal(SIGINT, SglFuncQuit) == SIG_ERR )
	{
		Log::Instance()->Output("Signal SIGINT error!");
		return false;
	}
	if ( SetSignal(SIGQUIT, SglFuncQuit) == SIG_ERR )
	{
		Log::Instance()->Output("Signal SIGQUIT error!");
		return false;
	}

	return true;
}

bool GSignal::IsRunning()
{
	return !s_bQuit;
}

sgl_fun* GSignal::SetSignal(int sig_no, sgl_fun* f)
{
	struct sigaction sig_ac;
	sig_ac.sa_handler = f;
	sigemptyset(&sig_ac.sa_mask);
	sig_ac.sa_flags = 0;
	if ( SIGALRM == sig_no )
	{
#ifdef SA_INTERRUPT
		sig_ac.sa_flags |= SA_INTERRUPT;
#endif 
	}
	else
	{
#ifdef SA_RESTART
		sig_ac.sa_flags |= SA_RESTART;
#endif
	}

	struct sigaction n_sig_ac;
	if ( sigaction(sig_no, &sig_ac, &n_sig_ac) < 0 )
	{
		return SIG_ERR;
	}

	return n_sig_ac.sa_handler;
}

void GSignal::SglFuncQuit(int sig)
{
	s_bQuit = true;

	if ( SIGTERM == sig )
	{
		Log::Instance()->Output("Signal SIGTERM received! Ready to quit ...");
	}
	if ( SIGINT == sig )
	{
		Log::Instance()->Output("Signal SIGINT received! Ready to quit ...");
	}
	if ( SIGQUIT == sig )
	{
		Log::Instance()->Output("Signal SIGQUIT received! Ready to quit ...");
	}
}

