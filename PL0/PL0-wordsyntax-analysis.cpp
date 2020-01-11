#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned long long symbol;
typedef unsigned long long symset;

const symbol nul = 0x1;
const symbol ident = 0x2;
const symbol number = 0x4;
const symbol plus = 0x8;
const symbol minus = 0x10;
const symbol times = 0x20;
const symbol slash = 0x40;
const symbol oddsym = 0x80;
const symbol eql = 0x100;
const symbol neq = 0x200;
const symbol lss = 0x400;
const symbol leq = 0x800;
const symbol gtr = 0x1000;
const symbol geq = 0x2000;
const symbol lparen = 0x4000;
const symbol rparen = 0x8000;
const symbol comma = 0x10000;
const symbol semicolon = 0x20000;
const symbol period = 0x40000;
const symbol becomes = 0x80000;
const symbol beginsym = 0x100000;
const symbol endsym = 0x200000;
const symbol ifsym = 0x400000;
const symbol thensym = 0x800000;
const symbol whilesym = 0x1000000;
const symbol dosym = 0x2000000;
const symbol callsym = 0x4000000;
const symbol constsym = 0x8000000;
const symbol varsym = 0x10000000;
const symbol procsym = 0x20000000;
const symbol readsym = 0x40000000;
const symbol writesym = 0x80000000;
const symbol repeatsym = 0x100000000;
const symbol untilsym = 0x200000000;
const symbol elsesym = 0x400000000;
const symbol allsym = 0x7FFFFFFFF;

const int IDENTIFIERS_LENGTH = 10; // length of identifiers
const int RESERVER_NUMBER = 16; // number of reserved word;
const int MAX_DIGITS_NUMBER = 14; //max. no. of digits in numbers
const int CODE_ARRAY_SIZE = 200; // size of code array
const int MAX_BLOCK_NESTING_DEPTH = 3; // max. depth of block nesting
const int MAX_ADDRESS = 2047; // max. address
const int IDENTIFIER_TABLE_LENGTH = 100; //length of identifier table
const int STACK_SIZE = 500;

enum fct { lit, opr, lod, sto, cal, Int/*int is reserved word in C*/, jmp, jpc, red, wrt }; // functions
struct instruction
{
	enum fct f; //function code
	int l; //level 0...
	int a; // displacement address 0...MAX_ADDRESS
};
enum objecttyp { constant, variable, procedure };
struct
{
	char name[IDENTIFIERS_LENGTH + 1];
	enum objecttyp kind;
	int val;
	int level;
	int adr;
} table[IDENTIFIER_TABLE_LENGTH + 1];

int err; // error count
int charCnt; //character count
int lineLen; //line length
FILE* fin; // source program file
int codeIndex; //code allocation index
char ch; //last character read
int kk; //last identifier length
char id[IDENTIFIERS_LENGTH + 1]; //last identifier read
symbol sym; //last symbol read
long long num; //last number read;
symset facbegsys, statbegsys, declbegsys;

char line[81];
char a[IDENTIFIERS_LENGTH + 1]; // every single word
char word[RESERVER_NUMBER][IDENTIFIERS_LENGTH + 1] = { "begin     ", "call      ", "const     ", "do        ", "else      ", "end       ", "if        ", "odd       ", "procedure ", "read      ", "repeat    ", "then      ", "until     ", "var       ", "while     ", "write     " };
symbol wsym[RESERVER_NUMBER] = { beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym, oddsym, procsym, readsym, repeatsym, thensym, untilsym, varsym, whilesym, writesym };
symbol ssym[256];
instruction code[CODE_ARRAY_SIZE];
char mnemonic[10][5] = { "LIT ", "OPR ", "LOD ", "STO ", "CAL ", "INT ", "JMP ", "JPC ", "RED ", "WRT " };
int s[STACK_SIZE];

char error_msg[40][64];
int tx;
long eof;

void error(int n)
{
	err++;
	printf("****");
	for (int i = 1; i < charCnt; i++)
	{
		printf(" ");
	}
	printf("^");
	printf("%s\n", error_msg[n - 1]);
}

void getch()
{
	if (charCnt == lineLen)
	{
		if (eof == ftell(fin))
		{
			printf("program incomplete\n");
			fclose(fin);
			exit(1);
		}
		lineLen = 0;
		charCnt = 0;
		printf("%4d ", codeIndex);
		while (eof != ftell(fin) && ((ch = getc(fin)) != '\n'))
		{
			lineLen++;
			printf("%c", ch);
			line[lineLen] = ch;
		}
		printf("\n");
		lineLen++;
		line[lineLen] = ' ';
	}
	charCnt++;
	ch = line[charCnt];
}

bool isLetter(char ch)
{
	if (ch >= 'a' && ch <= 'z')
	{
		return true;
	}
	if (ch >= 'A' && ch <= 'Z')
	{
		return true;
	}
	return false;
}

bool isDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return true;
	}
	return false;
}

int binSearch(int i, int j, char* id)
{
	int k;
	while (i <= j)
	{
		k = (i + j) / 2;
		if (!strcmp(id, word[k]))
		{
			return k;
		}
		else if (strcmp(id, word[k]) < 0)
		{
			j = k - 1;
		}
		else
		{
			i = k + 1;
		}
	}
	return -1;
}

void getsym()
{
	int k;
	while (ch == ' ' || ch == '\t')
	{
		getch();
	}
	if (isLetter(ch))
	{
		k = 0;
		do
		{
			if (k < IDENTIFIERS_LENGTH)
			{
				a[k] = ch;
				k++;
			}
			getch();
		} while (isLetter(ch) || isDigit(ch));

		if (k >= kk)
		{
			kk = k;
		}
		else
		{
			do
			{
				kk--;
				a[kk] = ' ';
			} while (kk > k);
		}
		strcpy(id, a);
		// binary search
		if ((k = binSearch(0, RESERVER_NUMBER - 1, id)) != -1)
		{
			sym = wsym[k];
		}
		else
		{
			sym = ident;
		}
	}
	else if (isDigit(ch))
	{
		k = 0;
		num = 0;
		sym = number;
		do
		{
			num = 10 * num + ch - '0';
			k++;
			getch();
		} while (isDigit(ch));
		if (k > MAX_DIGITS_NUMBER)
		{
			error(30);
		}
	}
	else if (ch == ':')
	{
		getch();
		if (ch == '=')
		{
			sym = becomes;
			getch();
		}
		else
		{
			sym = nul;
		}
	}
	else if (ch == '<')
	{
		getch();
		if (ch == '=')
		{
			sym = leq;
			getch();
		}
		else if (ch == '>')
		{
			sym = neq;
			getch();
		}
		else
		{
			sym = lss;
		}
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			sym = geq;
			getch();
		}
		else
		{
			sym = gtr;
		}
	}
	else
	{
		sym = ssym[(unsigned char)ch];
		getch();
	}
}

void gen(enum fct x, int y, int z)
{
	if (codeIndex > CODE_ARRAY_SIZE)
	{
		printf("program too long\n");

		fclose(fin);
		exit(1);
	}
	code[codeIndex].f = x;
	code[codeIndex].l = y;
	code[codeIndex].a = z;
	codeIndex++;
}

void test(symset s1, symset s2, int n)
{
	if (!(sym&s1))
	{
		error(n);
		s1 = s1 | s2;
		while (!(sym&s1))
		{
			getsym();
		}
	}
}

void enter(enum objecttyp k, int& tableIndex, int& dataIndex, int lev)
{
	tableIndex++;
	strcpy(table[tableIndex].name, id);
	table[tableIndex].kind = k;
	switch (k)
	{
	case constant:
		if (num > MAX_ADDRESS)
		{
			error(30);
			num = 0;
		}
		table[tableIndex].val = num;
		break;
	case variable:
		table[tableIndex].level = lev;
		table[tableIndex].adr = dataIndex;
		dataIndex++;
		break;
	case procedure:
		table[tableIndex].level = lev;
		break;
	}
}

int position(char* id, int& tableIndex)
{
	int i;
	strcpy(table[0].name, id);
	i = tableIndex;
	while (strcmp(table[i].name, id) != 0)
	{
		i--;
	}
	return i;
}

void constdeclaration(int& tableIndex, int& dataIndex, int lev)
{
	if (sym == ident)
	{
		getsym();
		if (sym&(eql | becomes))
		{
			if (sym == becomes)
			{
				error(1);
			}
			getsym();

			if (sym == number)
			{
				enter(constant, tableIndex, dataIndex, lev);
				getsym();
			}
			else
			{
				error(2);
			}
		}
		else
		{
			error(3);
		}
	}
	else
	{
		error(4);
	}
}

void vardeclaration(int& tableIndex, int& dataIndex, int lev)
{
	if (sym == ident)
	{
		enter(variable, tableIndex, dataIndex, lev);
		getsym();
	}
	else
	{
		error(4);
	}
}

void listcode(int cx0)
{
	for (int i = cx0; i < codeIndex; i++)
	{
		printf("%4d%7s%3d%5d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
	}
}
void expression(symset fsys, int& tableIndex, int& lev);
void factor(symset fsys, int& tableIndex, int& lev)
{
	int i;
	test(facbegsys, fsys, 24);
	if(sym&facbegsys)
	{
		if (sym == ident)
		{
			i = position(id, tableIndex);
			if (i == 0)
			{
				error(11);
			}
			else
			{
				switch (table[i].kind)
				{
				case constant:
					gen(lit, 0, table[i].val);
					break;
				case variable:
					gen(lod, lev - table[i].level, table[i].adr);
					break;
				case procedure:
					error(21);
					break;
				}
			}
			getsym();
		}
		else if (sym == number)
		{
			if (num > MAX_ADDRESS)
			{
				error(30);
				num = 0;
			}
			gen(lit, 0, num);
			getsym();
		}
		else if (sym == lparen)
		{
			getsym();
			expression(rparen | fsys, tableIndex, lev);
			if (sym == rparen)
			{
				getsym();
			}
			else
			{
				error(22);
			}
		}
		else
		{
			test(fsys, facbegsys, 23);
		}
	}
}

void term(symset fsys, int& tableIndex, int& lev)
{
	symbol mulop;
	factor(fsys | times | slash, tableIndex, lev);
	while (sym&(times | slash))
	{
		mulop = sym;
		getsym();
		factor(fsys | times | slash, tableIndex, lev);
		if (mulop == times)
		{
			gen(opr, 0, 4);
		}
		else
		{
			gen(opr, 0, 5);
		}
	}
}

void expression(symset fsys, int& tableIndex, int& lev)
{
	symbol addop;
	if (sym&(plus | minus))
	{
		addop = sym;
		getsym();
		term(fsys | plus | minus, tableIndex, lev);
		if (addop == minus)
		{
			gen(opr, 0, 1);
		}
	}
	else
	{
		term(fsys | plus | minus, tableIndex, lev);
	}
	while (sym&(plus|minus))
	{

		addop = sym;
		getsym();
		term(fsys | plus | minus, tableIndex, lev);
		if (addop == plus)
		{
			gen(opr, 0, 2);
		}
		else
		{
			gen(opr, 0, 3);
		}
	}
}

void condition(symset fsys, int& tableIndex, int& lev)
{
	symbol relop;
	if (sym == oddsym)
	{
		getsym();
		expression(fsys, tableIndex, lev);
		gen(opr, 0, 6);
	}
	else
	{
		expression(eql | neq | lss | gtr | leq | geq | fsys, tableIndex, lev);
		if (!(sym&(eql | neq | lss | leq | gtr | geq)))
		{
			error(20);
		}
		else
		{
			relop = sym;
			getsym();
			expression(fsys, tableIndex, lev);
			switch (relop)
			{
			case eql:
				gen(opr, 0, 8);
				break;
			case neq:
				gen(opr, 0, 9);
				break;
			case lss:
				gen(opr, 0, 10);
				break;
			case geq:
				gen(opr, 0, 11);
				break;
			case gtr:
				gen(opr, 0, 12);
				break;
			case leq:
				gen(opr, 0, 13);
				break;
			}
		}
	}
}

void statement(symset fsys, int& tableIndex, int& lev)
{
	int i, cx1, cx2;
	if (sym == ident)
	{
		i = position(id, tableIndex);
		if (i == 0)
		{
			error(11);
		}
		else
		{
			if (table[i].kind != variable)
			{
				error(12);
				i = 0;
				getsym();
			}
			else
			{
				getsym();
				if (sym == becomes)
				{
					getsym();
				}
				else
				{
					error(13);
				}
				expression(fsys, tableIndex, lev);
				if (i != 0)
				{
					gen(sto, lev - table[i].level, table[i].adr);
				}
			}
		}
	}
	else if (sym == callsym)
	{
		getsym();
		if (sym != ident)
		{
			error(14);
		}
		else
		{
			i = position(id, tableIndex);
			if (i == 0)
			{
				error(11);
			}
			else
			{
				if (table[i].kind == procedure)
				{
					gen(cal, lev - table[i].level, table[i].adr);
				}
				else
				{
					error(15);
				}
			}
			getsym();
		}
	}
	else if (sym == ifsym)
	{
		getsym();
		condition(thensym | dosym | fsys, tableIndex, lev);
		if (sym == thensym)
		{
			getsym();
		}
		else
		{
			test(thensym, statbegsys | semicolon | endsym | period, 16);
		}
		cx1 = codeIndex;
		gen(jpc, 0, 0);
		statement(elsesym | fsys, tableIndex, lev);
		code[cx1].a = codeIndex;
		if (sym == elsesym) {
			code[cx1].a++;
			getsym();
			cx2 = codeIndex;
			gen(jmp, 0, 0);
			statement(fsys, tableIndex, lev);
			code[cx2].a = codeIndex;
		}
	}
	else if (sym == beginsym)
	{
		getsym();
		statement(semicolon | endsym | fsys, tableIndex, lev);
		while (sym&(semicolon | statbegsys))
		{
			if (sym == semicolon)
			{
				getsym();
			}
			else
			{
				error(10);
			}
			statement(semicolon | endsym | fsys, tableIndex, lev);
		}

		if (sym == endsym)
		{
			getsym();
		}
		else
		{
			error(17);
		}
	}
	else if (sym == whilesym)
	{
		cx1 = codeIndex;
		getsym();
		condition(dosym | fsys, tableIndex, lev);
		cx2 = codeIndex;
		gen(jpc, 0, 0);
		if (sym == dosym)
		{
			getsym();
		}
		else
		{
			error(18);
		}
		statement(fsys, tableIndex, lev);
		gen(jmp, 0, cx1);
		code[cx2].a = codeIndex;
	}
	else if (sym == repeatsym)
	{
		cx1 = codeIndex;
		getsym();
		statement(semicolon |untilsym| fsys, tableIndex, lev);
		while (sym&(semicolon | statbegsys))
		{
			if (sym == semicolon)
			{
				getsym();
			}
			else
			{
				error(10);
			}
			statement(semicolon | untilsym | fsys, tableIndex, lev);
		}
		if (sym == untilsym)
		{
			getsym();
			condition(fsys, tableIndex, lev);
			gen(jpc, 0, cx1);
		}
		else
		{
			error(32);
		}

	}
	else if (sym == readsym)
	{
		getsym();
		if (sym == lparen)
		{
			do
			{
				getsym();
				if (sym == ident)
				{
					i = position(id, tableIndex);
					if (i == 0)
					{
						error(11);
					}
					else if (table[i].kind != variable)
					{
						error(12);
						i = 0;
					}
					else
					{
						gen(red, lev - table[i].level, table[i].adr);
					}
				}
				else
				{
					error(4);
				}
				getsym();
			} while (sym == comma);
		}
		else
		{
			error(40);
		}
		if (sym != rparen)
		{
			error(22);
			while (!sym&fsys)
			{
				getsym();
			}
		}
		else
		{
			getsym();
		}
	}
	else if (sym == writesym)
	{
		getsym();
		if (sym == lparen)
		{
			do
			{
				getsym();
				expression(rparen | comma | fsys, tableIndex, lev);
				gen(wrt, 0, 0);
			} while (sym == comma);

			if (sym != rparen)
			{
				error(22);
			}
			else
			{
				getsym();
			}
		}
		else
		{
			error(40);
		}
	}
	test(fsys, 0, 19);
}

void block(int lev, int& tableIndex, symset fsys)
{
	int dataIndex;
	int tx0, cx0;
	dataIndex = 3;
	tx0 = tableIndex;
	table[tableIndex].adr = codeIndex;
	gen(jmp, 0, 0);
	if (lev > MAX_BLOCK_NESTING_DEPTH)
	{
		error(32);
	}
	do
	{
		if (sym == constsym)
		{
			getsym();
			constdeclaration(tableIndex, dataIndex, lev);
			while (sym == comma)
			{
				getsym();
				constdeclaration(tableIndex, dataIndex, lev);
			}
			if (sym == semicolon)
			{
				getsym();
			}
			else
			{
				error(5);
			}
		}
		if (sym == varsym)
		{
			getsym();
			vardeclaration(tableIndex, dataIndex, lev);
			while (sym == comma)
			{
				getsym();
				vardeclaration(tableIndex, dataIndex, lev);
			}
			if (sym == semicolon)
			{
				getsym();
			}
			else
			{
				error(5);
			}
		}
		while (sym == procsym)
		{
			getsym();
			if (sym == ident)
			{
				enter(procedure, tableIndex, dataIndex, lev);
				getsym();
			}
			else
			{
				error(4);
			}
			if (sym == semicolon)
			{
				getsym();
			}
			else
			{
				error(5);
			}
			block(lev + 1, tableIndex, semicolon | fsys);

			if (sym == semicolon)
			{
				getsym();
				test(statbegsys | procsym, fsys, 6);
			}
			else
			{
				error(5);
			}
		}
		test(statbegsys, declbegsys, 7);
	} while (sym&declbegsys);

	code[table[tx0].adr].a = codeIndex;
	table[tx0].adr = codeIndex;
	cx0 = codeIndex;
	gen(Int, 0, dataIndex);
	statement(semicolon | endsym | fsys, tableIndex, lev);
	gen(opr, 0, 0);
	test(fsys, allsym, 8);
	//listcode(cx0);
	tx = tableIndex;
}

int base(int b, int l)
{
	int b1 = b;
	while (l > 0)
	{
		b1 = s[b1];
		l--;
	}
	return b1;
}

void interpret()
{
	int p, b, t; // program, base, top
	instruction i;
	printf("START PL/0\n");
	t = 0;
	b = 1;
	p = 0;
	s[0] = 0;
	s[1] = 0;
	s[2] = 0;
	s[3] = 0;
	do
	{
		i = code[p];
		p++;
		switch (i.f)
		{
		case lit:
			t++;
			s[t] = i.a;
			break;
		case opr:
			switch (i.a)
			{
			case 0:
				t = b - 1;
				p = s[t + 3];
				b = s[t + 2];
				break;
			case 1:
				s[t] = -s[t];
				break;
			case 2:
				t--;
				s[t] += s[t + 1];
				break;
			case 3:
				t--;
				s[t] -= s[t + 1];
				break;
			case 4:
				t--;
				s[t] *= s[t + 1];
				break;
			case 5:
				t--;
				s[t] /= s[t + 1];
				break;
			case 6:
				s[t] %= 2;
				break;
			case 8:
				t--;
				s[t] = (s[t] == s[t + 1]);
				break;
			case 9:
				t--;
				s[t] = (s[t] != s[t + 1]);
				break;
			case 10:
				t--;
				s[t] = (s[t] < s[t + 1]);
				break;
			case 11:
				t--;
				s[t] = (s[t] >= s[t + 1]);
				break;
			case 12:
				t--;
				s[t] = (s[t] > s[t + 1]);
				break;
			case 13:
				t--;
				s[t] = (s[t] <= s[t + 1]);
				break;
			}
			break;
		case lod:
			t++;
			s[t] = s[base(b, i.l) + i.a];
			break;
		case sto:
			s[base(b, i.l) + i.a] = s[t];
			t--;
			break;
		case cal:
			s[t + 1] = base(b, i.l);
			s[t + 2] = b;
			s[t + 3] = p;
			b = t + 1;
			p = i.a;
			break;
		case Int:
			t += i.a;
			break;
		case jmp:
			p = i.a;
			break;
		case jpc:
			if (s[t] == 0)
			{
				p = i.a;
			}
			t--;
			break;
		case red:
			printf("??:\n");
			scanf("%d", &s[base(b, i.l) + i.a]);
			break;
		case wrt:
			printf("%d\n", s[t]);
			t++;
			break;
		}
	} while (p != 0);
	printf("END PL/0\n");
}

void printCodeAndTable()
{
	for (int i = 0; i < codeIndex; i++)
	{
		printf("%d %d %d\n", code[i].f, code[i].l, code[i].a);
	}
	printf("\n");
	for (int i = 0; i < tx; i++)
	{
		printf("%s %d %d %d %d\n", table[i].name, table[i].kind, table[i].val, table[i].level, table[i].adr);
	}
}

int main()
{
	printf("please input source program file name:");
	char fileName[256];
	scanf("%s", fileName);

	if ((fin = fopen(fileName, "r")) == NULL)
	{
		printf("cannot open the file %s!", fileName);
		exit(1);
	}
	fseek(fin, 0, SEEK_END);
	eof = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	strcpy(error_msg[0], "应是=而不是:=");
	strcpy(error_msg[1], "=后应为数");
	strcpy(error_msg[2], "标识符后应为=");
	strcpy(error_msg[3], "const,var,procedure后应为标识符");
	strcpy(error_msg[4], "漏掉逗号或分号");
	strcpy(error_msg[5], "过程说明后的符号不正确");
	strcpy(error_msg[6], "应为语句");
	strcpy(error_msg[7], "程序体内语句部分后的符号不正确");
	strcpy(error_msg[8], "应为句号");
	strcpy(error_msg[9], "语句之间漏分号");
	strcpy(error_msg[10], "标识符未说明");
	strcpy(error_msg[11], "不可向常量或者过程赋值");
	strcpy(error_msg[12], "应为赋值运算符:=");
	strcpy(error_msg[13], "call后应为标识符");
	strcpy(error_msg[14], "不可调用常量或变量");
	strcpy(error_msg[15], "应为then");
	strcpy(error_msg[16], "应为分号或end");
	strcpy(error_msg[17], "应为do");
	strcpy(error_msg[18], "语句后的符号不正确");
	strcpy(error_msg[19], "应为关系运算符");
	strcpy(error_msg[20], "表达式内不可有过程标识符");
	strcpy(error_msg[21], "漏右括号");
	strcpy(error_msg[22], "因子后不可为此符号");
	strcpy(error_msg[23], "表达式不能以此符号开头");
	strcpy(error_msg[29], "这个数太大");
	strcpy(error_msg[31], "until未找到");
	strcpy(error_msg[39], "应为左括号");

	for (int i = 0; i < 256; i++)
	{
		ssym[i] = nul;
	}
	ssym['+'] = plus;
	ssym['-'] = minus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym['.'] = period;
	ssym['<'] = lss;
	ssym['>'] = gtr;
	ssym[';'] = semicolon;

	declbegsys = constsym | varsym | procsym;
	statbegsys = ident | beginsym | callsym | ifsym | whilesym | repeatsym;
	facbegsys = ident | number | lparen;

	err = 0;
	charCnt = 0;
	codeIndex = 0;
	lineLen = 0;
	ch = ' ';
	kk = IDENTIFIERS_LENGTH;

	getsym();
	int tableIndex = 0;
	block(0, tableIndex, period | declbegsys | statbegsys);
	if (sym != period)
	{
		error(9);
	}
	printCodeAndTable();
	if (err == 0)
	{
		listcode(0);
		interpret();
	}
	else
	{
		printf("ERRORS IN PL/0 PROGRAM\n");
	}
	fclose(fin);
	system("pause");
	return 0;
}
