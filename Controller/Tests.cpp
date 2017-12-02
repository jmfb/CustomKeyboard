#include "Arduino.h"
#include "MockArduino.h"
#include <iostream>
#include <exception>
#include <functional>
using namespace std;

class Test
{
public:
	Test(const char* name, function<void()> testFunction)
		: name(name), testFunction(testFunction)
	{
	}

	bool Run() const
	{
		try
		{
			mockArduino.Initialize();
			testFunction();
			return true;
		}
		catch (const exception& error)
		{
			cout << name << ": " << error.what() << '\n';
			return false;
		}
	}

	const char* name;
	function<void()> testFunction;
};

extern vector<Test> tests;

int main()
{
	auto passed = 0;
	auto failed = 0;
	for (auto& test : tests)
		if (test.Run())
			++passed;
		else
			++failed;
	cout << (passed - 1) << " passed, " << failed << " failed.\n"
		<< "Test run " << (failed > 0 ? "failed" : "succeeded") << ".\n";
	return failed > 0 ? 1 : 0;
}

#define BEGIN_TESTS() vector<Test> tests{ { "", [](){}
#define TEST_METHOD(name) }, { #name, []()
#define END_TESTS() } };

BEGIN_TESTS()

TEST_METHOD(InitializePinModesAndSignals)
{
	setup();
	mockArduino.AssertDelays({ 200 });
	mockArduino.AssertPinModes({
		//First 12 row pins marked as output
		OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT,
		OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT,
		OUTPUT, OUTPUT,
		//Next 11 column pins marked as input
		INPUT, INPUT, INPUT, INPUT, INPUT,
		INPUT, INPUT, INPUT, INPUT, INPUT,
		INPUT
	});
	mockArduino.AssertPinSignals({
		//First 12 row pins set to HIGH
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH, HIGH,
		//Next 11 column pins also all set to HIGH
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH
	});
	mockArduino.AssertKeyboardReports({});
}

TEST_METHOD(OnlyRegisterStableKeys)
{
	//TODO: implement this test
}

//TODO: more tests

END_TESTS()
