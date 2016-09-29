#ifndef __PROMISES__
#define __PROMISES__
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>

extern int promises;
struct Promise;
typedef void *( *PromiseCallBackFunctionPointer )( struct Promise *, void *, ... );
typedef struct Promise *( *PromiseFunctionPointer )( struct Promise *, PromiseCallBackFunctionPointer, ... );
typedef void ( *PromiseBaseFunctionPointer )( struct Promise *, PromiseCallBackFunctionPointer, PromiseCallBackFunctionPointer, ... );

typedef struct Promise{
	enum { PromiseStatePending, PromiseStateFulfilled, PromiseStateRejected } state;
	PromiseFunctionPointer then;
	PromiseFunctionPointer catch;
	PromiseFunctionPointer whatever;
	void * lastResult;
	pthread_t thread;
	// internal items
	int _promiseThenArrayLength;
	PromiseCallBackFunctionPointer * _promiseThenArray;
	int _promiseCatchArrayLength;
	PromiseCallBackFunctionPointer * _promiseCatchArray;
	int _promiseWhateverArrayLength;
	PromiseCallBackFunctionPointer * _promiseWhateverArray;
	PromiseBaseFunctionPointer _functionBase;
} Promise;

Promise * newPromise(PromiseBaseFunctionPointer functionBase, ...);

void freePromise(Promise *p);

#define CREATE_PROMISE(promiseName, ...) Promise * promiseName;\
void fn_##promiseName(Promise *promise, PromiseCallBackFunctionPointer resolve, PromiseCallBackFunctionPointer reject, ... )

#define DO_PROMISE(promiseName, ...) promiseName = newPromise(fn_##promiseName, ##__VA_ARGS__);
#define PROMISE_CALLBACK(func_name) void * func_name(Promise * promise, void * result, ... )

#endif
