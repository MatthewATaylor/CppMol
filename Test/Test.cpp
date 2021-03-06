#include <string>

#include "CppUnitTest.h"

#include "../CppMol/include/Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test {
	TEST_CLASS(Test) {
	public:
		TEST_METHOD(TestParser_getArg) {
			std::string rArg = "5:100";
			std::string cArg = "A";
			std::string command;

			//Single argument
			command = "select r=" + rArg;
			Assert::AreEqual(Parser::getArg("r", Parser::split(command, ' ')), rArg);
			Assert::AreEqual(Parser::getArg("c", Parser::split(command, ' ')), std::string(""));

			//Argument not provided
			command = "select r=";
			Assert::AreEqual(Parser::getArg("r", Parser::split(command, ' ')), std::string(""));
			Assert::AreEqual(Parser::getArg("c", Parser::split(command, ' ')), std::string(""));

			//Multiple arguments
			command = "select r=" + rArg + " c=" + cArg + " e=O";
			Assert::AreEqual(Parser::getArg("r", Parser::split(command, ' ')), rArg);
			Assert::AreEqual(Parser::getArg("c", Parser::split(command, ' ')), cArg);
		}
		TEST_METHOD(TestParser_removeTrailingSpaces) {
			Assert::AreEqual(
				Parser::removeTrailingSpaces("This is a test."),
				std::string("This is a test.")
			);
			Assert::AreEqual(
				Parser::removeTrailingSpaces("  This is a test.         "),
				std::string("  This is a test.")
			);
			Assert::AreEqual(
				Parser::removeTrailingSpaces("   "),
				std::string("")
			);
			Assert::AreEqual(
				Parser::removeTrailingSpaces(""),
				std::string("")
			);
		}
	};
}
