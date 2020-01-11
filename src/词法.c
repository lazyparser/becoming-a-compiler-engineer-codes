#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

#define CODE "E:\\code\\code.txt"
#define RESULT "E:\\code\\result.txt"

//token�����������չؼ��֣�������������ͽ��
//�������Ʊ������ĳ������Ϊ9
//prog����洢����Դ�����ַ�������
char *prog, token[10];
char ch;
//syn�Ǹ������ʷ��Ŷ�Ӧ������
int syn, p, m = 0, n, line, sum = 0;
//rwtab����洢���ǹؼ���
char *rwtab1[10] = { "begin","if","then","while","do","end" };
char *rwtab2[4] = { "const","var","procedure","call" };

void scaner()
{
	//�涨����ʶ��ֻ������ĸ�����ֹ���

	/*
	����Ϊ����飬�ֱ��Ǳ�ʾ�������֡����ţ���Ӧ����� if   else if  �� else
	*/

	//��ȫ���ÿ�
	for (n = 0; n<10; n++)
		//tokenΪ�Ѳ�����ַ���
		token[n] = NULL;
	ch = prog[p++];

	//������������ȥ���ո�
	while (ch == ' ')
	{
		ch = prog[p];
		p++;
	}
	//�����if�ж��У���Χ��a-z����A-Z����Ϊ�涨����ֻ������ĸ��ͷ
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))  //�����Ǳ�ʾ�����߱�����
	{
		m = 0;
		//����Ǳ����ĵ�һ���ַ��Ժ󣬿�������ĸ������
		while ((ch >= '0'&&ch <= '9') || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		{
			token[m++] = ch;
			ch = prog[p++];
		}
		//�������ַ���������־
		token[m++] = '\0';
		p--;
		syn = 10;

		//��ʶ��������ַ����Ѷ���ı�ʾ�����Ƚϣ� �ж��Ƿ��ǹؼ��֣����йؼ��ֶ���Сд
		for (n = 0; n<6; n++)
			if (strcmp(token, rwtab1[n]) == 0)
			{
				syn = n + 1;
				break;
			}
		for (n = 0; n < 4;n++)
		{
			if (strcmp(token, rwtab2[n]) == 0)
			{
				syn = n + 31;
				break;
			}
		}
	}
	else if ((ch >= '0'&&ch <= '9'))  //���� ����������֣�����sum�������������
	{
		sum = 0;
		while ((ch >= '0'&&ch <= '9'))
		{
			//����*10��ֻ����ʮ������
			sum = sum * 10 + ch - '0';
			ch = prog[p++];
		}
		p--;
		syn = 11;
		//�ɽ��յ����ֵ����ֵΪ32767��������󣬻ᱨ��
		if (sum>32767)
			syn = -1;
	}
	else switch (ch)   //������������ַ�
	{
	case '<':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '>')
		{
			//˵���ǲ��Ⱥ�
			syn = 21;
			token[m++] = ch;
		}
		else if (ch == '=')
		{
			//˵����<=
			syn = 22;
			token[m++] = ch;
		}
		else
		{
			//���򣬾�ֻ��һ��<����
			syn = 23;
			//��ʱp����һ��
			p--;
		}
		break;
	case '>':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '=')
		{
			syn = 24;
			token[m++] = ch;
		}
		else
		{
			syn = 20;
			p--;
		}
		break;
	case ':':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '=')
		{
			//˵���Ǹ�ֵ�����
			syn = 18;
			token[m++] = ch;
		}
		else
		{
			//�����ֻ�Ǹ�:
			syn = 17;
			p--;
		}
		break;
	case '*':
		syn = 13;
		token[0] = ch;
		break;
	case '/':
		syn = 14;
		token[0] = ch;
		break;
	case '+':
		syn = 15;
		token[0] = ch;
		break;
	case '-':
		syn = 16;
		token[0] = ch;
		break;
	case '=':
		syn = 25;
		token[0] = ch;
		break;
	case ';':
		syn = 26;
		token[0] = ch;
		break;
	case '(':
		syn = 27;
		token[0] = ch;
		break;
	case ')':
		syn = 28;
		token[0] = ch;
		break;
	case ',':
		syn = 29;
		token[0] = ch;
		break;
	case '!':
		syn = 30;
		token[0] = ch;
		break;
	case '.':
		//������յ�����.��˵������Դ����Ľ�β����syn=0����������
		syn = 0;
		token[0] = ch;
		break;
	case '\n':
		//������յ����ǻ��з�����syn=-2����+1
		syn = -2;
		break;
	default:
		//������յ���������δ������ַ�����syn=-1���ᱨ��
		syn = -1;
		break;
	}
}

/*
��ȡԴ�����ļ�(.txt)
*/
void read()
{
	FILE *fp;
	fp = fopen(CODE, "r");
	fseek(fp, 0, SEEK_END);
	int file_size;
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	prog = (char *)malloc(file_size * sizeof(char));
	fread(prog, file_size, sizeof(char), fp);

	//�ر��ļ���
	fclose(fp);
}

int main()
{
	int p = 0;
	int line = 1;
	ofstream outfile(RESULT);

	cout<< "���ش����ļ���......" << endl;
	Sleep(3000);
	//��ȡԴ�����ļ�
	read();

	p = 0;
	outfile << "�ʷ������Ľ��Ϊ��" << endl;
	do
	{
		scaner();
		switch (syn)
		{
		case 11:
			//cout << "(" << syn << "," << sum << ")" << endl;
			outfile << "(" << syn << "," << sum << ")" << endl;
			break;
		case -1:
			//cout << "Error in line " << line << "!" << endl;
			outfile << "Error in line" << line << "!" << endl;
			break;
		case -2:
			line = line++;
			break;
		default:
			//cout << "(" << syn << "," << token << ")" << endl;
			outfile << "(" << syn << "," << token << ")" << endl;
			break;
		}
	} while (syn != 0);

	outfile.close();

	cout << "�ʷ�������ϣ�����result.txt�в鿴" << endl;

	system("pause");
	return 0;
}
