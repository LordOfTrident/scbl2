#include <app.hh>

void Example::App::SCBLf_print(const std::vector<u8> p_params, void *_) {
	for (const char ch : p_params) {
		switch (ch) {
			case 0: break;
			default: std::cout << ch;
		};
	};

	std::cout << std::endl;
};

void Example::App::SCBLf_puts(const std::vector<u8> p_params, void *_) {
	for (const char ch : p_params) {
		switch (ch) {
			case 0: break;
			default: std::cout << ch;
		};
	};
};

void Example::App::SCBLf_set(const std::vector<u8> p_params, void *p_app) {
	Example::App &app = *static_cast<Example::App*>(p_app);
	SCBL::Environment &scble = app.GetSCBLe();
	SCBL::Tools::ParamHandler phandle(p_params);

	std::string varName;
	try {
		varName = phandle.GetNextParamStr();
	} catch (...) {
		std::cerr
			<< "\nset: Expected a variable name"
			<< std::endl;

		return;
	};

	std::vector<u8> bytes;
	bool hasParams = true;
	while (hasParams) {
		try {
			bytes.push_back(phandle.GetNextParam8());
		} catch (...) {
			hasParams = false;
		};
	};

	scble.SetConst(varName, SCBL::Const(bytes));
};

void Example::App::SCBLf_tostr(const std::vector<u8> p_params, void *p_app) {
	Example::App &app = *static_cast<Example::App*>(p_app);
	SCBL::Environment &scble = app.GetSCBLe();
	SCBL::Tools::ParamHandler phandle(p_params);

	u64 num;
	try {
		num = phandle.GetNextParamInt();
	} catch (...) {
		std::cerr
			<< "\ntostr: Expected a number"
			<< std::endl;

		return;
	};

	scble.SetConst("result", SCBL::Const(std::to_string(num)));
};

void Example::App::SCBLf_to64b(const std::vector<u8> p_params, void *p_app) {
	Example::App &app = *static_cast<Example::App*>(p_app);
	SCBL::Environment &scble = app.GetSCBLe();
	SCBL::Tools::ParamHandler phandle(p_params);

	u64 num;
	try {
		num = phandle.GetNextParamInt();
	} catch (...) {
		std::cerr
			<< "\nto64b: Expected a number"
			<< std::endl;

		return;
	};

	scble.SetConst("result", SCBL::Const(num, SCBL::Const::Size64b));
};

void Example::App::SCBLf_add(const std::vector<u8> p_params, void *p_app) {
	Example::App &app = *static_cast<Example::App*>(p_app);
	SCBL::Environment &scble = app.GetSCBLe();
	SCBL::Tools::ParamHandler phandle(p_params);

	u64 num1, num2;
	try {
		num1 = phandle.GetNextParam64();
		num2 = phandle.GetNextParam64();
	} catch (...) {
		std::cerr
			<< "\nadd: Expected a 64bit number"
			<< std::endl;

		return;
	};

	scble.SetConst("result", SCBL::Const(num1 + num2, SCBL::Const::Size64b));
};

void Example::App::SCBLf_sub(const std::vector<u8> p_params, void *p_app) {
	Example::App &app = *static_cast<Example::App*>(p_app);
	SCBL::Environment &scble = app.GetSCBLe();
	SCBL::Tools::ParamHandler phandle(p_params);

	u64 num1, num2;
	try {
		num1 = phandle.GetNextParam64();
		num2 = phandle.GetNextParam64();
	} catch (...) {
		std::cerr
			<< "\nsub: Expected a 64bit number"
			<< std::endl;

		return;
	};

	scble.SetConst("result", SCBL::Const(num1 - num2, SCBL::Const::Size64b));
};
