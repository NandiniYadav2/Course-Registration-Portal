#ifndef STRUCTURE_H
#define STRUCTURE_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h> 





typedef struct{

  char sname[20];
  char spassword[10];
  char rollno[10];
  char email[40];
  char courses[10][25];
  int ccount;

}Student;

typedef struct fcl{
   
   char fname[20];
   char fpassword[10];
   char femail[40];
   char subject[25];

}Faculty;

typedef struct{

 char aname[20];
 char apassword[10];

}Admin;

typedef struct{
  
  char cname[25];
  int count;
  int maxseat;
 
}Course;

#endif
