// pL/0语言词法分析器
#include<bits/stdc++.h>
using namespace std;

ifstream infile("c.txt");//词法分析的结果或语法分析的输入
string str;//string变量进行字符识别
string sym; //指针

void expressionAnalysis();//表达式分析
void termAnaysis();//项分析
void factorAnalysis();//因子分析
int advance();

int conterr=0;//记录错误
int lpnum=0;//记录左括号
int stru=0;
int found1,found2,str_long;//提取字符串中指针的位置
int flag=0;//记录往后移动一个指针SYM是否正确
string s;//用来保存要分析的字符串
struct _2tup
{
    string token;
    int id;
};

int advance(){//SYM的移动
    if(!getline(infile,str)){//从文件中提取字符
        return 0;
    }
    found1=str.find(',',0);
    found2=str.find('<',0);
    if(found1==-1){//当为error的时候，没有‘，’
        conterr++;
        cout<<"语法错误 识别字符错误"<<endl;
        return -1;
    }
    str_long=found1 - found2-1;
    sym=str.substr(found2+1,str_long);
    //cout<<sym<<endl;
    return 1;
}

void factorAnalysis(){//识别分析标识符
    if(sym=="29"||sym=="28"||sym=="8"){//如果是标识符和无符号整数，指针就往后移动
        flag=advance();
        if(conterr){
            return;
        }
        if(lpnum==0&&sym=="22"){//
            conterr++;
            cout<<"语法错误 ')'不匹配"<<endl;
            return;
        }
    }
    else if(sym=="21"){//如果是左括号，就要接下来判断是否为表达式，指针往后移动
        lpnum++;
      //  cout<<lpnum<<endl;
        flag=advance();
        if(conterr){
            return;
        }
        if(flag==0){//当为最后一个标志的时候，若没有右括号匹配就错误
            conterr++;
            cout<<"语法错误 '('后缺少表达式"<<endl;
            return;
        }
        expressionAnalysis();
        if(conterr){
            return;
        }
        if(flag==0||sym!="22"){
            conterr++;
            cout<<"语法错误 表达式后面缺少')'"<<endl;
            return;
        }else{
            lpnum--;
            flag=advance();
            if(conterr){
                return;
            }
            if(flag==0){
                return;
            }
        }
    }
    else{
        cout<<"语法错误 因子首部不为<标识符>|| <无符号整数> ||'('"<<endl;
        conterr++;
        return;
    }
    return;
}

void termAnalysis(){//识别分析乘除符号
    factorAnalysis();
    if(conterr){
        return;
    }
    while((sym=="14")||(sym=="15")){//当为'*'或'/'的时候，一直往后识别因子并循环
        flag=advance();
        if(conterr){
            return;
        }
        if(flag==0){
            conterr++;
            cout<<"语法错误 <乘除法运算符>后缺因子"<<endl;
            return;
        }
        if(conterr){
            return;
        }
        factorAnalysis();
        if(conterr){
            return;
        }
    }
    return;
};

void expressionAnalysis(){//识别分析加减符号
    if(conterr){
        return;
    }
    if((sym=="12")||(sym=="13")){//当为'+'或'-'的时候
        flag=advance();
        if(!conterr){
            return;
        }
        if(flag==0){
            cout<<"语法错误 <加减法运算符>后缺项"<<endl;
            conterr++;
            return;
        }
    }
    termAnalysis();
    if(conterr){
        return;
    }
    while((sym=="12")||(sym=="13")){//当为'+'或'-'的时候，一直往后识别项并循环
        flag=advance();
        if(conterr){
            return;
        }
        if(flag==0){
            cout<<"语法错误 <加法运算符>后缺项"<<endl;
            conterr++;
            return;
        }
        termAnalysis();
        if(conterr){
            return;
        }
    }
    return;
}


int main()
{
    flag=advance();
    if(flag){
        expressionAnalysis();
    }
    if(flag!=-1&&!conterr){
        cout<<"语法正确"<<endl;
    }

    return 0;
}
