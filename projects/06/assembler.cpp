#include <string>
using namespace std;

string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

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
            s_cmt = false, res+="\n";

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

int stringtoint(string s)
{
  stringstream blah(s);
  int x=0;
  blah>>x;
  return x;
}

string inttobinary(int n)
{
  string s(15,'0');

  for(int i=14; i>=0; i--)
  {
    s[i]='0'+n%2;
    n/=2;
  }
  //cout << s << endl;
  return s;
}

int main(int argc, char* argv[])
{
  if(argc>1){
    //pass 1


    //removes whitespaces
    ifstream fin, fin2;
    ofstream fout;
    fin.open(argv[1]);
    fout.open("ans.txt");
    string line;
    string a;
    while(fin)
    {
      getline(fin, line);
      while(fin && line[0]==0)
      {
        getline(fin,line);
      }
      if(!fin) break;
      line = removeSpaces(line);
      a.append(line);
      a.append("\n");
      //fout<<line<<endl;
      //cout<<"1";
    }
    //remove comments
    a = removeComments(a);
    fout<<a;
    fout.close();
    fin.close();
    fin.open("ans.txt");
    fout.open("ans2.txt");

    //remove extra blank lines
    while(fin)
    {
      getline(fin, line);
      while(fin && line[0]==0)
        getline(fin,line);
      if(!fin) break;
      fout<<line<<endl;
    }
    fout.close(); fin.close();

    //making symbol table
    fin.open("ans2.txt");
    map<string,int> symbolTable;
    map<string,int>::iterator itr;


    symbolTable["R0"]=0;
    symbolTable["R1"]=1;
    symbolTable["R2"]=2;
    symbolTable["R3"]=3;
    symbolTable["R4"]=4;
    symbolTable["R5"]=5;
    symbolTable["R6"]=6;
    symbolTable["R7"]=7;
    symbolTable["R8"]=8;
    symbolTable["R9"]=9;
    symbolTable["R10"]=10;
    symbolTable["R11"]=11;
    symbolTable["R12"]=12;
    symbolTable["R13"]=13;
    symbolTable["R14"]=14;
    symbolTable["R15"]=15;
    symbolTable["SCREEN"]=16384;
    symbolTable["KBD"]=24576;
    symbolTable["SP"]=0;
    symbolTable["LCL"]=1;
    symbolTable["ARG"]=2;
    symbolTable["THIS"]=3;
    symbolTable["THAT"]=4;

    int linecount=0;

    while(fin)
    {
      getline(fin,line);
      if(line[0]=='@')
      {
        string str(line.begin()+1, line.end());
        if(isdigit(str[0]))
          symbolTable[str]=stringtoint(str);
        else if(symbolTable.find(str)==symbolTable.end())
          symbolTable[str]=-1;
        
      }
      else if(line[0]=='(' && line[line.size()-1]==')')
      {

        string str = line.substr(1,line.size()-2);
        if(symbolTable.find(str)==symbolTable.end())
          symbolTable[str]=linecount;
        else
        {
          //auto itr=symbolTable.find(str);
          symbolTable.find(str)->second = linecount;
        }
        linecount--;
      }
      linecount++;
    }
    fin.close();
    fin.open("ans2.txt");
    int count=16;
    while(fin)
    {
      getline(fin,line);
      if(line[0]=='@')
      {
        string str(line.begin()+1, line.end());
        if(isdigit(str[0]));
        else if(symbolTable.find(str)->second==-1)
        {
          symbolTable.find(str)->second=count;
          count++;
        }
      }
    }

    
    for(itr=symbolTable.begin(); itr!=symbolTable.end(); itr++)
    {
      if(itr->second==-1)
      {
        itr->second=count;
        count++;
      }
    }

    /*for(itr=symbolTable.begin(); itr!=symbolTable.end(); itr++)
    {
      cout<<itr->first<<" "<<itr->second<<endl;
    }*/

    fin.close();


    //pass 2

    fin.open("ans2.txt");
    fout.open("ans.hack");

    string address;
    map<string,string> mp, mp3;
    map<string,string>::iterator it;
    mp["D+1"]="0011111";
    mp["A+1"]="0110111";
    mp["D-1"]="0001110";
    mp["A-1"]="0110010";
    mp["D+A"]="0000010";
    mp["A+D"]="0000010";
    mp["D-A"]="0010011";
    mp["A-D"]="0000111";
    mp["D&A"]="0000000";
    mp["A&D"]="0000000";
    mp["D|A"]="0010101";
    mp["A|D"]="0010101";
    mp["0"]="0101010";
    mp["1"]="0111111";
    mp["-1"]="0111010";
    mp["D"]="0001100";
    mp["A"]="0110000";
    mp["!D"]="0001101";
    mp["!A"]="0110001";
    mp["-A"]="0110011";
    mp["-D"]="0001111";
    mp["M"]="1110000";
    mp["!M"]="1110001";
    mp["-M"]="1110011";
    mp["M+1"]="1110111";
    mp["M-1"]="1110010";
    mp["D+M"]="1000010";
    mp["M+D"]="1000010";
    mp["D-M"]="1010011";
    mp["M-D"]="1000111";
    mp["D&M"]="1000000";
    mp["M&D"]="1000000";
    mp["D|M"]="1010101";
    mp["M|D"]="1010101";


    mp3["JGT"]="001";
    mp3["JEQ"]="010";
    mp3["JGE"]="011";
    mp3["JLT"]="100";
    mp3["JNE"]="101";
    mp3["JLE"]="110";
    mp3["JMP"]="111";


    while(getline(fin, line))
    {
      size_t found = line.find('=');
      if(line[0]=='@')
      {
        string s=line.substr(1,line.size()-1);
        //cout << s << endl;
        itr=symbolTable.find(s);
        //cout << inttobinary(itr->second) << endl;
        address="0";
        address+=inttobinary(itr->second);
        //cout<<address<<endl;
        fout<<address<<endl;
      }

      else if(found != string::npos)
      {
        address="111";
        it = mp.find(line.substr(found+1,line.size()-2));
        address+=it->second;
        string k(3,'0');
        string sub=line.substr(0, found);
        size_t f=sub.find('A');
        if(f!=string::npos)
          k[0]='1';
        f=sub.find('D');
        if(f!=string::npos)
          k[1]='1';
        f=sub.find('M');
        if(f!=string::npos)
          k[2]='1';
        address+=k;
        address+="000";
        fout<<address<<endl;
      }

      else if(line[1]==';')
      {
        address="111";
        it=mp.find(line.substr(0,1));
        address+=it->second;
        address+="000";
        it=mp3.find(line.substr(2,line.size()-2));
        address+=it->second;
        fout<<address<<endl;
      }
    }
}
}
