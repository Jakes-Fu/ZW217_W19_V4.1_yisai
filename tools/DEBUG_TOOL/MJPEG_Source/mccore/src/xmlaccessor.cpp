#include <windows.h>
#include <atlstr.h>
#include "xmlaccessor.h"

void CXmlAccessor::New(char* xmlVersion, char* rootNodeName, char* rootNodeValue)
{
	Close();
	doc = xmlNewDoc((xmlChar*)xmlVersion);
	cur = xmlNewDocNode(doc, 0, (xmlChar*)rootNodeName, (xmlChar*)rootNodeValue);
	xmlDocSetRootElement(doc, cur);
}

int CXmlAccessor::Open(char* xmlfile)
{
	doc = xmlParseFile(xmlfile);
	if (doc == NULL ) {
		return -1;
	}
	cur = xmlDocGetRootElement(doc);
	return 0;
}

int CXmlAccessor::Save(char* xmlfile)
{
	return xmlSaveFormatFile(xmlfile, doc, 1);
}
void CXmlAccessor::Close()
{
	if (value) {
		xmlFree(value);
		value = 0;
	}
	if (doc) {
		xmlFreeDoc(doc);
		xmlCleanupParser();
		doc = 0;
	}
}

int CXmlAccessor::LoadKey(char* xpath)
{
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
		return 0;
	}
	result = xmlXPathEvalExpression((xmlChar*)xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return 0;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
		printf("No result\n");
		return 0;
	}
	return result->nodesetval->nodeNr;
}


char* CXmlAccessor::GetKeyValue(int index)
{
	xmlNodeSetPtr nodeset = result->nodesetval;
	if (value) xmlFree(value);
	if (index >= nodeset->nodeNr)
		return NULL;
	value = xmlNodeListGetString(doc, nodeset->nodeTab[index]->xmlChildrenNode, 1);
	return (char*)value;
}

xmlNodePtr CXmlAccessor::GoRoot()
{
	return (cur = xmlDocGetRootElement(doc));
}

xmlNodePtr CXmlAccessor::GoNext()
{
	if (cur && cur->next)
		return (cur = cur->next);
	else
		return 0;
}

xmlNodePtr CXmlAccessor::GoPrev()
{
	if (cur && cur->prev)
		return cur = cur->prev;
	else
		return 0;
}

xmlNodePtr CXmlAccessor::GoChild()
{
	if (cur && cur->xmlChildrenNode)
		return (cur = cur->xmlChildrenNode->next);
	else
		return 0;
}

xmlNodePtr CXmlAccessor::GoParent()
{
	if (cur && cur->parent)
		return (cur = cur->parent);
	else
		return 0;
}

char* CXmlAccessor::GetKeyValue()
{
	if (value) xmlFree(value);
	value = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	//value = xmlNodeGetKeyValue(cur);
	return (char*)value;
}

void CXmlAccessor::SetKeyValue(char* value)
{
	xmlNodeSetContent(cur, (const xmlChar*)value);
}

bool CXmlAccessor::IsMatched(char* match)
{
	return cur ? (!xmlStrcmp(cur->name, (const xmlChar *)match)) : false;
}

char* CXmlAccessor::GetProperty(char* propname)
{
	if (value) xmlFree(value);
	value = xmlGetProp(cur, (const xmlChar *)propname);
	return (char*)value;
}

char* CXmlAccessor::GetNodeName()
{
	return (char*)(cur ? cur->name : 0);
}

xmlNodePtr CXmlAccessor::AddTextNode(char* nodename, char* value)
{
	return xmlNewTextChild(cur, NULL, (const xmlChar*)nodename, (const xmlChar*)value);;
}


void CXmlAccessor::SetNodeName(char* nodename)
{
	if (cur)
		xmlNodeSetName(cur, (xmlChar*)nodename);
}
