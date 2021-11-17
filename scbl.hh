/*
 *  SCBL - Simple Command Bar Language
 *  Version 2.1.1
 *  Github: https://github.com/LordOfTrident/scbl2
 *
 *  Example under example/ folder, documentation in
 *  README.md
 *
 *  define SCBL_DONT_USE_EXCEPTIONS symbol for the library to
 *  use return codes instead of exceptions
 */

#ifndef __SCBL_HH_HEADER_GUARD__
#define __SCBL_HH_HEADER_GUARD__

#include <string> // std::string
#include <vector> // std::vector
#include <unordered_map> // std::unordered_map
#include <cstdint> // std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                   // std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t
#include <cstddef> // std::size_t
#include <variant> // std::variant
#include <functional> // std::function

#define SCBL_VERSION_MAJOR 2
#define SCBL_VERSION_MINOR 1
#define SCBL_VERSION_PATCH 1

// Prevent long case repetition

// Sadly, C++ doesnt have something
// like 'a' ... 'z' like Zig, so im
// forced to do this
#define SCBL_NAME_CHARS_NO_HEX \
	     'g': case 'h': \
	case 'i': case 'j': \
	case 'k': case 'l': \
	case 'm': case 'n': \
	case 'o': case 'p': \
	case 'q': case 'r': \
	case 's': case 't': \
	case 'u': case 'v': \
	case 'w': case 'x': \
	case 'y': case 'z': \
	case 'G': case 'H': \
	case 'I': case 'J': \
	case 'K': case 'L': \
	case 'M': case 'N': \
	case 'O': case 'P': \
	case 'Q': case 'R': \
	case 'S': case 'T': \
	case 'U': case 'V': \
	case 'W': case 'X': \
	case 'Y': case 'Z': \
	case '_': case '-': \
	case '+': case '*': \
	case '.': case '&': \
	case '(': case ')'

#define SCBL_NAME_CHARS \
	     'a': case 'b': \
	case 'c': case 'd': \
	case 'e': case 'f': \
	case 'g': case 'h': \
	case 'i': case 'j': \
	case 'k': case 'l': \
	case 'm': case 'n': \
	case 'o': case 'p': \
	case 'q': case 'r': \
	case 's': case 't': \
	case 'u': case 'v': \
	case 'w': case 'x': \
	case 'y': case 'z': \
	case 'A': case 'B': \
	case 'C': case 'D': \
	case 'E': case 'F': \
	case 'G': case 'H': \
	case 'I': case 'J': \
	case 'K': case 'L': \
	case 'M': case 'N': \
	case 'O': case 'P': \
	case 'Q': case 'R': \
	case 'S': case 'T': \
	case 'U': case 'V': \
	case 'W': case 'X': \
	case 'Y': case 'Z': \
	case '_': case '-': \
	case '+': case '*': \
	case '.': case '&': \
	case '(': case ')'

#define SCBL_DEC_CHARS \
	     '0': case '1': \
	case '2': case '3': \
	case '4': case '5': \
	case '6': case '7': \
	case '8': case '9'

#define SCBL_HEX_CHARS \
	     '0': case '1': \
	case '2': case '3': \
	case '4': case '5': \
	case '6': case '7': \
	case '8': case '9': \
	case 'a': case 'b': \
	case 'c': case 'd': \
	case 'e': case 'f': \
	case 'A': case 'B': \
	case 'C': case 'D': \
	case 'E': case 'F'

namespace SCBL {
	// Type aliases for shorter and readable code
	using s8  = std::int8_t;
	using s16 = std::int16_t;
	using s32 = std::int32_t;
	using s64 = std::int64_t;

	using u8  = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	using usize = size_t;

	using Callback = std::function<void(std::vector<u8>, void*)>;

#ifndef SCBL_DONT_USE_EXCEPTIONS
	class Exception {
		public:
			Exception(const std::string &p_message):
				m_message(p_message)
			{};

			~Exception() {};

			const std::string& What() const noexcept {
				return m_message;
			};

			const std::string& what() const noexcept {
				return What();
			};

		protected:
			std::string m_message;
	}; // class Exception

	class CompilerException: public Exception {
		public:
			CompilerException(
				const std::string &p_message,
				const usize p_line,
				const usize p_col
			):
				Exception(p_message),
				m_line(p_line),
				m_col(p_col)
			{};

			~CompilerException() {};

			usize Line() const noexcept {
				return m_line;
			};

			usize Col() const noexcept {
				return m_col;
			};

		protected:
			usize m_line, m_col;
	}; // class CompilerException

	class RuntimeException: public Exception {
		public:
			RuntimeException(
				const std::string &p_message,
				const usize p_idx
			):
				Exception(p_message),
				m_idx(p_idx)
			{};

			~RuntimeException() {};

			usize Idx() const noexcept {
				return m_idx;
			};

		protected:
			usize m_idx;
	}; // class RuntimeException
#endif // SCBL_DONT_USE_EXCEPTIONS

	namespace Tools {
		// Byte Splitter and Joiner
		class ByteSJ {
		public:
			ByteSJ(std::vector<u8> &p_list):
				m_bytes(&p_list)
			{};

			ByteSJ():
				m_bytes(nullptr)
			{};

			void SetList(std::vector<u8> &p_list) {
				m_bytes = &p_list;
			};

			std::vector<u8> &GetList() {
				return *m_bytes;
			};

			bool ListExists() {
				return m_bytes != nullptr;
			};

			void Split16(const u16 p_num) {
				*m_bytes = {
					static_cast<u8>((p_num & 0xFF00) >> 010),
					static_cast<u8> (p_num & 0x00FF)
				};
			};

			void Split32(const u32 p_num) {
				*m_bytes = {
					static_cast<u8>((p_num & 0xFF000000) >> 030),
					static_cast<u8>((p_num & 0x00FF0000) >> 020),
					static_cast<u8>((p_num & 0x0000FF00) >> 010),
					static_cast<u8> (p_num & 0x000000FF)
				};
			};

			void Split64(const u64 p_num) {
				*m_bytes = {
					static_cast<u8>((p_num & 0xFF00000000000000) >> 070),
					static_cast<u8>((p_num & 0x00FF000000000000) >> 060),
					static_cast<u8>((p_num & 0x0000FF0000000000) >> 050),
					static_cast<u8>((p_num & 0x000000FF00000000) >> 040),
					static_cast<u8>((p_num & 0x00000000FF000000) >> 030),
					static_cast<u8>((p_num & 0x0000000000FF0000) >> 020),
					static_cast<u8>((p_num & 0x000000000000FF00) >> 010),
					static_cast<u8> (p_num & 0x00000000000000FF)
				};
			};

			u16 Join16() {
				return
					(static_cast<u16>((*m_bytes)[0]) << 010) |
					 static_cast<u16>((*m_bytes)[1]);
			};

			u32 Join32() {
				return
					(static_cast<u32>((*m_bytes)[0]) << 030) |
					(static_cast<u32>((*m_bytes)[1]) << 020) |
					(static_cast<u32>((*m_bytes)[2]) << 010) |
					 static_cast<u32>((*m_bytes)[3]);
			};

			u64 Join64() {
				return
					(static_cast<u64>((*m_bytes)[0]) << 070) |
					(static_cast<u64>((*m_bytes)[1]) << 060) |
					(static_cast<u64>((*m_bytes)[2]) << 050) |
					(static_cast<u64>((*m_bytes)[3]) << 040) |
					(static_cast<u64>((*m_bytes)[4]) << 030) |
					(static_cast<u64>((*m_bytes)[5]) << 020) |
					(static_cast<u64>((*m_bytes)[6]) << 010) |
					 static_cast<u64>((*m_bytes)[7]);
			};

		private:
			std::vector<u8> *m_bytes;
		}; // class ByteSJ

		class ParamHandler {
		public:
#ifdef SCBL_DONT_USE_EXCEPTIONS
			static constexpr const s8 Error = -1;
#endif // SCBL_DONT_USE_EXCEPTIONS

			ParamHandler(const std::vector<u8> &p_params):
#ifdef SCBL_DONT_USE_EXCEPTIONS
				m_outOfParams(false),
#endif // SCBL_DONT_USE_EXCEPTIONS
				m_params(p_params),
				m_it(p_params.cbegin())
			{};

#ifdef SCBL_DONT_USE_EXCEPTIONS
			bool IsOutOfParams() {
				return m_outOfParams;
			};
#endif // SCBL_DONT_USE_EXCEPTIONS

			u8 GetNextParam8() {
				if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

				const u8 toReturn = *m_it;
				++ m_it;

				return toReturn;
			};

			u16 GetNextParam16() {
				if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

				std::vector<u8> bytes;

				for (u8 i = 0; i < 2; ++ i, ++ m_it) {
					// if (m_params.size() <= m_it + i)
					if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

					bytes.push_back(*m_it);
				};

				m_byteSJ.SetList(bytes);
				return m_byteSJ.Join16();
			};

			u32 GetNextParam32() {
				if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

				std::vector<u8> bytes;

				for (u8 i = 0; i < 4; ++ i, ++ m_it) {
					if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS;

					bytes.push_back(*m_it);
				};

				m_byteSJ.SetList(bytes);
				return m_byteSJ.Join32();
			};

			u64 GetNextParam64() {
				if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

				std::vector<u8> bytes;

				for (u8 i = 0; i < 8; ++ i, ++ m_it) {
					if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

					bytes.push_back(*m_it);
				};

				m_byteSJ.SetList(bytes);
				return m_byteSJ.Join64();
			};

			u64 GetNextParamInt() {
				if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

				std::vector<u8> bytes;
				u8 size;

				for (size = 0; size < 8; ++ size, ++ m_it) {
					if (m_it == m_params.cend())
						break;

					bytes.push_back(*m_it);
				};

				if (bytes.empty())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

				m_byteSJ.SetList(bytes);

				switch (size) {
				case 1: return bytes[0];
				case 2: case 3:
					return m_byteSJ.Join16();

				case 4: case 5:
				case 6: case 7:
					return m_byteSJ.Join32();

				case 8: default:
					return m_byteSJ.Join64();
				};
			};

			std::string GetNextParamStr() {
				if (m_it == m_params.cend())
#ifdef SCBL_DONT_USE_EXCEPTIONS
					return Error;
#else // not SCBL_DONT_USE_EXCEPTIONS
					throw Exception("Out of parameters");
#endif // SCBL_DONT_USE_EXCEPTIONS

				std::string str;
				for (; m_it != m_params.cend(); ++ m_it) {
					if (*m_it == 0) {
						++ m_it;
						break;
					};

					str += *m_it;
				};

				return str;
			};

			void ResetCounter() {
				m_it = m_params.cbegin();
			};

		private:
#ifdef SCBL_DONT_USE_EXCEPTIONS
			bool m_outOfParams;
#endif // SCBL_DONT_USE_EXCEPTIONS

			const std::vector<u8> &m_params;
			std::vector<u8>::const_iterator m_it;

			ByteSJ m_byteSJ;
		}; // class ParamHandler
	}; // namespaces Tools


	inline std::string UnexpectedErrorMsg(const std::string &p_unexpect) {
		return (std::string)"Unexpected " + p_unexpect;
	};

	inline std::string ExpectedErrorMsg(
		const std::string &p_expect,
		const std::string &p_got
	) {
		return (std::string)
			"Expected " + p_expect + ", got " + p_got + " instead";
	};

	enum class TokenType {
		Int,
		Id,
		Str,
		Sym
	}; // enum class TokenType

	struct Token {
	public:
		Token() {};

		Token(
			const TokenType p_type,
			const std::variant<u64, std::string, char> p_data,
			const usize p_line,
			const usize p_col
		):
			type(p_type),
			data(p_data),
			line(p_line),
			col(p_col)
		{};

		TokenType type;
		std::variant<u64, std::string, char> data;

		usize line, col;
	}; // class Token

	class Lexer {
	public:
		Lexer() {};

		Lexer(const std::string &p_code) {
			Lex(p_code);
		};

		void Lex(const std::string &p_code) {
			m_tokens.clear();

			m_totalCol = 0;
			m_line = 1; // Line initialised to 1
			m_col = 0;  // because its incremented
			            // every time a new line char
			            // is found, while totalCol is
			            // incremented at the start of
			            // the for loop.

			m_code = &p_code;
			m_escape = false;
			for (
				m_it = p_code.cbegin();
				m_it != p_code.cend();
				Inc()
			) {
				m_col = m_totalCol;

				switch (*m_it) {
				case '\n':
					++ m_line;
					m_totalCol = 0;

					// Fall through
				case '\t': case '\r': case ' ':
					break;

				case ':': case '~':
					m_tokens.push_back(Token(
						TokenType::Sym,
						*m_it, m_line, m_col
					));

					break;

				case '#': Inc(); LexComm();     break;
				case '/': Inc(); LexFwdSlash(); break;
				case '"': Inc(); LexStr();      break;
				case '$': Inc(); LexHex();      break;
				case SCBL_NAME_CHARS: LexId();  break;
				case SCBL_DEC_CHARS: LexDec();  break;

				default:
					throw CompilerException(
							UnexpectedErrorMsg(GetCurrCharName()),
							m_line, m_col
						);
				};
			};
		};

		const std::vector<Token> &GetTokens() {
			return m_tokens;
		};

	private:
		// Functions for shorter code
		std::string GetCurrCharName() {
			if (m_it == m_code->cend())
				return "end of file";

			switch (*m_it) {
			case '\0': return "null terminator";
			case '\a': return "bell code";
			case '\b': return "backspace";
			case '\e': return "escape";
			case '\f': return "formfeed page break";
			case '\n': return "new line";
			case '\r': return "carriage return";
			case '\t': return "tab";
			case '\v': return "vertical tab";

			default: return (std::string)"character '" + *m_it + "'";
			};
		};

		void Inc() {
			++ m_it; ++ m_totalCol;
		};

		void Dec() {
			-- m_it; -- m_totalCol;
		};

		void LexFwdSlash() {
			switch (*m_it) {
			case '*':
				Inc();
				LexCommMul();
				break;

			case '/':
				Inc();
				LexComm();
				break;

			default:
				throw CompilerException(
						UnexpectedErrorMsg(std::string(1, '/')),
						m_line, m_col
					);
			};
		};

		void LexStr() {
			for (; m_it != m_code->cend(); Inc()) {
				switch (*m_it) {
				case '\\':
					if (m_escape) {
						m_token += *m_it;
						m_escape = false;
						break;
					};

					m_escape = true;
					break;

				case '"':
					if (m_escape) {
						m_token += *m_it;
						m_escape = false;
						break;
					};

					m_tokens.push_back(Token(
						TokenType::Str,
						m_token, m_line, m_col
					));

					m_token = "";

					return;

				case '\n':
					throw CompilerException(
							ExpectedErrorMsg("string end", GetCurrCharName()),
							m_line, m_col
						);

					break;

				default:
					if (m_escape) {
						switch(*m_it) {
						case '0': m_token += '\0'; break;
						case 'a': m_token += '\a'; break;
						case 'b': m_token += '\v'; break;
						case 'e': m_token += '\e'; break;
						case 'f': m_token += '\f'; break;
						case 'n': m_token += '\n'; break;
						case 'r': m_token += '\r'; break;
						case 't': m_token += '\t'; break;
						case 'v': m_token += '\v'; break;
						};

						m_escape = false;
					} else
						m_token += *m_it;
				};
			};

			throw CompilerException(
					ExpectedErrorMsg("string end", GetCurrCharName()),
					m_line, m_col
				);
		};

		void LexId() {
			for (; m_it != m_code->cend(); Inc()) {
				switch (*m_it) {
				case SCBL_NAME_CHARS:
				case SCBL_DEC_CHARS:
					m_token += *m_it;
					break;

				default: goto l_loopEnd;
				};
			};

		l_loopEnd: // Labels arent entirely apart of the
		           // "forbidden C++", when used correctly
		           // they can make code MORE readable and
		           // better
			m_tokens.push_back(Token(
				TokenType::Id,
				m_token, m_line, m_col
			));

			m_token = "";
			Dec();
		};

		void LexDec() {
			for (; m_it != m_code->cend(); Inc()) {
				switch (*m_it) {
				case SCBL_DEC_CHARS:
					m_token += *m_it;
					break;

				case SCBL_NAME_CHARS:
					throw CompilerException(
							ExpectedErrorMsg("a dec digit", GetCurrCharName()),
							m_line, m_col
						);

					break;

				default: goto l_loopEnd;
				};
			};

		l_loopEnd:
			m_tokens.push_back(Token(
				TokenType::Int,
				static_cast<u64>(std::stoull(m_token)),
				m_line, m_col
			));

			m_token = "";
			Dec();
		};

		void LexHex() {
			for (; m_it != m_code->cend(); Inc()) {
				switch (*m_it) {
				case SCBL_HEX_CHARS:
					m_token += *m_it;
					break;

				case SCBL_NAME_CHARS_NO_HEX:
					throw CompilerException(
							ExpectedErrorMsg("a hex digit", GetCurrCharName()),
							m_line, m_col
						);

					break;

				default: goto l_loopEnd;
				};
			};

		l_loopEnd:
			m_tokens.push_back(Token(
				TokenType::Int,
				static_cast<u64>(std::stoull(m_token, nullptr, 16)),
				m_line, m_col
			));

			m_token = "";
			Dec();
		};

		void LexComm() {
			for (; m_it != m_code->cend(); Inc()) {
				switch (*m_it) {
				case '\n':
					++ m_line;
					m_totalCol = 0;
					return;
				};
			};
		};

		void LexCommMul() {
			for (; m_it != m_code->cend(); Inc()) {
				switch(*m_it) {
				case '\n':
					++ m_line;
					m_totalCol = 0;
					break;

				case '*':
					Inc();

					switch (*m_it) {
					case '/': return;
					default: Dec();
					};
				};
			};

			throw CompilerException(
					ExpectedErrorMsg(
						"multi-line comment end", GetCurrCharName()
					), m_line, m_col
				);
		};

		std::vector<Token> m_tokens;
		std::string m_token;

		usize m_col, m_totalCol;
		usize m_line;

		const std::string *m_code;
		std::string::const_iterator m_it;

		bool m_escape;
	}; // class Lexer

	enum class StructcodeType {
		Int,
		Id
	}; // enum class StructcodeType

	struct Structcode {
	public:
		Structcode() {};

		Structcode(
			const StructcodeType p_type,
			const std::variant<u8, std::string> p_data
		):
			type(p_type),
			data(p_data)
		{};

		StructcodeType type;
		std::variant<u8, std::string> data;
	}; // struct Structcode

	class Compiler {
	public:
		Compiler() {};

		Compiler(const std::vector<Token> &p_tokens) {
			Compile(p_tokens);
		};

		void Compile(const std::vector<Token> &p_tokens) {
			m_scode.clear();

			m_tokens = &p_tokens;
			for (
				m_it = p_tokens.cbegin();
				m_it != p_tokens.cend();
				++ m_it
			) {
				switch (m_it->type) {
				case TokenType::Str: CompStr(); break;
				case TokenType::Int:
					PushSplit(GetSize(4), std::get<u64>(m_it->data));
					break;

				case TokenType::Id:
					m_scode.push_back(Structcode(
						StructcodeType::Id,
						std::get<std::string>(m_it->data)
					));

					break;

				default:
					throw CompilerException(
							UnexpectedErrorMsg(GetCurrTokName()),
							m_it->line, m_it->col
						);
				};
			};
		};

		const std::vector<Structcode> &GetStructcode() {
			return m_scode;
		};

	private:
		// Functions for shorter code
		std::string GetCurrTokName() {
			if (m_it == m_tokens->cend())
				return "end of file";

			switch (m_it->type) {
			case TokenType::Int:
				return (std::string)
					"number " + std::to_string(std::get<u64>(m_it->data));

			case TokenType::Str:
				return (std::string)
					"string '" + std::get<std::string>(m_it->data) + "'";

			case TokenType::Id:
				return (std::string)
					"identifier '" + std::get<std::string>(m_it->data) + "'";

			case TokenType::Sym:
				return (std::string)
					"symbol '" + std::get<char>(m_it->data) + "'";

			default: return "UNKNOWN";
			};
		};

		void CompStr() {
			const std::string &value = std::get<std::string>(m_it->data);
			bool strEndWithNull = true;

			++ m_it;
			if (IsSym('~'))
				strEndWithNull = false;
			else
				-- m_it;

			const u8 size = GetSize(1);
			for (const char ch : value)
				PushSplit(size, static_cast<u64>(ch));

			if (strEndWithNull)
				PushSplit(size, 0);
		};

		void PushSplit(const u8 p_size, const u64 p_num) {
			std::vector<u8> bytes;
			m_byteSJ.SetList(bytes);

			switch (p_size) {
			case 1: bytes = {static_cast<u8>(p_num)}; break;
			case 2: m_byteSJ.Split16(static_cast<u16>(p_num)); break;
			case 4: m_byteSJ.Split32(static_cast<u32>(p_num)); break;
			case 8: m_byteSJ.Split64(static_cast<u64>(p_num)); break;
			};

			for (const u8 byte : bytes)
				m_scode.push_back(Structcode(StructcodeType::Int, byte));
		};

		u8 GetSize(const u8 p_default) {
			++ m_it;
			if (IsSym(':')) {
				++ m_it;

				std::string currTokName;
				if (m_it != m_tokens->cend()) {
					switch (m_it->type) {
					case TokenType::Int: {
							const u64 value = std::get<u64>(m_it->data);
							switch (value) {
							case 1: case 2:
							case 4: case 8:
								return value;

							default:
								throw CompilerException(
										ExpectedErrorMsg(
											"size of 1, 2, 4 or 8 bytes",
											GetCurrTokName()
										), m_it->line, m_it->col
									);
							};
						};

						// It returns or throws before it gets to this
						// point, so it shouldnt even reach here

					default:
						currTokName = GetCurrTokName();
						break;
					};
				} else {
					currTokName = GetCurrTokName();
					-- m_it;
				};

				throw CompilerException(
						ExpectedErrorMsg("size specifier", currTokName),
						m_it->line, m_it->col
					);
			} else {
				-- m_it;
				return p_default;
			};
		};

		bool IsSym(const char p_sym) {
			if (m_it == m_tokens->cend())
				return false;

			switch (m_it->type) {
			case TokenType::Sym:
				return std::get<char>(m_it->data) == p_sym;

			default: return false;
			};
		};

		std::vector<Structcode> m_scode;

		const std::vector<Token> *m_tokens;
		std::vector<Token>::const_iterator m_it;

		Tools::ByteSJ m_byteSJ;
	}; // class Compiler

	struct Func {
	public:
		Func() {};

		Func(const Callback &p_callback):
			callback(p_callback)
		{};

		Callback callback;
	}; // struct Func

	struct Const {
	public:
		static constexpr const u8 Size8b  = 1;
		static constexpr const u8 Size16b = 2;
		static constexpr const u8 Size32b = 4;
		static constexpr const u8 Size64b = 8;

		Const() {};

		Const(const u64 p_num, const u8 p_size = Size32b) {
			Tools::ByteSJ byteSJ(value);

			Split(p_num, p_size, byteSJ);
		};

		Const(const std::string &p_str, const u8 p_size = Size8b) {
			std::vector<u8> bytes;
			Tools::ByteSJ byteSJ(bytes);

			for (const char ch : p_str) {
				Split(ch, p_size, byteSJ);

				for (const u8 byte : bytes)
					value.push_back(byte);
			};

			value.push_back(0); // Null terminator
		};

		Const(const std::vector<u8> p_list) {
			value = p_list;
		};

	private:
		void Split(
			const u64 p_value,
			const u8 p_size,
			Tools::ByteSJ &p_byteSJ
		) {
			switch (p_size) {
			case Size8b:
				p_byteSJ.GetList() = {static_cast<u8>(p_value)};
				break;

			case Size16b: p_byteSJ.Split16(p_value); break;
			case Size32b: p_byteSJ.Split32(p_value); break;
			case Size64b: p_byteSJ.Split64(p_value); break;

			default:
				throw Exception(
						"Invalid amount of bytes: " +
						std::to_string(p_size)
					);
			};
		};

	public:
		std::vector<u8> value;
	}; // struct Const

	class Environment {
	public:
		Environment() {};

		Environment(const std::vector<Structcode> &p_scode) {
			Run(p_scode);
		};

		void Run(const std::vector<Structcode> &p_scode) {
			m_idx = 0;

			m_scode = &p_scode;
			for (
				m_it = p_scode.cbegin();
				m_it != p_scode.cend();
				Inc()
			) {
				switch (m_it->type) {
				case StructcodeType::Id: {
						const std::string id = std::get<std::string>(m_it->data);
						if (not m_funcs.count(id))
							throw RuntimeException(
									"No function with " +
									GetCurrScodeName() +
									" exists",
									m_idx
								);

						const Func &func = m_funcs[id];

						m_params.clear();
						Inc();
						GetParams();

						func.callback(m_params, m_userData);
					};

					break;

				default:
					throw RuntimeException(
							UnexpectedErrorMsg(GetCurrScodeName()),
							m_idx
						);
				};
			};
		};

		void SetUserData(void* p_userData) {
			m_userData = p_userData;
		};

		void* GetUserData() {
			return m_userData;
		};

		void SetFunc(
			const std::string &p_name,
			const Func &p_func
		) {
			if (not CheckName(p_name))
				throw Exception(
						"Invalid characters in function name '" +
						p_name + "'"
					);

			if (m_consts.count(p_name))
				throw Exception(
						"Constant with the name '" +
						p_name + "' already exists"
					);

			m_funcs[p_name] = p_func;
		};

		void SetConst(
			const std::string &p_name,
			const Const &p_const
		) {
			if (not CheckName(p_name))
				throw Exception(
						"Invalid characters in function name '" +
						p_name + "'"
					);

			if (m_funcs.count(p_name))
				throw Exception(
						"Function with the name '" +
						p_name + "' already exists"
					);

			m_consts[p_name] = p_const;
		};

		std::unordered_map<std::string, Func> &GetFuncs() {
			return m_funcs;
		};

		void SetFuncs(std::unordered_map<std::string, Func> p_funcs) {
			m_funcs = p_funcs;
		};

		std::unordered_map<std::string, Const> &GetConsts() {
			return m_consts;
		};

		void SetConsts(std::unordered_map<std::string, Const> p_consts) {
			m_consts = p_consts;
		};

	private:
		// Functions for shorter code
		std::string GetCurrScodeName() {
			if (m_it == m_scode->cend())
				return "end of file";

			switch (m_it->type) {
			case StructcodeType::Int:
				return (std::string)
					"number " + std::to_string(std::get<u8>(m_it->data));

			case StructcodeType::Id:
				return (std::string)
					"identifier '" + std::get<std::string>(m_it->data) + "'";

			default: return "UNKNOWN";
			};
		};

		void Inc() {
			++ m_it; ++ m_idx;
		};

		void Dec() {
			-- m_it; -- m_idx;
		};

		void GetParams() {
			std::string id;
			for (; m_it != m_scode->cend(); Inc()) {
				switch (m_it->type) {
				case StructcodeType::Int:
					m_params.push_back(std::get<u8>(m_it->data));
					break;

				case StructcodeType::Id:
					id = std::get<std::string>(m_it->data);
					if (m_funcs.count(id)) {
						Dec();
						return;
					};

					if (not m_consts.count(id))
						throw RuntimeException(
								"No constant with " +
								GetCurrScodeName() +
								" exists",
								m_idx
							);

					for (const u8 byte : m_consts[id].value)
						m_params.push_back(byte);
				};
			};

			Dec();
		};

		bool CheckName(const std::string& p_name) {
			for (const char ch : p_name) {
				switch (ch) {
				case SCBL_NAME_CHARS:
				case SCBL_DEC_CHARS:
					break;

				default: return false;
				};
			};

			return true;
		};

		std::vector<u8> m_params;

		std::unordered_map<std::string, Func> m_funcs;
		std::unordered_map<std::string, Const> m_consts;

		usize m_idx;

		const std::vector<Structcode> *m_scode;
		std::vector<Structcode>::const_iterator m_it;

		void* m_userData;
	}; // class Environment
}; // namespace SCBL

#endif // __SCBL_HH_HEADER_GUARD__
