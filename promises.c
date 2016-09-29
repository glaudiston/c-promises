#include <promises.h>

int promises=0;
// REF https://computing.llnl.gov/tutorials/pthreads/#WhyPthreads
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
	p->_promiseThenArray=realloc(p->_promiseThenArray, sizeof(PromiseCallBackFunctionPointer) * ++(p->_promiseThenArrayLength));
	p->_promiseThenArray[(p, p->_promiseThenArrayLength)-1]=thenCallBack;
	if ( p->state != PromiseStatePending ){
		thenCallBack(p, p->lastResult);
	}
	return p;
}

Promise * setPromiseCatch(Promise *p, PromiseCallBackFunctionPointer catchCallBack)
{
	p->_promiseCatchArray=realloc(p->_promiseCatchArray, sizeof(PromiseCallBackFunctionPointer) * ++(p->_promiseCatchArrayLength));
	p->_promiseCatchArray[(p, p->_promiseCatchArrayLength)-1]=catchCallBack;
	if ( p->state == PromiseStateRejected ){
		catchCallBack(p, p->lastResult);
	}
	return p;
}

Promise * setPromiseWhatever(Promise *p, PromiseCallBackFunctionPointer whateverCallBack)
{
	p->_promiseWhateverArray=realloc(p->_promiseWhateverArray, sizeof(PromiseCallBackFunctionPointer) * ++(p->_promiseWhateverArrayLength));
	p->_promiseWhateverArray[(p, p->_promiseWhateverArrayLength)-1]=whateverCallBack;
	if ( p->state != PromiseStatePending ){
		whateverCallBack(p, p->lastResult);
	}
	return p;
}

void * whatever(Promise *p, void * result, ...)
{
	p->state=PromiseStateFulfilled;
	p->lastResult=result;
	int i;
	for ( i=0; i< (p->_promiseWhateverArrayLength); i++ ){
		p->lastResult=p->_promiseWhateverArray[i](p, p->lastResult);
	}
	return p->lastResult;
}

void * resolve(Promise *p, void * result, ...)
{
	p->state=PromiseStateFulfilled;
	p->lastResult=result;
	int i;
	for ( i=0; i< (p->_promiseThenArrayLength); i++ ){
		p->lastResult=p->_promiseThenArray[i](p, p->lastResult);
	}
	whatever(p, p->lastResult);
	return p->lastResult;
}

void * reject(Promise *p, void * result, ...)
{
	p->state=PromiseStateRejected;
	p->lastResult=result;
	int i;
	for ( i=0; i<p->_promiseCatchArrayLength; i++){
		p->lastResult=p->_promiseCatchArray[i](p, p->lastResult);
	}
	whatever(p,p->lastResult);
	return p->lastResult;
}

void * PromiseThreadFunction( void * vPromise )
{
	Promise * promise=(Promise *) vPromise;
	PromiseBaseFunctionPointer fn=(PromiseBaseFunctionPointer )promise->_functionBase;
	fn(promise, resolve, reject);
}

Promise * newPromise(PromiseBaseFunctionPointer functionBase, ...)
{
	Promise * retval=(Promise*) malloc( sizeof(Promise) );
	retval->state=PromiseStatePending;

	retval->_promiseThenArray = malloc(sizeof(PromiseCallBackFunctionPointer));
	retval->_functionBase=functionBase;
	retval->_promiseThenArrayLength=0;


	void * lastResult;

	int promiseCatchArrayLength=0;
	PromiseFunctionPointer * promiseCatchArray;

	retval->then=(PromiseFunctionPointer)setPromiseThen;
	retval->catch=(PromiseFunctionPointer)setPromiseCatch;
	retval->whatever=(PromiseFunctionPointer)setPromiseWhatever;
	// call it in a new thread
	int rc = pthread_create(&retval->thread, NULL, (void *)&PromiseThreadFunction, (void *) retval );
	//PromiseThreadFunction(retval);
	return retval;
}

