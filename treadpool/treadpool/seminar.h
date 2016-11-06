#pragma once
void f() {
	std::cout << "function";
}


void doSomething(const std::function<void() >& toDo) {
	toDo();
}

int g(int x, int y) {
	return std::max(x, y);
}

int main() {
	std::packaged_task<void() > task([]() {std::cout << "done"; });
	std::future<void> ticket = task.get_future();
	std::thread worker([](std::packaged_task<void()>&& task) {task(); }, std::move(task));
	ticket.get();
	worker.join();
	/*try {
	try {
	doSomething(std::function<void() >([]() {std::cout << "lambda"; }));
	std::function<int(int) > cut = std::bind(g, std::placeholders::_1, 5);
	std::cout << cut(4);
	}
	catch (...) {
	std::exception_ptr ptr = std::current_exception();
	std::throw_with_nested(std::exception("was here"));
	std::exception_ptr ptr2 = std::make_exception_ptr(5);
	std::rethrow_exception(ptr);

	}
	} catch(int val){

	}*/