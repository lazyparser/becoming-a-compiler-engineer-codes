#include<stdio.h>
#include<ctype.h>//ctype.h是C标准函数库中的头文件，定义了一批C语言字符分类函数，用于测试字符是否属于特定的字符类别
#include<conio.h>//conio.h库文件定义了通过控制台进行数据输入和数据输出的函数，主要是一些用户通过按键盘产生的对应操作
#include<string.h>
#include<stdlib.h>
#define keywordSum 9
//字符串指针数组保存保留字（扩充到9个char）（改动）
char *keyword[keywordSum] = { "if","else","for","while","do","read","write","char","int" };//return,switch,case,break,continue
//纯单分界符（改动）
char singleword[50] = { '+','-','*','(',')','{','}',',',';',':' ,'.'};
//双分界符的首字母
char doubleword[10] = { '>','<','=','!' };
//输入、输出文件名
char Scanin[300], Scanout[300],Result[300];
//指向文件的指针
FILE *fin, *fout,*fp,*fr;
char token[20], token1[40];//token保存单词符号，token1保存单词值

//函数声明
int TESTscan();
int TESTparse();
int program();
int declaration_list();
int statement_list();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int declaration_stat();
int compound_stat();
int expression_stat();
int statement();
int expression();
int bool_expr();
int additive_expr();
int term();
int factor();

//词法分析函数
int TESTscan() {//返回1，输入文件错误；返回2，输出文件错误；返回3，非法符号；
	char ch;//每次读入的字符
	char token[40];//保存识别的单词
	int es = 0, j, n;//es为0表示无错误
	printf("请输入源程序文件名（包括路径）：");
	scanf("%s", Scanin);
	getchar();
	printf("请输入词法分析输出文件名（包括路径）：");
	scanf("%s", Scanout);
	getchar();
	if ((fin = fopen(Scanin, "r")) == NULL)
	{
		printf("\n打开词法分析输入文件出错！\n");
		return(1);
	}
	if ((fout = fopen(Scanout, "w")) == NULL)
	{
		printf("\n创建词法分析输出文件出错！\n");
		return(2);
	}
	ch = getc(fin);//从文件中读取字符
	while (ch != EOF)//文件未读完时
	{
		while (ch == ' ' || ch == '\n' || ch == '\t')//滤过空格和转义字符
			ch = getc(fin);
		//判断是否是识别符还是保留字
		if (ch >= 'a'&&ch <= 'z' || ch >= 'A'&&ch <= 'Z' || ch == '_')//（扩充一个下划线）（改动）
		{
			token[0] = ch;
			j = 1;
			ch = getc(fin);
			while (ch >= 'a'&&ch <= 'z' || ch >= 'A'&&ch <= 'Z' || ch == '_' || ch >= '0'&&ch <= '9')
			{
				token[j++] = ch;
				ch = getc(fin);
			}
			token[j] = '\0';
			//查是否为保留字
			n = 0;
			while ((n<keywordSum) && strcmp(token, keyword[n]))
				n++;
			if (n >= keywordSum)//不是保留字
				fprintf(fout, "%s\t%s\n", "ID", token);
			else
				fprintf(fout, "%s\t%s\n", token, token);
		}
		//数字
		else if (ch >= '0'&&ch <= '9')
		{
			token[0] = ch;
			j = 1;
			ch = getc(fin);
			while (ch >= '0'&&ch <= '9')
			{
				token[j++] = ch;
				ch = getc(fin);
			}
			token[j] = '\0';
			fprintf(fout, "%s\t%s\n", "NUM", token);
		}
		//纯单界符
		else if (strchr(singleword, ch) > 0)//查找字符串s中首次出现字符ch的位置,返回首次出现ch的位置的指针，如果s中不存在ch则返回NULL。
		{
			token[0] = ch;
			token[1] = '\0';
			ch = getc(fin);
			fprintf(fout, "%s\t%s\n", token, token);
		}
		//双分界符
		else if (strchr(doubleword, ch) > 0)
		{
			token[0] = ch;
			ch = getc(fin);
			if (ch == '=')
			{
				token[1] = ch;
				token[2] = '\0';
				ch = getc(fin);
			}
			else
				token[1] = '\0';
			fprintf(fout, "%s\t%s\n", token, token);
		}
		//注释处理
		else if (ch == '/')
		{
			ch = getc(fin);
			if (ch == '*')
			{
				char ch1;
				ch1 = getc(fin);
				do
				{
					ch = ch1;
					ch1 = getc(fin);
				} while ((ch != '*' || ch1 != '/') && ch1 != EOF);
				ch = getc(fin);
			}
			else
			{
				token[0] = '/';
				token[1] = '\0';
				fprintf(fout, "%s\t%s\n", token, token);
			}

		}
		//错误处理
		else
		{
			token[0] = ch;
			token[1] = '\0';
			ch = getc(fin);//跳过错误，识别下一个单词。
			es = 3;
			fprintf(fout, "%s\t%s\n", "ERROR", token);
		}
	}
	fclose(fin);
	fclose(fout);
	return(es);
}

//语法分析函数
int TESTparse() 
{
	
	int es = 0;
	if ((fp = fopen(Scanout, "r")) == NULL)
	{
		printf("\n打开词法分析输出文件出错！\n");
		es=10;
	}
	printf("请输入语法分析输出文件名（包括路径）：");
	scanf("%s", Result);
	if ((fr = fopen(Result, "w")) == NULL)
	{
		printf("\n打开语法分析输出文件出错！\n");
		return(8);
	}
	if (es == 0)
		es = program();
	printf("\n语法分析结果如下：\n");
	switch (es)
	{
	case 0:
		printf("语法分析成功！\n");
		fprintf(fr,"\n%s\n","语法分析成功！\n");
		break;
	case 1:
		printf("缺少{\n");
		fprintf(fr, "\n%s\n", "缺少{\n");
		break;
	case 2:
		printf("缺少}\n");
		fprintf(fr, "\n%s\n", "缺少}\n");
		break;
	case 3:
		printf("缺少识别符\n");//declaration_stat()
		fprintf(fr, "\n%s\n", "缺少识别符\n");
		break;
	case 4:
		printf("缺少分号\n");
		fprintf(fr, "\n%s\n", "缺少分号\n");
		break;
	case 5:
		printf("缺少(\n");
		fprintf(fr, "\n%s\n", "缺少（\n");
		break;
	case 6:
		printf("缺少)\n");
		fprintf(fr, "\n%s\n", "缺少）\n");
		break;
	case 7:
		printf("缺少操作数\n");//factor()
		fprintf(fr, "\n%s\n", "缺少操作数\n");
		break;
	case 8:
		printf("\n打开语法分析输出文件出错！\n");
		fprintf(fr, "\n%s\n", "\n打开语法分析输出文件出错！\n");
		break;
	case 10:
		printf("\n打开词法分析输出文件出错！\n");
		fprintf(fr, "\n%s\n", "\n打开词法分析输出文件出错！\n");
		break;
	}
	fclose(fp);
	fclose(fr);
	return(es);
}

//程序 子函数
int program() 
{
	int es = 0;
	fscanf(fp,"%s\t%s\n",&token,&token1);//“读文件”函数
	fprintf(fr, "%s\t%s\n", &token, &token1);
	if (strcmp(token, "{"))//相等时，返回“0”，不执行该语句，比较的是token
	{
		es = 1;
		return es;
	}
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = declaration_list();//fscanf和printf操作包含在子函数中
	if (es > 0)
		return(es);
	es = statement_list();
	if (es > 0)
		return(es);
	if (strcmp(token, "}"))
	{
		es = 2;
		return(es);
	}
	return(es);
}

int declaration_list() 
{
	int es = 0;
	while ((strcmp(token,"int")==0)||(strcmp(token,"char")==0))//扩充了char型
	{
		es = declaration_stat();
		if (es > 0)
			return(es);
	}
	return(es);
}

int declaration_stat() 
{
	int es = 0;
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	if (strcmp(token, "ID"))
		return(es=3);//缺少识别符
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	if (strcmp(token, ";"))
		return(es = 4);
	fscanf(fp, "%s\t%s\n", &token, &token1);//读下一个token，用于判断declaration_list()的循环条件
	fprintf(fr, "%s\t%s\n", &token, &token1);
	return(es);
}

int statement_list() 
{
	int es = 0;
	while (strcmp(token, "}"))//当扫描到token是“}”时，while（0）,此语句不执行，即不再有statement了。program结束！
		                      //结合program()函数来看
	{
		es = statement();
		if (es > 0)
			return(es);
	}
	return(es);
}

int compound_stat() 
{
	int es = 0;
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = statement_list();
	return(es);
}

int statement()
{
	int es = 0;
	if (es == 0 && strcmp(token, "if") == 0)
	{
		fprintf(fr, "\n%s\n", "\n---if语句---\n");
		printf("\n源程序存在if语句\n");
		es = if_stat();
	}
	if (es == 0 && strcmp(token, "while") == 0)
		{
		fprintf(fr, "\n%s\n", "\n---while语句---\n");
			printf("\n源程序存在while语句\n");
			es = while_stat();
		}
	if (es == 0 && strcmp(token, "for") == 0)
		{
		fprintf(fr, "\n%s\n", "\n---for语句---\n");
			printf("\n源程序存在for语句\n");
			es = for_stat();
		}
	if (es == 0 && strcmp(token, "read") == 0)
		{
		fprintf(fr, "\n%s\n", "\n---read语句---\n");
			printf("\n源程序存在read语句\n");
			es = read_stat();
		}
	if (es == 0 && strcmp(token, "write") == 0)
		{
		fprintf(fr, "\n%s\n", "\n---write语句---\n");
			printf("\n源程序存在write语句\n");
			es = write_stat();
		}
	if (es == 0 && strcmp(token, "{") == 0)
		{
		fprintf(fr, "\n%s\n", "\n---复合语句---\n");
			printf("\n源程序存在复合语句\n");
			es = compound_stat();
		}
	if (es == 0 && strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0)
		{
		
			printf("\n源程序存在表达式语句\n");
			es = expression_stat();
		}
	return(es);
}

int if_stat(){
    int es=0;
	
    fscanf(fp,"%s\t%s\n",&token,&token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
    if(strcmp(token,"("))    
		return(es=5);
    fscanf(fp,"%s\t%s\n",&token,&token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
    es=expression();
    if(es>0) 
		return(es);
    if(strcmp(token,")"))    
		return(es=6);
    fscanf(fp,"%s\t%s\n",&token,&token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
    es=statement();
    if(es>0) 
		return(es);
    if(strcmp(token,"else")==0)
    {
      fscanf(fp,"%s\t%s\n",&token,&token1);
	  fprintf(fr, "%s\t%s\n", &token, &token1);
      es=statement();
      if(es>0) 
		  return(es);
    }
    return(es);
}

int while_stat()
{
	int es = 0;
	
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	if (strcmp(token, "(")) 
		return(es = 5);
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = expression();
	if (es>0)
		return(es);
	if (strcmp(token, ")")) 
		return(es = 6);
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = statement();
	return(es);
}

int for_stat()
{
	int es = 0;
	
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	if (strcmp(token, "(")) 
		return(es = 5);
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = expression();
	if (es>0) 
		return(es);
	if (strcmp(token, ";")) 
		return(es = 4);
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = expression();
	if (es>0) 
		return(es);
	if (strcmp(token, ";")) 
		return(es = 4);
	fscanf(fp, "%s\t%s\n",&token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = expression();
	if (es>0) 
		return(es);
	if (strcmp(token, ")")) 
		return(es = 6);
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = statement();

	return(es);
}

int write_stat()
{
	int es = 0;
	
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	es = expression();
	if (es>0) 
		return(es);
	if (strcmp(token, ";")) 
		return(es = 4);
	fscanf(fp, "%s\t%s\n", &token, &token1); 
	fprintf(fr, "%s\t%s\n", &token, &token1);
	return(es);
}

int read_stat()
{
	int es = 0;
	
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	if (strcmp(token, "ID")) 
		return(es = 3);
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	if (strcmp(token, ";")) 
		return(es = 4);
	fscanf(fp, "%s\t%s\n", &token, &token1);
	fprintf(fr, "%s\t%s\n", &token, &token1);
	return (es);
}

int expression_stat()
{
	int es = 0;
	if (strcmp(token, ";") == 0)
	{
		fscanf(fp, "%s\t%s\n", &token, &token1);
		fprintf(fr, "%s\t%s\n", &token, &token1);
		return(es);
	}
	es = expression();
	if (es > 0)
		return(es);
	if (es == 0 && strcmp(token, ";") == 0) 
	{
		fscanf(fp, "%s\t%s\n", &token, &token1);
		fprintf(fr, "%s\t%s\n", &token, &token1);
		return(es);
	}
	else
	{
		es = 4;
		return(es);//缺少；
	}
}

int expression()
{
	int es = 0, fileadd;
	char token2[20], token3[40];
	if (strcmp(token, "ID") == 0)
	{
		fileadd = ftell(fp);
		fscanf(fp, "%s\t%s\n", &token2, &token3);
		fprintf(fr, "%s\t%s\n", &token2, &token3);
		if (strcmp(token2, "=") == 0)
		{
			fscanf(fp, "%s\t%s\n", &token, &token1);
			fprintf(fr, "%s\t%s\n", &token, &token1);
			es = bool_expr();
			if (es>0)
				return(es);
		}
		else
		{
			fseek(fp, fileadd, 0);
			  
			es = bool_expr();
			if (es>0)
				return(es);
		}
	}
	else
		es = bool_expr();
	return(es);
}

int bool_expr()
{
	int es = 0;
	es = additive_expr();
	if (es>0)
		return(es);
	if (strcmp(token, ">") == 0 || strcmp(token, ">=") == 0 || strcmp(token, "<") == 0 || strcmp(token, "<=") == 0 || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0)
	{
		fscanf(fp, "%s\t%s\n", &token, &token1);
		fprintf(fr, "%s\t%s\n", &token, &token1);
		es = additive_expr();
		if (es>0)
			return(es);
	}
	return(es);
}

int additive_expr(){
	int es = 0;
	es = term();
	if (es>0) 
		return(es);
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		fscanf(fp, "%s\t%s\n", &token, &token1);
		fprintf(fr, "%s\t%s\n", &token, &token1);
		es = term();
		if (es>0) return(es);
	}
	return(es);
}

int term(){
	int es = 0;
	es = factor();
	if (es>0) 
		return(es);
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		fscanf(fp, "%s\t%s\n", &token, &token1);
		fprintf(fr, "%s\t%s\n", &token, &token1);
		es = factor();
		if (es>0) 
			return(es);
	}
	return(es);
}

int factor(){
	int es = 0;
	if (strcmp(token, "(") == 0)
	{
		fscanf(fp, "%s\t%s\n", &token, &token1);
		fprintf(fr, "%s\t%s\n", &token, &token1);
		es = expression();
		if (es>0) 
			return(es);
		if (strcmp(token, ")")) 
			return(es = 6);
		fscanf(fp, "%s\t%s\n", &token, &token1);
		fprintf(fr, "%s\t%s\n", &token, &token1);
	}
	else
	{
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0)
		{
			fscanf(fp, "%s\t%s\n", &token, &token1);
			fprintf(fr, "%s\t%s\n", &token, &token1);
			return(es);
		}
		else
		{
			es = 7;
			return(es);
		}
	}
	return(es);
}

//主函数
int main() 
{
	int es = 0;
	es = TESTscan();
	switch (es) {//switch语句详细解释错误情况
	case 0:
		printf("词法分析成功！\n");
		break;
	case 1:
		printf("输入文件有错，编译停止！\n");
		break;
	case 2:
		printf("输出文件有错，编译停止！\n");
		break;
	case 3:
		printf("存在非法符号，编译停止！\n");
		break;
	}
	if (es == 0) 
	{
		es = TESTparse();
		if (es == 0)
			printf("语法分析成功！\n");
		else
			printf("语法分析错误！\n");
	}
	system("pause");
	return 0; 
}

