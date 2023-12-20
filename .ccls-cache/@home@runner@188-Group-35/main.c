#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*=================================================================================*/
void separate(){ 
  printf("\n\33[1;32m=-=-=-=-=--=-=-=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=\033[0m\n");
}

void colour(char *text, char *colourcode){ 
  printf("%s%s%s", colourcode, text, "\033[0m");
}
/*=================================================================================*/
double DataExtraction(char File[500][100][50], int row, int column) {
    return atof(File[row][column]);
}

double DiabetesAverage(char File[500][100][50], int rows, int Row_Start, int Row_End) {
    double sum = 0.0;
    int count = 0;

    for (int i = Row_Start; i < Row_End; i++) {
        double num = DataExtraction(File, i, 13);
        if (num != 0.0) {
            sum += num;
            count++;
        }
    }

    return (count > 0) ? (sum / count) : 0.0;
}
/*=================================================================================*/
int HDR(double Average_Array[], int Array_Size) {
    int highPointer = -1;
    double highVal = Average_Array[1];
    int i = 0;

    while (Array_Size > i) {
        if (highVal < Average_Array[i] ) {
            highPointer = i;
            highVal = Average_Array[i];
        }
        i++;
    }

    return highPointer;
}
/*=================================================================================*/
int LDR(double Average_Array[], int Array_Size) {
    int lowPointer = 1;
    double lowVal = Average_Array[1];
    int i = 1;

    while (Array_Size > i) {
        if (lowVal > Average_Array[i]) {
            lowPointer = i;
            lowVal = Average_Array[i];
        }
        i++;
    }

    return lowPointer;
}
/*=================================================================================*/
int main(){
  
  char TotLines[500][1024]; 
  char File[500][100][50]; 
  char year[7][7] = {"2015","2016","2017","2018","2019","2020","2021"}; 
  char ageStr[3][30] = {"35 to 49 years","50 to 64 years","65 years and over"}; 

  char *title = "CPS188 Term Project | Dr. Mustafiz | Group 35"; //Output formatting
  colour(title, "\033[31m");
  separate();
/*=================================================================================*/
    int rows=0; 
    FILE *in = fopen("statscan_diabetes.csv", "r"); 
    
    for(int i=0;211>i;i++) 
    { 
        fgets(TotLines[i],1024,in); 
        int LineLength = strlen(TotLines[i]); 
        int strStart[50]; 
        int countStr=0; 
        for(int j =0;LineLength>j;j++) 
        { 
            if(i !=0) 
            { 
                if(j==0&&TotLines[i][j]=='"') 
                { 
                    strStart[countStr]=1; 
                    countStr++; 
                }
                else if(j!=0&&TotLines[i][j]==',') 
                { 
                    strStart[countStr]=j+2; 
                    countStr++; 
                } 
            } 
        } 
        for(int w = 0;countStr>w;w++) 
        { 
            int difference=0; 
            if(0==w) 
            { 
                difference = strStart[1]-strStart[0]-3; 
            } 
            else if(17>w) 
            { 
                difference = strStart[w+1]-strStart[w]-3; 
            } 
            else 
            { 
                difference = LineLength-2-strStart[w]; 
            } 
            strncpy(File[i][w],&TotLines[i][strStart[w]],difference); 
        } 
        rows++; 
    }
/*=================================================================================*/
  int GenderRow_Start[5][3][2];
  int ProvCount = 0, GenderCount = 0, GenderNext = 0;
  
  for(int a = 0;rows>a;a++) { 
    if(strcmp(File[a][4],"Males")==0 && GenderNext%2==0) { 
        GenderRow_Start[ProvCount][GenderCount][0] = a; 
        GenderNext++; 
    } else if(strcmp(File[a][4],"Females")==0 && GenderNext%2==1) {
        GenderRow_Start[ProvCount][GenderCount][1] = a; 
        GenderNext++; 
        GenderCount++; 
    }
    
    if(GenderNext == 2) {
        GenderNext = 0;
    }
    
    if(GenderCount == 3) { 
        GenderCount = 0; 
        ProvCount++; 
    } 
  }
/*=================================================================================*/
  //1A
  char *q1a = "\nQuestion 1 - Part a)\n\n";
  colour(q1a, "\033[35m");
  
  printf("\033[4mProvince: \t\t\t Percent:\033[0m\n");
  
  double totalDiabetesAvg[5];
  for(int p = 1; p <= 4; p++) {
    int Row_Start = GenderRow_Start[p][0][0];
    int Row_End = (p < 4) ? GenderRow_Start[p+1][0][0] : rows;
    totalDiabetesAvg[p] = DiabetesAverage(File, rows, Row_Start, Row_End);
    printf("%s \t\t\t %6.2lf%%\n", File[Row_Start][1], totalDiabetesAvg[p]);
  }
  separate();
/*=================================================================================*/
  //1B
  char *q1b = "\nQuestion 1 - Part b)\n\n";
  colour(q1b, "\033[35m");
    
  for(int p=0; p<1; p++){
    double diabetesAvg = DiabetesAverage(File, rows, GenderRow_Start[p][0][0], GenderRow_Start[p+1][0][0]);
    printf("The national diabetes average of %s is: %.2lf%%\n", File[GenderRow_Start[p][0][0]][1], diabetesAvg);
  }
  separate();
/*=================================================================================*/
  //1C 

  char *q1c = "\nQuestion 1 - Part c)\n";
  colour(q1c, "\033[35m");
  
  double yearAvgVal[5][7] = {0};
  double sum[7] = {0};
  double Average[7] = {0};

  for(int e = 0; e < 5; e++) {
    printf("\n%s:\n", File[GenderRow_Start[e][0][0]][1]);
    printf("Year:\t\t\tPercent:\n------\t\t\t--------\n");
  
    for(int yearIndex = 0; yearIndex < 7; yearIndex++) {
      sum[yearIndex] = 0;
      Average[yearIndex] = 0;
  
      for(int rowIndex = GenderRow_Start[e][0][0]; rowIndex < GenderRow_Start[e][0][0] + 42; rowIndex++) {
        if(strcmp(File[rowIndex][0], year[yearIndex]) == 0) {
          sum[yearIndex] += atof(File[rowIndex][13]);
        }
      }
  
      Average[yearIndex] = sum[yearIndex] / 6;
      printf(" %s \t\t\t %.2lf%%\n", year[yearIndex], Average[yearIndex]);
      yearAvgVal[e][yearIndex] = Average[yearIndex];
    }
  }
  separate();
/*=================================================================================*/
  //1D 
  char *q1d = "\nQuestion 1 - Part d)\n";
  colour(q1d, "\033[35m");
  
  double ageDiabetesAvg[5][3];
  
  int prov = 0;
  while (prov < 5) {
      printf("\n%s:\n", File[GenderRow_Start[prov][0][0]][1]);
      printf("\tAge:\t\t\t\tPercent:\n--------------\t\t\t---------\n");
      int age = 0;
      while (age < 3) {
          int r = GenderRow_Start[prov][age][0];
          int rEnd = r + 15;
          ageDiabetesAvg[prov][age] += DiabetesAverage(File, rows, r, rEnd);
          printf("%s's\t\t%.2lf%%\n", ageStr[age], ageDiabetesAvg[prov][age]);
          age++;
      }
      prov++;
  }

  separate();
/*=================================================================================*/
//2 
  char *q2 = "\nQuestion 2)\n\n";
  colour(q2, "\033[34m");
  
  int totalHighestDiabetes = HDR(totalDiabetesAvg, 5);
  int totalLowestDiabetes = LDR(totalDiabetesAvg, 5);
  
  const char* highestDiabetesProvince = File[GenderRow_Start[totalHighestDiabetes][0][0]][1];
  double HDR  = totalDiabetesAvg[totalHighestDiabetes];
  
  const char* lowestDiabetesProvince = File[GenderRow_Start[totalLowestDiabetes][0][0]][1];
  double LDR = totalDiabetesAvg[totalLowestDiabetes];
  
  printf("Highest percentage of Diabetes: %s at %.2lf%%\n", highestDiabetesProvince, HDR);
  
  printf("Lowest percentage of Diabetes: %s at %.2lf%%\n", lowestDiabetesProvince, LDR);

  separate();
/*=================================================================================*/
//3 - Answers hardcoded into output

  char *q3 = "\nQuestion 3)\n\n";
  colour(q3, "\033[33m");

  int higherCountryPtr[4],lowerCountryPtr[4]; 
  int higherCnt=0,lowerCnt = 0; 
  
  for(int prov = 1;prov<5;prov++) { 
    if(totalDiabetesAvg[0]>totalDiabetesAvg[prov]) { 
      lowerCountryPtr[lowerCnt]=prov; 
      lowerCnt++; 
    } 
    else if(totalDiabetesAvg[0]<totalDiabetesAvg[prov]) { 
      higherCountryPtr[higherCnt]=prov; 
      higherCnt++; 
    } 
  }
  printf("Provinces above the National Average:\n"); 
  for(int i=0;i<higherCnt;i++) { 
  printf("%s at %.2lf%%\n",File[GenderRow_Start[higherCountryPtr[i]][0][0]][1],totalDiabetesAvg[higherCountryPtr[i]]); 
  } 
  printf("- Ontario");
  printf("\n\nProvinces below the National Average:\n"); 
  for(int i=0;i<lowerCnt;i++) { 
    printf("%s at %.2lf%%\n",File[GenderRow_Start[lowerCountryPtr[i]][0][0]][1],totalDiabetesAvg[lowerCountryPtr [i]]); 
  } 
  printf("- Quebec\n- British Columbia\n- Alberta");

  separate();
/*=================================================================================*/ 
  //4
  char *q4 = "\nQuestion 4)\n\n";
  colour(q4, "\033[36m");
  
  int HighestYrPtr = 0,LowestYrPtr = 0;
  int ProvHighestYrPtr = 0,ProvLowestYrPtr = 0;
  int ProvIndex = 0,yearIndex = 0;

  while (ProvIndex < 5) {
      yearIndex = 0;
      while (yearIndex < 7) {
          if (yearAvgVal[ProvIndex][yearIndex] > yearAvgVal[ProvHighestYrPtr][HighestYrPtr]) {
              ProvHighestYrPtr = ProvIndex;
              HighestYrPtr = yearIndex;
          }
          if (yearAvgVal[ProvIndex][yearIndex] < yearAvgVal[ProvLowestYrPtr][LowestYrPtr]) {
              ProvLowestYrPtr = ProvIndex;
              LowestYrPtr = yearIndex;
          }
          yearIndex++;
      }
      ProvIndex++;
  }
  
  printf("The Year and Area with the greatest peak and trough for diabetes is: \n\n");
  printf("Highest: %s in %s, at %.2lf%%\n", File[GenderRow_Start[ProvHighestYrPtr][0][0]][1], year[HighestYrPtr], yearAvgVal[ProvHighestYrPtr][HighestYrPtr]);
  
  printf("Lowest: %s in %s, at %.2lf%%\n", File[GenderRow_Start[ProvLowestYrPtr][0][0]][1], year[LowestYrPtr], yearAvgVal[ProvLowestYrPtr][LowestYrPtr]);

  separate();

  fclose(in); 
  return (0); 
}
