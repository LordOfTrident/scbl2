<p align="center">
	<img width="100px" src="assets/logo_plain.png"/>
	<p align="center">Simple Command Bar Language version 2</p>
</p>
<p align="center">
	<a href="./LICENSE">
		<img alt="License" src="https://img.shields.io/badge/license-GPL-blue?color=7aca00"/>
	</a>
	<a href="https://github.com/LordOfTrident/scbl2/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/LordOfTrident/scbl2?color=0088ff"/>
	</a>
	<a href="https://github.com/LordOfTrident/scbl2/pulls">
		<img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/LordOfTrident/scbl2?color=0088ff"/>
	</a>
	<br><br><br>
</p>

A simple, small coding language to be embedded with a C++ program. See SCBL Version 1
[here](https://github.com/LordOfTrident/scbl). This library has to be compiled as atleast C++17.

## Features
- Functions (can be set at runtime)
- Constants (can be set at runtime)
- Unsigned integers
- Strings
- Multi line and single line comments
- Parameters handler class for easier dealing with parameters from the C++ side

## Documentation
SCBL syntax is constructed of functions and parameters:
```
<function> <parameters>
```

### Integers
Parameters are actually always 1 byte large integers, but the larger integers are split into multiple single
byte integers during the compiling.

By default, all integers are 4 bytes in size, but this can be changed by putting a `:` after the integer and then
the size in bytes. Any size other than `1`, `2`, `4` and `8` bytes will error.

> Example of a 2 bytes large integer: `1024:2`

### Strings
Strings also exist, but they are turned into 1 byte integers at compile time and get assigned a null terminator
at the end. If you want the null terminator to not be there, put a `~` after the string.

The size of each character in a string can also be changed the same way as in integers (But if you include the
`~` symbol to skip a null terminator, put the size specifier after that). By default, each character is 1 byte
large.

Just like in C++, the `\` character is an escape character. The escape characters are the same as in C++:
- `\0`: null terminator
- `\a`: bell code
- `\b`: backspace
- `\e`: escape
- `\f`: formfeed page break
- `\n`: new line
- `\r`: carriage return
- `\t`: tab
- `\v`: vertical tab

> Example: `"Hello, World!\n"~:4` - this string doesnt have a null terminator and each char in it is 4 bytes large

### Functions
Functions are defined from the C++ side. The only characters allowed in a function name are all the alphabet
letters, numbers and the symbols `_`, `-`, `+`, `*`, `.`, `&`, `(`, `)`. They can be changed at runtime.

### Constants
Constants are just like functions defined from the C++ side, can contain the same characters and can be
aswell changed at runtime. Constants can contain an integer or even a string (which is turned into multiple)
integers).

### Comments
SCBL has multi line and single line comments. Multi line comments are the same as C++ multi line comments; they
start with a `/*` and end with a `*/`.

Single line comments have two variants. The first one is just like in C++, it starts with `//`. The second one
is like in bash and starts with `#`.

> Example: `/* Hello */ # There`

## Example
As an example, i created a really simple "language" with SCBL by making several functions like `set` which
simulates variables. The source code of it is under the `example/` folder.

I also created simple SCBL programs to run in it, you can find them under the `tests/` folder.

## Embedding
SCBL source is a single header file, so you just need to include `scbl.hh`. All SCBL-related stuff is
under the `SCBL` namespace. The main things you will need are the classes `SCBL::Lexer`, `SCBL::Compiler`
and `SCBL::Environment`.

### Code example:
```cc
#include <iostream>
#include "scbl.hh"

using SCBL::u8;
using SCBL::u16;
using SCBL::u32;
using SCBL::u64;

using SCBL::s8;
using SCBL::s16;
using SCBL::s32;
using SCBL::s64;

using SCBL::usize;

// I wont use the user data pointer, so i name it _
void SCBLf_print(const std::vector<u8> p_params, void *_) {
	SCBL::Tools::ParamHandler phandle(p_params);

	std::string out;
	try {
		out = phandle.GetNextParamStr();
		// Other GetNext functions are:
		//   GetNextParam8 - Gets the next 8bit integer
		//   GetNextParam16 - Gets the next 16bit integer
		//   GetNextParam32 - Gets the next 32bit integer
		//   GetNextParam64 - Gets the next 64bit integer
		//   GetNextParamInt - Gets the next 64bit integer, if there is not
		//     enough bytes in the parameters for a 64bit one, it returns what
		//     it has
	} catch (...) { // throws a SCBL::Exception
		std::cerr
			<< "\nprint: Expected a string to output"
			<< std::endl;

		return;
	};

	std::cout << out;
};

int main() {
	SCBL::Lexer scbll; // Turns code into tokens
	SCBL::Compiler scblc; // Turns tokens into structcode
	SCBL::Environment scble; // Interprets structcode
	// Structcode is a struct that is either
	// an identifier or a single byte integer

	// The second parameter is optional, it
	// defaults to SCBL::Const::Size32b for an integer,
	// SCBL::Const::Size8b for an std::string
	scble.SetConst("TestConst", SCBL::Const("Foo\n"));
	scble.SetConst("N", SCBL::Const(65, SCBL::Const::Size8b));

	scble.SetFunc("print", SCBL::Func(SCBLf_print)); // Dont forget to set the function

	try {
		scbll.Lex("\
print \"Hello, World!\\n\" \n\
print N TestConst \
		");
		scblc.Compile(scbll.GetTokens());
		scble.Run(scblc.GetStructcode());
	} catch (const SCBL::CompilerException &error) {
		std::cout
			<< "At line " << error.Line()
			<< "col " << error.Col()
			<< ":" << error.What()
			<< std::endl;
	} catch (const SCBL::RuntimeException &error) {
		std::cout
			<< "At index " << error.Idx()
			<< ":" << error.What()
			<< std::endl;
	} catch (const SCBL::Exception &error) {
		std::cout
			<< error.What()
			<< std::endl;
	};

	return 0;
};


```
Output:
```
Hello, World!
AFoo
```

## Make
Use `make all` to see all the make targets.

## Compiling
Run `make` or `make compile` to compile the example. This will create a binary in the `bin/` folder.
