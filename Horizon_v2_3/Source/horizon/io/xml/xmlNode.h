#ifndef xmlNode_H
#define xmlNode_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace horizon {
namespace io {
namespace xml {

class xmlNode
{
	map<string, string> attributes;
	vector<xmlNode> childNodes;
	string tagName;
	string data;

public:
	xmlNode(string tagName);

	xmlNode(string tagName, string data);

	xmlNode(string tagName, map<string, string> attributes);	

	xmlNode(string tagName, map<string, string> attributes, string data);

	xmlNode(string tagName, vector<xmlNode> childNodes);

	xmlNode(string tagName, vector<xmlNode> childNodes, string data);

	xmlNode(string tagName, map<string, string> attributes, vector<xmlNode> childNodes);

	xmlNode(string tagName, map<string, string> attributes, vector<xmlNode> childNodes, string data);

	~xmlNode();

	template <typename T>
	void addAttribute(string attribute, T value) {
		ostringstream o;
		o << value;
		attributes[attribute] = o.str();
	}

	void addAttribute(string attribute, string value);

	void removeAttribute(string attribute);

	void addChildNode(xmlNode);

	void writeFile(string filename);

private:
	void writeNodeToFile(ostringstream& outstream, int nestIndex);

};

}}} //end namespace horizon::io::xml
#endif