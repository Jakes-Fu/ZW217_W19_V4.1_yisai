#include <libxml/xpath.h>
#include <libxml/parser.h>

typedef enum {
	XA_READ = 0,
	XA_WRITE,
} XA_MODE;

class CXmlAccessor
{
public:
	CXmlAccessor():doc(0),value(0),result(0) {}
	~CXmlAccessor() {
		Close();
	}
	void New(char* xmlVersion, char* rootNodeName, char* rootNodeValue);
	int Open(char* xmlfile);
	int Save(char* xmlfile);
	int LoadKey(char *xpath);
	xmlNodePtr GoRoot();
	xmlNodePtr GoNext();
	xmlNodePtr GoPrev();
	xmlNodePtr GoChild();
	xmlNodePtr GoParent();
	xmlNodePtr AddTextNode(char* nodename, char* value);
	void SetNodeName(char* nodename);
	char* GetNodeName();
	char* GetKeyValue();
	void SetKeyValue(char* value);
	char* GetProperty(char* propname);
	char* GetKeyValue(int index);
	bool IsMatched(char* match);
	void Close();
	xmlNodePtr cur;
private:
	xmlAttrPtr attr;
	xmlDocPtr doc;
	xmlXPathObjectPtr result;
	xmlChar *value;
};