#ifndef __OUT__H__
#define __OUT__H_

/*
本文件主要实现输出相关东西，比如error输出，warning输出等.
*/

#include <string>
#include <exception>
#include <iostream>

using std::cout;
using std::cerr;
using std::clog;
using std::endl;

void qError(std::string msg = ""){
	cerr<<msg<<endl;
	std::terminate();
}

void qWarning(std::string msg = ""){
	clog<<msg<<endl;
}

void qDebug(std::string msg = ""){
#ifdef DEBUG
	cout<<msg<<endl;
#endif
}

#endif
