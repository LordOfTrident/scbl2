#ifndef __UTILS_HH_HEADER_GUARD__
#define __UTILS_HH_HEADER_GUARD__

#include <components.inc>
#include <types.hh>
#include <platform.inc>
#include <config.inc>

// Use GNU readline if it is allowed and possible
#if defined(PLATFORM_LINUX) and defined(UTILS_USE_GNU_READLINE)
#	define __UTILS_USING_READLINE__
namespace readline {
#	include <readline/readline.h>
#	include <readline/history.h>
};
#endif

namespace Utils {
	class Input {
	public:
		Input();
		~Input();

		std::string GetInput(std::string p_prompt = "") const;

		void GetInput(
			std::string &p_str,
			std::string p_prompt = ""
		) const;

	private:
		void Read(
			std::string &p_str,
			const std::string &p_prompt
		) const;
	}; // class Input
}; // namespace Utils

#endif // __UTILS_HH_HEADER_GUARD__
