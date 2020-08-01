#include <bits/stdc++.h>
using namespace std;


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


int main(int argc, char* argv[])
{
	if(argc>1){
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

    string file = argv[1];
    int found = file.find('.', 0);
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
                    		fout<<"<keyword>" <<word << "/keyword>\n";

                    	else
                    	{
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

                    fout<<"<symbol>"<<what<<"</symbol>\n";
                    
                }

                else
                	word.append(what);
            }

            if(!word.empty()){

            	if(keywords.end() != find(keywords.begin(), keywords.end(), word))
                            fout<<"<keyword>" <<word << "/keyword>\n";

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
}
}