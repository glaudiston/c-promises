// REF https://computing.llnl.gov/tutorials/pthreads/#WhyPthreads
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>

int promises=0;
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
} Promise;

/*
void * PromiseThreadFunction()
{
	//functionBase(resolve, reject);
	//resolveTest()
}
*/

// store all promises here to avoid lost reference
struct Promises;

Promise * setPromiseThen(Promise *p, PromiseCallBackFunctionPointer thenCallBack)
{
	printf("DBG: in then promise is at %p\n",p);
	printf("DBG: then value set for promise\n"); fflush(stdout);
	// printf("DBG: size %i..\n", sizeof(FunctionPointer)); fflush(stdout);
	p->_promiseThenArray=realloc(p->_promiseThenArray, sizeof(PromiseCallBackFunctionPointer) * ++(p->_promiseThenArrayLength));
	printf("DBG: realocated, set...%i\n", (p->_promiseThenArrayLength)); fflush(stdout);
	p->_promiseThenArray[(p, p->_promiseThenArrayLength)-1]=thenCallBack;
	if ( p->state != PromiseStatePending ){
		printf("DBG: lets call it\n"); fflush(stdout);
		thenCallBack(p, p->lastResult);
	}
	//printf("DBG: return p %i\n", (int)p);fflush(stdout);
	return p;
}

Promise setPromiseCatch(Promise *p, PromiseCallBackFunctionPointer catchCallBack)
{
	printf("DBG: catch value set for promise\n"); fflush(stdout);
	return *p;
}

Promise setPromiseWhatever(Promise *p, PromiseCallBackFunctionPointer whateverCallBack)
{
	printf("DBG: whatever value seted for promise: [%p]\n", p); fflush(stdout);
	return *p;
}

void * whatever(Promise *p, void * result)
{
	printf("DBG: whatever value call: [%p]\n", p); fflush(stdout);
	return result;
}

void * resolve(Promise *p, void * result)
{
	p->state=PromiseStateFulfilled;
	printf("DBG: intern resolve fn called: [%p]\n", p); fflush(stdout);
	p->lastResult=result;
	int i;
	for ( i=0; i< (p->_promiseThenArrayLength); i++ ){
		p->lastResult=p->_promiseThenArray[i](p, p->lastResult);
	}
	whatever(p, p->lastResult);
	return p->lastResult;
}

void * reject(Promise *p, void * result)
{
	p->state=PromiseStateRejected;
	printf("DBG: inter reject fn arg called\n");
	p->lastResult=result;
	int i;
	for ( i=0; i<p->_promiseCatchArrayLength; i++){
		p->lastResult=p->_promiseCatchArray[i](p, p->lastResult);
	}
	whatever(p,p->lastResult);
	return p->lastResult;
}

Promise * newPromise(PromiseBaseFunctionPointer functionBase)
{
	Promise * retval=(Promise*) malloc( sizeof(Promise) );
	retval->state=PromiseStatePending;

	retval->_promiseThenArray = malloc(sizeof(PromiseCallBackFunctionPointer));
	retval->_promiseThenArrayLength=0;

	printf("DBG: in newPromise promise is at %p\n",retval);

	void * lastResult;

	int promiseCatchArrayLength=0;
	PromiseFunctionPointer * promiseCatchArray;

	retval->then=(PromiseFunctionPointer)setPromiseThen;
	retval->catch=(PromiseFunctionPointer)setPromiseCatch;
	retval->whatever=(PromiseFunctionPointer)setPromiseWhatever;
	// call it in a new thread
	functionBase(retval, resolve, reject);
	//PromiseThreadFunction(retval);
	return retval;
}


void promiseTest()
{
	void functionBaseTest(Promise *ptrPromise, PromiseCallBackFunctionPointer resolve, PromiseCallBackFunctionPointer reject)
	{
		printf("DBG: running the base test function ptrPromise at [%p]...\n", ptrPromise);
		resolve(ptrPromise, "WELL DONE!!!\n");
	}

	void * fnThenTest(Promise *p, void * result)
	{
		printf("DBG: Promise resolved: [%p], fnThenTest result.\n\nPROMISE RESULT: [%s]\n", p, (char *) result);
		return result;
	}
	Promise * promise = newPromise(functionBaseTest);
	printf("DBG: in promiseTest promise is allocated at %p\n", promise);
	promise->then(promise,fnThenTest)->then(promise, fnThenTest);
}

int main(int argc, char * argv[])
{
	promiseTest();
	/*
	rc = pthread_join(thread[0], &status);
	if (rc) {
		printf("ERROR; return code from pthread_join() is %d\n", rc);
		exit(-1);
	}
	*/
	// end the main thread
	pthread_exit(NULL);
	return 0;
}

