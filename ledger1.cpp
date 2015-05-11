#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <string>
#include <map>
using namespace std;
#define MAX 2000

/*1.uppercase */
typedef struct set{
    int IoE;    /* 1 == income ; -1 == expense */
    int Amt;
    int year, month, day;
    int Type;     /* (1)income : 1.salary, 2.other
                     (2)expense : 1.breakfast, 2.lunch, 3.dinner, 4.snack, 5.shopping, 6.drink, 7.yoyocard, 8, recreation, 9.others*/
    int amount;
    char PS[200];
} SET;
#define salary 1
#define other 2
#define breakfast -1
#define lunch -2
#define  dinner -3
#define snack -4
#define shopping -5
#define drink -6
#define yoyocard -7
#define recreation -8
#define others -9
vector<SET> S;
string NAME;
void whoami()
{
}
void in(int index)
{
    /*  cout << "輸入個日期吧！" << endl; */
    /*  日期打一次，in, ex 打一次 */
    FILE *fptr;
    fptr = fopen("data.ledger", "a");

    int newNum = index;
    char tmp[MAX];
    string YorN;
    cout << endl << "Please enter by the following form. (comma included)" << endl;
    cout << "20XX/YY/ZZ,Amount,Type(number),P.S" << endl;
    cout << "-------Type-------" << endl;
    cout << "|    1.salary    |" << endl;
    cout << "|    2.others    |" << endl;
    cout << "-------Type-------" << endl;
    scanf("%s", tmp);
    cout << "Please make sure again" <<endl;
    printf("%s\n", tmp);
    cout << "yes or no?" << endl;
    cin >> YorN;
    if(YorN == "no")
    {
      cout << endl << "you asshole" << endl;
      while(YorN != "yes")
      {
        cout << "form" << endl;
        cout << "KEY,amount" << endl;
        scanf("%s", tmp);
        cout << "Please make sure again" <<endl;
        printf("%s\n", tmp);
        cout << "yes or no?" << endl;
        cin >> YorN;
      }

    }

    if(YorN == "yes")
    {
      newNum++; //index
      printf("tmp = %s\n", tmp);
      fputs("1,", fptr);
      fputs(tmp, fptr);
      fputc('\n', fptr);
    }
      //cout << endl << "get out" << endl;
    fclose(fptr);
}
void ex(int index)
{
  FILE *fptr;
  fptr = fopen("data.ledger", "a");
  int newNum = index;
  char tmp[MAX];
  string YorN;
  cout << endl << "Please enter by the following form. (comma included)" << endl;
  cout << "20XX/YY/ZZ,Amount,Type(number),P.S" << endl;
  cout << "-------Type-------" << endl;
  cout << "|   1.breakfast  |" << endl;
  cout << "|   2.lunch      |" << endl;
  cout << "|   3.dinner     |" << endl;
  cout << "|   4.snack      |" << endl;
  cout << "|   5.shopping   |" << endl;
  cout << "|   6.drink      |" << endl;
  cout << "|   7.yoyocard   |" << endl;
  cout << "|   8.recreation |" << endl;
  cout << "|   9.others     |" << endl;
  cout << "-------Type-------" << endl;
  scanf("%s", tmp);
  cout << "Please make sure again" <<endl;
  printf("%s\n", tmp);
  cout << "yes or no?" << endl;
  cin >> YorN;
  if(YorN == "no")
  {
    cout << endl << "you asshole" << endl;
    while(YorN != "yes")
    {
      cout << "form" << endl;
      cout << "KEY,amount" << endl;
      scanf("%s", tmp);
      cout << "Please make sure again" <<endl;
      printf("%s\n", tmp);
      cout << "yes or no?" << endl;
      cin >> YorN;
    }

  }

  if(YorN == "yes")
  {
    printf("tmp = %s\n", tmp);
    fputs("-1,", fptr);
    fputs(tmp, fptr);
    fputc('\n', fptr);
  }
  fclose(fptr);
}
void update()
{
  FILE *fptr = fopen("data.ledger", "r");
  char number[30];
  char newN[30];
  int numberInt;
//  int newNum;
  fgets(number, 30, fptr);
  numberInt = atoi(number);
  //newNum = number;
  printf("cases = %d\n", numberInt);
  fclose(fptr);
//  FILE *fptr;
//  fptr = fopen("data.ledger", "a");
   cout << "in or ex?" << endl;
//  char tmp[MAX];
  string inorex;
  cin >> inorex;

  if(inorex == "in")
  {
    in(numberInt);
  }


  if(inorex == "ex")
  {
    ex(numberInt);
  }


//  fclose(fptr);
  cout << "-------Update Finished-------" << endl << endl;
  return;
}
void show()
{
  char tmp[MAX];
  char *piece;
  int totalmoney = 0;
  int monthExpense = 0;
  int DATE;
  int TODAY;
  int CasesNumber;
  string YorN;
  map<int, SET> yearMap;
  FILE *fptr = fopen("data.ledger", "r");
  cout << "Enter date, ex:20150412" << endl;
  cin >> TODAY;
  cout << TODAY << " ?" << endl;
  cin >> YorN;
  if(YorN == "no")
  {
    while(YorN != "yes")
    {
      cout << "Enter date, ex:20150412" << endl;
      cin >> TODAY;
      cout << TODAY << " ?" << endl;
      cin >> YorN;
    }
  }
  cout << "TODAY is " << TODAY << endl;

  fgets(tmp, MAX, fptr);
  CasesNumber = atoi(tmp);
  cout << "CasesNumber = " << CasesNumber << endl;
//show you what???????????????
//1.show money
  while(fgets(tmp, MAX, fptr) != NULL)
  {
    SET tmpSET;
    tmp[strlen(tmp) - 1] = '\0';
    //20XX/YY/ZZ,Amount,Type(number),P.S
    piece = strtok(tmp, ",");
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

    if((TODAY / 100 - DATE / 100) == 1 || (TODAY / 100 - DATE / 100) == 89)
      if(tmpSET.IoE == -1)
        monthExpense -= tmpSET.Amt;
    totalmoney += (tmpSET.Amt * tmpSET.IoE);
    yearMap.insert(pair<int, SET> (DATE, tmpSET));
  }
  //cout << yearMap[20151211].Amt<<endl;
  cout << "monthlyExpense = " << monthExpense << endl;
  cout << "money = " << totalmoney << endl;

  /*月結餘
    各項花費 */
  cout << "此月結餘多少" << endl;
  fclose(fptr);
}
void orderme()
{
  string order;
  cout << "-------Orders-------" << endl;
  cout << "|      whoami      |" << endl;
  cout << "|      update      |" << endl;
  cout << "|       show       |" << endl;
  cout << "|       exit       |" << endl;
  cout << "-------Orders-------" << endl;
  cout << "Come on! "<< NAME << ", Order me!" << endl;

  while(cin >> order)
  {


    if(order != "whoami" && order != "update" && order != "exit" && order != "show"){
      cout << "sorry, I don't know how to " << order << " ." << endl;
      cin >> order;
      while(order != "whoami" && order != "update" && order != "exit" && order != "show")
      {
        cout << "sorry, I don't know how to " << order << "." << endl;
        cin >> order;

      }
    }

    if(order == "whoami"){
      whoami();
    }
    if(order == "update"){
      update();
    }
    if(order == "show"){
      show();
    }

    if(order == "exit"){
      break;
    }

    cout << "-------Orders-------" << endl;
    cout << "|      whoami      |" << endl;
    cout << "|      update      |" << endl;
    cout << "|       show       |" << endl;
    cout << "|       exit       |" << endl;
    cout << "-------Orders-------" << endl;
    cout << "Come on! "<< NAME << ", Order me!" << endl;


  }
}
int main(void)
{
  FILE *fptr = fopen("data.ledger", "a+");
  cout <<"Please enter your name:"<<endl;
  cin >> NAME;


  orderme();
  fclose(fptr);
  return 0;
}
