#ifndef WMHANDLERS_H
#define WMHANDLERS_H

#include <WString.h>
#include <functional>

typedef std::function<void(void)> VoidHandler;
typedef std::function<String(void)> RtStringHandler;
typedef std::function<bool(void)> RtBoolHandler;
typedef std::function<void(String)> StringHandler;

#endif