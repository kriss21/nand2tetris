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

int main(int argc, char* argv[])
{
	if(argc>1)
	{
		ifstream fin;
		ofstream fout;
		//string blah = (string)argv[1] + ".vm";
		fin.open(argv[1]);
		fout.open("ans.txt");
		string filename = (string)argv[1];
		string line,a;
		while(getline(fin,line))
		{
			if(line[0]!=0)
			{
				a.append(line);
				a.append("\n");
			}
		}

		a=removeComments(a);
		fout<<a;
		fout.close();
		fin.close();
		fin.open("ans.txt");
		fout.open("ans1.txt");
		while(getline(fin,line))
		{
			if(line[0]!=0)
				fout<<line<<endl;
		}
		fin.close();
		fout.close();

		fin.open("ans1.txt");
		fout.open("ans.asm");

		map<string,string> m;
		m["add"] = "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M\n";
		m["sub"] = "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n";
		m["and"] = "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M\n";
		m["or"]  = "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M\n";
		m["not"] = "@SP\nA=M-1\nM=!M\n";
		m["neg"] = "@SP\nA=M-1\nM=-M\n";
		
		int count=0;
		int fcount=0;


		while(getline(fin,line))
		{
			if(line.substr(0,4)=="push")
			{
				int fspace = line.find(" ",0);
				int sspace = line.find(" ", fspace+1);
				string segment = line.substr(fspace+1, sspace-fspace-1);
				string index = line.substr(sspace+1, line.size()-sspace-1);
				int num = stoi(index);
				if(segment=="constant")
				{
					fout<<"@" << index << endl << "D=A" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
				else if(segment=="local")
				{
					fout<<"@" << "LCL" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
				else if(segment == "argument")
				{
					fout<<"@" << "ARG" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
				else if(segment == "this")
				{
					fout<<"@" << "THIS" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
				else if(segment == "that")
				{
					fout<<"@" << "THAT" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
				else if(segment == "pointer")
				{
					fout << "@" << "3" << endl << "D=A" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
				else if(segment == "temp")
				{
					fout << "@" << "5" << endl << "D=A" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
				else if(segment == "static")
				{
					fout<< "@" << argv[1] << "." << index << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
				}
			}
			else if(line.substr(0,3)=="pop")
			{
				int fspace = line.find(" ",0);
				int sspace = line.find(" ", fspace+1);
				string segment = line.substr(fspace+1, sspace-fspace-1);
				string index = line.substr(sspace+1, line.size()-sspace-1);
				int num = stoi(index);
				if(segment == "constant")
				{
					cerr<<"constant cannot be popped.\n";
				}
				if(segment == "local")
				{
					fout<< "@" << "LCL" << endl << "D=M" << endl << "@" << index << endl << "D=D+A" << endl;
					fout<< "@" << "13"<< endl << "M=D" << endl;
					fout<< "@SP" << endl << "AM=M-1" << endl;
					fout<< "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
				}
				else if(segment == "argument")
				{
					fout<< "@" << "ARG" << endl << "D=M" << endl << "@" << index << endl << "D=D+A" << endl;
					fout<< "@" << "13"<< endl << "M=D" << endl;
					fout<< "@SP" << endl << "AM=M-1" << endl;
					fout<< "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
				}
				else if(segment == "this")
				{
					fout<< "@" << "THIS" << endl << "D=M" << endl << "@" << index << endl << "D=D+A" << endl;
					fout<< "@" << "13"<< endl << "M=D" << endl;
					fout<< "@SP" << endl << "AM=M-1" << endl;
					fout<< "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
				}
				else if(segment == "that")
				{
					fout<< "@" << "THAT" << endl << "D=M" << endl << "@" << index << endl << "D=D+A" << endl;
					fout<< "@" << "13"<< endl << "M=D" << endl;
					fout<< "@SP" << endl << "AM=M-1" << endl;
					fout<< "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
				}
				else if(segment == "pointer")
				{
					fout<< "@" << "3" << endl << "D=A" << endl << "@" << index << endl << "D=D+A" << endl;
					fout<< "@" << "13"<< endl << "M=D" << endl;
					fout<< "@SP" << endl << "AM=M-1" << endl;
					fout<< "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
				}
				else if(segment == "temp")
				{
					fout<< "@" << "5" << endl << "D=A" << endl << "@" << index << endl << "D=D+A" << endl;
					fout<< "@" << "13"<< endl << "M=D" << endl;
					fout<< "@SP" << endl << "AM=M-1" << endl;
					fout<< "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
				}
				else if(segment == "static")
				{
					fout << "@" << argv[1]<< "." << index << endl << "D=A" << endl << "@R13" << endl << "M=D" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@R13" <<
				endl << "A=M" << endl << "M=D" << endl;
				}
			}

			else if(line=="eq")
				{
					fout<<"@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@IF"<<count<<"\nD;JEQ\n";
					fout<<"@SP\nA=M-1\nM=0\n@END"<<count<<"\n0;JMP\n(IF"<<count<<")\n";
					fout<<"@SP\nA=M-1\nM=-1\n(END"<<count<<")\n";
					count++;
				}
			else if(line == "gt")
				{
					fout<<"@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@IF"<<count<<"\nD;JGT\n";
					fout<<"@SP\nA=M-1\nM=0\n@END"<<count<<"\n0;JMP\n(IF"<<count<<")\n";
					fout<<"@SP\nA=M-1\nM=-1\n(END"<<count<<")\n";
					count++;
				}
			else if(line == "lt")
				{
					fout<<"@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@IF"<<count<<"\nD;JLT\n";
					fout<<"@SP\nA=M-1\nM=0\n@END"<<count<<"\n0;JMP\n(IF"<<count<<")\n";
					fout<<"@SP\nA=M-1\nM=-1\n(END"<<count<<")\n";
					count++;
				}
			else if(m.find(line)!=m.end())
				{
					fout<<m[line];
				}

			else if(line.substr(0,5)=="label")
			{
				fout<<"("<<filename<<"_";
				int fspace = line.find(" ", 0);
				string label = line.substr(fspace+1, line.size()-fspace-1);
				fout<<label<<")"<<endl;
			}

			else if(line.substr(0,4)=="goto")
			{
				fout<<"@"<<filename<<"_";
				int fspace = line.find(" ",0);
				string label = line.substr(fspace+1, line.size()-fspace-1);
				fout<<label<<endl;
				fout<<"0;JMP"<<endl;
			}

			else if(line.substr(0,7)=="if-goto")
			{
				fout << "@SP" << endl << "AM=M-1" << endl << "D=M" << endl;
				fout<<"@"<<filename<<"_";
				int fspace = line.find(" ",0);
				string label = line.substr(fspace+1, line.size()-fspace-1);
				fout<<label<<endl;
				fout<<"D;JNE"<<endl;

			}

			else if(line.substr(0,8)=="function")
			{
				int fspace = line.find(" ", 0);
				int sspace = line.find(" ", fspace+1);
				int tspace = line.find(" ", sspace+1);
				string fname = line.substr(fspace+1, sspace-fspace-1);
				string nlocals = line.substr(sspace+1, tspace-sspace-1);
				int n = stoi(nlocals);
				fout<<"("<<filename<<"-"<<fname<<")"<<endl;
				for(int i=0; i<n; i++)
				{
					fout<<"@" << 0 << endl << "D=A" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
					fout<<"@SP" << endl << "AM=M+1" << endl;
				}
			}

			else if(line.substr(0,4) == "call")
			{
				int fspace = line.find(" ", 0);
				int sspace = line.find(" ", fspace+1);
				int tspace = line.find(" ", sspace+1);
				string fname = line.substr(fspace+1, sspace-fspace-1);
				string margs = line.substr(sspace+1, tspace-sspace-1);
				int m = stoi(margs);
				fout << "@RETURN" << fcount << endl << "D=A" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
				fout << "@SP" << endl << "AM=M+1" << endl;
				fout << "@LCL" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
				fout << "@SP" << endl << "AM=M+1" << endl;
				fout << "@ARG" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
				fout << "@SP" << endl << "AM=M+1" << endl;
				fout << "@THIS" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
				fout << "@SP" << endl << "AM=M+1" << endl;
				fout << "@THAT" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
				fout << "@SP" << endl << "AM=M+1" << endl;
				fout << "@" << m << endl << "D=A" << endl << "@SP" << endl << "D=M-D" << endl << "@5" << endl << "D=D-A" << endl << "@ARG" << endl << "M=D" << endl;
				fout << "@SP" << endl << "D=M" << endl << "@LCL" << endl << "M=D" << endl;
				fout << "@" << fname << endl << "0;JMP" << endl;
				fout << "(RETURN" << fcount << ")" << endl;
				fcount++;
			}

			else if(line.substr(0,6)=="return")
			{
				fout << "@LCL" << endl << "D=M" << endl << "@FRAME" << endl << "M=D" << endl;
				fout << "@5" << endl << "D=A" << endl << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl << "@RET" << endl << "M=D" << endl;
				fout << "@SP" << endl << "AM=M-1" << endl << "D=M" << endl;
				fout << "@ARG" << endl << "A=M" << endl << "M=D" << endl;
				fout << "@ARG" << endl << "D=M+1" << endl << "@SP" << endl << "M=D" << endl;
				fout << "@FRAME" << endl << "A=M-1" << endl << "D=M" << endl << "@THAT" << endl << "M=D" << endl;
				fout << "@2" << endl << "D=A" << endl << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl << "@THIS" << endl << "M=D" << endl;
				fout << "@3" << endl << "D=A" << endl << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl << "@ARG" << endl << "M=D" << endl;
				fout << "@4" << endl << "D=A" << endl << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl << "@LCL" << endl << "M=D" << endl;
				fout << "@RET" << endl << "A=M" << endl << "0;JMP" << endl;

			}

			else
			{
				cerr<<"wrong command input.\n";
			}
			
			

		}

		// fin.close();
		// fout.close();
	}
}
