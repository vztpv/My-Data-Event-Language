

#define _CRT_SECURE_NO_WARNINGS

//#define ARRAYS_DEBUG
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

#include <wiz/load_data.h>
#include <wiz/stacks.h>
#include <wiz/cpp_string.h>


class EventInfo
{
public:
	wiz::load_data::UserType* eventUT;
	wiz::ArrayStack< wiz::load_data::UserType* > nowUT; //
	int item_idx;
	wiz::ArrayStack<int> userType_idx;
	vector<pair<string, string>> parameters;
	string id; //
	wiz::ArrayStack<string> conditionStack;
	int state;
public:
	EventInfo() : eventUT(NULL), state(0)
	{

	}
};


// only one exist or do not exist?
inline string GetItem(const wiz::load_data::UserType* ut, const string& name) {
	return ut->GetItem(name)[0].Get(0);
}

inline wiz::load_data::UserType* GetUserType(const wiz::load_data::UserType* ut, const string& name)
{
	return ut->GetUserTypeItem(name)[0].Get(0);
}
string Find(wiz::load_data::UserType* ut, const string& str)
{
	int idx = -1;
	for (int i = str.size() - 1; i >= 0; --i) {
		if ('/' == str[i]) {
			idx = i;
			break;
		}
	}
	//
	// exception?
	//
	return wiz::load_data::UserType::Find(ut,
		wiz::String::substring(str, 0, idx)).second[0]->GetItem(wiz::String::substring(str, idx + 1))[0].Get(0);
}
// to do - rename!
pair<string, string> Find2(wiz::load_data::UserType* ut, const string& str)
{
	int idx = -1;
	for (int i = str.size() - 1; i >= 0; --i) {
		if ('/' == str[i]) {
			idx = i;
			break;
		}
	}
	return{ wiz::String::substring(str, 0, idx), wiz::String::substring(str,idx + 1) };
}
string reverse(string str) { /// to std::reverse ?
	std::reverse(str.begin(), str.end());
	return str;
}

string GetType(const string& str) {
	if (wiz::load_data::Utility::IsInteger(str)) { return "INTEGER"; }
	else if (wiz::load_data::Utility::IsDouble(str)) { return "DOUBLE"; }
	else if (wiz::load_data::Utility::IsDate(str)) { return "DATE"; }
	else return "STRING";
}
string Compare(const string& str1, const string& str2, const int type = 0)
{
	string type1 = GetType(str1);
	string type2 = GetType(str2);

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
		if (wiz::load_data::Utility::IsMinus(str1) && !wiz::load_data::Utility::IsMinus(str2)) { return "< 0"; }
		else if (!wiz::load_data::Utility::IsMinus(str1) && wiz::load_data::Utility::IsMinus(str2)) { return "> 0"; }

		const bool minusComp = wiz::load_data::Utility::IsMinus(str1) && wiz::load_data::Utility::IsMinus(str2);

		if (false == minusComp) {
			string x = reverse(str1);
			string y = reverse(str2);

			if (x[0] == '+') { x = string(x.c_str() + 1); }
			if (y[0] == '+') { y = string(y.c_str() + 1); }

			if (x.size() < y.size()) {
				for (int i = 0; i < y.size() - x.size(); ++i) {
					x.push_back('0');
				}
			}
			else {
				for (int i = 0; i < x.size() - y.size(); ++i) {
					y.push_back('0');
				}
			}
			return Compare(reverse(x), reverse(y), 1);
		}
		else {
			return Compare(string(str2.c_str() + 1), string(str1.c_str() + 1));
		}
	}
	else if ("DOUBLE" == type1)
	{
		wiz::StringTokenizer tokenizer1(str1, ".");
		wiz::StringTokenizer tokenizer2(str2, ".");

		string x = tokenizer1.nextToken();
		string y = tokenizer2.nextToken();

		string z = Compare(x, y);
		if ("= 0" == z)
		{
			x = tokenizer1.nextToken();
			y = tokenizer2.nextToken();

			if (x.size() < y.size()) {
				for (int i = 0; i < y.size() - x.size(); ++i) {
					x.push_back('0');
				}
			}
			else {
				for (int i = 0; i < x.size() - y.size(); ++i) {
					y.push_back('0');
				}
			}
			return Compare(x, y, 1);
		}
		else
		{
			return z;
		}
	}
	else if ("DATE" == type1)
	{
		wiz::StringTokenizer tokenizer1(str1, ".");
		wiz::StringTokenizer tokenizer2(str2, ".");

		for (int i = 0; i < 3; ++i) {
			const string x = tokenizer1.nextToken();
			const string y = tokenizer2.nextToken();

			const string comp = Compare(x, y);

			if (comp == "< 0") { return comp; }
			else if (comp == "> 0") { return comp; }
		}
		return "== 0";
	}

	return "ERROR";
}

inline string FindParameters(const vector<pair<string, string>>& parameters, const string& operand)
{
	for (int i = 0; i < parameters.size(); ++i) {
		if (wiz::String::startsWith(operand, "$parameter.")
			&& wiz::String::substring(operand, 11) == parameters[i].first) {
			return parameters[i].second;
		}
	}
	return "";
}
void operation(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& str, wiz::ArrayStack<string>& operandStack)
{
	if (!operandStack.empty() && operandStack.top() == "ERROR") { return; }

	if ("$EQ" == str) {
		string x, y;
		int idx = -1;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}

		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (x == y) {
			operandStack.push("TRUE");
		}
		else {
			operandStack.push("FALSE");
		}
	}
	else if ("$NOTEQ" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (x != y) {
			operandStack.push("TRUE");
		}
		else {
			operandStack.push("FALSE");
		}
	}
	else if ("$COMP<" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (Compare(x, y) == "< 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$COMP>" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (Compare(x, y) == "> 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$COMP<EQ" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (Compare(x, y) == "< 0" || Compare(x, y) == "== 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$COMP>EQ" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (Compare(x, y) == "> 0" || Compare(x, y) == "== 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$add" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer? -> BigInteger?
			operandStack.push(wiz::toStr(atoll(x.c_str()) + atoll(y.c_str())));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$multiple" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer? -> BigInteger?
			operandStack.push(wiz::toStr(atoll(x.c_str()) * atoll(y.c_str())));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$divide" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer? -> BigInteger?
			operandStack.push(wiz::toStr(atoll(x.c_str()) / atoll(y.c_str())));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$modular" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if ('/' == x[0])
		{
			x = Find(&global, x);
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}

		if ('/' == y[0])
		{
			y = Find(&global, y);
		}
		{
			string temp = FindParameters(parameters, y);
			if (!temp.empty()) { y = temp; }
		}

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer? -> BigInteger?
			operandStack.push(wiz::toStr(atoll(x.c_str()) % atoll(y.c_str())));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$rand" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();
		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer? -> BigInteger?
			long long _x = atoll(x.c_str());
			long long _y = atoll(y.c_str());
			long long _z = rand() % (_y - _x + 1) + _x;
			operandStack.push(wiz::toStr(_z));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
}

// todo - rename! ToBool ~ ToBool4
string ToBool(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& temp)
{
	wiz::ArrayStack<string> operandStack; // 피연산자
	wiz::ArrayStack<string> operatorStack; // 연산자

	wiz::StringTokenizer tokenizer(temp, { " ", "\n", "\t", "\r", "{", "=", "}" });
	vector<string> tokenVec;

	while (tokenizer.hasMoreTokens()) {
		tokenVec.push_back(tokenizer.nextToken());
	}

	for (int i = tokenVec.size() - 1; i >= 0; --i)
	{
		if ('/' == tokenVec[i][0])
		{
			tokenVec[i] = Find(&global, tokenVec[i]);
		}
		else if (wiz::String::startsWith(tokenVec[i], "$parameter.")) {
			string temp = FindParameters(parameters, tokenVec[i]);
			if (!temp.empty()) { tokenVec[i] = temp; }
		}

		if ('$' != tokenVec[i][0]) {
			operandStack.push(tokenVec[i]);
		}
		else
		{
			operatorStack.push(tokenVec[i]);
			operation(global, parameters, tokenVec[i], operandStack);
		}
	}

	return operandStack[0];
}
wiz::ArrayStack<string> ToBool2(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& temp)
{
	wiz::ArrayStack<string> operandStack; // 피연산자
	wiz::ArrayStack<string> operatorStack; // 연산자

	wiz::StringTokenizer tokenizer(temp, { " ", "\n", "\t", "\r", "{", "=", "}" });
	vector<string> tokenVec;

	while (tokenizer.hasMoreTokens()) {
		tokenVec.push_back(tokenizer.nextToken());
	}

	for (int i = tokenVec.size() - 1; i >= 0; --i)
	{
		if ('/' == tokenVec[i][0])
		{
			tokenVec[i] = Find(&global, tokenVec[i]);
		}
		else if(wiz::String::startsWith(tokenVec[i], "$parameter.")) {
			string temp = FindParameters(parameters, tokenVec[i]);
			if (!temp.empty()) { tokenVec[i] = temp; }
		}

		if ('$' != tokenVec[i][0]) {
			operandStack.push(tokenVec[i]);
		}
		else
		{
			operatorStack.push(tokenVec[i]);
			operation(global, parameters, tokenVec[i], operandStack);
		}
	}

	return operandStack;
}
string ToBool3(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& temp)
{
	wiz::StringTokenizer tokenizer(temp, "/" );
	vector<string> tokenVec;
	string result;

	while (tokenizer.hasMoreTokens()) {
		tokenVec.push_back(tokenizer.nextToken());
	}

	for (int i = 0; i < tokenVec.size(); ++i)
	{
		result = result + "/";
		if (wiz::String::startsWith(tokenVec[i], "$parameter.")) {
			string temp = FindParameters(parameters, tokenVec[i]);
			if (!temp.empty()) { tokenVec[i] = temp; }
		}
		result = result + tokenVec[i];
	}

	return result;
}
string ToBool4(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& temp)
{
	wiz::StringTokenizer tokenizer(temp, { " ", "\n", "\t", "\r", "{", "=", "}" });
	vector<string> tokenVec;
	string result = temp;
	wiz::load_data::UserType ut;

	while (tokenizer.hasMoreTokens()) {
		tokenVec.push_back(tokenizer.nextToken());
	}

	for (int i = tokenVec.size() - 1; i >= 0; --i)
	{
		string before = tokenVec[i];
		if ('/' == tokenVec[i][0])
		{
			tokenVec[i] = Find(&global, tokenVec[i]);
			wiz::String::replace(temp, before, tokenVec[i]);
		}
		else if (wiz::String::startsWith(tokenVec[i], "$parameter.")) {
			string temp = FindParameters(parameters, tokenVec[i]);
			if (!temp.empty()) { 
				tokenVec[i] = temp; 
				wiz::String::replace(temp, before, tokenVec[i]);
			}
		}
	}
	
	wiz::load_data::LoadData::LoadDataFromString(result, ut);	
	wiz::ArrayStack<int> depthStack;
	wiz::ArrayStack<wiz::load_data::UserType*> utStack;
	depthStack.push(0);
	utStack.push(&ut);

	while (!depthStack.empty())
	{
		if (depthStack.top() >= utStack.top()->GetUserTypeListSize())
		{
			depthStack.pop();
			utStack.pop();
			continue;
		}
		wiz::load_data::UserType* temp = utStack.top()->GetUserTypeList(depthStack.top()).Get(0);
		if (temp->GetName()[0] == '$') // not $parameter.~
		{
			string value = ToBool(global, parameters, temp->GetName() + " = { " + temp->ToString() + " } ");
			string tempName = utStack.top()->GetName();

			utStack.top()->RemoveUserTypeList(depthStack.top());
			wiz::load_data::LoadData::AddData(*(utStack.top()), string(""), value, "TRUE");

			depthStack.top()++;
		}
		else
		{
			depthStack.top()++;
			depthStack.push(0);
			utStack.push(temp);
		}
	}
	
	result = ut.ToString();
	return result;
}


int main(void)
{
	srand(time(NULL));

	// data, event load..
	stack<EventInfo> eventStack;
	map<string, int> convert;
	wiz::load_data::UserType global;
	wiz::load_data::LoadData::LoadDataFromFile("test.txt", global);
	auto events = global.GetUserTypeItem("Event");
	auto Main = GetUserType(&global, "Main");
	//

	// event table setting?
	for (int i = 0; i < events.size(); ++i)
	{
		auto x = events[i].Get(0)->GetItem("id");
		if (!x.empty()) {
			//cout <<	x[0].Get(0) << endl;
			convert.insert(pair<string, int>(x[0].Get(0), i));
		}
		else {
			// error?
		}
	}

	// start from Main.
	{
		EventInfo info;
		info.eventUT = Main;
		//info.nowUT = NULL;
		info.item_idx = 0;
		info.userType_idx.push(0);
		info.parameters.push_back(
			make_pair("id", info.eventUT->GetUserTypeItem("$call")[0].Get(0)->GetItem("id")[0].Get(0))
		);
		info.id = info.parameters[0].second;
		eventStack.push(info);
	}

	// main loop
	while (!eventStack.empty())
	{
		EventInfo info = eventStack.top();
		string str;

		for (int i = 0; i < info.parameters.size(); ++i) {
			if (info.parameters[i].first == "id") {
				str = info.parameters[i].second;
				break;
			}
		}

		const int no = convert[str];
		bool pass = false;
		int state = 0;

		if (info.userType_idx.size() == 1 && info.userType_idx[0] >= events[no].Get(0)->GetUserTypeListSize())
		{
			eventStack.pop();
			continue;
		}

		//for (int i = eventStack.top().userType_idx.top(); i < events[no].Get(0)->GetUserTypeListSize(); ++i) 
		{
			wiz::load_data::UserType* val = NULL;
			if (eventStack.top().userType_idx.size() == 1) {
				val = events[no].Get(0)->GetUserTypeList(eventStack.top().userType_idx.top()).Get(0);
			}
			else
			{
				val = eventStack.top().nowUT.top();
			}

			while (true) {
				cout << val->GetName() << " id " << eventStack.top().id << endl;
				if ("$call" == val->GetName()) {
					cout << "$call " << val->GetItem("id")[0].Get(0) << endl;
					info.id = val->GetItem("id")[0].Get(0);
					info.eventUT = events[no].Get(0);
					info.userType_idx.clear();
					info.userType_idx.push(0);
					if (info.id != eventStack.top().id) {
						info.parameters.clear();
					}
					info.conditionStack.clear();

					//
					if (info.id != eventStack.top().id) {
						for (int j = 0; j < val->GetItemListSize(); ++j) {
							if (val->GetItemListSize() > 0) {
								string temp = ToBool(global, info.parameters, val->GetItemList(j).Get(0));
								info.parameters.push_back(make_pair(val->GetItemList(j).GetName(), temp));
							}
						}
						for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
							if (val->GetUserTypeListSize() > 0) {
								string temp = ToBool(global, info.parameters, val->GetUserTypeList(j).Get(0)->ToString());
								info.parameters.push_back(make_pair(val->GetUserTypeList(j).GetName(), temp));
							}
						}

						eventStack.top().userType_idx.top()++;
					}
					else {
						if (val->GetUserTypeListSize() > 0) {
							for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
								string temp = ToBool(global, info.parameters, val->GetUserTypeList(j).Get(0)->ToString());
								for (int k = 0; k < info.parameters.size(); ++k)
								{
									if (info.parameters[k].first == val->GetUserTypeList(j).GetName())
									{
										info.parameters[k].second = temp;
									}
								}
								// debug 
								//cout << temp << endl;
							}
						}
						eventStack.pop();
					}
					
					eventStack.push(info);
					pass = true;
					break;
				}
				else if ("$assign" == val->GetName())
				{
					pair<string, string> dir = Find2( &global, val->GetItemList(0).Get(0) );
					string data = ToBool(global, eventStack.top().parameters, val->GetUserTypeList(0).Get(0)->ToString());
					wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE");

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$insert" == val->GetName())
				{
					string dir = string(val->GetItemList(0).Get(0).c_str()+1);
					string value = val->GetUserTypeList(0).Get(0)->ToString();
					wiz::ArrayStack<string> value2 = ToBool2(global, eventStack.top().parameters, value);
					wiz::load_data::UserType ut, ut2;

					ut.Remove();// removal?
					ut.SetName(dir);
					if (!wiz::load_data::LoadData::ExistItem(global, dir, dir + "_count", "TRUE"))
					{
						ut2.SetName("0");
						global.AddUserTypeItem(wiz::load_data::UserType(dir));
						wiz::load_data::LoadData::AddData(global, dir, dir + "_count = 0", "TRUE");
					}
					else
					{
						string temp = global.GetUserTypeItem(dir)[0].Get(0)->GetItem(dir + "_count")[0].Get(0);
						ut2.SetName(wiz::toStr( atoi(temp.c_str()) + 1));
						wiz::load_data::LoadData::SetData(global, dir, dir + "_count", ut2.GetName(), "TRUE");
					}
					
					while (!value2.empty())
					{
						auto x = value2.pop();
						auto y = value2.pop();
						ut2.AddItem(x, y);
					}

					ut.AddUserTypeItem(ut2);
					wiz::load_data::LoadData::AddData(global, dir, ut.ToString(), "TRUE");

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$insert2" == val->GetName())
				{
					string dir = string(val->GetItemList(0).Get(0).c_str() + 1);
					string value = val->GetUserTypeList(0).Get(0)->ToString();
					dir = ToBool3(global, eventStack.top().parameters, dir);
					value = ToBool4(global, eventStack.top().parameters, value);
					
					wiz::load_data::LoadData::AddData(global, dir, value, "TRUE");
					
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove" == val->GetName())
				{
					//to do..
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$swap" == val->GetName())
				{
					string dir = string(val->GetItemList(0).Get(0).c_str() + 1);
					string value1 = val->GetUserTypeList(0).Get(0)->ToString();
					string value2 = val->GetUserTypeList(1).Get(0)->ToString();

					value1 = ToBool(global, eventStack.top().parameters, value1);
					value2 = ToBool(global, eventStack.top().parameters, value2);
					if (value1 != value2) {
						long long x = atoll(value1.c_str());
						long long y = atoll(value2.c_str());

						string temp = global.GetUserTypeItem(dir)[0].Get(0)->GetUserTypeList(x).Get(0)->ToString();
						string temp2 = global.GetUserTypeItem(dir)[0].Get(0)->GetUserTypeList(y).Get(0)->ToString();

						global.GetUserTypeItem(dir)[0].Get(0)->GetUserTypeList(x).Get(0)->Remove();
						global.GetUserTypeItem(dir)[0].Get(0)->GetUserTypeList(y).Get(0)->Remove();

						wiz::load_data::LoadData::AddData(global, dir + "/" + value1, temp2, "TRUE");
						wiz::load_data::LoadData::AddData(global, dir + "/" + value2, temp, "TRUE");
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$print" == val->GetName())
				{
					// to do
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$parameter" == val->GetName())
				{
					// to do..
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$if" == val->GetName()) // ToDo!!
				{
					string temp = val->GetUserTypeList(0).Get(0)->ToString();
					temp = ToBool(global, eventStack.top().parameters, temp);

					eventStack.top().conditionStack.push(temp);
					if ("TRUE" == temp)
					{
						eventStack.top().nowUT.push(val->GetUserTypeList(1).Get(0));
						val = eventStack.top().nowUT.top()->GetUserTypeList(0).Get(0); // empty chk?
						eventStack.top().userType_idx.push(0);
						eventStack.top().state = 1;
						state = 1;
					}
					else if ("FALSE" == temp)
					{
						eventStack.top().userType_idx.top()++;
						break;
					}
					else
					{
						// debug..
						cout << "Error Debug : " << temp << endl;
						return -1;
					}
				}
				else if ("$else" == val->GetName())
				{
					// if가 바로 앞에 나와야한다. - later?
					//
					if (!eventStack.top().conditionStack.empty() && "FALSE" == eventStack.top().conditionStack.top())
					{
						eventStack.top().nowUT.push(val->GetUserTypeList(0).Get(0));
						val = eventStack.top().nowUT.top()->GetUserTypeList(0).Get(0); // empty chk?
						eventStack.top().userType_idx.push(0);
						eventStack.top().state = 2;
						state = 2;
					}
					else
					{
						eventStack.top().userType_idx.top()++;
						break;
					}
				}

				//else 
				{
					///if (eventStack.top().state == 1 || eventStack.top().state == 2) // it is in "$if", "$else" statments..
					if( eventStack.top().state == 1 || eventStack.top().state == 2 )
					{
						if (eventStack.top().userType_idx.top() < eventStack.top().nowUT.top()->GetUserTypeListSize())
						{
 							val = eventStack.top().nowUT.top()->GetUserTypeList(eventStack.top().userType_idx.top()).Get(0);
							//eventStack.top().userType_idx.top()++;
						}
						else
						{
							eventStack.top().nowUT.pop();
							eventStack.top().userType_idx.pop();
							eventStack.top().conditionStack.pop();

							if (eventStack.top().nowUT.size() < 1)
							{
								eventStack.top().state = 0;
								state = 0;
							}
							eventStack.top().userType_idx.top()++;
							break;
						}
					}
					else {
						eventStack.top().userType_idx.top()++;
						break;
					}
				}
			}
		}
	}

	cout << global << endl;
	return 0;
}