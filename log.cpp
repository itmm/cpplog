#include <exception>
#include <iostream>
#include <string>

template <void f()>
class LogContainer {
	public:
		bool log;
		bool first;
		LogContainer(): log(false), first(true) {}
		LogContainer(const std::string file, int line):
			log(true), first(true)
		{
			std::cerr << '[' << file << ':' << line << "]";
		}
		LogContainer(LogContainer &&lc) {
			log = lc.log; lc.log = false;
			first = lc.first;
		}
		~LogContainer() {
			if (log) { f(); }
		}
};

template<void f(), typename T>
inline LogContainer<f> operator<<(
	LogContainer<f> c, T t
) {
	if (c.log) {
		if (c.first) {
			std::cerr << ": ";
			c.first = false;
		}
		std::cerr << t;
	}
	return c;
}

inline void logNewLine() {
	std::cerr << std::endl;
}

inline void logThrow() {
	logNewLine();
	throw std::exception();
}

#define BASE_LOG(F) LogContainer<F>(__FILE__, __LINE__)
#define LOG BASE_LOG(logNewLine)

#define EXPECT(COND) \
	(COND) ? LogContainer<logThrow>() : BASE_LOG(logThrow)

int main() {
	EXPECT(true) << "test true: " << 1;
	EXPECT(false) << "test false: " << 2;
}
