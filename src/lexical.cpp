#include<bits/stdc++.h>
using namespace std;
struct _2tup
{
    string token;
    int id;
};
bool is_blank(char ch)
{
    return ch == ' ' || ch == '    ';//�ո������ַ�
}
bool gofor(char& ch, string::size_type& pos, const string& prog)//����ָ��λ�õ��ַ�
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
        ��ʾ��
    else if
        ����
    else
        ����
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
    // �жϱ�ʾ�����ؼ���
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
    {
        //��֤��ȡһ������
        while((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
        {
            token += ch;//׷�ӱ�ʾ�����ؼ���
            if (!gofor(ch, pos, prog))
            {
                break;
            }
        }
        // �����ȿ�������������ʾ��
        id = keys.size();

        // ��֤�Ƿ��ǹؼ���
        map<string, int>::const_iterator cit = keys.find(token);//����string���͵�token����int���͵�id��ֵ��cit
        if (cit != keys.end())
        {
            id = cit->second;//��ʱ�ǹؼ��֣���¼����id
        }
    }
    // ʶ����
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
        case '-': // - ������
            token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '-' || ch == '=' || ch == '>') // -- ������
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
                if (ch == '=') // -- ������
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
                if (ch == '=') // !% %= ������
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

        case '/': // / ������
            token += ch;
            if (gofor(ch, pos, prog))
            {
                if (ch == '=') // /= ������
                {
                    token += ch;
                    gofor(ch, pos, prog);
                }
                else if (ch == '/') // ����ע��
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
                else if (ch == '*') // ע��
                {
                    token += ch;
                    if (!gofor(ch, pos, prog))
                    {
                        token += "\n!!!ע�ʹ���!!!";
                        id = -10;
                        break;
                    }
                    if (pos + 1 >= prog.size())
                    {
                        token += ch;
                        token += "\n!!!ע�ʹ���!!!";
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
                            token += "\n!!!ע�ʹ���!!!";
                            id = -10;
                            ret.token = token;
                            ret.id    = id;
                            return ret;
                        }
                        //ch = prog[pos];
                        if (pos + 1 >= prog.size())
                        {
                            token += ch;
                            token += "\n!!!ע�ʹ���!!!";
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
         case '(': // / ������
            token += ch;
            if (gofor(ch, pos, prog))

            {
                 if (ch == '*') // ע��
                {
                    token += ch;
                    if (!gofor(ch, pos, prog))
                    {
                        token += "\n!!!ע�ʹ���!!!";
                        id = -10;
                        break;
                    }
                    if (pos + 1 >= prog.size())
                    {
                        token += ch;
                        token += "\n!!!ע�ʹ���!!!";
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
                            token += "\n!!!ע�ʹ���!!!";
                            id = -10;
                            ret.token = token;
                            ret.id    = id;
                            return ret;
                        }
                        //ch = prog[pos];
                        if (pos + 1 >= prog.size())
                        {
                            token += ch;
                            token += "\n!!!ע�ʹ���!!!";
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
            token += "����";
            ++pos;
            ch = prog[pos];
            id = -2;
            break;
        default:
            token += "����";
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


void init_keys(const string& file, map<string, int>& keys)//��ȡ���ʷ��ź��ֱ���
{
    ifstream fin(file.c_str());//.c_str���ص��ǵ�ǰ�ַ������׵�ַ
    if (!fin)
    {
        cerr << file << " doesn't exist!" << endl;//cerr�����������ֱ�����,һ������Ѹ�����������Ϣ
      //  exit(1);
    }
    keys.clear();//���map�������������
    string line;
    string key;
    int id;
    while (getline(fin, line))//���������������������һ�������������һ��string����getline����������������һ�ж�ȡ���������ȡ�����ݵ�string��
    {
        istringstream sin(line);//istringstream sin(s);����һ���ַ����������Ķ���sin,������sin�ĸ��ƹ��캯������line�����������ַ�������sin �����У�
        sin >> key >> id;//��ȡ������ַ���ÿһ��һ��key id
        keys[key] = id;
    }
}

void read_prog(const string& file, string& prog){//��ȡ���룬��׷�ӵ�prog��
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

    string::size_type pos = 0;//size_type����string��׼�⣬���ÿɿ����൱��unsigned��int
    int row  = 1;

    _2tup tu;
    int no = 0;
    freopen("c.txt","w",stdout);
    do
    {
        tu = scanner(prog, pos, keys, row);

        switch (tu.id)
        {
        case -1://���ص��Ǵ���
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
