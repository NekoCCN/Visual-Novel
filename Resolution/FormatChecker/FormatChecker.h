#pragma once
#pragma execution_character_set("utf-8")
#ifndef VIUSALNOVEL_FORMATCHECKER_H
#define VIUSALNOVEL_FORMATCHECKER_H
#include <regex>
#include <string>

namespace vn
{
	namespace resolution
	{
		class RegexChecker
		{
		private:
			std::regex regex_;
		public:
			RegexChecker(const std::string& regex) : regex_(regex)
			{  }
            bool checkString(const std::string& str) const
            {
				return std::regex_match(str, regex_);
			}
			bool operator()(const std::string& str) const
			{
                return checkString(str);
			}
			std::regex getRegex()
            {
                return regex_;
            }
		};
		namespace presets
		{
			inline RegexChecker normalLineRegexChecker()
			{
				return { R"(^\[.*\]\s+[^\[\]\{\}]*\s+\{[\d\w\s,]*\}\s*$)" };
			}
			inline RegexChecker definedLineRegexChecker()
			{
				return { R"(^\w+(\s+\w+)+\s*$)" };
			}
			inline RegexChecker backgroundLineRegexChecker()
			{
				return { R"(^\((.*)\)##\((.*)\)$)" };
			}
			inline RegexChecker bootDefinedLineRegexChecker()
			{
				return { R"(^-{3,}$)" };
			}
		}
	}
}

#endif // VIUSALNOVEL_FORMATCHECKER_H
