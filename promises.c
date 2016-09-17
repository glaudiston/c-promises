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

Promise * setPromiseCatch(Promise *p, PromiseCallBackFunctionPointer catchCallBack)
{
	printf("DBG: catch value set for promise\n"); fflush(stdout);
	// printf("DBG: size %i..\n", sizeof(FunctionPointer)); fflush(stdout);
	p->_promiseCatchArray=realloc(p->_promiseCatchArray, sizeof(PromiseCallBackFunctionPointer) * ++(p->_promiseCatchArrayLength));
	printf("DBG: realocated, set...%i\n", (p->_promiseCatchArrayLength)); fflush(stdout);
	p->_promiseCatchArray[(p, p->_promiseCatchArrayLength)-1]=catchCallBack;
	if ( p->state == PromiseStateRejected ){
		printf("DBG: lets call it\n"); fflush(stdout);
		catchCallBack(p, p->lastResult);
	}
	//printf("DBG: return p %i\n", (int)p);fflush(stdout);
	return p;
}

Promise * setPromiseWhatever(Promise *p, PromiseCallBackFunctionPointer whateverCallBack)
{
	printf("DBG: whatever value seted for promise: [%p]\n", p); fflush(stdout);
	p->_promiseWhateverArray=realloc(p->_promiseWhateverArray, sizeof(PromiseCallBackFunctionPointer) * ++(p->_promiseWhateverArrayLength));
	printf("DBG: realocated, set...%i\n", (p->_promiseWhateverArrayLength)); fflush(stdout);
	p->_promiseWhateverArray[(p, p->_promiseWhateverArrayLength)-1]=whateverCallBack;
	if ( p->state != PromiseStatePending ){
		printf("DBG: lets call it\n"); fflush(stdout);
		whateverCallBack(p, p->lastResult);
	}
	//printf("DBG: return p %i\n", (int)p);fflush(stdout);
	return p;
}

void * whatever(Promise *p, void * result)
{
	p->state=PromiseStateFulfilled;
	printf("DBG: intern whatever fn called: [%p]\n", p); fflush(stdout);
	p->lastResult=result;
	int i;
	for ( i=0; i< (p->_promiseWhateverArrayLength); i++ ){
		p->lastResult=p->_promiseWhateverArray[i](p, p->lastResult);
	}
	return p->lastResult;
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

void * PromiseThreadFunction( void * vPromise )
{
	Promise * promise=(Promise *) vPromise;
	PromiseBaseFunctionPointer fn=(PromiseBaseFunctionPointer )promise->_functionBase;
	fn(promise, resolve, reject);
}

Promise * newPromise(PromiseBaseFunctionPointer functionBase)
{
	Promise * retval=(Promise*) malloc( sizeof(Promise) );
	retval->state=PromiseStatePending;

	retval->_promiseThenArray = malloc(sizeof(PromiseCallBackFunctionPointer));
	retval->_functionBase=functionBase;
	retval->_promiseThenArrayLength=0;

	printf("DBG: in newPromise promise is at %p\n",retval);

	void * lastResult;

	int promiseCatchArrayLength=0;
	PromiseFunctionPointer * promiseCatchArray;

	retval->then=(PromiseFunctionPointer)setPromiseThen;
	retval->catch=(PromiseFunctionPointer)setPromiseCatch;
	retval->whatever=(PromiseFunctionPointer)setPromiseWhatever;
	// call it in a new thread
	pthread_t threads[1];
	int rc = pthread_create(&threads[0], NULL, (void *)&PromiseThreadFunction, (void *) retval );
	//PromiseThreadFunction(retval);
	return retval;
}

