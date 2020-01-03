#include "stdafx.h"
#include "grocery/tinyxml2.h"
#include "grocery/tinyxml2_wrap.h"
#include <cassert>

using namespace std;
using namespace guru;

//int main()
//{
//	guru::XMLDocument doc;
//	auto rst = doc.LoadFile("rdv2_config.xml");
//	assert(rst == guru::XML_SUCCESS);
//	guru::XMLElement* root = doc.RootElement();
//	assert(0 == ::strcmp("CORE_ID", root->Name()));
//
//	rst = doc.LoadFile("model.config");
//	assert(rst == guru::XML_SUCCESS);
//	root = doc.RootElement();
//	assert(0 == ::strcmp("ROOT", root->Name()));
//
//	system("pause");
//	return 0;
//}

//int main()
//{
//	tinyxml2_wrap tinyw("rdv2_config.xml");
//	vector<tinyxml2_wrap::element_type> ids = tinyw.all("ID");
//	assert(ids.size() == 15);
//
//	tinyxml2_wrap tinyw2("book.xml");
//	ids = tinyw2.all("book");
//	assert(ids.size() == 6);
//	ids = tinyw2.all("nonexists");
//	assert(ids.size() == 0);
//	ids = tinyw2.all("book", "root");
//	assert(ids.size() == 1);
//	ids = tinyw2.all("book", "section1");
//	assert(ids.size() == 2);
//	ids = tinyw2.all("book", "section1", "root");
//	assert(ids.size() == 2);
//	ids = tinyw2.all("book", "section2");
//	assert(ids.size() == 1);
//	ids = tinyw2.all("book", "section22", "section2");
//	assert(ids.size() == 2);
//
//	system("pause");
//	return 0;
//}

int main()
{
	tinyxml2_wrap tiny("model.config");
	vector<tinyxml2_wrap::element_type> ids = tiny.all("CLASS", "CLASS_DECLARE");
	assert(ids.size() == 2);
	ids = tiny.all("DATA", "INPUT_DATA");
	assert(ids.size() == 4);
	ids = tiny.all("DATA", "INNER_DATA");
	assert(ids.size() == 3);
	ids = tiny.all("DATA", "OUTPUT_DATA");
	assert(ids.size() == 16);
	system("pause");
	return 0;
}