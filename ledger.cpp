#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <string>
#include <time.h>
void in(int);
void ex(int);
void revise(int);
void update();
void show();
void preshow();
int CorrectDay(int);
using namespace std;
#define MAX 200

/* 1.uppercase
  v2.到幾日前買了啥 show all

  v3.pay on shopping     |
  v4.pay on eating       |->  到幾日前？ eating部分分細項
  v5.pay on recreation   |

  v7.輸入選擇改用字串 防呆 （怕int被輸入char）
  v8.日期輸入位置 一進入program -> show 2015/05/12 updated
  v8.2 如果today有值 那下次進入show就不用輸入 2015/05/13
  v9.show 一直 show
  v10.修改資料 透過id 2015/05/13
  11.revise()第一個while可以優化 因為只是要印出東西，可以不用弄到tmpSET
  12.優化格子
  v13. revise 的補零還沒做 2015/05/13
  v14. 新增README, updateManual 2015/5/13
  v15. total 列出total 2015/5/13
  ***16. update格式自動修正 || 確認? 2015/5/15
  ***16-2. revise 錯誤格式修正
  ***16-3. revise 第二層back exit while（可用typechosen當參數 再跑一次revise）
  v17. TRANSPORT expenses and others expenses 2015/05/13 updated
  18. rm line?
  v19. exit everywhere 2015/05/13
  v20. howmanydaysago 優化 1.多少天都ok了 2.輸入非整數提醒
  v21. preshow 一開始 輸入非整數提醒 2015/05/13 updated
  v22. update內函示滬跳 目前in ex可跳
  v23. stomach exp (wrong command implemt) 2015/05/14
  v24. yorn->區域變數 ！！！！！！！ 2015/0513 完成 global variables還是儘量別碰！
  v25. 換行換行換行
  v26. 第二次進入show seg 2015/05/14 修正
  v27. show date 8數字但非格式 2015/05/14 達成
  v28. total exp 補零 2015/05/14
  v29. revise 超過 casesnumber 2015/05/15
  30. 初開ledger 自建檔案
  31. update只輸入月日
  32. revise all

  N.自己增加type


  多用new,不然常會buffer overflow
  define最好都大寫 不然會撞變數
  */
typedef struct set{
    int ID;
    int IoE;    /* 1 == income ; -1 == expense */
    int Amt;
    int year, month, day;
    int Type;     /* (1)income : 1.Salary, 2.other
                     (2)expense : 1.breakfast, 2.lunch, 3.dinner, 4.snack, 5.shopping, 6.drink, 7.TRANSPORT, 8, recreation, 9.others*/
    int amount;
    char PS[200];
} SET;

typedef struct line{
  char data[MAX];
}LINE;

/* in */
#define Salary 1
#define other 2

/* exp */
#define breakfast -1
#define lunch -2
#define dinner -3
#define snack -4
#define shopping -5
#define drink -6
#define TRANSPORT -7
#define recreation -8
#define others -9

/* revise */
#define Date 1
#define Amount 2
#define T 3
#define P 4

/* show */
#define yearExp 1
#define monExp 2
#define dailyExp 3
#define dayagoExp 4
#define stomach 5
#define desire 6
#define fun 7
#define TotalExp 8
#define TotalInc 9
#define TotalBal 10
#define back 11

vector<SET> S;
string NAME;
int TODAY, TODAYTyped;
int systemYEAR, systemMONTH, systemDAY;
char PorN[3] = { '-', 'N', '+'};
char type[200][30]; //typetable of incomes and expenses
void YearMonthDayis()
{
  tm *today;
  time_t t;
  t=time(0);
  today=localtime(&t);
  systemYEAR = today->tm_year+1900;
  systemMONTH = today->tmp_mon+1;
  systemDAY = today->tm_monday;
}
void whoami()
{
  cout << "Hi, I'm Baymax, your personal money care ledger." << endl;
  getchar();
  cout << "Press Enter ..";
  getchar();

}
int IsAnumber(char *input)
{

  int len = strlen(input);
  for(int i = 0; i < len; i++)
    if(input[i] - '0' < 0 || input[i] - '0' > 9)
      return -1;
  return 1;
}
int checkUpdate(char* input, int IoE)
{
  int len = strlen(input);
  int dilnum = 0;
  int comnum = 0;
  int comflag1 = -1;
  int comflag2 = -1;
  int comflag3 = -1;
  char tmp[MAX];
  char *piece;
  char input2cut[100];
  strcpy(input2cut, input);
  for(int i = 0; i < len; i++)
  {
    if(input2cut[i] == '/')
      dilnum++;
    if(input2cut[i] == ','){
      if(dilnum != 2)
      {
        cout << "Wrong form! Please enter again." << endl;
        return -1;
      }
      else
      {
        if(comflag1 == -1)
          comflag1 = i;
        else if(comflag1 != -1 && comflag2 == -1)
          comflag2 = i;
        else if(comflag1 != -1 && comflag2 != -1 && comflag3 == -1)
          comflag3 = i;
        comnum++;
      }
    }
  }
  if(comnum < 3){
    cout << "Wrong form! Please enter again." << endl;
    return -1;
  }
  if(comflag3 - comflag2 == 1 || comflag2 - comflag1 == 1){
    cout << "Wrong form! Please enter again." << endl;
    return -1;
  }
  if(dilnum != 2){
    cout << "Wrong form! Please enter again." << endl;
    return -1;
  }

  piece = strtok(input2cut, "/"); //year
  for(int i = 0; i < strlen(piece); i++)
    if(piece[i] - '0' < 0 || piece[i] - '0' > 9 ){
      cout << "Wrong form! Please enter again." << endl;
      return -1;
    }
  strcpy(tmp, piece);
  piece = strtok(NULL, "/"); //month
  for(int i = 0; i < strlen(piece); i++)
    if(piece[i] - '0' < 0 || piece[i] - '0' > 9 ){
      cout << "Wrong form! Please enter again." << endl;
      return -1;
    }
  if(strlen(piece) == 1){
    strcat(tmp, "0");
    strcat(tmp, piece);
  }
  else
    strcat(tmp, piece);

  piece = strtok(NULL, ","); // day
  for(int i = 0; i < strlen(piece); i++)
    if(piece[i] - '0' < 0 || piece[i] - '0' > 9 ){
      cout << "Wrong form! Please enter again." << endl;
      return -1;
    }
  if(strlen(piece) == 1){
      strcat(tmp, "0");
      strcat(tmp, piece);
  }
  else
    strcat(tmp, piece);

  if(CorrectDay(atoi(tmp)) != 1)
    return -1;

  piece = strtok(NULL, ","); // amount
  if(strlen(piece) == 0){
    cout << "Wrong form! Please enter again." << endl;
    return -1;
  }
  piece = strtok(NULL, ","); // IoE type
  if(piece == NULL){
    cout << "Wrong form! Please enter again." << endl;
    return -1;
  }else if(IoE == 1){
    if(atoi(piece) != 1 && atoi(piece) != 2){
      cout << "Wrong form! Please enter again." << endl;
      return -1;
    }
  }else if(IoE == -1){
    if(atoi(piece) < 1 || atoi(piece) > 9){
      cout << "Wrong form! Please enter again." << endl;
      return -1;
    }
  }
  piece = strtok(NULL, ","); // PS
  if(piece == NULL){
    cout << "Wrong form! Please enter again." << endl;
    return -1;
  }
  return 1;

}
void in(int index)
{
  FILE *fptr;
  string YorN;
  int newNum = index;
  char tmp[MAX];
  char num[9];
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << "-----------Type--Table-----------" << endl;
  cout << "|          (1). Salary          |" << endl;
  cout << "|          (2). Others          |" << endl;
  cout << "---------------------------------" << endl;
  cout << "Keep entering data until type end" << endl;
  cout << "Please enter by the following form. (comma included)" << endl;
  cout << "20XX/YY/ZZ,Amount,Type(1 or 2),P.S" << endl;
  scanf("%s", tmp);
  while(strcmp(tmp, "end") != 0 && strcmp(tmp, "no") != 0 && strcmp(tmp, "n") != 0 && strcmp(tmp, "exit") && strcmp(tmp, "back") != 0 && strcmp(tmp, "ex") != 0 && strcmp(tmp, "revise") != 0)
  {
    if(checkUpdate(tmp, 1) == 1){
      cout << "Please make sure again .. ";
      printf("%s ? ", tmp);
      cout << "yes, no or back ? ";
      cin >> YorN;
      while(YorN != "no" && YorN != "n" && YorN != "yes" && YorN != "y" && YorN != "back" && YorN != "b" && YorN != "ex" && YorN != "exit")
      {
        cout << "Wrong command. " << endl;
        printf("%s ? ", tmp);
        cin >> YorN;
      }
      if(YorN == "no" || YorN == "n")
      {
        in(newNum);
        break;
      }
      else if(YorN == "yes" || YorN == "y")
      {
          newNum++; //index
          fptr = fopen("data.ledger", "a");
          sprintf(num, "%d", newNum);
          fputs(num, fptr);
          fputc(',',fptr);
          fputs("1,", fptr);
          fputs(tmp, fptr);
          fputc('\n', fptr);
          fclose(fptr);
          cout << "What else ? ";
      }
      else if(YorN == "back" || YorN == "b")
        break;
      else if(YorN == "ex")
      {
        ex(newNum);
        break;
      }
      else if(YorN == "revise")
      {
        revise(-1);
        break;
      }
      else if(YorN == "exit")
        exit(0);
    }
    scanf("%s", tmp);
  }
  if(strcmp(tmp, "ex") == 0)
    ex(newNum);
  else if(strcmp(tmp, "revise") == 0)
    revise(-1);
  else if(strcmp(tmp, "exit") == 0)
    exit(0);
  return;
}
void ex(int index)
{
  FILE *fptr;
  int newNum = index;
  char num[9];
  char tmp[MAX];
  string YorN;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  { cout << "-----------Type-Table-----------" << endl;
  cout << "|        (1). breakfast        |" << endl;
  cout << "|        (2). lunch            |" << endl;
  cout << "|        (3). dinner           |" << endl;
  cout << "|        (4). snack            |" << endl;
  cout << "|        (5). shopping         |" << endl;
  cout << "|        (6). drink            |" << endl;
  cout << "|        (7). Transport        |" << endl;
  cout << "|        (8). recreation       |" << endl;
  cout << "|        (9). others           |" << endl;
  cout << "--------------------------------" << endl;
  cout << "Please enter by the following form. (comma included)" << endl;
  cout << "20XX/YY/ZZ,Amount,Type(number),P.S" << endl;
  cout << "Keep entering data until type end" << endl;
  }
  scanf("%s", tmp);
  while(strcmp(tmp, "end") != 0 && strcmp(tmp, "no") != 0 && strcmp(tmp, "n") != 0 && strcmp(tmp, "exit") && strcmp(tmp, "back") != 0 && strcmp(tmp, "in") != 0 && strcmp(tmp, "revise") != 0)
  {
    if(checkUpdate(tmp, -1) == 1){
      cout << "Please make sure again .. ";
      printf("%s ? ", tmp);
      cout << "yes, no or back ? ";
      cin >> YorN;
      while(YorN != "no" && YorN != "n" && YorN != "yes" && YorN != "y" && YorN != "back" && YorN != "b" && YorN != "in" && YorN != "exit")
      {
        cout << "Wrong command. " << endl;
        printf("%s ? ", tmp);
        cin >> YorN;
      }
      if(YorN == "no" || YorN == "n")
      {
        ex(newNum);
        break;
      }
      else if(YorN == "yes" || YorN == "y")
      {
          newNum++; //index
          fptr = fopen("data.ledger", "a");
          sprintf(num, "%d", newNum);
          fputs(num, fptr);
          fputc(',',fptr);
          fputs("-1,", fptr);
          fputs(tmp, fptr);
          fputc('\n', fptr);
          fclose(fptr);
          cout << "What else ? ";
      }
      else if(YorN == "back" || YorN == "b")
        break;
      else if(YorN == "in")
      {
        in(newNum);
        break;
      }
      else if(YorN == "revise")
      {
        revise(-1);
        break;
      }
      else if(YorN == "exit")
        exit(0);
    }
    scanf("%s", tmp);
  }
  if(strcmp(tmp, "revise") == 0)
    revise(-1);
  else if(strcmp(tmp, "in") == 0)
    in(newNum);
  else if(strcmp(tmp, "exit") == 0)
    exit(0);
  return;
}
void revise(int matchedNumber)
{
  string YorN;
  int CasesNumber = 0;
  char number[10];  //number to be revised
  char *piece;
  vector<LINE> LV2cut;
  vector<LINE> LV;
  int tmpIoE;
  int tmp2;
  string choose;
  string YN;
  char *oldyear, *oldmonth, *oldday;
  char *tmpPS = new char[MAX];
  char *newPartOfLine = new char[MAX];
  char *newLine = new char[MAX];
  char line2cut[MAX];
  char *finalLineFront = new char[MAX];
  char *finalLineBack = new char[MAX];
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  FILE *fptr = fopen("data.ledger", "r");
  char *tmp = new char[MAX];
  cout << endl << "No." << endl;
  while(fgets(tmp, MAX, fptr) != NULL) //showing all
  {
    CasesNumber++;
    LINE tmpLINE;
    SET tmpSET;

    strcpy(tmpLINE.data, tmp);
    tmp[strlen(tmp) - 1] = '\0';
    //20XX/YY/ZZ,Amount,Type(number),P.S
    piece = strtok(tmp, ",");
    tmpSET.ID = atoi(piece);
    //printf("ID = %d\n", tmpSET.ID);
    piece = strtok(NULL, ",");
    tmpSET.IoE = atoi(piece);
    //printf("IoE = %d\n", tmpSET.IoE);
    piece = strtok(NULL, "/");
    tmpSET.year = atoi(piece);
    //printf("year = %d\n", tmpSET.year);
    piece = strtok(NULL, "/");
    tmpSET.month = atoi(piece);
    //printf("month = %d\n", tmpSET.month);
    piece = strtok(NULL, ",");
    tmpSET.day = atoi(piece);
    //printf("day = %d\n", tmpSET.day);
    piece = strtok(NULL, ",");
    tmpSET.Amt = atoi(piece);
    //printf("amt = %d\n", tmpSET.Amt);
    piece = strtok(NULL, ",");
    tmpSET.Type = atoi(piece);
    //printf("Type = %d\n", tmpSET.Type);
    piece = strtok(NULL, ",");
    strcpy(tmpSET.PS, piece);
    //printf("tmpSET.PS = %s\n", tmpSET.PS);
    LV.push_back(tmpLINE);
    LV2cut.push_back(tmpLINE);
    if(matchedNumber == -1){
      if(tmpSET.IoE == -1){
        if((tmpSET).month >= 10 && (tmpSET).day >= 10)
          printf("#%-4d |%c| %d/%d/%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type * -1 + 20], tmpSET.PS);
        else if((tmpSET).month < 10 && (tmpSET).day >= 10)
          printf("#%-4d |%c| %d/0%d/%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type * -1 + 20], tmpSET.PS);
        else if((tmpSET).month >= 10 && (tmpSET).day < 10)
          printf("#%-4d |%c| %d/%d/0%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type * -1 + 20], tmpSET.PS);
        else if((tmpSET).month < 10 && (tmpSET).day < 10)
          printf("#%-4d |%c| %d/0%d/0%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type * -1 + 20], tmpSET.PS);
      }
      else{
        if((tmpSET).month >= 10 && (tmpSET).day >= 10)
          printf("#%-4d |%c| %d/%d/%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type + 20], tmpSET.PS);
        else if((tmpSET).month < 10 && (tmpSET).day >= 10)
          printf("#%-4d |%c| %d/0%d/%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type + 20], tmpSET.PS);
        else if((tmpSET).month >= 10 && (tmpSET).day < 10)
          printf("#%-4d |%c| %d/%d/0%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type + 20], tmpSET.PS);
        else if((tmpSET).month < 10 && (tmpSET).day < 10)
          printf("#%-4d |%c| %d/0%d/0%d  $:%-5d  %-12s  P.S. %s\n", tmpSET.ID, PorN[tmpSET.IoE + 1], tmpSET.year, tmpSET.month, tmpSET.day, tmpSET.Amt, type[tmpSET.Type + 20], tmpSET.PS);
      }
    }
  }
  delete [] tmp;
  fclose(fptr);
  if(matchedNumber == -1)
  {
    cout << "Which piece of data needs to be revised?" << endl;
    cout << "#";
    scanf("%s", number);
    while((IsAnumber(number) == 1 && (atoi(number) > CasesNumber || atoi(number) < 1)) || ((IsAnumber(number) == -1 && strcmp(number,"exit") != 0 && strcmp(number, "back") != 0)))
    {
      if(IsAnumber(number) == 1 && (atoi(number) > CasesNumber || atoi(number) < 1))
        cout << "Wrong number!" << endl;
      if(IsAnumber(number) == -1)
        cout << "Please enter a number .." << endl;
      cout << "#";
      scanf("%s", number);
    }
    if(strcmp(number, "exit") == 0)
      exit(0);
    if(strcmp(number, "back") == 0)
    {
      update();
      return;
    }
  }
  else if(matchedNumber != -1)
    sprintf(number, "%d", matchedNumber);
  int back2number, typeChosen, changed;
  typeChosen = 0;  // for "back" functionality in choose == 3
  for(unsigned int i = 0; i < LV2cut.size(); i++)
  {
    changed = 0;
    back2number = 0;
    piece = strtok(LV2cut[i].data, ",");
    if(strcmp(number, piece) == 0) // if之後就會break了
    {
      matchedNumber = atoi(number);
      strcpy(newLine, LV[i].data);
      printf("#%-4s ", piece);
      piece = strtok(NULL, ",");   tmpIoE = atoi(piece);   printf("|%c| ", PorN[tmpIoE + 1]);
      piece = strtok(NULL, "/");   oldyear = piece;   printf("%s/", piece);
      piece = strtok(NULL, "/");   oldmonth = piece;  printf("%s/", piece);
      piece = strtok(NULL, ",");   oldday = piece;    printf("%s  ", piece);
      piece = strtok(NULL, ",");   printf("$:%s  ", piece);
      piece = strtok(NULL, ",");   tmp2 = atoi(piece);
      if(tmpIoE == -1)  printf("%s  ", type[tmp2 * -1 + 20]);
      else  printf("%s  ", type[tmp2 + 20]);
      piece = strtok(NULL, ",");   strcpy(tmpPS, piece);   printf("P.S.  %s", piece);

      cout << "What to be revised in ↑" << endl;
      cout << "---------------------" << endl;
      cout << "|    (1). Date      |" << endl;
      cout << "|    (2). Amount    |" << endl;
      cout << "|    (3). Type      |" << endl;
      cout << "|    (4). P.S.      |" << endl;
      cout << "|    (5). Back      |" << endl;
      cout << "---------------------" << endl;
      cin >> choose;
      while(choose != "1" && choose != "2" && choose != "3" && choose != "4" && choose != "5" && choose != "back" && choose != "exit")
      {
          cout << "Wrong choose!" << endl;
          cin >> choose;
      }
      if(choose == "1") //date 可以不輸入0嗎？
      {
          char check[MAX];
          cout << "YYYY/MM/DD : ";
          scanf("%s", newPartOfLine);
          if(strcmp(newPartOfLine, "exit") == 0)
            exit(0);
          strcpy(check, newPartOfLine);
          for(unsigned int j = 0; j < strlen(check); j++)
            if(check[j] == '/')
              for(unsigned int k = j; k < strlen(check); k++)
                check[k] = check[k+1];
          while(strcmp(newPartOfLine, "back") != 0 && (IsAnumber(check) == -1 || CorrectDay(atoi(check)) == -1))
          {
            scanf("%s",newPartOfLine);
            if(strcmp(newPartOfLine, "exit") == 0)
              exit(0);
            strcpy(check, newPartOfLine);
            for(unsigned int j = 0; j < strlen(check); j++)
              if(check[j] == '/')
                for(unsigned int k = j; k < strlen(check); k++)
                  check[k] = check[k+1];
          }
          if(strcmp(newPartOfLine, "back") == 0)
          {
            revise(matchedNumber);
            return;
          }

          cout << "Sure? ";
          cin >> YN;
          if(YN == "exit")
            exit(0);
          if(YN == "y" || YN == "yes" || YN == "Y")
          {
            strcpy(line2cut, LV[i].data);
            piece = strtok(line2cut, ",");  strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");    //cut date
            piece = strtok(NULL, ",");      strcat(finalLineBack, ",");     strcat(finalLineBack, piece);   strcat(finalLineBack, ",");
            piece = strtok(NULL, ",");      strcat(finalLineBack, piece);   strcat(finalLineBack, ",");
            piece = strtok(NULL, ",");      strcat(finalLineBack, piece);
            strcpy(LV[i].data, finalLineFront);
            strcat(LV[i].data, newPartOfLine);
            strcat(LV[i].data, finalLineBack);
            changed = 1;
            printf("%s/%s/%s -> %s\n", oldyear, oldmonth, oldday, newPartOfLine);
            getchar(); cout << "Press Enter .."; getchar();
          }
          else
            cout << "Bye Byeee" << endl;
      }
      else if(choose == "2") //amount        防呆, back ,exit 已做
      {
          cout << "$: ";
          scanf("%s", newPartOfLine);
          while((strcmp(newPartOfLine, "back") != 0 && strcmp(newPartOfLine, "exit") != 0) && IsAnumber(newPartOfLine) == -1)
          {
            cout << "Please enter a number." << endl;
            scanf("%s", newPartOfLine);
          }
          if(strcmp(newPartOfLine, "back") == 0){
            revise(matchedNumber);
            return;
          }
          if(strcmp(newPartOfLine, "exit") == 0)
            exit(0);
          cout << "Sure? ";
          cin >> YN;
          if(YN == "y" || YN == "yes" || YN == "Y")
          {
            strcpy(line2cut, LV[i].data);
            char tmpAmount[10];
            piece = strtok(line2cut, ",");  strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcpy(tmpAmount, piece);//cut amount
            piece = strtok(NULL, ",");      strcat(finalLineBack, ",");     strcat(finalLineBack, piece);   strcat(finalLineBack, ",");
            piece = strtok(NULL, ",");      strcat(finalLineBack, piece);
            strcpy(LV[i].data, finalLineFront);
            strcat(LV[i].data, newPartOfLine);
            strcat(LV[i].data, finalLineBack);
            changed = 1;
            printf("$:%s -> $:%s\n", tmpAmount, newPartOfLine);
            getchar(); cout << "Press Enter .."; getchar();
          }
          else
            cout << "Bye Byeee" << endl;
      }
      else if(choose == "3")  //type
      {
          typeChosen = 1;
          if(tmpIoE == -1)
          {
            cout << "---------Type-Of-Expenses--------" << endl;
            cout << "|        (1).  Breakfast        |" << endl;
            cout << "|        (2).  Lunch            |" << endl;
            cout << "|        (3).  Dinner           |" << endl;
            cout << "|        (4).  Snack            |" << endl;
            cout << "|        (5).  Shopping         |" << endl;
            cout << "|        (6).  Drink            |" << endl;
            cout << "|        (7).  Transport        |" << endl;
            cout << "|        (8).  Recreation       |" << endl;
            cout << "|        (9).  Others           |" << endl;
            cout << "|        (10). Back             |" << endl;
            cout << "---------------------------------" << endl;
            scanf("%s", newPartOfLine);
            while(strcmp(newPartOfLine, "1") != 0 && strcmp(newPartOfLine, "2") != 0 && strcmp(newPartOfLine, "3") != 0 && strcmp(newPartOfLine, "4") != 0 && strcmp(newPartOfLine, "5") != 0 && strcmp(newPartOfLine, "6") != 0 && strcmp(newPartOfLine, "7") != 0 && strcmp(newPartOfLine, "8") != 0 && strcmp(newPartOfLine, "9") != 0 && strcmp(newPartOfLine, "10") != 0 && strcmp(newPartOfLine, "exit") != 0 && strcmp(newPartOfLine, "back") != 0)
            {
              cout << "Wrong choose!" << endl;
              scanf("%s", newPartOfLine);
            }
            if(strcmp(newPartOfLine, "back") == 0 || strcmp(newPartOfLine, "10") == 0)
            {
              revise(matchedNumber);
              break;
            }
            if(strcmp(newPartOfLine, "exit") == 0)
            {
              revise(matchedNumber);
              break;
            }
            cout << "Sure? ";
            cin >> YN;
            while(YN != "yes" && YN != "y" && YN != "Y")
            {
              cout << "";
            }
            if(YN == "y" || YN == "yes" || YN == "Y")
            {
              strcpy(line2cut, LV[i].data);
              piece = strtok(line2cut, ",");  strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");     //cut type
              piece = strtok(NULL, ",");      strcat(finalLineBack, ",");     strcat(finalLineBack, piece);
              strcpy(LV[i].data, finalLineFront);
              strcat(LV[i].data, newPartOfLine);
              strcat(LV[i].data, finalLineBack);
              changed = 1;
              printf("%s -> %s\n", type[-1 * tmp2 + 20],type[-1 * atoi(newPartOfLine) + 20]);
              getchar(); cout << "Press Enter .."; getchar();
            }
            else
              cout << "Bye Byeee" << endl;

          }
          else // IoE == 1
          {
            cout << "-----------Type--Table-----------" << endl;
            cout << "|          (1). Salary          |" << endl;
            cout << "|          (2). Others          |" << endl;
            cout << "---------------------------------" << endl;
            scanf("%s", newPartOfLine);
            cout << "Sure? ";
            cin >> YN;
            if(YN == "y" || YN == "yes" || YN == "Y")
            {
              strcpy(line2cut, LV[i].data);
              piece = strtok(line2cut, ",");  strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
              piece = strtok(NULL, ",");     //cut type
              piece = strtok(NULL, ",");      strcat(finalLineBack, ",");     strcat(finalLineBack, piece);
              strcpy(LV[i].data, finalLineFront);
              strcat(LV[i].data, newPartOfLine);
              strcat(LV[i].data, finalLineBack);
              changed = 1;
              printf("%s -> %s\n", type[tmp2 + 20],type[atoi(newPartOfLine) + 20]);
              getchar(); cout << "Press Enter .."; getchar();
            }
            else
              cout << "Bye Byeee" << endl;

          }
      }
      else if(choose == "4") //ps
      {
          cout << "New P.S. ";
          scanf("%s", newPartOfLine);
          cout << "Sure? ";
          cin >> YN;
          if(YN == "y" || YN == "yes" || YN == "Y")
          {
            strcpy(line2cut, LV[i].data);
            piece = strtok(line2cut, ",");  strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            piece = strtok(NULL, ",");      strcat(finalLineFront, piece);  strcat(finalLineFront, ",");
            strcpy(LV[i].data, finalLineFront);
            strcat(LV[i].data, newPartOfLine);
            strcat(LV[i].data, "\n");
            changed = 1;
            tmpPS[strlen(tmpPS) - 1] = '\0';
            printf("%s -> %s\n", tmpPS, newPartOfLine);
            getchar(); cout << " Press Enter .."; getchar();

          }
          else
            cout << "Bye Byeee" << endl;
      }
      else if(choose == "5" || choose == "back") //back
      {
        back2number = 1;
        break;
      }
      else if(choose == "exit")
      {
        exit(0);
      }
      break;
    }

  }
  delete [] newPartOfLine;
  delete [] newLine;
  delete [] finalLineFront;
  delete [] finalLineBack;
  delete [] tmpPS;
  if(back2number)
    revise(-1);
  if(changed == 1)
  {
    fptr = fopen("data.ledger", "wb");
    for(vector<LINE>::iterator it = LV.begin(); it != LV.end(); it++)
    {
      fputs((*it).data, fptr);
    }
    fclose(fptr);
  }
}
void UpdateManual()
{
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << "*********************************************************************" << endl;
  cout << "*                                                                   *" << endl;
  cout << "*                             MUST READ                             *" << endl;
  cout << "*                                                                   *" << endl;
  cout << "*   (***) Please enter every piece data by the following form.      *" << endl;
  cout << "*            YYYY/MM/DD,Amount,Type(a number),P.S                   *" << endl;
  cout << "*            '/', ',' and P.S are required.                         *" << endl;
  cout << "*            The numbers of types will be shown in a type-table     *" << endl;
  cout << "*            during every update.                                   *" << endl;
  cout << "*            Example: 2099/12/31,100,1,Nothing2say                  *" << endl;
  cout << "*   (***) in > ex ex > in  no back                                                          *" << endl;
  cout << "*   (***) When Revising Date, '/' is required.                      *" << endl;
  cout << "*                                                                   *" << endl;
  cout << "*                                                                   *" << endl;
  cout << "*********************************************************************" << endl;
  getchar();
  cout << "Press Enter..";
  getchar();
}
void update()
{
  string YorN;
  FILE *fptr = fopen("data.ledger", "r");
  int number = 0;
  char *tmp = new char[MAX];
  while(fgets(tmp, MAX, fptr) != NULL) number++;
  delete [] tmp;
  fclose(fptr);

  string choose;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << "------------------------" << endl;
  cout << "|    (1). Incomes      |" << endl;
  cout << "|    (2). Expenses     |" << endl;
  cout << "|    (3). Revise       |" << endl;
  cout << "|    (4). Manual       |" << endl;
  cout << "|    (5). Back         |" << endl;
  cout << "------------------------" << endl;
  cin >> choose;
  while(choose != "1" && choose != "in" && choose != "i" && choose != "I" && choose != "2" && choose != "e" && choose != "E" && choose != "ex" && choose != "3" && choose != "r" && choose != "R" && choose != "revise" && choose != "4" && choose != "M" && choose != "m" && choose != "manual"
    && choose != "5" && choose != "b" && choose != "back" && choose != "B" && choose != "exit" && choose != "back")
  {
    cout << "Wrong command!" << endl;
    cin >> choose;
  }
  if(choose == "1" || choose == "in" || choose == "i" || choose == "I")          in(number);
  else if(choose == "2" || choose == "ex" || choose == "e" || choose == "E")     ex(number);
  else if(choose == "3" || choose == "revise" || choose =="r" || choose == "R")     revise(-1);
  else if(choose == "4" || choose == "Man" || choose == "m" || choose == "M")     UpdateManual();
  else if(choose == "5" || choose == "back" || choose == "b" || choose == "B")     return;
  else if(choose == "exit")  exit(0);
  return;
}
int leapY(int input)
{
  if((input % 4 == 0 && input % 100 != 0) || (input % 400 == 0))
    return 1;
  else
    return -1;
}
int CorrectDay(int input) // 8 位數必須
{
  if(input < 10000000 || input > 99999999)
  {
    cout << "Wrong form! Please enter again." << endl;
    return -1;
  }
  if((input % 10000) / 100 == 1 || (input % 10000) / 100 == 3 || (input % 10000) / 100 == 5 || (input % 10000) / 100 == 7 || (input % 10000) / 100 == 8 || (input % 10000) / 100 == 10 || (input % 10000) / 100 == 12)
  {
    if(input % 100 < 1 || input % 100 > 31 ){
      cout << "Wrong day! Please enter again." << endl;
      return -1;
    }
    return 1;
  }
  else if((input % 10000) / 100 == 2)
  {
    if(input % 100 < 1){
      cout << "Wrong day! Please enter again." << endl;
      return -1;
    }
    if(leapY(input / 10000) == 1){
      if(input % 100 > 29){
        cout << "Wrong day! Please enter again." << endl;
        return -1;
      }
      else{
        return 1;
      }
    }
    else if(input % 100 > 28){
      cout << "Wrong day! Please enter again." << endl;
      return -1;
    }
    else
      return 1;
  }
  else if((input % 10000) / 100 == 4 || (input % 10000) / 100 == 6 || (input % 10000) / 100 == 9 || (input % 10000) / 100 == 11)
  {
    if(input % 100 < 1 || input % 100 > 30){
      cout << "Wrong day! Please enter again." << endl;
      return -1;
    }
    return 1;
  }
  else{
    cout << "Wrong month! Please enter again." << endl;
    return -1;
  }
}

int month2days(int month, int leap)
{
  int output;
  switch (month){
    case 1:
      output = 0;
      break;
    case 2:
      output = 31;
      break;
    case 3:
      output = 31+28;
      break;
    case 4:
      output = 31+28+31;
      break;
    case 5:
      output = 31+28+31+30;
      break;
    case 6:
      output = 31+28+31+30+31;
      break;
    case 7:
      output = 31+28+31+30+31+30;
      break;
    case 8:
      output = 31+28+31+30+31+30+31;
      break;
    case 9:
      output = 31+28+31+30+31+30+31+31;
      break;
    case 10:
      output = 31+28+31+30+31+30+31+31+30;
      break;
    case 11:
      output = 31+28+31+30+31+30+31+31+30+31;
      break;
    case 12:
      output = 31+28+31+30+31+30+31+31+30+31+30;
      break;
    default:
    {
      output = -1;
      cout << "wrong input : "<< month << endl;
      break;
    }
  }
  if(leap == 1 && month != 1 && month != 2)
    output += 1;
  return output;
}
int year2days(int input)
{
  int output = 0;
  for(int i = 1; i < input; i++)
  {
    if(leapY(input))
      output += 366;
    else
      output += 365;
  }
  return output;
}
int days(int input) //  days 不夠精準 有關大數字days遇到閏年 ex input:20151223
{
  int tmpyear = input / 10000;
  int output;
  int leap = leapY(tmpyear);
  output = year2days(tmpyear) + month2days((input % 10000) / 100, leap) + (input % 100);
  return output;
}
void show()
{
  string YorN;
  int flag = 0; // for cases, wheter thers's data to show or not
  char tmp[MAX]; char *piece;
  int totalmoney = 0; int totalExpense = 0; int totalIncomes = 0; int dailyExpense = 0; int monthExpense = 0; int yearExpense = 0;
  int DATE;
  int year, month;
  string choice;
  char showmanydays[5];
  int howmanydays;
  char monthTable[13][20] = {"Wrong", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  vector<SET> stomachV, desireV, funV, transV, otherV , allExV, allInV, allV;

  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << "-------------------------------" << endl;
  cout << "|  (1).  Yearly     Expenses  |" << endl;
  cout << "|  (2).  Monthly    Expenses  |" << endl;
  cout << "|  (3).  Daily      Expenses  |" << endl;
  cout << "|  (4).  Day-ago    Expenses  |" << endl;
  cout << "|  (5).  Stomach    Expenses  |" << endl;
  cout << "|  (6).  Desire     Expenses  |" << endl;
  cout << "|  (7).  Fun        Expenses  |" << endl;
  cout << "|  (8).  Transport  Expenses  |" << endl;
  cout << "|  (9).  Other      Expenses  |" << endl;
  cout << "|  (10). Total      Expenses  |" << endl;
  cout << "|  (11). Total      Incomes   |" << endl;
  cout << "|  (12). Total      Balance   |" << endl;
  cout << "|  (13). Back                 |" << endl;
  cout << "-------------------------------" << endl;
  cin >> choice;
  if(choice == "exit")
    exit(0);
  if(choice == "back")
    return;
  FILE *fptr = fopen("data.ledger", "r");
  while(fgets(tmp, MAX, fptr) != NULL) // saving -ly expenses and total
  {
    SET tmpSET;
    tmp[strlen(tmp) - 1] = '\0';
    //20XX/YY/ZZ,Amount,Type(number),P.S
    piece = strtok(tmp, ",");
    tmpSET.ID = atoi(piece);
    //printf("ID = %d\n", tmpSET.ID);
    piece = strtok(NULL, ",");
    tmpSET.IoE = atoi(piece);
    //printf("IoE = %d\n", tmpSET.IoE);
    piece = strtok(NULL, "/");
    tmpSET.year = atoi(piece);
    //printf("year = %d\n", tmpSET.year);
    piece = strtok(NULL, "/");
    tmpSET.month = atoi(piece);
    //printf("month = %d\n", tmpSET.month);
    piece = strtok(NULL, ",");
    tmpSET.day = atoi(piece);
    //printf("day = %d\n", tmpSET.day);
    piece = strtok(NULL, ",");
    tmpSET.Amt = atoi(piece);
    //printf("amt = %d\n", tmpSET.Amt);
    piece = strtok(NULL, ",");
    tmpSET.Type = atoi(piece);
    //printf("Type = %d\n", tmpSET.Type);
    piece = strtok(NULL, ",");
    strcpy(tmpSET.PS, piece);
    //printf("tmpSET.PS = %s\n", tmpSET.PS);
    DATE = tmpSET.year * 10000 + tmpSET.month * 100 + tmpSET.day;
    //cout << tmpSET.DATE << endl;
    if(TODAY - DATE == 0)
      if(tmpSET.IoE == -1)
        dailyExpense += tmpSET.Amt;
    if((TODAY / 100 - DATE / 100) == 0)
      if(tmpSET.IoE == -1)
        monthExpense += tmpSET.Amt;
    if(TODAY / 10000 - DATE / 10000 == 0)
      if(tmpSET.IoE == -1)
        yearExpense += tmpSET.Amt;
    if(tmpSET.IoE == -1 && (tmpSET.Type == 1 || tmpSET.Type == 2 || tmpSET.Type == 3 || tmpSET.Type == 4 || tmpSET.Type == 6))
      stomachV.push_back(tmpSET);
    if(tmpSET.IoE == -1 && tmpSET.Type == 5)
      desireV.push_back(tmpSET);
    if(tmpSET.IoE == -1 && tmpSET.Type == 8)
      funV.push_back(tmpSET);
    if(tmpSET.IoE == -1 && tmpSET.Type == 7)
      transV.push_back(tmpSET);
    if(tmpSET.IoE == -1 && tmpSET.Type == 9)
      otherV.push_back(tmpSET);
    if(tmpSET.IoE == -1){
      totalExpense += tmpSET.Amt;
      allExV.push_back(tmpSET);
    }
    else if(tmpSET.IoE == 1){
      totalIncomes += tmpSET.Amt;
      allInV.push_back(tmpSET);
    }
    allV.push_back(tmpSET);
    totalmoney += (tmpSET.Amt * tmpSET.IoE);
  }
  fclose(fptr);
  //cout << yearMap[20151211].Amt<<endl;
  while(choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" && choice != "6" && choice != "7" && choice != "8" && choice != "9" && choice != "10" && choice != "11" && choice != "12" && choice != "13" && choice != "exit" && choice != "back"){
      cout << "Wrong command." << endl;
      cin >> choice;
  }
  if(choice == "1") //yearly
  {
    year = TODAY / 10000;
    cout << "In " << year <<", ";
    cout << "you've spent " << yearExpense << " dollars." << endl;
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "2") //monthly
  {
    month = (TODAY % 10000) / 100;
    cout << "In this month (" << monthTable[month] <<"), ";
    cout << "you've spent " << monthExpense << " dollars." << endl;
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "3") //daily
  {
    cout << "Today, you've spent ";
    cout << dailyExpense << " dollars." << endl;
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "4") //dayago
  {
    cout << "How many days ago?" << endl;
    scanf("%s",showmanydays);
    if(strcmp(showmanydays, "exit") == 0)
      exit(0);
    if(strcmp(showmanydays, "back") == 0)
    {
      preshow();
      return;
    }
    while(IsAnumber(showmanydays) != 1  || atoi(showmanydays) <= 0)
    {
      cout << "I dont't think it's a correct number .." << endl;
      if (atoi(showmanydays) <= 0)
        cout << "How many days ago (at least 1) before today?" << endl;
      scanf("%s",showmanydays);
    }
    howmanydays = atoi(showmanydays);
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
    cout << "In the past " << howmanydays << " day(s),";
    cout << " you've spent on the orders below" << endl;
    FILE *fptr = fopen("data.ledger", "r");
    int newTODAY, newDATE;
    int daysagomoney = 0;
    vector<SET> outputV;
    newTODAY = days(TODAY);
    while(fgets(tmp, MAX, fptr) != NULL)
    {
      SET tmpSET;
      tmp[strlen(tmp) - 1] = '\0';
      //20XX/YY/ZZ,Amount,Type(number),P.S
      piece = strtok(tmp, ",");
      tmpSET.ID = atoi(piece);
      //printf("ID = %d\n", tmpSET.ID);
      piece = strtok(NULL, ",");
      tmpSET.IoE = atoi(piece);
      //printf("IoE = %d\n", tmpSET.IoE);
      piece = strtok(NULL, "/");
      tmpSET.year = atoi(piece);
      //printf("year = %d\n", tmpSET.year);
      piece = strtok(NULL, "/");
      tmpSET.month = atoi(piece);
      //printf("month = %d\n", tmpSET.month);
      piece = strtok(NULL, ",");
      tmpSET.day = atoi(piece);
      //printf("day = %d\n", tmpSET.day);
      piece = strtok(NULL, ",");
      tmpSET.Amt = atoi(piece);
      //printf("amt = %d\n", tmpSET.Amt);
      piece = strtok(NULL, ",");
      tmpSET.Type = atoi(piece) * tmpSET.IoE;
      //printf("Type = %d\n", tmpSET.Type);
      piece = strtok(NULL, ",");
      strcpy(tmpSET.PS, piece);
      //printf("tmpSET.PS = %s\n", tmpSET.PS);
      DATE = tmpSET.year * 10000 + tmpSET.month * 100 + tmpSET.day;
      //cout << tmpSET.DATE << endl;
      newDATE = days(DATE);
      if(tmpSET.IoE == -1 && TODAY / 10000 - tmpSET.year >= 0 && (newTODAY >= newDATE) && (newTODAY - howmanydays) < newDATE)
      {
        daysagomoney += tmpSET.Amt;
        outputV.push_back(tmpSET);
      }
    }
    fclose(fptr);
    for(vector<SET>::iterator it = outputV.begin(); it != outputV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
    }
    cout << "---------Totally you've spent "<< daysagomoney <<" dollars in the past " << howmanydays << " days---------" << endl;
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "5") //stomach
  {
    string choose;
    int flag = 0;
    cout << "--------------------" << endl;
    cout << "|  (1). Breakfast  |" << endl;
    cout << "|  (2). Lunch      |" << endl;
    cout << "|  (3). Dinner     |" << endl;
    cout << "|  (4). Snack      |" << endl;
    cout << "|  (5). Drink      |" << endl;
    cout << "|  (6). All        |" << endl;
    cout << "|  (7). Back       |" << endl;
    cout << "--------------------" << endl;
    cin >> choose;
      while(choose != "1" && choose != "2" && choose != "3" && choose != "4" && choose != "5" && choose != "6" && choose != "7" && choose != "exit" && choose != "back")
      {
          cout << " Wrong command!" << endl;
          cin >> choose ;
      }
      if(choose == "1")
      {
        for(vector<SET>::iterator it = stomachV.begin(); it != stomachV.end(); it++)
        {
          if((*it).Type == 1){
            if((*it).month >= 10 && (*it).day >= 10)
              printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day >= 10)
              printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month >= 10 && (*it).day < 10)
              printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day < 10)
              printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            flag = 1;
          }
        }
        if(flag != 1)
          printf("Nothing to be shown\n");

      }
      else if(choose == "2")
      {
        for(vector<SET>::iterator it = stomachV.begin(); it != stomachV.end(); it++)
        {
          if((*it).Type == 2){
            if((*it).month >= 10 && (*it).day >= 10)
              printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day >= 10)
              printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month >= 10 && (*it).day < 10)
              printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day < 10)
              printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            flag = 1;
          }
        }
        if(flag != 1)
          printf("Nothing to be shown\n");

      }
      else if(choose == "3")
      {
        for(vector<SET>::iterator it = stomachV.begin(); it != stomachV.end(); it++)
        {
          if((*it).Type == 3){
            if((*it).month >= 10 && (*it).day >= 10)
              printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day >= 10)
              printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month >= 10 && (*it).day < 10)
              printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day < 10)
              printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            flag = 1;
          }
        }
        if(flag != 1)
          printf("Nothing to be shown\n");

      }
      else if(choose == "4")
      {
        for(vector<SET>::iterator it = stomachV.begin(); it != stomachV.end(); it++)
        {
          if((*it).Type == 4){
            if((*it).month >= 10 && (*it).day >= 10)
              printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day >= 10)
              printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month >= 10 && (*it).day < 10)
              printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day < 10)
              printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            flag = 1;
          }
        }
        if(flag != 1)
          printf("Nothing to be shown\n");

      }
      else if(choose == "5")
      {
        for(vector<SET>::iterator it = stomachV.begin(); it != stomachV.end(); it++)
        {
          if((*it).Type == 6){
            if((*it).month >= 10 && (*it).day >= 10)
              printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day >= 10)
              printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month >= 10 && (*it).day < 10)
              printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            else if((*it).month < 10 && (*it).day < 10)
              printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
            flag = 1;
          }
        }
        if(flag != 1)
          printf("Nothing to be shown\n");

      }
      else if(choose == "6")
      {
        for(vector<SET>::iterator it = stomachV.begin(); it != stomachV.end(); it++)
        {
          if((*it).month >= 10 && (*it).day >= 10)
            printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
          else if((*it).month < 10 && (*it).day >= 10)
            printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
          else if((*it).month >= 10 && (*it).day < 10)
            printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
          else if((*it).month < 10 && (*it).day < 10)
            printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
          flag = 1;
        }
        if(flag != 1)
          printf("Nothing to be shown\n");
      }
      else if(choose == "7" || choose == "back")
      {
        show();
        return;
      }
      else if(choose == "exit")
        exit(0);

    if(choose == "1" || choose == "2" || choose == "3" || choose == "4" || choose == "5" || choose == "6")
    {
      cout << "Press Enter ..";
      getchar();
      getchar();
    }
    show();
  }
  else if(choice == "6") // desire
  {
    for(vector<SET>::iterator it = desireV.begin(); it != desireV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      flag = 1;
    }
    if(flag != 1)
      printf("Nothing to be shown\n");
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "7") // fun
  {
    for(vector<SET>::iterator it = funV.begin(); it != funV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      flag = 1;
  }
  if(flag != 1)
    printf("Nothing to be shown\n");
  cout << "Press Enter ..";
  getchar();
  getchar();
  show();
  }
  else if(choice == "8") // tran
  {
    for(vector<SET>::iterator it = transV.begin(); it != transV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      flag = 1;
    }
    if(flag != 1)
      printf("Nothing to be shown\n");
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "9") // other
  {
    for(vector<SET>::iterator it = otherV.begin(); it != otherV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      flag = 1;
    }
    if(flag != 1)
      printf("Nothing to be shown\n");
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "10") // total expense
  {
    for(vector<SET>::iterator it = allExV.begin(); it != allExV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[-1 * (*it).Type + 20], (*it).PS);
    }
    cout << "___________________________________________________________" << endl;
    cout << "You've burn $ " << totalExpense << " in history" << endl;
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "11") // total incomes
  {
    for(vector<SET>::iterator it = allInV.begin(); it != allInV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",(*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
    }
    cout << "___________________________________________________________" << endl;
    cout << "You should've possesed $ " << totalIncomes << " in history" << endl;
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "12") // balance
  {
    for(vector<SET>::iterator it = allV.begin(); it != allV.end(); it++)
    {
      if((*it).month >= 10 && (*it).day >= 10)
        printf("|%c| Date: %d/%d/%d $: %-5d, %-12s, P.S. %s\n",PorN[(*it).IoE + 1], (*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day >= 10)
        printf("|%c| Date: %d/0%d/%d $: %-5d, %-12s, P.S. %s\n",PorN[(*it).IoE + 1], (*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month >= 10 && (*it).day < 10)
        printf("|%c| Date: %d/%d/0%d $: %-5d, %-12s, P.S. %s\n",PorN[(*it).IoE + 1], (*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
      else if((*it).month < 10 && (*it).day < 10)
        printf("|%c| Date: %d/0%d/0%d $: %-5d, %-12s, P.S. %s\n",PorN[(*it).IoE + 1], (*it).year,(*it).month,(*it).day, (*it).Amt, type[(*it).Type + 20], (*it).PS);
    }
    cout << "___________________________________________________________" << endl;
    cout << "$ " << totalmoney << endl;
    cout << "Press Enter ..";
    getchar();
    getchar();
    show();
  }
  else if(choice == "13" || choice == "back") // back
    return;
  else if(choice == "exit")
    exit(0);

  return;
}
void preshow()
{
  string YorN;
  tm *today;
	time_t t;
	t=time(0);
	today=localtime(&t);
  TODAY = ((today->tm_year+1900) * 10000 + (today ->tm_mon+1) * 100 + today->tm_mday);
  show();
  return;
}
void readme()
{
  cout << "***************************************************************" << endl;
  cout << "*                                                             *" << endl;
  cout << "*                           READ ME                           *" << endl;
  cout << "*                                                             *" << endl;
  cout << "*   1. This is a ledger with two main functions.              *" << endl;
  cout << "*        In (U)pdate, you could update the ledger.            *" << endl;
  cout << "*        In (S)how, the ledger shows where your money went.   *" << endl;
  cout << "*                                                             *" << endl;
  cout << "*   2. Answering yes no question, you could type              *" << endl;
  cout << "*        'y', 'Y', 'yes'  for a yes.                          *" << endl;
  cout << "*        'n', 'N', 'no' for a no.                             *" << endl;
  cout << "*                                                             *" << endl;
  cout << "*   3. Type 'exit' anywhere to exit the program.              *" << endl;
  cout << "*                                                             *" << endl;
  cout << "*   4. If you don't know what to do, maybe you should type    *" << endl;
  cout << "*         back                                                *" << endl;
  cout << "*                                                             *" << endl;
  cout << "*   5. If it's buggy, I'm sorry. Please let me know,          *" << endl;
  cout << "*         b03902014@ntu.edu.tw                                *" << endl;
  cout << "*                                                             *" << endl;
  cout << "*                                                Morris Chang.*" << endl;
  cout << "***************************************************************" << endl;
  getchar(); cout << "Press enter to continue ..";  getchar();
}
void orderme()
{
  string YorN;
  string order;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
  cout << "----------------------" << endl;
  cout << "|      (W)hoAmI      |" << endl;
  cout << "|      (U)pdate      |" << endl;
  cout << "|      (S)how        |" << endl;
  cout << "|      (R)eadme      |" << endl;
  cout << "|      (E)xit        |" << endl;
  cout << "----------------------" << endl;
  cout << "Come on! "<< NAME << ", Order me!" << endl;

  while(cin >> order)
  {


    if(order != "whoami" && order != "W" && order != "w" && order != "update" && order != "U" && order != "u" && order != "exit" && order != "E" && order != "e" && order != "show" && order != "S" && order != "s" && order != "readme" && order != "R" && order != "r"){
      cout << "Wrong command." << endl;
      cin >> order;
      while(order != "whoami" && order != "W" && order != "w" && order != "update" && order != "U" && order != "u" && order != "exit" && order != "E" && order != "e" && order != "show" && order != "S" && order != "s" && order != "readme" && order != "R" && order != "r")
      {
        cout << "Wrong command." << endl;
        cin >> order;
      }
    }

    if(order == "whoami" || order == "w" || order == "W"){
      whoami();
    }
    if(order == "update" || order == "u" || order == "U"){
      update();
    }
    if(order == "show" || order == "S" || order == "s"){
      preshow();
    }
    if(order == "readme" || order == "r" || order == "R")
    {
      readme();
    }
    if(order == "exit" || order == "e" || order =="E"){
      break;
    }

    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl ;
    cout << "----------------------" << endl;
    cout << "|      (W)hoAmI      |" << endl;
    cout << "|      (U)pdate      |" << endl;
    cout << "|      (S)how        |" << endl;
    cout << "|      (R)eadme      |" << endl;
    cout << "|      (E)xit        |" << endl;
    cout << "----------------------" << endl;
    cout << "Come on! "<< NAME << ", Order me!" << endl;


  }
}
void defineTypeChart()
{
  strcpy(type[Salary + 20], "Salary");          strcpy(type[other + 20], "Others");
  strcpy(type[breakfast + 20], "Breakfast");    strcpy(type[lunch + 20], "Lunch");
  strcpy(type[dinner + 20], "Dinner");          strcpy(type[snack + 20], "Snack");
  strcpy(type[shopping + 20], "Shopping");      strcpy(type[drink + 20], "Drink");
  strcpy(type[TRANSPORT + 20], "Transport");      strcpy(type[recreation + 20], "Recreation");
  strcpy(type[others + 20], "Others");
}
int main(void)
{
  FILE *fptr = fopen("data.ledger", "r");
  if(fptr == NULL)
  {
    fclose(fptr);
    fptr = fopen("data.ledger", "w");
    cout << "hi";
  }
  fclose(fptr);
  defineTypeChart();
  YearMonthDayis();
  orderme();
  return 0;
}
