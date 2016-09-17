# c-promises
A user friendly Promises / Futures implementation in C.

I really like javascript ECMA6 promises. It seems a little complicated to learn at begining, but after it solve to many complex problems using a nice pattern.
In C we have various ways to achieve asynchronous tasks, but I don't know a clear or better then ECMA promises.

So here, is my way to implement it.

I'm not sure about memory management yet. But looks very nice and simple

look at promise-test.c for sample code.

```C

void functionBaseTest(Promise *ptrPromise, PromiseCallBackFunctionPointer resolve, PromiseCallBackFunctionPointer reject)
{
	resolve(ptrPromise, "SUCCESS");
}

void * fnThenTest(Promise *p, void * result)
{
	printf("Promise resolved: %s\n", p, (char *) result);
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
