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

int main()
{
	tinyxml2_wrap tinyw("rdv2_config.xml");
	vector<tinyxml2_wrap::element_type> ids = tinyw.all("ID");
	assert(ids.size() == 15);

	system("pause");
	return 0;
}