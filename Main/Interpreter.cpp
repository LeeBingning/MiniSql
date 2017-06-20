#pragma once
#include "stdafx.h"
#include "Interpreter.h"
#include "API.h"

Interpreter::Interpreter()
{
}
bool Interpreter::judge(char k)
{
	if (k == ' ' || k == '\n' || k == '\'' || k == ',')
		return false;
	return true;
}
void Interpreter::getCondition(string&cmd,vector<Condition>* conditionList)
{
	Condition * condition;
	string attri, ope, value;
	while (true)
	{
		attri = getWord(cmd);
		ope = getWord(cmd);
		value = getWord(cmd);
		condition = new Condition(attri, value, 0);
		if (ope == "=")
		{
			condition->setOperateType(2);
		}
		else if (ope == "<>")
		{
			condition->setOperateType(-1);
		}
		else if (ope == "<")
		{
			condition->setOperateType(0);
		}
		else if (ope == ">")
		{
			condition->setOperateType(4);
		}
		else if (ope == "<=")
		{
			condition->setOperateType(1);
		}
		else if (ope == ">=")
		{
			condition->setOperateType(3);
		}
		conditionList->push_back(*condition);
		delete condition;
		if (cmd.length() == 0) break;
	}
}
bool Interpreter::getCreateTable(string& cmd)
{
	string tableName=getWord(cmd);
	vector<Attribute>* attriList;
	API* api=new API();
	Attribute attribute;
	string word,type,foo;
	word = getWord(cmd);
	type = getWord(cmd);
	while (!(word == "primary"&&type == "key"))
	{
		attribute.name = word;
		if (type == "int")
			attribute.type = attribute.INT;
		else if (type == "float")
			attribute.type = attribute.FLOAT;
		else
		{
			foo = getWord(cmd);
			attribute.type=atoi(foo.c_str);
		}
		foo = "";
		foo = getWord(cmd);
		if (foo == "unique")
			attribute.unique = 1;
		else
			attribute.unique = 0;
		attriList->push_back(attribute);
		if (foo == "unique")
		{
			word = getWord(cmd);
			type = getWord(cmd);
		}
		else
		{
			word = foo;
			type = getWord(cmd);
		}
	}
	word = getWord(cmd);
	int i,pos,flag=0;
	for (i=0;i<attriList->size;i++)
		if ((*attriList)[i].name == word)
		{
			pos = i;
			if (api->createTable(tableName, attriList, word, pos) == false)
			{
				flag = 1;
				break;
			}
				
		}
	if (i == attriList->size)
		flag = 1;
	delete attriList;
	delete api;
	return flag;
	
}
bool Interpreter::getCreateIndex(string&cmd)
{
	string indexName, tableName,columnName;
	indexName = getWord(cmd);
	if (getWord(cmd) != "on")
		return false;
	tableName = getWord(cmd);
	if (getWord(cmd) != "(")
		return false;
	columnName = getWord(cmd);
	if (getWord(cmd) != ")")
		return false;
	API* api = new API();
	api->createIndex(indexName, tableName, columnName);
}
bool Interpreter::getDropTable(string&cmd)
{
	string tableName = getWord(cmd);
	API* api = new API();
	bool flag=api->dropTable(tableName);
	delete api;
	return flag;
}
bool Interpreter::getDropIndex(string&cmd)
{
	string indexName = getWord(cmd);
	API* api = new API();
	bool flag = api->dropIndex(indexName);
	delete api;
	return flag;
}
bool Interpreter::getSelect(string&cmd)
{
	if (getWord(cmd) != "*")
		return false;
	if (getWord(cmd) != "from")
		return false;
	string tableName = getWord(cmd);
	if (getWord(cmd) != "where")
		return false;
	vector<Condition>* conditionList;
	getCondition(cmd, conditionList);
	API* api = new API();
	api->select(tableName, conditionList);
	delete api;
}
bool Interpreter::getInsert(string&cmd)
{
	string tableName;
	if (getWord(cmd) != "into")
		return false;
	tableName = getWord(cmd);
	if (getWord(cmd) != "values")
		return false;
	API * api = new API();
	string record="";
	while (cmd.length != 0)
	{
		record += getWord(cmd);
	}
	api->insert(tableName,&record);
	return 0;
}
bool Interpreter::getDelete(string&cmd)
{
	API *api = new API();
	if (getWord(cmd) != "from")
		return false;
	string tableName = getWord(cmd);
	vector<Condition> *conditionList;
	getCondition(cmd, conditionList);
	api->deleteFromTable(tableName,conditionList);
	return true;
}
bool Interpreter::getExecfile(string&)
{
	return 0;
}
string Interpreter::getWord(string &cmd)
{
	while (judge(cmd[0])==false)
	{
		cmd.erase(0, 1);
	}
	string ret = "";
	while (cmd.length() != 0 && judge(cmd[0]) == true)
	{
		ret += cmd[0];
		cmd.erase(0, 1);
	}
	return ret;
}
string Interpreter::lowwerCase(string& cmd)
{
	int i = 0;
	while (i < cmd.length())
	{
		if (int(cmd[i]) >= 65 && int(cmd[i]) <= 90) cmd[i] = char(int(cmd[i]) + 32);
		i++;
	}
	return cmd;
}
bool Interpreter::Parse(string cmd)
{
	cmd = cmd.substr(0, cmd.find(';'));
	cmd = lowwerCase(cmd);
	string option = getWord(cmd);
	if (option == "select")
	{
		return getSelect(cmd);
	}
	else
	if (option == "create")
	{
		string word = getWord(cmd);
		if (word == "index")
		{
			return getCreateIndex(cmd);
		}
		else
		if (word == "table")
		{
			return getCreateTable(cmd);
		}
		else
			return false;
	}
	else
	if (option == "drop")
	{
		string word = getWord(cmd);
		if (word == "index")
		{
			return getDropIndex(cmd);
		}
		else
		if (word == "table")
		{
			return getDropTable(cmd);
		}
		else
			return false;
	}
	else
	if (option == "insert")
	{
		return getInsert(cmd);
	}
	else
	if (option == "delete")
	{
		return getDelete(cmd);
	}
	else
	if (option == "execfile")
	{
		return getExecfile(cmd);
	}
	else
	{
		return false;
	}
}
Interpreter::~Interpreter()
{
}