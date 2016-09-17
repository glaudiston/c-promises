#include <promises.h>
// Sample Test Code

void functionBaseTest(Promise *ptrPromise, PromiseCallBackFunctionPointer resolve, PromiseCallBackFunctionPointer reject)
{
	printf("DBG: running the base test function ptrPromise at [%p]...\n", ptrPromise);
	reject(ptrPromise, "CANCELED DONE!!!\n");
}

void * fnThenTest(Promise *p, void * result)
{
	printf("Promise resolved: [%p], fnThenTest result.\n\nPROMISE RESULT: [%s]\n", p, (char *) result);
	return result;
}
void * fnCatchTest(Promise *p, void * result)
{
	printf("Promise returned error: %s", (char *)result);
	return result;
}
void promiseTest()
{
	Promise * promise = newPromise(functionBaseTest);
	printf("DBG: in promiseTest promise is allocated at %p\n", promise);
	promise->then(promise,fnThenTest)->catch(promise,fnCatchTest)->then(promise, fnThenTest)->whatever(promise, fnWhatever);
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

