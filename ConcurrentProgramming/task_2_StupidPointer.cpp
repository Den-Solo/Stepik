#include <iostream>

#include <string>


class StringPointer {
private:
	std::string* data{ nullptr };
	bool error = false;
public:
	std::string* operator->() {
		if (!data) {
			data = new std::string();
			error = true;
		}
		return data;
	}
	operator std::string* () {
		if (!data) {
			data = new std::string();
			error = true;
		}
		return data;
	}
	StringPointer(std::string* Pointer) 
		: data(Pointer)
	{
		
	}
	~StringPointer() {
		if (error)
			delete data;
	}
};

int main() {
	std::string s1 = "Hello, world!";

	StringPointer sp1(&s1);
	StringPointer sp2(NULL);

	std::cout << sp1->length() << std::endl;
	std::cout << *sp1 << std::endl;
	std::cout << sp2->length() << std::endl;
	std::cout << *sp2 << std::endl;
}