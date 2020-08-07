#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <assert.h>

using namespace std;

template <class T>
class Stack {
private:
	T* bottom_ptr_;
	T* top_ptr_;
	int max_size_;
public:
	Stack(int n = 10) : max_size_(n) {
		bottom_ptr_ = new T[max_size_];
		top_ptr_ = bottom_ptr_;
	}
	bool empty() {
		return bottom_ptr_ == top_ptr_;
	}
	T& top() {
		if (!empty()) {
			return *(top_ptr_ - 1);
		}
		else {
			throw runtime_error("Empty");
		}
	}
	T pop() {
		if (!empty()) {
			--top_ptr_;
			return *top_ptr_;
		}
		else {
			throw runtime_error("Empty");
		}
	}
	int current_size() {
		return top_ptr_ - bottom_ptr_;
	}
	void realloc() {
		int prev_size = max_size_;
		max_size_ *= 2;
		T* tmp = new T[max_size_];
		memcpy(tmp, bottom_ptr_, sizeof(T) * prev_size);
		delete[] bottom_ptr_;
		bottom_ptr_ = tmp;
		top_ptr_ = bottom_ptr_ + prev_size;
	}
	void push(const T& val) {
		if (current_size() < max_size_) {
			*top_ptr_ = val;
			++top_ptr_;
		}
		else {
			realloc();
			push(val);
		}
	}
	void push(T&& val) {
		if (current_size() < max_size_) {
			*top_ptr_ = move(val);
			++top_ptr_;
		}
		else {
			realloc();
			push(val);
		}
	}
	~Stack() {
		delete[] bottom_ptr_;
	}
};
char getOpposite(char c) {
	switch (c)
	{
	case '{':
		return '}';
		break;
	case '[':
		return ']';
		break;
	case '(':
		return ')';
		break;
	}
}

int CheckBrackets(istream& istr) {
	char c;
	int idx = 0;

	Stack<pair<char, int>> stack(500);
	while (istr >> c) {
		if (c == '{' || c == '[' || c == '(') {
			stack.push({ c, idx });
		}
		else
			if (c == '}' || c == ']' || c == ')') {
				if (!stack.empty() && c == getOpposite(stack.top().first)) {
					stack.pop();
				}
				else {
					return idx + 1;
				}
			}
		++idx;

	}
	if (!stack.empty()) {
		return stack.top().second + 1;
	}
	else {
		return 0;
	}
	
}

void Test() {
	{
		istringstream istr("[]");
		assert(CheckBrackets(istr) == 0);
	}
	{
		istringstream istr("[])");
		assert(CheckBrackets(istr) == 3);
	}

	{
		istringstream istr;
		istr.str("[*])");
		assert(CheckBrackets(istr) == 4);
	}

	{
		istringstream istr;
		istr.str("}");
		assert(CheckBrackets(istr) == 1);
	}

	{
		istringstream istr;
		istr.str("*[]");
		assert(CheckBrackets(istr) == 0);
	}

	{
		istringstream istr;
		istr.str("{{{**[][][]");
		assert(CheckBrackets(istr) == 3);
	}

	{
		istringstream istr;
		istr.str("{{[()]]");
		assert(CheckBrackets(istr) == 7);
	}

	{
		istringstream istr;
		istr.str("");
		assert(CheckBrackets(istr) == 0);
	}

	{
		istringstream istr;
		istr.str("{[}");
		assert(CheckBrackets(istr) == 3);
	}
}
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int a = CheckBrackets(cin);
	if (!a) {
		cout << "Success";
	}
	else {
		cout << a;
	}
	
	return 0;

}