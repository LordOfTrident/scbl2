#ifndef __APP_HH_HEADER_GUARD__
#define __APP_HH_HEADER_GUARD__

#include "config.inc"

#include "components.inc"
#include "types.hh"
#include "utils.hh"

namespace Example {
	class App {
	public:
		// SCBL Functions
		static void SCBLf_print(const std::vector<u8> p_params, void *_);
		static void SCBLf_puts (const std::vector<u8> p_params, void *_);
		static void SCBLf_set  (const std::vector<u8> p_params, void *p_app);
		static void SCBLf_add  (const std::vector<u8> p_params, void *p_app);
		static void SCBLf_sub  (const std::vector<u8> p_params, void *p_app);
		static void SCBLf_to64b(const std::vector<u8> p_params, void *p_app);
		static void SCBLf_tostr(const std::vector<u8> p_params, void *p_app);

		// Exitcode error IDs
		static const constexpr u8 CompilerError = 1;
		static const constexpr u8 RuntimeError  = 2;
		static const constexpr u8 GenericError  = 3;
		static const constexpr u8 FileNotFound  = 4;

		App();
		App(const u8 p_argc, const char *p_argv[]);

		~App();

		u8 GetExitcode() const;

		void Start(const u8 p_argc, const char *p_argv[]);
		void Repl(); // Read Eval Print Loop
		void InterpretFiles(const std::vector<std::string> &p_files);

	private:
		SCBL::Environment &GetSCBLe();
		void Init();

		bool FileExists(const std::string &p_name) const;
		std::string ReadFile(const std::string& p_fileName);

		bool ReadParameters(
			const u8 p_argc,
			const char* p_argv[],
			std::vector<std::string> &p_files
		);

		void SCBLInterpret(const std::string &p_code);

		SCBL::Lexer m_scbll;
		SCBL::Compiler m_scblc;
		SCBL::Environment m_scble;

		usize m_exitCode;
	}; // class App
}; // namespace Example

#endif // __APP_HH_HEADER_GUARD__
