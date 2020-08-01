#include <bits/stdc++.h>
using namespace std;

vector<string> final;


ofstream ferr;

string removeComments(string prgm) 
{ 
    int n = prgm.length(); 
    string res; 
  
    // Flags to indicate that single line and multpile line comments 
    // have started or not. 
    bool s_cmt = false; 
    bool m_cmt = false; 
  
  
    // Traverse the given program 
    for (int i=0; i<n; i++) 
    { 
        // If single line comment flag is on, then check for end of it 
        if (s_cmt == true && prgm[i] == '\n') 
            s_cmt = false; 
  
        // If multiple line comment is on, then check for end of it 
        else if  (m_cmt == true && prgm[i] == '*' && prgm[i+1] == '/') 
            m_cmt = false,  i++; 
  
        // If this character is in a comment, ignore it 
        else if (s_cmt || m_cmt) 
            continue; 
  
        // Check for beginning of comments and set the approproate flags 
        else if (prgm[i] == '/' && prgm[i+1] == '/') 
            s_cmt = true, i++; 
        else if (prgm[i] == '/' && prgm[i+1] == '*') 
            m_cmt = true,  i++; 
  
        // If current character is a non-comment character, append it to res 
        else  res += prgm[i]; 
    } 
    return res; 
} 

class node{
public:
    int index;
    string type;
    string kind;

    node()
    {
        index=0;
        type = "";
        kind = "";
    }
};

class classSymbolTable{
public:
    map<string,node> m;
    int staticCount;
    int fieldCount;

    classSymbolTable()
    {
        staticCount = 0;
        fieldCount = 0;
    }

    void insert(string kind, string type, string name)
    {
        
        if(kind=="static"){
            node temp;
            temp.index = staticCount;
            temp.type = type;
            temp.kind = kind;
            m[name] = temp;
            staticCount++;
        }
        if(kind == "field") {
            node temp;
            temp.index = fieldCount;
            temp.type = type;
            temp.kind = "this";
            m[name] = temp;
            fieldCount++;
        }
    }
    void clean()
    {
        m.clear();
        staticCount=0;
        fieldCount=0;
    }
};

class subroutineSymbolTable{
public:
    map<string,node> m;
    int var_count;
    int arg_count;
    subroutineSymbolTable(){var_count=0; arg_count = 0;}

    void insert(string kind, string type, string name)
    {

        if(kind == "var"){
            node temp;
            temp.index = var_count;
            temp.type = type;
            temp.kind = "local";
            m[name] = temp;
            var_count++;
        }
        if(kind == "arg"){
            node temp;
            temp.index = arg_count;
            temp.type = type;
            temp.kind = "argument";
            m[name] = temp;
            arg_count++;
        }
    }


    void clean()
    {
        m.clear();
        arg_count=0;
        var_count=0;
    }
};


string currentClass = "";
string currentSubroutine = "";
classSymbolTable mysym;
subroutineSymbolTable subsym;
string currSubType = "";
int tlabelnum=0;

string firsto(string s)
{
    int fo = s.find('<', 0);
    int so = s.find('>', 0);
    string res = s.substr(fo+1, so-fo-1); 
    return res;
}

string split(string line)
{
    int first = line.find('>', 0);
    int second = line.find('<', first);
    string word = line.substr(first+1, second-first-1);
    return word;
}

string process(string line, int spaces)
{
    string res;
    for(int i=0; i<spaces; i++)
        res+=" ";
    res+=line;
    return res;
}
vector<string> out;

void error(string s, string toip)
{
    if(firsto(s)!=toip)
    {
        ferr<<"Error: Expecting <"<<toip<<"> but "<<split(s);
        exit(0);
    }
}
void error1(string s, string to)
{
    if(split(s)!=to)
    {
        ferr<<"Error: <"<<firsto(s)+">";
        exit(0);
    }
}

void err(string s, string to, string toip)
{
    error(s,toip);
    error1(s,to);
}

void decl_err(string s)
{
    ferr<<"Declaration error: "<<split(s)+" undeclared.";
    exit(0);
}


void compileClass(vector<string> &tokens);
void compileClassVarDec(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileSubroutine(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileParameterList(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileVarDec(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileStatements(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileDo(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileReturn(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileLet(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileIf(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileWhile(vector<string> &tokens, vector<string>::iterator &it, int spaces);
int compileExpressionList(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileExpression(vector<string> &tokens, vector<string>::iterator &it, int spaces);
bool isop(string s);
void compileTerm(vector<string> &tokens, vector<string>::iterator &it, int spaces);

void compileClass(vector<string> &tokens)
{
    auto it = tokens.begin();
    err(*it, "class", "keyword");

    mysym.clean();
    final.clear();
    int spaces = 2;
    out.push_back("<class>");
    out.push_back(process(*it++, spaces));
    error(*it, "identifier");
    currentClass = split(*it);
    out.push_back(process(*it++, spaces));
    err(*it, "{", "symbol");
    
    out.push_back(process(*it++, spaces));
    
    
    compileClassVarDec(tokens, it, spaces);
    compileSubroutine(tokens,it, spaces);
    err(*it, "}", "symbol");
    
    out.push_back(process(*it, spaces));
    out.push_back("</class>");
}

void compileClassVarDec(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    if(split(*it)=="field"||split(*it)=="static")
    {
        while(split(*it)=="field"||split(*it)=="static")
        {
            out.push_back(process("<classVarDec>", spaces));
            string _kind = split(*it);
            out.push_back(process(*it++, spaces+2));
            // error(*it, "keyword");
            string _type = split(*it);
            out.push_back(process(*it++, spaces+2));
            error(*it, "identifier");
            mysym.insert(_kind, _type, split(*it));
            out.push_back(process(*it++, spaces+2));
            error(*it, "symbol");
            
            while(split(*it)==",")
            {
                out.push_back(process(*it++, spaces+2));
                error(*it, "identifier");
                mysym.insert(_kind, _type, split(*it));
                out.push_back(process(*it++, spaces+2));
            }
            err(*it, ";","symbol");
            out.push_back(process(*it++, spaces+2));
            out.push_back(process("</classVarDec>", spaces));
        }
    }
}

void compileSubroutine(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    string found = split(*it);
    if(found == "function"||found=="constructor"||found=="method")
    {
        while(split(*it) == "function"||split(*it)=="constructor"||split(*it)=="method")
        {
            subsym.clean();
            out.push_back(process("<subroutineDec>", spaces));
            currSubType = split(*it);
            out.push_back(process(*it++, spaces+2));
            // error(*it, "keyword");//error here
            out.push_back(process(*it++, spaces+2));
            error(*it, "identifier");//error here
            currentSubroutine = split(*it);
            if(currSubType=="method")
                subsym.insert("arg",currentClass, "this");

            out.push_back(process(*it++, spaces+2));
            err(*it, "(", "symbol"); //error
            
            out.push_back(process(*it++, spaces+2));
            compileParameterList(tokens, it,spaces+2);
            err(*it, ")","symbol"); //error
            
            out.push_back(process(*it++, spaces+2));
            out.push_back(process("<subroutineBody>", spaces+2));
            err(*it, "{","symbol");
            
            out.push_back(process(*it++, spaces+2));
            compileVarDec(tokens, it, spaces+2);

            final.push_back("function "+currentClass+"."+currentSubroutine+" "+to_string(subsym.var_count));
            if(currSubType == "constructor")
            {
                final.push_back("push constant "+to_string(mysym.fieldCount));
                final.push_back("call Memory.alloc 1");
                final.push_back("pop pointer 0");
            }
            if(currSubType == "method")
            {
                final.push_back("push argument 0");
                final.push_back("pop pointer 0");
            }
            compileStatements(tokens, it, spaces+2);
            err(*it, "}", "symbol");//error
            out.push_back(process(*it++, spaces+2));
            out.push_back(process("</subroutineBody>", spaces+2));
            out.push_back(process("<subroutineDec>", spaces));
        }
    }
}

void compileParameterList(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    int count=0;
    out.push_back(process("<parameterList>",spaces));
    while(split(*it)!=")")
    {
        string _type = split(*it);
        // error(*it, "keyword");
        out.push_back(process(*it++,spaces+2));
        subsym.insert("arg", _type, split(*it));
        out.push_back(process(*it++,spaces+2));
        error(*it, "symbol");
        if(split(*it)==",")
            out.push_back(process(*it++,spaces+2));
    }
    out.push_back(process("</parameterList>",spaces));
}

void compileVarDec(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    if(split(*it)=="var")
    {
        while(split(*it)=="var")
        {
            out.push_back(process("<varDec>",spaces));
            out.push_back(process(*it++, spaces+2));
            string _type = split(*it);
            out.push_back(process(*it++, spaces+2));
            error(*it, "identifier");
            subsym.insert("var", _type, split(*it));
            out.push_back(process(*it++, spaces+2));
            // error(*it, "symbol");
            while(split(*it)==",")
            {
                out.push_back(process(*it++, spaces+2));
                error(*it, "identifier");
                subsym.insert("var", _type, split(*it));
                out.push_back(process(*it++, spaces+2));
            }
            err(*it, ";", "symbol");//error

            out.push_back(process(*it++, spaces+2));
            out.push_back(process("</varDec>", spaces+2));
        }
    }
}


void compileStatements(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    string f = split(*it);
    if(f=="let"||f=="if"||f=="while"||f=="do"||f=="return")
    {
        string _initial = split(*it);
        if(_initial!="return")
            out.push_back(process("<statements>", spaces));
        while(split(*it)=="let"||split(*it)=="while"||split(*it)=="do"||split(*it)=="if"||split(*it)=="return")
        {
            if(split(*it)=="return")
            {
                compileReturn(tokens, it, spaces+2);
                break;
            }
            else if(split(*it)=="let")
                compileLet(tokens, it, spaces+2);
            else if(split(*it)=="do")
                compileDo(tokens,it,spaces+2);
            else if(split(*it)=="while")
                compileWhile(tokens, it, spaces+2);
            else if(split(*it)=="if")
                compileIf(tokens,it,spaces+2);
        }
        if(_initial!="return")
            out.push_back(process("</statements>", spaces));
    }
}

void compileDo(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<doStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    error(*it, "identifier");
    string id1 = split(*it);
    out.push_back(process(*it++, spaces+2));
    if(split(*it)=="(")
    {
        final.push_back("push pointer 0");
        out.push_back(process(*it++, spaces+2));
        int np = compileExpressionList(tokens,it,spaces+2);
        out.push_back(process(*it++,spaces+2));
        final.push_back("call "+currentClass+"."+id1+" "+to_string(np+1));
        final.push_back("pop temp 0");
    }
    else if(split(*it)==".")
    {
        out.push_back(process(*it++, spaces+2));
        error(*it, "identifier");
        string id2 = split(*it);

        if(mysym.m.find(id1)!=mysym.m.end())
        {
            final.push_back("push "+mysym.m[id1].kind+" "+to_string(mysym.m[id1].index));
        }
        if(subsym.m.find(id1)!=subsym.m.end())
        {
            final.push_back("push "+subsym.m[id1].kind+" "+to_string(subsym.m[id1].index));
        }
        out.push_back(process(*it++, spaces+2));
        err(*it, "(", "symbol");
        out.push_back(process(*it++, spaces+2));
        int np = compileExpressionList(tokens,it,spaces+2);
        err(*it,")","symbol");
        out.push_back(process(*it++, spaces+2));
        if(mysym.m.find(id1)!=mysym.m.end())
        {
            final.push_back("call "+mysym.m[id1].type+"."+id2+" "+to_string(np+1));
        }
        else if(subsym.m.find(id1)!=subsym.m.end())
        {
            final.push_back("call "+subsym.m[id1].type+"."+id2+" "+to_string(np+1));
        }
        else
            final.push_back("call "+id1+"."+id2+" "+to_string(np));
        final.push_back("pop temp 0");
    }
    else
        {err(*it, "(", "symbol");}
    err(*it, ";", "symbol");
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</doStatement>", spaces));
}

void compileReturn(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<returnStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    if(split(*it)!=";"){
        compileExpression(tokens, it, spaces+2);
        err(*it, ";","symbol");
        final.push_back("return");
    }
    else
    {
        final.push_back("push constant 0");
        final.push_back("return");
    }
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</returnStatement>", spaces));
}

void compileLet(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<letStatement>",spaces));
    out.push_back(process(*it++, spaces+2));
    error(*it, "identifier");
    string id1 = split(*it);
    string _kind = "";
    string _type = "";
    int _index;
    if(mysym.m.find(id1)!=mysym.m.end())
    {
        _kind = mysym.m[id1].kind;
        _index = mysym.m[id1].index;
        _type = mysym.m[id1].type;
    }
    else if(subsym.m.find(id1)!=subsym.m.end())
    {
        _kind = subsym.m[id1].kind;
        _index = subsym.m[id1].index;
        _type = subsym.m[id1].type;
    }
    else
        decl_err(*it);
    out.push_back(process(*it++, spaces+2));
    bool is_array = 0;
    if(split(*it)=="[")
    {
        is_array = 1;
        out.push_back(process(*it++, spaces+2));
        compileExpression(tokens,it,spaces+2);
        err(*it,"]","symbol");
        out.push_back(process(*it++, spaces+2));
        final.push_back("push "+_kind+" "+to_string(_index));
        final.push_back("add");
        // final.push_back("pop pointer 1");
        // final.push_back("push that 0");
    }
    if(split(*it) == "=")
    {
        out.push_back(process(*it++, spaces+2));
        compileExpression(tokens,it,spaces+2);
        if(is_array)
            final.push_back("pop temp 0");
    }
    else
        {err(*it,"=","symbol");}
    if(!is_array)
    {
        final.push_back("pop "+_kind+" "+to_string(_index));
    }
    else
    {
        final.push_back("pop pointer 1");
        final.push_back("push temp 0");
        final.push_back("pop that 0");
    }
    err(*it,";","symbol");
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</letStatement>", spaces));
}



void compileIf(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    int _tlabel = tlabelnum;
    out.push_back(process("<ifStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    err(*it, "(","symbol");
    out.push_back(process(*it++, spaces+2));
    compileExpression(tokens,it,spaces+2);
    err(*it, ")","symbol");
    out.push_back(process(*it++, spaces+2));
    err(*it, "{","symbol");
    out.push_back(process(*it++, spaces+2));
    final.push_back("not");
    final.push_back("if-goto "+currentClass+"."+to_string(tlabelnum));
    tlabelnum+=2;
    compileStatements(tokens, it, spaces+2);
    err(*it, "}","symbol");
    out.push_back(process(*it++, spaces+2));

    final.push_back("goto "+currentClass+"."+to_string(_tlabel+1));
    final.push_back("label "+currentClass+"."+to_string(_tlabel));

    if(split(*it)=="else")
    {
        out.push_back(process(*it++, spaces+2));
        err(*it, "{","symbol");
        out.push_back(process(*it++, spaces+2));
        compileStatements(tokens,it,spaces+2);
        err(*it, "}","symbol");
        out.push_back(process(*it++, spaces+2));
    }

    final.push_back("label "+currentClass+"."+to_string(_tlabel+1));
    out.push_back(process("</ifStatement>", spaces));
}


void compileWhile(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    final.push_back("//while");
    int _tlabel = tlabelnum;
    out.push_back(process("<whileStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    err(*it, "(","symbol");
    out.push_back(process(*it++, spaces+2));
    final.push_back("label "+currentClass+"."+to_string(tlabelnum));
    tlabelnum+=2;
    compileExpression(tokens,it,spaces+2);
    err(*it, ")","symbol");
    out.push_back(process(*it++, spaces+2));
    final.push_back("not");
    final.push_back("if-goto "+currentClass+"."+to_string(_tlabel+1));
    err(*it, "{","symbol");
    out.push_back(process(*it++, spaces+2));
    compileStatements(tokens,it,spaces+2);
    err(*it, "}","symbol");
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</whileStatement>", spaces));
    final.push_back("goto "+currentClass+"."+to_string(_tlabel));
    final.push_back("label "+currentClass+"."+to_string(_tlabel+1));
    
    final.push_back("//while end");
}

int compileExpressionList(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    int np=0;
    out.push_back(process("<expressionList>",spaces));
    // compileExpression(tokens,it,spaces+2);
    if(split(*it)!=")")
    {
        // out.push_back(process(*it++, spaces+2));
        np++;
        compileExpression(tokens,it,spaces+2);
        error(*it, "symbol");
        while(split(*it)==",")
        {
            np++;
            out.push_back(process(*it++, spaces+2));
            compileExpression(tokens,it,spaces+2);
            error(*it, "symbol");
        }
    }
    out.push_back(process("</expressionList>",spaces));
    return np;
}

void writeOp(string oper)
{
    if(oper=="+")
        final.push_back("add");
    if(oper == "-")
        final.push_back("sub");
    if(oper == "&amp;")
        final.push_back("and");
    if(oper == "|")
        final.push_back("or");
    if(oper == "&gt;")
        final.push_back("gt");
    if(oper == "&lt;")
        final.push_back("lt");
    if(oper == "=")
        final.push_back("eq");
    if(oper == "*")
        final.push_back("call Math.multiply 2");
    if(oper == "/")
        final.push_back("call Math.divide 2");
}

void compileExpression(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<expression>", spaces));
    compileTerm(tokens, it, spaces+2);
    while(isop(split(*it)))
    {
        string _op = split(*it);
        out.push_back(process(*it++,spaces+2));
        compileTerm(tokens,it,spaces+2);
        writeOp(_op);
    }
    out.push_back(process("</expression>", spaces));
}

bool isop(string s)
{
    if(s=="+"||s=="-"||s=="*"||s=="/"||s=="&amp;"||s=="|"||s=="&lt;"||s=="&gt;"||s=="~"||s=="=")
        return true;
    return false;
}



void compileTerm(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<term>",spaces));
    if(firsto(*it)=="integerConstant")
    {
        final.push_back("push constant " + split(*it));
        out.push_back(process(*it++,spaces+2));
    }
    else if(firsto(*it)=="stringConstant")
    {
        int _strlen = split(*it).size();
        string _mystr = split(*it);
        final.push_back("push constant "+to_string(_strlen));
        final.push_back("call String.new 1");
        for(int ii=0; ii<_strlen; ii++)
        {
            final.push_back("push constant "+to_string((int)_mystr[ii]));
            final.push_back("call String.appendChar 2");
        }
        out.push_back(process(*it++,spaces+2));
    }
    else if(split(*it)=="true"||split(*it)=="false"||split(*it)=="null"||split(*it)=="this")
    {
        string todo = split(*it);
        if(todo == "true")
        {
            final.push_back("push constant 0");
            final.push_back("not");
        }
        else if(todo == "false" || todo == "null")
        {
            final.push_back("push constant 0");
        }
        else if(todo == "this")
        {
            final.push_back("push pointer 0");
        }
        out.push_back(process(*it++,spaces+2));
    }
    else if(firsto(*it)=="identifier")
    {
        string id1 = split(*it);
        string _kind = "";
        string _type = "";
        int _index;
        if(mysym.m.find(id1)!=mysym.m.end())
        {
            _kind = mysym.m[id1].kind;
            _index = mysym.m[id1].index;
            _type = mysym.m[id1].type;
        }
        if(subsym.m.find(id1)!=subsym.m.end())
        {
            _kind = subsym.m[id1].kind;
            _index = subsym.m[id1].index;
            _type = subsym.m[id1].type;
        }
        if(split(*(it+1))!="[" && split(*(it+1))!="(" && split(*(it+1))!=".")
        { 
            if(_kind=="")
                decl_err(*it);
            final.push_back("push "+_kind+" "+to_string(_index));
        }
        out.push_back(process(*it++,spaces+2));
        if(split(*it)=="[")
        {
            out.push_back(process(*it++,spaces+2));
            compileExpression(tokens,it,spaces+2);
            err(*it,"]","symbol");
            out.push_back(process(*it++,spaces+2));
            final.push_back("push "+_kind+" "+to_string(_index));
            final.push_back("add");
            final.push_back("pop pointer 1");
            final.push_back("push that 0");
        }
        if(split(*it)=="(")
        {
            final.push_back("push pointer 0");
            out.push_back(process(*it++,spaces+2));
            int np = compileExpressionList(tokens,it, spaces+2);
            err(*it,")","symbol");
            out.push_back(process(*it++,spaces+2));
            final.push_back("call "+currentClass+"."+id1+" "+to_string(np+1));
        }
        if(split(*it)==".")
        {
            out.push_back(process(*it++,spaces+2));
            error(*it,"identifier");
            string id2 = split(*it);
            if(!_kind.empty())
                final.push_back("push "+_kind+" "+to_string(_index));
            out.push_back(process(*it++,spaces+2));
            err(*it, "(","symbol");
            out.push_back(process(*it++,spaces+2));
            int np = compileExpressionList(tokens,it,spaces+2);
            err(*it,")","symbol");
            out.push_back(process(*it++,spaces+2));
            if(!_kind.empty())
                final.push_back("call "+_type+"."+id2+" "+to_string(np+1));
            else
                final.push_back("call "+id1+"."+id2+" "+to_string(np));
        }
    }
    else if(split(*it)=="(")
    {
        out.push_back(process(*it++,spaces+2));
        int np = compileExpressionList(tokens,it,spaces+2);
        err(*it,")","symbol");
        out.push_back(process(*it++,spaces+2));
    }
    else if(split(*it)=="-"||split(*it)=="~")
    {
        string which_op = split(*it);
        
        out.push_back(process(*it++,spaces+2));
        compileTerm(tokens, it, spaces+2);
        if(which_op=="-")
            final.push_back("neg");
        if(which_op=="~")
            final.push_back("not");
    }
    out.push_back(process("</term>",spaces));
}

void engine(vector<string> &tokens)
{
    compileClass(tokens);


}

int main(int argc, char* argv[])
{

    set<string> keywords;
    set<char> symbols;

        keywords.insert("class");
        keywords.insert("constructor");
        keywords.insert("function");
        keywords.insert("method");
        keywords.insert("field");
        keywords.insert("static");
        keywords.insert("var");
        keywords.insert("int");
        keywords.insert("char");
        keywords.insert("boolean");
        keywords.insert("void");
        keywords.insert("true");
        keywords.insert("false");
        keywords.insert("null");
        keywords.insert("this");
        keywords.insert("let");
        keywords.insert("do");
        keywords.insert("if");
        keywords.insert("else");
        keywords.insert("while");
        keywords.insert("return");


        symbols.insert('{');
        symbols.insert('}');
        symbols.insert('(');
        symbols.insert(')');
        symbols.insert('[');
        symbols.insert(']');
        symbols.insert('.');
        symbols.insert(',');
        symbols.insert(';');
        symbols.insert('+');
        symbols.insert('-');
        symbols.insert('*');
        symbols.insert('/');
        symbols.insert('&');
        symbols.insert('|');
        symbols.insert('<');
        symbols.insert('>');
        symbols.insert('=');
        symbols.insert('~');


    int arg_num = 2;
    string num = (string)argv[1];
    int nn = stoi(num);
    while(nn--){
    

    ofstream f_out;
    string file = argv[arg_num];
    arg_num++;
    int found = file.find('.', 0);
    cout<<file<<endl;
    string file_name = file.substr(0, found);
    ifstream fin;
    ofstream fout;
    fin.open(file);
    string output = file_name + ".txt";
    fout.open(output);

    string line;
    int isstr=0;

    
    string a;
    while(getline(fin, line))
    {
        a = a+line+"\n";
    }
    a = removeComments(a);
    fout<<a;
    fin.close();
    fout.close();

    fin.open(output);
    output = file_name+".tok";
    fout.open(output);

    fout<<"<tokens>\n";

    while(getline(fin, line))
    {
        if(line.empty())
            continue;
        if(line[0] == 0 || line[0]=='\n')
        	continue;
        istringstream inp(line);
        string word;
        string part;

        isstr=0;
        bool isnum = 0;

        while(inp >> part)
        {
        		// cout<<part<<endl;
            for(auto it=part.begin(); it!=part.end(); it++)
            {
                string what(1,*it);
                if(*it == '"')
                    isstr++;
                if(find(symbols.begin(), symbols.end(), *it)!= symbols.end())
                {
                    if(*it == '<')
                    	what = "&lt;";
                    if(*it == '>')
                    	what = "&gt;";
                    if(*it == '&')
                    	what = "&amp;";
              

                    if(!word.empty())
                    {
                    	if(keywords.end() != find(keywords.begin(), keywords.end(), word))
                    		fout<<"<keyword>" <<word << "</keyword>\n";

                    	else
                    	{
                    		for(auto it1 = word.begin(); it1 != word.end(); it1++)
                    			isnum = (isdigit(*it1)) ? 1 : 0;

                    		if(isstr==0 && isnum)
                    			fout<< "<integerConstant>"<<word<<"</integerConstant>\n";
                    		else if(isstr == 2)
                    			fout<<"<stringConstant>"<<word.substr(1,word.size()-2)<<"</stringConstant>\n";
                    		else if(isstr==0)
                    			fout<<"<identifier>"<<word<<"</identifier>\n";
                    	}
                    }

                    if(isstr != 1)
                    	word.clear();
                    else
                    	word.append(" ");

                    if(isstr!=1)
                    fout<<"<symbol>"<<what<<"</symbol>\n";
                    
                }

                else
                	word.append(what);
            }

            if(!word.empty()){
                if(keywords.end() != find(keywords.begin(), keywords.end(), word))
                            fout<<"<keyword>" <<word << "</keyword>\n";

                else{

            	for(auto it1 = word.begin(); it1 != word.end(); it1++)
            	   	isnum = (isdigit(*it1)) ? 1 : 0;

            	if(isstr==0 && isnum)
            		fout<< "<integerConstant>"<<word<<"</integerConstant>\n";
            	else if(isstr == 2)
            	 	fout<<"stringConstant>"<<word.substr(1,word.size()-3)<<"</stringConstant>\n";
            	else if(isstr==0)
            	 	fout<<"<identifier>"<<word<<"</identifier>\n";
                }
         	}

            if(isstr != 1)
                word.clear();
            else
                word.append(" "); 

        }


    }

    fout<<"</tokens>\n";

    fin.close();
    fout.close();
    
    ferr.open(file_name + ".err");
    fin.open(file_name + ".tok");
    output = file_name+".xml";
    fout.open(file_name + ".xml");
    f_out.open(file_name+".vm");

    vector<string> tokens;

    while(getline(fin, line))
    {
        if(line.find("<tokens>")!=string::npos || line.find("</tokens>")!=string::npos)
            continue;
        tokens.push_back(line);
    }

    // cout<<split(tokens[0])<<endl;
    engine(tokens);
    // cout<<file_name<<endl;
    for(int i=0; i<out.size(); i++)
        fout<<out[i]<<"\n";

    for(int i=0; i<final.size(); i++)
        f_out<<final[i]<<endl;



    out.clear();
    fout.close();
    fin.close();
    ferr.close();
    f_out.close();
    }
}