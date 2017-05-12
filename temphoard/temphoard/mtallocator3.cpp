#include<thread>

extern void* mtalloc(size_t bytes) {
	return std::malloc(bytes);
}

extern void mtfree(void* ptr) {
	std::free(ptr);
}