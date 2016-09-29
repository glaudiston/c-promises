#include <promises.h>
// Sample Test Code

CREATE_PROMISE(promiseTest1)
{
	printf("DBG: tes1 - running the base test function ptrPromise at [%p]...\n", promise);
	resolve(promise, "RESOLVED DONE!!!");
}
CREATE_PROMISE(promiseTest2)
{
	printf("DBG: test 2 - running the base test function ptrPromise at [%p]...\n", promise);
	reject(promise, "CANCELED DONE!!!");
}
PROMISE_CALLBACK(fnThenTest)
{
	printf("Promise resolved: [%p], fnThenTest result.\n\nPROMISE RESULT: [%s]\n", promise, (char *) result);
	return result;
}
PROMISE_CALLBACK(fnCatchTest)
{
	printf("Promise catch error: %s\n", (char *)result);
	return result;
}
PROMISE_CALLBACK(fnWhatever)
{
	printf("Promise whatever happens: %s\n", (char *)result);
	return result;
}

void promiseTest()
{
	DO_PROMISE(promiseTest1)
	printf("DBG: in promiseTest1 promise is allocated at %p\n", promiseTest1);
	promiseTest1->then(promiseTest1,fnThenTest)->catch(promiseTest1,fnCatchTest)->then(promiseTest1, fnThenTest)->whatever(promiseTest1, fnWhatever);
	DO_PROMISE(promiseTest2)
	printf("DBG: in promiseTest2 promise is allocated at %p\n", promiseTest2);
	promiseTest2->then(promiseTest2,fnThenTest)->catch(promiseTest2,fnCatchTest)->then(promiseTest2, fnThenTest)->whatever(promiseTest2, fnWhatever);
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

