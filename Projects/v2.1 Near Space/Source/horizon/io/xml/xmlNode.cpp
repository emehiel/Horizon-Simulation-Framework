#include "xmlNode.h"

namespace horizon {
namespace io {
namespace xml {

xmlNode::xmlNode(string tagName) :
	tagName(tagName) {}

xmlNode::xmlNode(string tagName, string data) :
	tagName(tagName),
	data(data) {}

xmlNode::xmlNode(string tagName, map<string, string> attributes) : 
	tagName(tagName),
	attributes(attributes) {}

xmlNode::xmlNode(string tagName, map<string, string> attributes, string data) : 
	tagName(tagName),
	attributes(attributes),
	data(data) {}

xmlNode::xmlNode(string tagName, vector<xmlNode> childNodes) : 
	tagName(tagName),
	childNodes(childNodes) {}

xmlNode::xmlNode(string tagName, vector<xmlNode> childNodes, string data) : 
	tagName(tagName),
	childNodes(childNodes),
	data(data) {}

xmlNode::xmlNode(string tagName, map<string, string> attributes, vector<xmlNode> childNodes) :
	tagName(tagName),
	attributes(attributes),
	childNodes(childNodes) {}

xmlNode::xmlNode(string tagName, map<string, string> attributes, vector<xmlNode> childNodes, string data) :
	tagName(tagName),
	attributes(attributes),
	childNodes(childNodes),
	data(data) {}

xmlNode::~xmlNode() {}

void xmlNode::addAttribute(string attribute, string value) {
	attributes[attribute] = value;
}

void xmlNode::removeAttribute(string attribute) {
	attributes.erase(attribute);
}


void xmlNode::addChildNode(xmlNode child) {
	childNodes.push_back(child);
}

void xmlNode::writeFile(string filename) {
	ofstream ofile;
	ofile.open(filename.c_str());

	string datatab(1, ' ');

	ostringstream o;
	o << "<";
	o << tagName;

	for(map<string, string>::const_iterator attIt = attributes.begin(); attIt != attributes.end(); attIt++)
		o << " " << attIt->first << "=\"" << attIt->second << "\"";

	o << ">\n";

	if(!data.empty())
		o << datatab << data << "\n";

	if(!childNodes.empty()) {
		for(vector<xmlNode>::iterator childIt = childNodes.begin(); childIt != childNodes.end(); childIt++) {
			childIt->writeNodeToFile(o, 1);
		}
	}

	o << "</";
	o << tagName;
	o << ">";
	ofile << o.str();
	ofile.close();
}

void xmlNode::writeNodeToFile(ostringstream& outstream, int nestIndex) {
	string tab(nestIndex, ' ');
	string datatab(nestIndex+1, ' ');

	outstream << tab << "<";
	outstream << tagName;
	
	for(map<string, string>::const_iterator attIt = attributes.begin(); attIt != attributes.end(); attIt++)
		outstream << " " << attIt->first << "=\"" << attIt->second << "\"";

	outstream << ">";
	outstream << "\n";

	if(!data.empty())
		outstream << datatab << data << "\n";

	if(!childNodes.empty()) {
		for(vector<xmlNode>::iterator childIt = childNodes.begin(); childIt != childNodes.end(); childIt++) {
			childIt->writeNodeToFile(outstream, nestIndex+1);
		}
	}

	outstream << tab << "</";
	outstream << tagName;
	outstream << ">";
	outstream << "\n";
}
}}} //end namespace horizon::io::xml