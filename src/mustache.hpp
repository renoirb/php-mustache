
#ifndef MUSTACHE_HPP
#define MUSTACHE_HPP

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <stack>
#include <exception>
#include <vector>
#include <stdexcept>
#include <memory>

using namespace std;

const int MUSTACHE_NODE_ROOT = 1;
const int MUSTACHE_NODE_OUTPUT = 2;
const int MUSTACHE_NODE_TAG = 3;

const int MUSTACHE_FLAG_NONE = 0;
const int MUSTACHE_FLAG_ESCAPE = 1;
const int MUSTACHE_FLAG_NEGATE = 2;
const int MUSTACHE_FLAG_SECTION = 4;
const int MUSTACHE_FLAG_STOP = 8;
const int MUSTACHE_FLAG_COMMENT = 16;
const int MUSTACHE_FLAG_PARTIAL = 32;
const int MUSTACHE_FLAG_INLINE_PARTIAL = 64;

const int MUSTACHE_CAN_HAVE_CHILDREN = MUSTACHE_FLAG_SECTION | MUSTACHE_FLAG_NEGATE | MUSTACHE_FLAG_INLINE_PARTIAL;

const int MUSTACHE_OUTPUT_BUFFER_LENGTH = 100000;

class MustacheException : public runtime_error {
  public:
      MustacheException(const string& desc) : runtime_error(desc) { }
};

class MustacheData {
  public:
    typedef auto_ptr<MustacheData> Ptr;
    typedef string String;
    typedef map<string,MustacheData *> Map;
    typedef list<MustacheData *> List;
    typedef MustacheData * Array;
    enum Type { TypeNone = 0, TypeString = 1, TypeList = 2, TypeMap = 3, TypeArray = 4 };
    
    MustacheData::Type type;
    int length;
    MustacheData::String * val;
    MustacheData::Map data;
    MustacheData::List children;
    MustacheData::Array array;
    
    ~MustacheData();
    int isEmpty();
    void init(MustacheData::Type type, int size);
};

class MustacheNode {
  public:
    typedef auto_ptr<MustacheNode> Ptr;
    typedef list<MustacheNode *> Children;
    
    int type;
    int flags;
    string * data;
    MustacheNode::Children children;
    
    ~MustacheNode();
};

class Mustache {
  private:
    string startSequence;
    string stopSequence;
    bool escapeByDefault;
    void _renderNode(MustacheNode * node, list<MustacheData*> * dataStack, string * output);
  public:
    typedef auto_ptr<Mustache> Ptr;
    Mustache();
    ~Mustache();
    void setStartSequence(string start);
    void setStopSequence(string stop);
    string getStartSequence();
    string getStopSequence();
    string * render(string * tmpl, MustacheData * data);
    MustacheNode * tokenize(string * tmpl);
    string * renderTree(MustacheNode * root, MustacheData * data);
};


#endif
