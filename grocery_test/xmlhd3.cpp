#include "stdafx.h"
//#include "grocery/xmlhd3.h"
//#include "grocery/path.h"
//#include <iostream>
//#include "grocery/xmlhd3_wrap.h"
//#include "grocery/file.h"
//#include <cassert>
//
//using namespace std;
//using namespace guru;
//
////int main()
////{
////	xmlhd3_wrap::base_type* x = new xmlhd3_wrap("sample.xml");
////	assert(x->ready());
////
////	XMLElement& root = x->root();
////	cout << root["product"].v(0) << endl;
////	cout << root["product"]["catalog_item"]["price"].GetContent() << endl;
////	cout << root.AddVariable("n", "val").Serialize() << endl;
////	delete x;
////
////	x = new xmlhd3_wrap("config1.xml");
////	assert(x->ready());
////	std::vector<std::shared_ptr<XMLElement>> ids = x->all("ID");
////	//assert(ids.size() == 16);
////	ids = x->all("nonexist");
////	assert(ids.size() == 0);
////	delete x;
////
////	//x = new xmlhd3_wrap("1.config");
////	//assert(x->ready());
////
////	system("pause");
////	return 0;
////}
//
//int main()
//{
//	xmlhd3_wrap::base_type* x = new xmlhd3_wrap("sample.config");
//	assert(x->ready());
//	//assert(x->root().GetElementName() == "ROOT");
//
//	//std::vector<std::shared_ptr<XMLElement>> data = x->all("DATA");
//	//assert(data.size() == 0);
//
//	system("pause");
//	return 0;
//
//}