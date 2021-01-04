/*
 * UK Skeleton
 * File: Util/Error.h
 * 
 * Purpose: Declare error class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once
#include <iostream>
#include <vector>
#include <string>

//Error class
class Error
{
	private:
		//Error list
		std::vector<std::string> error_list;
		
	public:
		//Error interface
		bool Push(const Error &error) { error_list.push_back(error.ToString()); return true; }
		bool Push(const std::string &error) { error_list.push_back(error); return true; }
		void Clear() { error_list.clear(); }
		std::string ToString() const;
		
		//Operators
		friend std::ostream& operator<<(std::ostream &out, const Error &x) { out << x.ToString(); return out; }
		operator bool() const { return error_list.size() > 0; }
		operator std::string() const { return ToString(); }
};

//Error check return methods
#define CheckNonVoidReturn(proc) \
	{ proc; return true; }

#define CheckVoidReturn(proc) \
	{ proc; return; }

#define CheckNullReturn(proc) \
	{ proc; return nullptr; }

//Error check base
#define CheckInstanceCreate_Base(x, null_error, ret) \
{ \
	if (x == nullptr) \
		ret(error.Push(null_error)) \
	else if (x->GetError()) \
		ret(error.Push(x->GetError())) \
}

#define CheckInstanceError_Base(x, ret) \
{ \
	if (x.GetError()) \
		ret(error.Push(x.GetError())) \
}

#define CheckInstanceErrorPointer_Base(x, ret) \
{ \
	if (x->GetError()) \
		ret(error.Push(x->GetError())) \
}

#define CheckInstanceMethodPointer_Base(x, method, ret) \
{ \
	if (x->method) \
		ret(error.Push(x->GetError())) \
}

#define CheckInstanceMethod_Base(x, method, ret) \
{ \
	if (x.method) \
		ret(error.Push(x.GetError())) \
}

#define CheckFunction_Base(f, msg, ret) \
{ \
	if (f) \
		ret(error.Push(msg)) \
}

//Non-void error check
#define CheckInstanceCreate(x, null_error) CheckInstanceCreate_Base(x, null_error, CheckNonVoidReturn)
#define CheckInstanceError(x) CheckInstanceError_Base(x, CheckNonVoidReturn)
#define CheckInstanceErrorPointer(x) CheckInstanceError_Base(x, CheckNonVoidReturn)
#define CheckInstanceMethodPointer(x, method) CheckInstanceMethodPointer_Base(x, method, CheckNonVoidReturn)
#define CheckInstanceMethod(x, method) CheckInstanceMethod_Base(x, method, CheckNonVoidReturn)
#define CheckFunction(f, msg) CheckFunction_Base(f, msg, CheckNonVoidReturn)

//Void error check
#define CheckInstanceCreate_Void(x, null_error) CheckInstanceCreate_Base(x, null_error, CheckVoidReturn)
#define CheckInstanceError_Void(x) CheckInstanceError_Base(x, CheckVoidReturn)
#define CheckInstanceErrorPointer_Void(x) CheckInstanceError_Base(x, CheckVoidReturn)
#define CheckInstanceMethodPointer_Void(x, method) CheckInstanceMethodPointer_Base(x, method, CheckVoidReturn)
#define CheckInstanceMethod_Void(x, method) CheckInstanceMethod_Base(x, method, CheckVoidReturn)
#define CheckFunction_Void(f, msg) CheckFunction_Base(f, msg, CheckVoidReturn)

//Null error check
#define CheckInstanceCreate_Null(x, null_error) CheckInstanceCreate_Base(x, null_error, CheckNullReturn)
#define CheckInstanceError_Null(x) CheckInstanceError_Base(x, CheckNullReturn)
#define CheckInstanceErrorPointer_Null(x) CheckInstanceError_Base(x, CheckNullReturn)
#define CheckInstanceMethodPointer_Null(x, method) CheckInstanceMethodPointer_Base(x, method, CheckNullReturn)
#define CheckInstanceMethod_Null(x, method) CheckInstanceMethod_Base(x, method, CheckNullReturn)
#define CheckFunction_Null(f, msg) CheckFunction_Base(f, msg, CheckNullReturn)
