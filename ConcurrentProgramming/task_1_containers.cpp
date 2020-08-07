#include <iostream>

#include <string>
#include <map>
#include <cassert>
#include <cmath>

int readCoef(const std::string& s, size_t& pos) {
	if (pos == s.size())
		return 0;

	bool is_neg = false;
	if (s[pos] == '+' || (is_neg = (s[pos] == '-'))) {
		++pos; 
	}

	if (s[pos] == 'x')
		return is_neg ? -1 : 1;

	int coef{ 0 };
	while (pos != s.size() && isdigit(s[pos])) {
		coef *= 10;
		coef += s[pos] - '0';
		++pos;
	}
	if (s[pos] == '*')
		++pos;
	return is_neg ? -1 * coef : coef;
}

int readPower(const std::string& s, size_t& pos) {
	if (pos == s.size() || s[pos] == '+' || s[pos] == '-')
		return 0;

	assert(s[pos++] == 'x');
	if (s[pos] != '^')
		return 1;

	++pos;
	int power{ 0 };
	while (pos != s.size() && isdigit(s[pos])) {
		power *= 10;
		power += s[pos] - '0';
		++pos;
	}
	return power;
}


std::map<int, int> Parse(const std::string& s) {
	std::map<int, int> powers_to_coefs; //[power,coef]
	size_t pos{ 0 };

	while (pos != s.size()) {
		int coef = readCoef(s, pos);
		int power = readPower(s, pos);
		powers_to_coefs[power] += coef;
	}
	return powers_to_coefs;
}

std::string Print(const std::map<int, int> &powers_to_coefs) {
	std::string res;
	res.reserve(50);

	for (auto rit = powers_to_coefs.rbegin(); rit != powers_to_coefs.rend(); ++rit) {
		if (!rit->first || !rit->second)
			continue;
		int new_coef = rit->first * rit->second;

		if (res.size() != 0) {
			res.push_back(new_coef > 0 ? '+' : '-');
			new_coef = std::abs(new_coef);
		}

		res += std::to_string(new_coef);

		if (rit->first == 1)
			continue;
		res.push_back('*');
		res.push_back('x');
		if (rit->first == 2)
			continue;
		res.push_back('^');
		res += std::to_string(rit->first - 1);
	}
	return res;
}

std::string derivative(std::string poly) {
	
	return Print(Parse(poly));
}

int main() {
	std::cout << "Answer is: " << std::endl
		<< derivative("2x-123*x^2-x+1") << "\n :: done";
}