// pL/0���Դʷ�������
#include<bits/stdc++.h>
using namespace std;

ifstream infile("c.txt");//�ʷ������Ľ�����﷨����������
string str;//string���������ַ�ʶ��
string sym; //ָ��

void expressionAnalysis();//���ʽ����
void termAnaysis();//�����
void factorAnalysis();//���ӷ���
int advance();

int conterr=0;//��¼����
int lpnum=0;//��¼������
int stru=0;
int found1,found2,str_long;//��ȡ�ַ�����ָ���λ��
int flag=0;//��¼�����ƶ�һ��ָ��SYM�Ƿ���ȷ
string s;//��������Ҫ�������ַ���
struct _2tup
{
    string token;
    int id;
};

int advance(){//SYM���ƶ�
    if(!getline(infile,str)){//���ļ�����ȡ�ַ�
        return 0;
    }
    found1=str.find(',',0);
    found2=str.find('<',0);
    if(found1==-1){//��Ϊerror��ʱ��û�С�����
        conterr++;
        cout<<"�﷨���� ʶ���ַ�����"<<endl;
        return -1;
    }
    str_long=found1 - found2-1;
    sym=str.substr(found2+1,str_long);
    //cout<<sym<<endl;
    return 1;
}

void factorAnalysis(){//ʶ�������ʶ��
    if(sym=="29"||sym=="28"||sym=="8"){//����Ǳ�ʶ�����޷���������ָ��������ƶ�
        flag=advance();
        if(conterr){
            return;
        }
        if(lpnum==0&&sym=="22"){//
            conterr++;
            cout<<"�﷨���� ')'��ƥ��"<<endl;
            return;
        }
    }
    else if(sym=="21"){//����������ţ���Ҫ�������ж��Ƿ�Ϊ���ʽ��ָ�������ƶ�
        lpnum++;
      //  cout<<lpnum<<endl;
        flag=advance();
        if(conterr){
            return;
        }
        if(flag==0){//��Ϊ���һ����־��ʱ����û��������ƥ��ʹ���
            conterr++;
            cout<<"�﷨���� '('��ȱ�ٱ��ʽ"<<endl;
            return;
        }
        expressionAnalysis();
        if(conterr){
            return;
        }
        if(flag==0||sym!="22"){
            conterr++;
            cout<<"�﷨���� ���ʽ����ȱ��')'"<<endl;
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
        cout<<"�﷨���� �����ײ���Ϊ<��ʶ��>|| <�޷�������> ||'('"<<endl;
        conterr++;
        return;
    }
    return;
}

void termAnalysis(){//ʶ������˳�����
    factorAnalysis();
    if(conterr){
        return;
    }
    while((sym=="14")||(sym=="15")){//��Ϊ'*'��'/'��ʱ��һֱ����ʶ�����Ӳ�ѭ��
        flag=advance();
        if(conterr){
            return;
        }
        if(flag==0){
            conterr++;
            cout<<"�﷨���� <�˳��������>��ȱ����"<<endl;
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

void expressionAnalysis(){//ʶ������Ӽ�����
    if(conterr){
        return;
    }
    if((sym=="12")||(sym=="13")){//��Ϊ'+'��'-'��ʱ��
        flag=advance();
        if(!conterr){
            return;
        }
        if(flag==0){
            cout<<"�﷨���� <�Ӽ��������>��ȱ��"<<endl;
            conterr++;
            return;
        }
    }
    termAnalysis();
    if(conterr){
        return;
    }
    while((sym=="12")||(sym=="13")){//��Ϊ'+'��'-'��ʱ��һֱ����ʶ���ѭ��
        flag=advance();
        if(conterr){
            return;
        }
        if(flag==0){
            cout<<"�﷨���� <�ӷ������>��ȱ��"<<endl;
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
        cout<<"�﷨��ȷ"<<endl;
    }

    return 0;
}
