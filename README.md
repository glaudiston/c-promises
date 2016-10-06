# c-promises
A user friendly Promises / Futures implementation in C.

I really like javascript ECMA6 promises. It seems a little complicated to learn at begining, but after it solve to many complex problems using a nice pattern.
In C we have various ways to achieve asynchronous tasks, but I don't know a clear or better then ECMA promises.

So here, is my way to implement it.

I'm not sure about memory management yet. But looks very nice and simple

look at promise-test.c for sample code.

```C

#include <promises.h>
// Sample Test Code

CREATE_PROMISE(promiseTest1)
{
	printf("DBG: tes1 - running the base test function ptrPromise at [%p]...\n", promise);
	resolve(promise, "RESOLVED DONE!!!");
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

DO_PROMISE(promiseTest1)
promiseTest1
	->then(promiseTest1,fnThenTest)
	->catch(promiseTest1,fnCatchTest)
	->then(promiseTest1, fnThenTest)
	->whatever(promiseTest1, fnWhatever);


```

For now, just tested using GNU C compiler / GNU Linux environment.

To build it just:

`make` - build shared and static libraries
`make static` - build staic libraries
`make shared` - build libpromises.so.1.0.0
`make check` - build sample program using shared library.

Enjoy, and if you liked it, help is welcome.

Best Regards,
Glaudiston Gomes da Silva
