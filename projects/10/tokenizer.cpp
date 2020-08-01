#include <bits/stdc++.h>
using namespace std;



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
void compileExpressionList(vector<string> &tokens, vector<string>::iterator &it, int spaces);
void compileExpression(vector<string> &tokens, vector<string>::iterator &it, int spaces);
bool isop(string s);
void compileTerm(vector<string> &tokens, vector<string>::iterator &it, int spaces);

void compileClass(vector<string> &tokens)
{
    auto it = tokens.begin();
    if(split(*it)!="class")
    {
        ferr<<"Syntax error: class expected\n";
        return;
    }
    int spaces = 2;
    out.push_back("<class>");
    out.push_back(process(*it++, spaces));
    out.push_back(process(*it++, spaces));
    if(split(*it) !="{")
    {
        ferr<<"Syntax error: { expected\n";
        return;
    }
    out.push_back(process(*it++, spaces));
    
    
    compileClassVarDec(tokens, it, spaces);
    compileSubroutine(tokens,it, spaces);

    if(split(*it) != "}")
    {
        ferr<<"Syntax error: } expected\n";
        return;
    }
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
            out.push_back(process(*it++, spaces+2));
            out.push_back(process(*it++, spaces+2));
            out.push_back(process(*it++, spaces+2));
            while(split(*it)==",")
            {
                out.push_back(process(*it++, spaces+2));
                out.push_back(process(*it++, spaces+2));
            }
            if(split(*it)!=";")
            {
                ferr<<"Syntax error: ; expected\n";
                return;
            }
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
            out.push_back(process("<subroutineDec>", spaces));
            out.push_back(process(*it++, spaces+2));
            out.push_back(process(*it++, spaces+2));
            out.push_back(process(*it++, spaces+2));
            if(split(*it) != "(")
            {
                ferr<<"Syntax error: ( expected\n";
                return;
            }
            out.push_back(process(*it++, spaces+2));
            compileParameterList(tokens, it,spaces+2);
            if(split(*it) != ")")
            {
                ferr<<"Syntax error: ) expected\n";
                return;
            }
            out.push_back(process(*it++, spaces+2));
            out.push_back(process("<subroutineBody>", spaces+2));
            if(split(*it)!="{")
            {
                ferr<<"Syntax error: { expected";
                return;
            }
            out.push_back(process(*it++, spaces+2));
            compileVarDec(tokens, it, spaces+2);
            compileStatements(tokens, it, spaces+2);
            if(split(*it)!="}")
            {
                ferr<<"Syntax error: } expected";
                return;
            }
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
        out.push_back(process(*it++,spaces+2));
        out.push_back(process(*it++,spaces+2));
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
            out.push_back(process(*it++, spaces+2));
            out.push_back(process(*it++, spaces+2));
            while(split(*it)==",")
            {
                out.push_back(process(*it++, spaces+2));
                out.push_back(process(*it++, spaces+2));
            }
            if(split(*it)!=";")
            {
                ferr<<"Syntax error: ; expected\n";
                return;
            }
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
        if(split(*it)!="return")
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
        out.push_back(process("</statements>", spaces));
    }
}

void compileDo(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<doStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    out.push_back(process(*it++, spaces+2));
    if(split(*it)=="(")
    {
        out.push_back(process(*it++, spaces+2));
        compileExpressionList(tokens,it,spaces+2);
        out.push_back(process(*it++,spaces+2));
    }
    else
    {
        out.push_back(process(*it++, spaces+2));
        out.push_back(process(*it++, spaces+2));
        out.push_back(process(*it++, spaces+2));
        compileExpressionList(tokens,it,spaces+2);
        out.push_back(process(*it++, spaces+2));
    }
    if(split(*it)!=";")
    {
        ferr<<"Syntax error: ; expected\n";
        return;
    }
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</doStatement>", spaces));
}

void compileReturn(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<returnStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    if(split(*it)!=";")
        compileExpression(tokens, it, spaces+2);
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</returnStatement>", spaces));
}

void compileLet(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<letStatement>",spaces));
    out.push_back(process(*it++, spaces+2));
    out.push_back(process(*it++, spaces+2));
    if(split(*it)=="[")
    {
        out.push_back(process(*it++, spaces+2));
        compileExpression(tokens,it,spaces+2);
        out.push_back(process(*it++, spaces+2));
    }
    if(split(*it) == "=")
    {
        out.push_back(process(*it++, spaces+2));
        compileExpression(tokens,it,spaces+2);
    }
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</letStatement>", spaces));
}

void compileIf(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<ifStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    //insert check
    out.push_back(process(*it++, spaces+2));
    compileExpression(tokens,it,spaces+2);
    out.push_back(process(*it++, spaces+2));
    out.push_back(process(*it++, spaces+2));
    compileStatements(tokens, it, spaces+2);
    out.push_back(process(*it++, spaces+2));
    if(split(*it)=="else")
    {
        out.push_back(process(*it++, spaces+2));
        out.push_back(process(*it++, spaces+2));
        compileStatements(tokens,it,spaces+2);
        out.push_back(process(*it++, spaces+2));
    }
    out.push_back(process("</ifStatement>", spaces));
}


void compileWhile(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<whileStatement>", spaces));
    out.push_back(process(*it++, spaces+2));
    out.push_back(process(*it++, spaces+2));
    compileExpression(tokens,it,spaces+2);
    out.push_back(process(*it++, spaces+2));
    out.push_back(process(*it++, spaces+2));
    compileStatements(tokens,it,spaces+2);
    out.push_back(process(*it++, spaces+2));
    out.push_back(process("</whileStatement>", spaces));
}

void compileExpressionList(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<expressionList>",spaces));
    // compileExpression(tokens,it,spaces+2);
    if(split(*it)!=")")
    {
        // out.push_back(process(*it++, spaces+2));
        compileExpression(tokens,it,spaces+2);
        while(split(*it)==",")
        {
            out.push_back(process(*it++, spaces+2));
            compileExpression(tokens,it,spaces+2);
        }
    }
    out.push_back(process("</expressionList>",spaces));
}

void compileExpression(vector<string> &tokens, vector<string>::iterator &it, int spaces)
{
    out.push_back(process("<expression>", spaces));
    compileTerm(tokens, it, spaces+2);
    while(isop(split(*it)))
    {
        out.push_back(process(*it++,spaces+2));
        compileTerm(tokens,it,spaces+2);
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
        out.push_back(process(*it++,spaces+2));
    }
    else if(firsto(*it)=="stringConstant")
    {
        out.push_back(process(*it++,spaces+2));
    }
    else if(split(*it)=="true"||split(*it)=="false"||split(*it)=="null"||split(*it)=="this")
    {
        out.push_back(process(*it++,spaces+2));
    }
    else if(firsto(*it)=="identifier")
    {
        out.push_back(process(*it++,spaces+2));
        if(split(*it)=="[")
        {
            out.push_back(process(*it++,spaces+2));
            compileExpression(tokens,it,spaces+2);
            out.push_back(process(*it++,spaces+2));
        }
        if(split(*it)=="(")
        {
            out.push_back(process(*it++,spaces+2));
            compileExpressionList(tokens,it, spaces+2);
            out.push_back(process(*it++,spaces+2));
        }
        if(split(*it)==".")
        {
            out.push_back(process(*it++,spaces+2));
            out.push_back(process(*it++,spaces+2));
            out.push_back(process(*it++,spaces+2));
            compileExpressionList(tokens,it,spaces+2);
            out.push_back(process(*it++,spaces+2));
        }
    }
    else if(split(*it)=="(")
    {
        out.push_back(process(*it++,spaces+2));
        compileExpressionList(tokens,it,spaces+2);
        out.push_back(process(*it++,spaces+2));
    }
    else if(split(*it)=="-"||split(*it)=="~")
    {
        out.push_back(process(*it++,spaces+2));
        compileTerm(tokens, it, spaces+2);
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

    out.clear();
    fout.close();
    fin.close();
    ferr.close();
    }
}