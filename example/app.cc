#include <app.hh>

// public
Example::App::App() {
	Init();
};

Example::App::App(const u8 p_argc, const char* p_argv[]):
	m_exitCode(0)
{
	Init();
	Start(p_argc, p_argv);
};

Example::App::~App() {};

u8 Example::App::GetExitcode() const {
	return m_exitCode;
};

void Example::App::SCBLInterpret(const std::string &p_code) {
	m_scbll.Lex(p_code);
	m_scblc.Compile(m_scbll.GetTokens());
	m_scble.Run(m_scblc.GetStructcode());
};

void Example::App::Start(const u8 p_argc, const char* p_argv[]) {
	std::vector<std::string> files = {};

	bool startRepl = true;

	startRepl = ReadParameters(p_argc, p_argv, files);

	if (files.empty() and startRepl) {
		Repl();

		return;
	};

	InterpretFiles(files);
};

bool Example::App::ReadParameters(
	const u8 p_argc,
	const char* p_argv[],
	std::vector<std::string> &p_files
) {
	bool startRepl = true;

	for (u8 i = 1; i < p_argc; ++ i) {
		std::string arg = p_argv[i];

		switch (arg[0]) {
		case '-': {
				if (arg == "-h" or arg == "--help") {
					std::cout
						<< "Usage: app [Options]"
						<< "Options:\n"
						<< "    -h, --help      Show the usage\n"
						<< "    -v  --version   Show the current version"
						<< std::endl;

					startRepl = false;
				} else if (arg == "-v" or arg == "--version") {
					std::cout
						<< "SCBL version "
						<< SCBL_VERSION_MAJOR
						<< "."
						<< SCBL_VERSION_MINOR
						<< "."
						<< SCBL_VERSION_PATCH
						<< std::endl;

					startRepl = false;
				};
			};

			break;

		// Treat all other arguments as file names
		default: p_files.push_back(arg); break;
		};
	};

	return startRepl;
};

void Example::App::Repl() {
	std::cout
		<< "SCBL v"
		<< SCBL_VERSION_MAJOR
		<< "."
		<< SCBL_VERSION_MINOR
		<< "."
		<< SCBL_VERSION_PATCH
		<< " example\ntype in 'help' to see the commands and functions\n";

	Utils::Input inputHandle;

	bool running = true;
	while (running) {
		std::string input;
		inputHandle.GetInput(input, "> ");

		// Control commands
		if (input == "help") {
			std::cout
				<< "Commands:\n"
				<< "  help  Show the available commands and functions\n"
				<< "  exit  Exit the shell"
				<< "\nFunctions:\n"
				<< "  print <string>       Print a string to the console and go to a new line\n"
				<< "  puts <string>        Print a string to the console\n"
				<< "  set <string> <value> Create a variable and set the value\n"
				<< "  add <int64> <int64>  add numbers (result in var result)\n"
				<< "  sub <int64> <int64>  subtract numbers (result in var result)\n"
				<< "  to64b <int>          convert any size integer into a 64bit integer (result in var result)\n"
				<< "  tostr <int>          convert an integer into a string (result in var result)\n"
				<< std::endl;
		} else if (input == "exit") {
			running = false;

			std::cout << "Exited." << std::endl;

			continue;
		} else { // Else interpret
			try {
				SCBLInterpret(input);
			} catch (const SCBL::CompilerException &error) {
				std::cerr
					<< "\nREPL:" << error.Line()
					<< ":" << error.Col()
					<< ": error:\n  "
					<< error.What()
					<< std::endl;

				m_exitCode = CompilerError;
			} catch (const SCBL::RuntimeException &error) {
				std::cerr
					<< "\nREPL:" << error.Idx()
					<< ": error:\n  "
					<< error.What()
					<< std::endl;

				m_exitCode = RuntimeError;
			} catch (const SCBL::Exception &error) {
				std::cerr
					<< "\nREPL: error:\n  "
					<< error.What()
					<< std::endl;

				m_exitCode = GenericError;
			};
		};

		if (input.find('.') != std::string::npos)
			std::cout << std::endl;
	};
};

void Example::App::InterpretFiles(const std::vector <std::string> &p_files) {
	// Execute all files if multiple were specified in
	// the command line parameters
	for (const std::string& file : p_files) {
		if (not FileExists(file)) {
			std::cerr
				<< "\nerror:\n  File '"
				<< file
				<< "' not found"
				<< std::endl;

			m_exitCode = FileNotFound;
			return;
		};

		try {
			SCBLInterpret(ReadFile(file));
		} catch (const SCBL::CompilerException &error) {
			std::cerr
				<< "\n" << file
				<< ":" << error.Line()
				<< ":" << error.Col()
				<< ": error:\n  "
				<< error.What()
				<< std::endl;

			m_exitCode = CompilerError;
		} catch (const SCBL::RuntimeException &error) {
			std::cerr
				<< "\n" << file
				<< ":" << error.Idx()
				<< ": error:\n  "
				<< error.What()
				<< std::endl;

			m_exitCode = RuntimeError;
		} catch (const SCBL::Exception &error) {
			std::cerr
				<< "\n" << file
				<< ": error:\n  "
				<< error.What()
				<< std::endl;

			m_exitCode = GenericError;
		};
	};
};

// private
SCBL::Environment &Example::App::GetSCBLe() {
	return m_scble;
};

void Example::App::Init() {
	m_scble.SetUserData(static_cast<void*>(this));
	m_scble.SetFunc("print", SCBL::Func(SCBLf_print));
	m_scble.SetFunc("puts",  SCBL::Func(SCBLf_puts));
	m_scble.SetFunc("set",   SCBL::Func(SCBLf_set));
	m_scble.SetFunc("add",   SCBL::Func(SCBLf_add));
	m_scble.SetFunc("sub",   SCBL::Func(SCBLf_sub));
	m_scble.SetFunc("to64b", SCBL::Func(SCBLf_to64b));
	m_scble.SetFunc("tostr", SCBL::Func(SCBLf_tostr));
};

bool Example::App::FileExists(const std::string &p_name) const {
	std::ifstream fileHandle(p_name);

	return fileHandle.good();
};

std::string Example::App::ReadFile(const std::string& p_fileName) {
	std::string fileContents = "";
	std::ifstream fileHandle(p_fileName);

	if (fileHandle.is_open()) {
		std::string fileLine = "";

		while (std::getline(fileHandle, fileLine))
			fileContents += fileLine + '\n';

		fileHandle.close();

		return fileContents;
	};

	m_exitCode = GenericError;

	throw SCBL::Exception("Could not open the file '" + p_fileName + "'");
};
