

// change to char chk. (from addspace, chk #, chk " ") - 2016.02.17 

#ifndef LOAD_DATA_UTILITY_H
#define LOAD_DATA_UTILITY_H

#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <thread> // maybe error with C++/CLI?
#include <algorithm>

#include <wiz/cpp_string.h>
#include <wiz/STRINGBUILDER.H>

namespace wiz {
	namespace load_data {
		class UserType;

		class Utility
		{
		public:
			static bool IsInteger(std::string str) {
				if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
					str = str.substr(1, str.size() - 2);
				}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else return false;
					}
				}
				return 1 == state; /// chk..
			}
			static bool IsNumberInJson(std::string str)
			{
				if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
					str = str.substr(1, str.size() - 2);
				}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ( // '+' == str[i] || // why can`t +
							'-' == str[i]
							) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else { return false; }
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else { return false; }
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else { return false; }
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if ('e' == str[i] || 'E' == str[i]) {
							state = 4;
						}
						else { return false; }
						break;
					case 4:
						if (str[i] == '+' || str[i] == '-') {
							state = 5;
						}
						else {
							state = 5;
						}
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
					}
				}
				return 3 == state || 6 == state;
			}
			static bool IsDouble(std::string str) {
				if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
					str = str.substr(1, str.size() - 2);
				}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else { return false; }
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else { return false; }
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else { return false; }
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if ('e' == str[i] || 'E' == str[i]) {
							state = 4;
						}
						else { return false; }
						break;
					case 4:
						if (str[i] == '+' || str[i] == '-') {
							state = 5;
						}
						else {
							state = 5;
						}
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
					}
				}
				return 3 == state || 6 == state;
			}
			static bool IsDate(std::string str) /// chk!!
			{
				if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
					str = str.substr(1, str.size() - 2);
				}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else return false;
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					}
				}
				return 5 == state;
			}
			static bool IsDateTimeA(std::string str) // yyyy.MM.dd.hh
			{
				if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
					str = str.substr(1, str.size() - 2);
				}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else return false;
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else if (str[i] == '.') { state = 6; }
						else return false;
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					case 7:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					}
				}
				return 7 == state;
			}
			static bool IsDateTimeB(std::string str) // yyyy.MM.dd.hh.mm
			{
				if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
					str = str.substr(1, str.size() - 2);
				}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else return false;
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else if (str[i] == '.') { state = 6; }
						else return false;
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					case 7:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else if (str[i] == '.') {
							state = 8;
						}
						else return false;
						break;
					case 8:
						if (str[i] >= '0' && str[i] <= '9') { state = 9; }
						else return false;
						break;
					case 9:
						if (str[i] >= '0' && str[i] <= '9') { state = 9; }
						else return false;
						break;
					}
				}
				return 9 == state;
			}
			static bool IsMinus(std::string str)
			{
				if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
					str = str.substr(1, str.size() - 2);
				}
				return str.empty() == false && str[0] == '-';
			}

			static std::string reverse(std::string str) { /// to std::reverse ?
				std::reverse(str.begin(), str.end());
				return str;
			}
			static std::string GetType(std::string str) {
				if (IsInteger(str)) { return "INTEGER"; }
				else if (IsDouble(str)) { return "DOUBLE"; }
				else if (IsDateTimeB(str)) { return "DATETIMEB"; }
				else if (IsDateTimeA(str)) { return "DATETIMEA"; }
				else if (IsDate(str)) { return "DATE"; }
				else return "STRING";
			}
			static std::string Compare(std::string str1, std::string str2, wiz::StringBuilder* builder, const int type = 0)
			{
				if (str1.size() > 2 && str1[0] == str1.back() && (str1[0] == '\"' || str1[0] == '\''))
				{
					str1 = str1.substr(1, str1.size() - 2);
				}
				if (str2.size() > 2 && str2[0] == str2.back() && (str2[0] == '\"' || str2[0] == '\''))
				{
					str2 = str2.substr(1, str2.size() - 2);
				}

				std::string type1 = GetType(str1);
				std::string type2 = GetType(str2);

				if (type1 != type2) {
					return "ERROR";
				}

				if ("STRING" == type1 || type == 1)
				{
					if (str1 < str2) {
						return "< 0";
					}
					else if (str1 == str2) {
						return "== 0";
					}
					return "> 0";
				}
				else if ("INTEGER" == type1)
				{
					if (Utility::IsMinus(str1) && !Utility::IsMinus(str2)) { return "< 0"; }
					else if (!Utility::IsMinus(str1) && Utility::IsMinus(str2)) { return "> 0"; }

					const bool minusComp = Utility::IsMinus(str1) && Utility::IsMinus(str2);

					if (false == minusComp) {
						if (str1[0] == '+') { str1 = std::string(str1.c_str() + 1); }
						if (str2[0] == '+') { str2 = std::string(str2.c_str() + 1); }

						std::string x = reverse(str1);
						std::string y = reverse(str2);



						if (x.size() < y.size()) {
							while (x.size() < y.size()) {
								x.push_back('0');
							}
						}
						else {
							while (y.size() < x.size()) {
								y.push_back('0');
							}
						}
						return Compare(reverse(x), reverse(y), builder, 1);
					}
					else {
						return Compare(std::string(str2.c_str() + 1), std::string(str1.c_str() + 1), builder);
					}
				}
				else if ("DOUBLE" == type1)
				{
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					std::string x = tokenizer1.nextToken();
					std::string y = tokenizer2.nextToken();

					std::string z = Compare(x, y, builder);
					if ("== 0" == z)
					{
						x = tokenizer1.nextToken();
						y = tokenizer2.nextToken();

						if (x.size() < y.size()) {
							while (x.size() < y.size()) {
								x.push_back('0');
							}
						}
						else {
							while (y.size() < x.size()) {
								y.push_back('0');
							}
						}
						return Compare(x, y, builder, 1);
					}
					else
					{
						return z;
					}
				}
				else if ("DATE" == type1)
				{
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					for (int i = 0; i < 3; ++i) {
						const std::string x = tokenizer1.nextToken();
						const std::string y = tokenizer2.nextToken();

						const std::string comp = Compare(x, y, builder);

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				else if ("DATETIMEA" == type1) {
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					for (int i = 0; i < 4; ++i) {
						const std::string x = tokenizer1.nextToken();
						const std::string y = tokenizer2.nextToken();

						const std::string comp = Compare(x, y, builder);

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				else if ("DATETIMEB" == type2) {
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					for (int i = 0; i < 5; ++i) {
						const std::string x = tokenizer1.nextToken();
						const std::string y = tokenizer2.nextToken();

						const std::string comp = Compare(x, y, builder);

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				return "ERROR";
			}

			static std::string BoolOperation(const std::string& op, const std::string& x, const std::string& y)
			{
				if (x == "ERROR" || y == "ERROR") { return "ERROR"; }
				if ("NOT" == op) { return x == "TRUE" ? "FALSE" : "TRUE"; }
				else if ("AND" == op) {
					if (x == "TRUE" && y == "TRUE") { return "TRUE"; }
					else {
						return "FALSE";
					}
				}
				else if ("OR" == op) {
					if (x == "TRUE" || y == "TRUE") { return "TRUE"; }
					else {
						return "FALSE";
					}
				}
				else {
					return "ERROR";
				}
			}

		public:
			class DoThread // need to rename!
			{
			private:
				StringBuilder* strVec;
			public:
				ArrayQueue<Token>* aq;
				const wiz::LoadDataOption* option;
				//int strVecStart;
				//int strVecEnd;
			public:
				DoThread(StringBuilder* strVec, ArrayQueue<Token>* aq, const wiz::LoadDataOption* option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
					: strVec(strVec), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					//
				}
			private:
				// slow?
				int checkDelimiter(const StringBuilder& statement, const int start, const std::vector<std::string>& delimiter)
				{
					int sum = 0;
					const int _size = delimiter.size();

					for (int delim_num = 0; delim_num < _size; ++delim_num) {
						// size check
						if (start + delimiter[delim_num].size() - 1 > statement.Size() - 1) {
							continue;
						}

						const int last = start + delimiter[delim_num].size() - 1;
						for (int i = start; last >= start && i <= last; ++i) {
							if (statement[i] == delimiter[delim_num][i - start]) {

							}
							else {
								sum--;
								break;
							}
						}
						sum++;

						if (sum > 0) {
							return delim_num;
						}
					}

					return -1;
				}
			public:
				void operator() () {
					//std::string* strVecTemp = strVec; // enterkey 기준으로 나뉘어져있다고 가정한다.
					//for (int x = 0; x <= 0; ++x)
					{
						//StringTokenizer tokenizer(std::move( (*strVecTemp)[x] ) );
						//while (tokenizer.hasMoreTokens()) {
						//	aq.push(tokenizer.nextToken());
						//}
						StringBuilder& statement = *strVec;
						int token_first = 0, token_last = 0; // idx of token in statement.
						int state = 0;
						bool isMultipleLineComment = false;
						std::string token;
						token.reserve(1024);
					
						
						for (int i = 0; i < statement.Size(); ++i) {
							int idx;

							if (isMultipleLineComment && -1 != (idx = checkDelimiter(statement, i, option->MuitipleLineCommentEnd)))
							{
								isMultipleLineComment = false;

								for (int j = 0; j < option->MuitipleLineCommentEnd[idx].size(); ++j)
								{
									token.push_back(statement[i + j]);
								}

								aq->emplace_push(token, true);
								
								i = i + option->MuitipleLineCommentEnd[idx].size() - 1;

								statement.Divide(i);
								statement.LeftShift(i + 1);
								
								token.clear();

								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else if (isMultipleLineComment) {
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (0 == state && '\'' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 2;
								//token_first = i; 
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (2 == state && '\\' == statement[i - 1] && '\'' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (2 == state && '\'' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && '\"' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 1;
								//token_first = i; 
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\\' == statement[i - 1] && '\"' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\"' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && -1 != (idx = Equal(option->Removal, statement[i])))
							{
								token_last = i - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_push(token, false);
									token.clear();

									statement.LeftShift(i + 1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = Equal(option->Assignment, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									
									statement.Divide(i);
									
									aq->emplace_push(token, false);

									token.clear();

									statement.LeftShift(i + 1);

									aq->emplace_push(std::string("") + option->Assignment[idx], false);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_push(std::string("") + option->Assignment[idx], false);
									statement.LeftShift(1);

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && isWhitespace(statement[i])) { // isspace ' ' \t \r \n , etc... ?
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_push(token, false);

									statement.LeftShift(i + 1);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else
								{
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option->Left, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_push(token, false);
									statement.LeftShift(i + 1);

									aq->emplace_push(std::string("") + option->Left[idx], false);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_push(std::string("") + option->Left[idx], false);
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option->Right, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);


									aq->emplace_push(token, false);
									statement.LeftShift(i + 1);

									aq->emplace_push(std::string("") + option->Right[idx], false);

									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_push(std::string("") + option->Right[idx], false);

									statement.LeftShift(1);

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && option->MuitipleLineCommentStart.empty() == false 
								&& -1 != (idx = checkDelimiter(statement, i, option->MuitipleLineCommentStart))) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);
									aq->emplace_push(token, false);
								
									statement.LeftShift(i + option->MuitipleLineCommentStart[idx].size());
									i = -1;

									token_first = 0;
									token_last = 0;
								}
								else {
									statement.LeftShift(i + option->MuitipleLineCommentStart[idx].size());
									i = -1;
								}

								token = option->MuitipleLineCommentStart[idx];

								isMultipleLineComment = true;
							}
							else if (0 == state && option->LineComment.empty() == false &&
									-1 != checkDelimiter(statement, i, option->LineComment)) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									char temp = statement[i];

									statement.Divide(i);
									aq->emplace_push(token, false);
									token.clear();
									
									statement[i] = temp;
									statement.LeftShift(i);
									i = 0;
								}
								int j = 0;
								for (j = i; j < statement.Size(); ++j) {
									if (statement[j] == '\n') // cf) '\r' ? '\0'?
									{
										break;
									}
								}
								--j; // "before enter key" or "before end"

								if (j - i + 1 > 0) {
									statement.Divide(j + 1);

									aq->emplace_push(std::string(statement.Str(), j - i + 1), true);
									statement.LeftShift(j + 2);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(j + 2);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
							}
							else {
								token.push_back(statement[i]);
							}
						}

						if (token.empty() == false)
						{
							aq->emplace_push(token, false);
						}
					}
				}
				//
				//DoThread(DoThread& other, tbb::split) : strVec(other.strVec) // , aq(other.aq)
				//{
					//
				//}
				//void join(DoThread& other) 
				//{
				//	aqpush(std::move(other.aq));
				//}
			};
			class DoThread2 // need to rename!
			{
			private:
				StringBuilder * strVec;
			public:
				ArrayQueue<Token>* aq;
				wiz::LoadDataOption option;
				//int strVecStart;
				//int strVecEnd;
			public:
				DoThread2(StringBuilder* strVec, ArrayQueue<Token>* aq, const wiz::LoadDataOption& option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
					: strVec(strVec), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					//
				}
			private:
				int checkDelimiter(const StringBuilder& statement, const int start, const std::vector<std::string>& delimiter)
				{

					int sum = 0;
					for (int delim_num = 0; delim_num < delimiter.size(); ++delim_num) {
						// size check
						if (start + delimiter[delim_num].size() - 1 > statement.Size() - 1) {
							continue;
						}

						for (int i = start; i <= start + delimiter[delim_num].size() - 1; ++i) {
							if (statement[i] == delimiter[delim_num][i - start]) {

							}
							else {
								sum--;
								break;
							}
						}
						sum++;

						if (sum > 0) {
							return delim_num;
						}
					}

					return -1;
				}
			public:
				void operator() () {
					//std::string* strVecTemp = strVec; // enterkey 기준으로 나뉘어져있다고 가정한다.
					//for (int x = 0; x <= 0; ++x)
					{
						//StringTokenizer tokenizer(std::move( (*strVecTemp)[x] ) );
						//while (tokenizer.hasMoreTokens()) {
						//	aq.push(tokenizer.nextToken());
						//}
						StringBuilder& statement = *strVec;
						int token_first = 0, token_last = 0; // idx of token in statement.
						int state = 0;
						bool isMultipleLineComment = false;
						std::string token;
						token.reserve(1024);

						for (int i = 0; i < statement.Size(); ++i) {
							int idx;

							if (statement[i] == '\0') {
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_push(token, false);
									token.clear();

									statement.LeftShift(i + 1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								aq->emplace_push("", true); // chk!
							}
							else if (isMultipleLineComment && -1 != (idx = checkDelimiter(statement, i, option.MuitipleLineCommentEnd)))
							{
								isMultipleLineComment = false;

								for (int j = 0; j < option.MuitipleLineCommentEnd[idx].size(); ++j)
								{
									token.push_back(statement[i + j]);
								}

								//aq->emplace_push(Token(token, true));

								i = i + option.MuitipleLineCommentEnd[idx].size() - 1;

								statement.Divide(i);
								statement.LeftShift(i + 1);

								token.clear();
								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else if (isMultipleLineComment) {
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (0 == state && '\'' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 2;
								//token_first = i; 
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (2 == state && '\\' == statement[i - 1] && '\'' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (2 == state && '\'' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && '\"' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 1;
								//token_first = i; 
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\\' == statement[i - 1] && '\"' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\"' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && -1 != (idx = Equal(option.Removal, statement[i])))
							{
								token_last = i - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_push(token, false);
									token.clear();

									statement.LeftShift(i + 1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = Equal(option.Assignment, statement[i]))) {
								token_last = i - 1;
								std::cout << "chk " << std::endl;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);
				
									aq->emplace_push(token, false);
									
									token.clear();
									

									aq->emplace_push(std::string("") + option.Assignment[idx], false);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_push(std::string("") + option.Assignment[idx], false);
									statement.LeftShift(1);

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && isWhitespace(statement[i])) { // isspace ' ' \t \r \n , etc... ?
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_push(token, false);

									statement.LeftShift(i + 1);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else
								{
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option.Left, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_push(token, false);
									statement.LeftShift(i + 1);

									aq->emplace_push(std::string("") + option.Left[idx], false);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_push(std::string("") + option.Left[idx], false);
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option.Right, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);


									aq->emplace_push(token, false);
									statement.LeftShift(i + 1);

									aq->emplace_push(std::string("") + option.Right[idx], false);

									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_push(std::string("") + option.Right[idx], false);

									statement.LeftShift(1);

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && option.MuitipleLineCommentStart.empty() == false
								&& -1 != (idx = checkDelimiter(statement, i, option.MuitipleLineCommentStart))) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);
									aq->emplace_push(token, false);

									statement.LeftShift(i + option.MuitipleLineCommentStart[idx].size());
									i = -1;

									token_first = 0;
									token_last = 0;
								}
								else {
									statement.LeftShift(i + option.MuitipleLineCommentStart[idx].size());
									i = -1;
								}

								token = option.MuitipleLineCommentStart[idx];

								isMultipleLineComment = true;
							}
							else if (0 == state && option.LineComment.empty() == false &&
								-1 != checkDelimiter(statement, i, option.LineComment)) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									char temp = statement[i];

									statement.Divide(i);
									aq->emplace_push(token, false);
									token.clear();

									statement[i] = temp;
									statement.LeftShift(i);
									i = 0;
								}
								int j = 0;
								for (j = i; j < statement.Size(); ++j) {
									if (statement[j] == '\n') // cf) '\r' ? '\0'?
									{
										break;
									}
								}
								--j; // "before enter key" or "before end"

								if (j - i + 1 > 0) {
									statement.Divide(j + 1);

									//aq->emplace_push(Token(std::string(statement.Str(), j - i + 1), true));
									statement.LeftShift(j + 2);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(j + 2);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
							}
							else {
								token.push_back(statement[i]);
							}
						}

						if (token.empty() == false)
						{
							aq->emplace_push(token, false);
						}
					}
				}
				//
				//DoThread(DoThread& other, tbb::split) : strVec(other.strVec) // , aq(other.aq)
				//{
				//
				//}
				//void join(DoThread& other) 
				//{
				//	aqpush(std::move(other.aq));
				//}
			};
		public:
			static bool Reserve3(std::ifstream& inFile, StringBuilder& strVecTemp, const int min_num = 1)
			{
				int count = 0;
				std::string temp;
				int brace = 0;
				int i = 0;

				for (int i = 0; i < min_num && (getline(inFile, temp)); ++i) {
					if (temp.empty()) { continue; }
					strVecTemp.Append(temp.c_str(), temp.size());
					strVecTemp.AppendChar('\n');
					for (int j = 0; j < temp.size(); ++j) {
						if (temp[j] == '{') {
							brace++;
						}
						else if (temp[j] == '}') {
							brace--;
						}
					}
					count++;
				}

				while (brace != 0 && getline(inFile, temp)) {
					if (temp.empty()) { continue; }
					strVecTemp.Append(temp.c_str(), temp.size());
					for (int j = 0; j < temp.size(); ++j) {
						if (temp[j] == '{') {
							brace++;
						}
						else if (temp[j] == '}') {
							brace--;
						}
					}
					count++;
				}

				return count > 0 && 0 == brace;
			}

			static std::pair<bool, int> Reserve2(std::ifstream& inFile, ArrayQueue<Token>& aq, const int num, const wiz::LoadDataOption& option)
			{
				//int a = clock();

				int count = 0;
				std::string temp;
				wiz::StringBuilder builder(128 * num); //

				builder.Clear();

				for (int i = 0; i < num &&
					(std::getline(inFile, temp)); ++i) {
					if (temp.empty()) { continue; }
					builder.Append(temp.c_str(), temp.size());
					builder.AppendChar('\n');
					count++;
				}
				
				DoThread doThread(&builder, &aq, &option);

				doThread(); // (0, count - 1);

				//tbb::parallel_reduce(tbb::blocked_range<size_t>(0, count), doThread);
				//aq.push(std::move(doThread.aq));
				//int b = clock();

				//std::cout << b - a << "ms" << std::endl;
				return{ count > 0, count };
			}

			// maybe remove?
			static std::pair<bool, int> Reserve2_2(std::ifstream& inFile, std::vector<ArrayQueue<Token>>& aq, const int num, const wiz::LoadDataOption& option, const int coreNum, const int offset)
			{
				// clear aq? empty empty data  ( remove empty )
				for (int i = 0; i < aq.size(); ++i) {
					for (int j = 0; j < aq[i].size(); ++j) {
						if (aq[i][j].str.empty()) {
							aq[i].pop_front();
							--j;
						}
						else {
							break;
						}
					}
				}

				int pass = 0;
				int count = 0;
				int before_count = 0;
				int count2 = 0;
				std::string temp;
				std::vector<wiz::StringBuilder> builder(coreNum, wiz::StringBuilder(32 * num / coreNum)); // *num;
				
				for (int i = 0; i < coreNum; ++i) {
					builder[i].Clear();
				}

				for (int i = 0; i < num &&
					(std::getline(inFile, temp)); ++i) {
					if (temp.empty()) { continue; }

					builder[(count) % coreNum].Append(temp.c_str(), temp.size());
					builder[(count) % coreNum].AppendChar('\n');
					

					if ((count2 + 1) % 50000 == 0)
					{
						builder[(count) % coreNum].AppendChar('\0'); // chk!
						before_count = count;
						count++;

						pass = 1;
					}
					else {
						pass = 0;
					}
					
					count2++;
				}

				if (count2) {
					if (0 == pass) {
						builder[(before_count) % coreNum].AppendChar('\0');
					}

					std::vector<std::thread*> threads(coreNum);

					// do parallel!
					for (int i = 0; i < coreNum; ++i) {
						DoThread2 doThread2(&builder[i], &aq[(i + offset) % coreNum], option);
						threads[(i + offset) % coreNum] = new std::thread(doThread2);
					}

					// wait
					for (int i = 0; i < coreNum; ++i) {
						threads[i]->join();
					}
					for (int i = 0; i < coreNum; ++i) {
						delete threads[i];
					}
				}

				return{ count2 > 0, count2 };
			}

			/// must lineNum > 0
			static std::pair<bool, int> Reserve(std::ifstream& inFile, std::list<std::string>& strVec, const int num = 1)
			{
				std::string temp;
				int count = 0;

				for (int i = 0; i < num && (inFile >> temp); ++i) {
					strVec.push_back(temp);
					count++;
				}
				return{ count > 0, count };
			}
		private:
			// chk!!
			template <class Reserver>
			static bool ChkComment(ArrayQueue<Token>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const int offset, const wiz::LoadDataOption& option)
			{
				if (strVec.size() < offset) {
					reserver(strVec, option);
					while (strVec.size() < offset) // 
					{
						reserver(strVec, option);
						if (
							strVec.size() < offset &&
							reserver.end()
							) {
							return false;
						}
					}
				}

				auto x = strVec.begin();
				int count = 0;

				do {
					if (x.ptr->isComment) {
						ut->PushComment((std::move(x.ptr->str)));
						x = strVec.erase(x);
					}
					else if (count == offset - 1) {
						return true;
					}
					else {
						count++;
						x.ptr++;
						x.pos++;
					}

					if (x == strVec.end()) {
						reserver(strVec, option);
						x = strVec.begin(count);

						while (strVec.size() < offset) // 
						{
							reserver(strVec, option);
							x = strVec.begin(count);
							if (
								strVec.size() < offset &&
								reserver.end()
								) {
								return false;
							}
						}
					}
				} while (true);
			}
		public:
			template<class Reserver>
			static std::string_view Top(ArrayQueue<Token>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option)
			{
				if (strVec.empty() || strVec[0].isComment) {
					if (false == ChkComment(strVec, ut, reserver, 1, option)) {
						return std::string_view();
					}
				}
				if (strVec.empty()) { return std::string_view(); }
				return strVec[0].str;
			}
			template <class Reserver>
			static bool Pop(ArrayQueue<Token>& strVec, std::string* str, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option)
			{
				if (strVec.empty() || strVec[0].isComment) {
					if (false == ChkComment(strVec, ut, reserver, 1, option)) {
						return false;
					}
				}

				if (strVec.empty()) {
					return false;
				}

				if (str) {
					Token token;
					strVec.pop_front(&token);
					*str = (std::move(token.str));
					//*str = move(strVec.front().str);
				}
				else {
					strVec.pop_front();
				}
				//strVec.pop_front();

				return true;
			}

			// lookup just one!
			template <class Reserver>
			static std::pair<bool, std::string_view> LookUp(ArrayQueue<Token>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option)
			{
				if (!(strVec.size() >= 2 && false == strVec[0].isComment && false == strVec[1].isComment)) {
					if (false == ChkComment(strVec, ut, reserver, 2, option)) {
						return{ false, "" };
					}
				}
			
				if (strVec.size() >= 2) {
					return{ true, strVec[1].str };
				}
				return{ false, "" };
			}
			
			template<class Reserver>
			static std::string_view Top(std::vector<ArrayQueue<Token>>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option, int* thread_id)
			{
				const int coreNum = strVec.size();
				int next_thread_id = *thread_id;
				int ok = 0;

				while (strVec[next_thread_id].empty()) {
					next_thread_id = (*thread_id + 1) % coreNum;
					*thread_id = next_thread_id;
					ok++;
					if (ok == strVec.size() && strVec[next_thread_id].empty()) {
						//std::cout << "errorA" << std::endl;
						break;
					}
				}

				while (strVec[next_thread_id].empty() == false && strVec[next_thread_id].front().str.empty()) {
					strVec[next_thread_id].pop_front();

					next_thread_id = (*thread_id + 1) % coreNum;

					*thread_id = next_thread_id;
				}

				if (strVec[next_thread_id].empty()) { return std::string_view(); }
				return strVec[next_thread_id][0].str;
			}
			template <class Reserver>
			static bool Pop(std::vector<ArrayQueue<Token>>& strVec, std::string* str, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option, int* thread_id)
			{
				const int coreNum = strVec.size();
				int next_thread_id = *thread_id;
				int ok = 0;

				while (strVec[next_thread_id].empty()) {
					next_thread_id = (*thread_id + 1) % coreNum;
					*thread_id = next_thread_id;
					ok++;
					if (ok == strVec.size() && strVec[next_thread_id].empty()) {
						//std::cout << "errorB" << std::endl;
						break;
					}
				}

				while (strVec[next_thread_id].empty() == false && strVec[next_thread_id].front().str.empty()) {
					strVec[next_thread_id].pop_front();
					next_thread_id = (*thread_id + 1) % coreNum;

					*thread_id = next_thread_id;
				}

				if (strVec[next_thread_id].empty()) {
					return false;
				}

				if (str) {
					Token token;
					strVec[next_thread_id].pop_front(&token);
					*str = (std::move(token.str));
					//*str = move(strVec.front().str);
				}
				else {
					strVec[next_thread_id].pop_front();
				}
				//strVec.pop_front();

				return true;
			}

			// lookup just one!
			template <class Reserver>
			static std::pair<bool, std::string_view> LookUp(std::vector<ArrayQueue<Token>>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option, int* thread_id)
			{
				const int coreNum = strVec.size();
				int next_thread_id = *thread_id;
				int next_thread_id2 = *thread_id;
				int ok = 0;

				while (strVec[next_thread_id].empty()) {
					next_thread_id = (*thread_id + 1) % coreNum;
					*thread_id = next_thread_id;
					ok++;
					if (ok == strVec.size() && strVec[next_thread_id].empty()) {
						//std::cout << "errorC" << std::endl;
						break;
					}
				}

				while (strVec[next_thread_id].empty() == false && strVec[next_thread_id].front().str.empty()) {
					strVec[next_thread_id].pop_front();

					next_thread_id = (*thread_id + 1) % coreNum;
					next_thread_id2 = next_thread_id;

					if (strVec[next_thread_id].size() == 1) {
						next_thread_id2 = (next_thread_id + 1) % coreNum;
					}
					
					*thread_id = next_thread_id2;
				}

				if (strVec[next_thread_id].size() >= 2) {
					return{ true, strVec[next_thread_id][1].str };
				}

				return{ false, "" };
			}

		public:
			//
			static bool ChkExist(const std::string& str) // for \", str is separated by enterkey
			{
				bool start = false;
				int state = 0;

				for (std::string::size_type i = 0; i < str.size(); ++i)
				{
					if (0 == state && i == 0 && '\"' == str[i]) {
						state = 1;
						start = true;
					}
					else if (0 == state && i == 0 && '\'' == str[i]) {
						state = 2;
						start = true;
					}
					else if (0 == state && i > 0 && '\"' == str[i] && '\\' != str[i - 1])
					{
						state = 1;
						start = true;
					}
					else if (0 == state && i > 0 && '\'' == str[i] && '\\' != str[i - 1])
					{
						state = 2;
						start = true;
					}
					else if (1 == state && i > 0 && '\\' != str[i - 1] && '\"' == str[i]) {
						state = 0;
					}
					else if (2 == state && i > 0 && '\\' != str[i - 1] && '\'' == str[i]) {
						state = 0;
					}
					else if (0 == state && str[i] == '#') {
						break;
					}
				}

				return start && 0 == state; // exist and valid !! chk - todo!
			}
			

			// AddSpace : return std::string
			static void AddSpace(const std::string& str, std::string& temp)
			{
				temp = "";

				for (int i = 0; i < str.size(); ++i)
				{
					/// To Do - chabnge to switch statement.
					if ('=' == str[i]) {
						temp.push_back(' ');
						temp.push_back('=');
						temp.push_back(' ');
					}
					else if ('{' == str[i]) {
						temp.push_back(' ');
						temp.push_back('{');
						temp.push_back(' ');
					}
					else if ('}' == str[i]) {
						temp.push_back(' ');
						temp.push_back('}');
						temp.push_back(' ');
					}
					else {
						temp.push_back(str[i]);
					}
				}

				//return temp;
			}

			/// need testing!
			static void PassSharp(const std::string& str, std::string& temp) { // txt file로 들어온다?
				temp = "";
				int state = 0;

				for (int i = 0; i < str.size(); ++i) {
					if (str[i] == '#') { state = 1; continue; }
					else if (str[i] == '\n') { state = 0; continue; }

					if (0 == state) {
						temp.push_back(str[i]);
					}
				}
				//return temp;
			}
			
			static bool _ChangeStr(const std::string& str, const std::vector<std::string>& changed_str, const std::vector<std::string>& result_str, std::string::size_type& i, int& state, std::string& temp) {
				for (std::string::size_type j = 0; j < changed_str.size(); ++j) {
					if (wiz::String::Comp(changed_str[j].c_str(), str.c_str() + i, changed_str[j].size())) {
						state = 1;
						temp.append(result_str[j]);
						i = i + changed_str[j].size() - 1;
						return true;
					}
				}
				return false;
			}
			/*

			// 길이가 긴 문자열이 먼저 나와야 한다?
			static void ChangeStr(const std::string& str, const std::vector<std::string>& changed_str, const std::vector<std::string>& result_str, std::string& temp) {
				temp = "";
				int state = 0;


				for (std::string::size_type i = 0; i < str.size(); ++i)
				{
					if (0 == state && i == 0 && '\"' == str[i]) {
						state = 1;
						temp.push_back(str[i]);
					}
					else if (0 == state && i > 0 && '\"' == str[i] && '\\' != str[i - 1])
					{
						state = 1;
						temp.push_back(str[i]);
					}
					else if (1 == state  && _ChangeStr(str, changed_str, result_str, i, state, temp)) {
						//
					}
					else if ((1 == state && i > 0 && '\\' != str[i - 1] && '\"' == str[i])) {
						state = 0;
						temp.push_back('\"');
					}
					else
					{
						temp.push_back(str[i]);
					}
				}

				//return temp;
			}
			*/
			// no chk "
			static void ChangeStr2(const std::string& str, const std::vector<std::string>& changed_str, const std::vector<std::string>& result_str, std::string& temp) {
				temp = "";
				int state = 1;

				for (std::string::size_type i = 0; i < str.size(); ++i)
				{
					if(_ChangeStr(str, changed_str, result_str, i, state, temp)) {
						//
					}
					else {
						temp.push_back(str[i]);
					}
				}

				//return temp;
			}
		};
	}
}

#endif
