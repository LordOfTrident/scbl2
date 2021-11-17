#include <utils.hh>

// Input

// public
Utils::Input::Input() {
#ifdef __UTILS_USING_READLINE__
	readline::using_history();
#endif // __UTILS_USING_READLINE__
};

Utils::Input::~Input() {};

std::string Utils::Input::GetInput(std::string p_prompt) const {
	std::string input;

	Read(input, p_prompt);

	return input;
};

void Utils::Input::GetInput(
	std::string &p_str,
	std::string p_prompt
) const {
	Read(p_str, p_prompt);
};

// private
void Utils::Input::Read(
	std::string &p_str,
	const std::string &p_prompt
) const {
#ifdef __UTILS_USING_READLINE__
	char *rawInput = readline::readline(p_prompt.c_str());

	p_str = rawInput;
	readline::add_history(rawInput);

	std::free(rawInput);
#else // not __UTILS_USING_READLINE__
	std::cout << p_prompt;
	std::getline(std::cin, p_str);
#endif // __UTILS_USING_READLINE__
};
