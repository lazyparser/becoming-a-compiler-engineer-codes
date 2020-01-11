#include<bits/stdc++.h>
using namespace std;
struct _2tup
{
    string token;
    int id;
};
bool is_blank(char ch)
{
    return ch == ' ' || ch == '    ';//空格或控制字符
}
bool gofor(char& ch, string::size_type& pos, const string& prog)//返回指定位置的字符
{
    ++pos;
    if (pos >= prog.size())
    {
        return false;
    }
    else
    {
        ch = prog[pos];
        return true;
    }
}


_2tup scanner(const string& prog, string::size_type& pos, const map<string, int>& keys, int& row)
{
    /*
    if
        标示符
    else if
        数字
    else
        符号
    */
    _2tup ret;
    string token;
    int id = 0;

    char ch;
    ch = prog[pos];

    while(is_blank(ch))
    {
        ++pos;
        ch = prog[pos];
    }
    // 判断标示符、关键字
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
    {
        //保证读取一个单词
        while((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
        {
            token += ch;//追加标示符、关键字
            if (!gofor(ch, pos, prog))
            {
                break;
            }
        }
        // 这里先看做都是其他标示符
        id = keys.size();

        // 验证是否是关键字
        map<string, int>::const_iterator cit = keys.find(token);//根据string类型的token返回int类型的id赋值给cit
        if (cit != keys.end())
        {
            id = cit->second;//此时是关键字，记录他的id
        }
    }
    // 识别常数
    else if ((ch >= '0' && ch <= '9') || ch == '.')
    {
        while (ch >= '0' && ch <= '9' || ch == '.')
        {
            token += ch;
            if (!gofor(ch, pos, prog))
            {
                break;
            }
        }
        id = keys.size() - 1;
        int dot_num = 0;
        for (string::size_type i = 0; i != token.size(); ++i)
        {
            if (token[i] == '.')
            {
                ++dot_num;
            }
        }
        if (dot_num > 1)
        {
            id = -1;
        }
    }
    else
    {
        map<string, int>::const_iterator cit;
        switch (ch)
        {
        case '-': // - 操作符
            token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '-' || ch == '=' || ch == '>') // -- 操作符
                {
                    token += ch;
                    gofor(ch, pos, prog);
                }
            }
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;
        case ':':
             token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '=') // -- 操作符
                {
                    token += ch;
                    gofor(ch, pos, prog);
                }
            }
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;

        case '=':
            token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '=') // !% %= 操作符
                {
                    token += ch;
                    gofor(ch, pos, prog);
                }
            }
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;

        case '/': // / 操作符
            token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '=') // /= 操作符
                {
                    token += ch;
                    gofor(ch, pos, prog);
                }
                else if (ch == '/') // 单行注释
                {
                    token += ch;
                    ++pos;
                    while (pos < prog.size())
                    {
                        ch = prog[pos];
                        if (ch == '\n')
                        {
                            break;
                        }
                        token += ch;
                        ++pos;
                    }
                    if (pos >= prog.size())
                    {
                        ;
                    }
                    else
                    {
                        ;
                    }
                    id = keys.size() - 2;
                    break;
                }
                else if (ch == '*') // 注释
                {
                    token += ch;
                    if (!gofor(ch, pos, prog))
                    {
                        token += "\n!!!注释错误!!!";
                        id = -10;
                        break;
                    }
                    if (pos + 1 >= prog.size())
                    {
                        token += ch;
                        token += "\n!!!注释错误!!!";
                        id = -10;
                        break;
                    }
                    char xh = prog[pos + 1];
                    while (ch != '*' || xh != '/')
                    {
                        token += ch;
                        if (ch == '\n')
                        {
                            ++row;
                        }
                        //++pos;
                        if (!gofor(ch, pos, prog))
                        {
                            token += "\n!!!注释错误!!!";
                            id = -10;
                            ret.token = token;
                            ret.id    = id;
                            return ret;
                        }
                        //ch = prog[pos];
                        if (pos + 1 >= prog.size())
                        {
                            token += ch;
                            token += "\n!!!注释错误!!!";
                            id = -10;
                            ret.token = token;
                            ret.id    = id;
                            return ret;
                        }
                        xh = prog[pos + 1];
                    }
                    token += ch;
                    token += xh;
                    pos += 2;
                    ch = prog[pos];
                    id = keys.size() - 2;
                    break;
                }
            }
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;
        case '+':
            token += ch;
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            gofor(ch, pos, prog);
            break;

        case '<':
            token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '<')
                {
                    token += ch;
                    if (gofor(ch, pos, prog))
                    {
                        if (ch == '=')
                        {
                            token += ch;
                            gofor(ch, pos, prog);
                        }
                    }
                }
                else if (ch == '=')
                {
                    token += ch;
                    gofor(ch, pos, prog);
                }
            }
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;

        case '>':
            token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '>')
                {
                    token += ch;
                    if (gofor(ch, pos, prog))
                    {
                        if (ch == '=')
                        {
                            token += ch;
                            gofor(ch, pos, prog);
                        }
                    }
                }
                else if (ch == '=')
                {
                    token += ch;
                    gofor(ch, pos, prog);
                }
            }
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;
         case '(': // / 操作符
            token += ch;
            if (gofor(ch, pos, prog))

            {
                 if (ch == '*') // 注释
                {
                    token += ch;
                    if (!gofor(ch, pos, prog))
                    {
                        token += "\n!!!注释错误!!!";
                        id = -10;
                        break;
                    }
                    if (pos + 1 >= prog.size())
                    {
                        token += ch;
                        token += "\n!!!注释错误!!!";
                        id = -10;
                        break;
                    }
                    char xh = prog[pos + 1];
                    while (ch != '*' || xh != ')')
                    {
                        token += ch;
                        if (ch == '\n')
                        {
                            ++row;
                        }
                        //++pos;
                        if (!gofor(ch, pos, prog))
                        {
                            token += "\n!!!注释错误!!!";
                            id = -10;
                            ret.token = token;
                            ret.id    = id;
                            return ret;
                        }
                        //ch = prog[pos];
                        if (pos + 1 >= prog.size())
                        {
                            token += ch;
                            token += "\n!!!注释错误!!!";
                            id = -10;
                            ret.token = token;
                            ret.id    = id;
                            return ret;
                        }
                        xh = prog[pos + 1];
                    }
                    token += ch;
                    token += xh;
                    pos += 2;
                    ch = prog[pos];
                    id = keys.size() - 2;
                    break;
                }
            }
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;

        case '*':
            token += ch;
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
             gofor(ch, pos, prog);
            break;

        case ',':
        case ')':
        case '#':
        case '.':
        case ';':
            token += ch;
            gofor(ch, pos, prog);
            //++pos;
            //ch = prog[pos];
            cit = keys.find(token);
            if (cit != keys.end())
            {
                id = cit->second;
            }
            break;

        case '\n':
            token += "换行";
            ++pos;
            ch = prog[pos];
            id = -2;
            break;
        default:
            token += "错误";
            ++pos;
            ch = prog[pos];
            id = -1;
            break;
        }
    }
    ret.token = token;
    ret.id    = id;

    return ret;
}


void init_keys(const string& file, map<string, int>& keys)//读取单词符号和种别码
{
    ifstream fin(file.c_str());//.c_str返回的是当前字符串的首地址
    if (!fin)
    {
        cerr << file << " doesn't exist!" << endl;//cerr不经过缓冲而直接输出,一般用于迅速输出出错信息
      //  exit(1);
    }
    keys.clear();//清空map对象里面的内容
    string line;
    string key;
    int id;
    while (getline(fin, line))//这个函数接收两个参数：一个输入流对象和一个string对象，getline函数从输入流的下一行读取，并保存读取的内容到string中
    {
        istringstream sin(line);//istringstream sin(s);定义一个字符串输入流的对象sin,并调用sin的复制构造函数，将line中所包含的字符串放入sin 对象中！
        sin >> key >> id;//读取里面的字符串每一行一个key id
        keys[key] = id;
    }
}

void read_prog(const string& file, string& prog){//读取代码，并追加到prog上
    ifstream fin(file.c_str());
    if (!fin)
    {
        cerr << file << " error!" << endl;
      //  exit(2);
    }
    prog.clear();
    string line;
    while (getline(fin, line))
    {
        prog += line + '\n';
    }
}
int main()
{
    map<string, int> keys;
    init_keys("a.txt", keys);

    string prog;
    read_prog("test1.txt", prog);

    vector< _2tup > tups;
    string token, id;

    string::size_type pos = 0;//size_type属于string标准库，作用可看做相当于unsigned·int
    int row  = 1;

    _2tup tu;
    int no = 0;
    freopen("c.txt","w",stdout);
    do
    {
        tu = scanner(prog, pos, keys, row);

        switch (tu.id)
        {
        case -1://返回的是错误
            ++no;
            cout << no << ": ";
            cout << "Error in row" << row << "!" << '<' << tu.token<< "," << tu.id << '>' << endl;
            tups.push_back(tu);
            break;
        case -2:
            ++row;
            // cout << '<' << tu.token<< "," << tu.id << '>' << endl;
            break;
        default:
            ++no;
            cout << no << ": ";
            if(tu.id==28 || tu.id==29){
                cout << '<' << tu.id<< "," << tu.token << '>' << endl;
            }
            else{
                cout << '<' << tu.id<< "," << tu.token << '>' << endl;
            }
            tups.push_back(tu);
            break;
        }
    } while (pos < prog.size());

    fclose(stdout);

    return 0;
}
