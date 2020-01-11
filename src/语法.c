#include "stdio.h"
#include "string.h"
char prog[100],token[8],ch;
char *rwtab[6]={"begin","if","then","while","do","end"};
int syn,p,m,n,sum;
int kk;
factor();
expression();
yucu();
term();
statement();
lrparser();
scaner();
main()
{
 p=kk=0;
 printf("\nplease input a string (end with '#'): \n");
do
   { scanf("%c",&ch);
     prog[p++]=ch;
   }while(ch!='#');
 p=0;
 scaner();
 lrparser();
 getch();
}
lrparser()
{
 if(syn==1)
   {
scaner();       /*读下一个单词符号*/
     yucu();     /*调用yucu()函数；*/
     if (syn==6)
       { scaner();
  if ((syn==0)&&(kk==0))
 printf("success!\n");
 }
     else { if(kk!=1) printf("the string haven't got a 'end'!\n");
     kk=1;
   }
    }
 else { printf("haven't got a 'begin'!\n");
 kk=1;
       }
 return;
}
yucu()
{
statement();         /*调用函数statement();*/
  while(syn==26)
   {
 scaner();          /*读下一个单词符号*/
    if(syn!=6)
statement();         /*调用函数statement();*/
   }
  return;
}
statement()
{ if(syn==10)
   {
 scaner();        /*读下一个单词符号*/
   if(syn==18)
       { scaner();      /*读下一个单词符号*/
  expression();      /*调用函数statement();*/
       }
     else { printf("the sing ':=' is wrong!\n");
     kk=1;
    }
   }
  else { printf("wrong sentence!\n");
  kk=1;
       }
  return;
}
expression()
{ term();
  while((syn==13)||(syn==14))
    { scaner();             /*读下一个单词符号*/
      term();               /*调用函数term();*/
    }
  return;
}
term()
{ factor();
  while((syn==15)||(syn==16))
    { scaner();             /*读下一个单词符号*/
      factor();              /*调用函数factor(); */
    }
  return;
}
factor()
{ if((syn==10)||(syn==11)) scaner();
  else if(syn==27)
    { scaner();           /*读下一个单词符号*/
      expression();        /*调用函数statement();*/
      if(syn==28)
scaner();          /*读下一个单词符号*/
      else { printf("the error on '('\n");
      kk=1;
     }
    }
  else { printf("the expression error!\n");
  kk=1;
       }
  return;
}
 scaner()
 {  sum=0;
    for(m=0;m<8;m++)token[m++]=NULL;
    m=0;
    ch=prog[p++];
    while(ch==' ')ch=prog[p++];
    if(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A')))
      { while(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9')))
   {token[m++]=ch;
    ch=prog[p++];
   }
      p--;
      syn=10;
      token[m++]='\0';
      for(n=0;n<6;n++)
 if(strcmp(token,rwtab[n])==0)
    { syn=n+1;
      break;
    }
      }
    else if((ch>='0')&&(ch<='9'))
      { while((ch>='0')&&(ch<='9'))
 { sum=sum*10+ch-'0';
   ch=prog[p++];
 }
 p--;
 syn=11;
      }
    else switch(ch)
       { case '<':m=0;
    ch=prog[p++];
    if(ch=='>')
      {  syn=21;
      }
    else if(ch=='=')
      {  syn=22;
      }
    else
      {  syn=20;
         p--;
      }
    break;
  case '>':m=0;
    ch=prog[p++];
    if(ch=='=')
      { syn=24;
      }
    else
      { syn=23;
        p--;
      }
    break;
  case ':':m=0;
    ch=prog[p++];
    if(ch=='=')
      { syn=18;
      }
    else
      { syn=17;
        p--;
      }
    break;
  case '+': syn=13; break;
  case '-': syn=14; break;
  case '*': syn=15;break;
  case '/': syn=16;break;
  case '(': syn=27;break;
  case ')': syn=28;break;
  case '=': syn=25;break;
  case ';': syn=26;break;
  case '#': syn=0;break;
 default: syn=-1;break;
       }
    }

