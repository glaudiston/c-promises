#ifndef __PROMISES__
#define __PROMISES__
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>

extern int promises;
struct Promise;
typedef void *( *PromiseCallBackFunctionPointer )( struct Promise *, void * );
typedef struct Promise *( *PromiseFunctionPointer )( struct Promise *, PromiseCallBackFunctionPointer );
typedef void ( *PromiseBaseFunctionPointer )( struct Promise *, PromiseCallBackFunctionPointer, PromiseCallBackFunctionPointer );

typedef struct Promise{
	enum { PromiseStatePending, PromiseStateFulfilled, PromiseStateRejected } state;
	PromiseFunctionPointer then;
	PromiseFunctionPointer catch;
	PromiseFunctionPointer whatever;
	void * lastResult;
	// internal items
	int _promiseThenArrayLength;
	PromiseCallBackFunctionPointer * _promiseThenArray;
	int _promiseCatchArrayLength;
	PromiseCallBackFunctionPointer * _promiseCatchArray;
	int _promiseWhateverArrayLength;
	PromiseCallBackFunctionPointer * _promiseWhateverArray;
	PromiseBaseFunctionPointer _functionBase;
} Promise;

Promise * newPromise(PromiseBaseFunctionPointer functionBase);

void freePromise(Promise *p);

#endif
