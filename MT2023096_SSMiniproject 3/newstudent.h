#ifndef STUDENT_H
#define STUDENT_H


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include<errno.h>
#include <stdbool.h>
#include "newstructure.h"

int noStudents(){
  FILE *fi=fopen("student.txt","r");
  int count=0;
  Student s2;
  while(fread(&s2, sizeof(Student), 1, fi) == 1){
    count++;
  }
 fclose(fi);
 return count;
}


void viewAllCourses(int fd){
 
  FILE *file2 = fopen("course.txt","r");
  //char result[300];
  char buffer[1024];
  int n=1;
  Course c;
     memset(buffer,0,sizeof(buffer));
    while(fread(&c, sizeof(Course), 1, file2) == 1){
     
     sprintf(buffer + strlen(buffer), "%d.%s, ", n, c.cname);
     n++;
    }
    fclose(file2);
    send(fd,buffer, sizeof(buffer), 0);
    
    memset(buffer,0,sizeof(buffer));
    recv(fd,buffer,sizeof(buffer),0);//for menu
    
    return;

}

void enrollnewCourse(int fd ,Student *s1){
  
  viewAllCourses(fd);
  Course c;

char buffer[1024];

char type;

memset(buffer,0,sizeof(buffer));
send(fd,"Enter course name :", strlen("Enter course name:"), 0);
recv(fd,buffer,sizeof(buffer),0);

strncpy(c.cname, buffer, sizeof(c.cname) - 1);

//update in course.txt: count of enrolled students "c.count"
FILE *file3=fopen("course.txt","r+");
Course c1;

int found=0;
long pos = 0;
while (fread(&c1, sizeof(Course), 1, file3) == 1) {

        if(strcmp(c1.cname,c.cname)==0)
        {   
            struct flock flck;
            memset(&flck, 0, sizeof(flck));
            flck.l_type = F_WRLCK;  // Write lock
            flck.l_whence = SEEK_SET;
            flck.l_start = pos;     // Start position of the record
            flck.l_len = sizeof(Course);  // Length of the course record
            flck.l_pid = getpid();
            
          if(c1.count<c1.maxseat)
          {
            if (fcntl(fileno(file3), F_SETLKW, &flck) == -1) {
                perror("File lock error");
                send(fd,"error occured in updation try again!\ntype continue for menu\n", strlen( "error occured in updation try again!\ntype continue for menu\n"), 0);
                memset(buffer,0,sizeof(buffer));
                recv(fd,buffer,sizeof(buffer),0);//again go to menu
                fclose(file3);
                return;
            }
            
          
            (c1.count)++;
            found=1;
        
            fseek(file3, pos, SEEK_SET);
      
            fwrite(&c1, sizeof(Course), 1, file3);
            
            flck.l_type = F_UNLCK;
            if (fcntl(fileno(file3), F_SETLK, &flck) == -1) 
            {
                 perror("File unlock error");
                 fclose(file3);
                 return;
             }

            
              break;
            }
            else{
              fclose(file3); 
              send(fd,"No vacant seat in this course.\ntype continue for menu\n", strlen( "No vacant seat in this course.\ntype continue for menu\n"), 0);
              memset(buffer,0,sizeof(buffer));
              recv(fd,buffer,sizeof(buffer),0);//again go to menu
                
              return;
            
            }
           
        }
        pos = ftell(file3); //curr position after a cycle
    }

fclose(file3); 
if(found==0){
 send(fd,"you Entered  wrong course name \ntype continue for menu\n", strlen("you Entered wrong course name \ntype continue for menu\n"), 0);
 memset(buffer,0,sizeof(buffer));
 recv(fd,buffer,sizeof(buffer),0);//again go to menu
 return;
}


//update in student.txt: count of enrolled courses "s1->ccount"        


         
FILE *file4 = fopen("student.txt", "r+");
Student s2;    

found =0;          
 pos=0;            
while(fread(&s2, sizeof(Student), 1, file4) == 1) {
       
        if (strcmp(s2.sname, s1->sname) == 0) 
        {   
              
              struct flock flc;
            memset(&flc, 0, sizeof(flc));
            flc.l_type = F_WRLCK;  // Write lock
            flc.l_whence = SEEK_SET;
            flc.l_start = pos;     // Start position of the record
            flc.l_len = sizeof(Student);  // Length of the course record
            flc.l_pid = getpid();

            if (fcntl(fileno(file4), F_SETLKW, &flc) == -1) {
                perror("File lock error");
                send(fd,"error occured in updation try again!\ntype continue for menu\n", strlen( "error occured in updation try again!\ntype continue for menu\n"), 0);
                memset(buffer,0,sizeof(buffer));
                recv(fd,buffer,sizeof(buffer),0);//again go to menu
                fclose(file4);
                return;
            }
          
            
           
              
              strncpy( s2.courses[s2.ccount],c.cname ,sizeof(c.cname) - 1);
              s2.ccount++;
              
              found=1;
              
              fseek(file4, pos, SEEK_SET);
              fwrite(&s2, sizeof(Student), 1, file4);
              
              flc.l_type = F_UNLCK;
              if (fcntl(fileno(file4), F_SETLK, &flc) == -1)
               {
                 perror("File unlock error");
                 fclose(file4);  
                 return;
                }     
              break; 
        }
        
        pos = ftell(file4); //curr position after a cycle
        
    }
 fclose(file4);   
             
 if(found==1){
            
    send(fd,"Successfully Enrolled .\ntype continue for menu\n", strlen("Successfully Enrolled .\ntype continue for menu\n"), 0);
    memset(buffer,0,sizeof(buffer));
    recv(fd,buffer,sizeof(buffer),0);//again go to menu
 } 
 else{
    send(fd,"Unknown error .\ntype continue for menu\n", strlen("Unknown error .\ntype continue for menu\n"), 0);
    memset(buffer,0,sizeof(buffer));
    recv(fd,buffer,sizeof(buffer),0);//again go to menu
 }
           
      
   
return;
}

void dropCourse(int fd,Student *s1)
{

  viewAllCourses(fd);
  Course c;

char buffer[1024];

char type;

memset(buffer,0,sizeof(buffer));
send(fd,"Enter course name you want to drop :", strlen("Enter course name you want to drop :"), 0);
recv(fd,buffer,sizeof(buffer),0);

strncpy(c.cname, buffer, sizeof(c.cname) - 1);



//update in student.txt: count of enrolled courses "s1->ccount"        


FILE *file4 = fopen("student.txt", "r+");
if (file4 == NULL)
 {
        perror("File4 opening failed");
        return;
 }
Student s2;    

int found =0;          
int  pos=0; 
 int flag=0;           
while(fread(&s2, sizeof(Student), 1, file4) == 1) {
       
        if (strcmp(s2.sname, s1->sname) == 0) 
        {   
              
              struct flock flcc;
            memset(&flcc, 0, sizeof(flcc));
            flcc.l_type = F_WRLCK;  // Write lock
            flcc.l_whence = SEEK_SET;
            flcc.l_start = pos;     // Start position of the record
            flcc.l_len = sizeof(Student);  // Length of the course record
            flcc.l_pid = getpid();

            
          
              
              for(int i=0;i<s2.ccount-1;i++){
                 if(strcmp(s2.courses[i], c.cname) == 0){
                    flag=1;
                    if (fcntl(fileno(file4), F_SETLKW, &flcc) == -1) 
                    {
                        perror("File lock error");
                        send(fd,"error occured in updation try again!\ntype continue for menu\n", strlen( "error occured in updation try again!\ntype continue for menu\n"), 0);
                        memset(buffer,0,sizeof(buffer));
                        recv(fd,buffer,sizeof(buffer),0);//again go to menu
                        fclose(file4);
                        return;
                    }
                    
                    found=i;
                    strncpy( s2.courses[i],s2.courses[i+1] ,sizeof(c.cname) - 1);
                 }
                 if(flag==1 && i>found){
                   strncpy( s2.courses[i],s2.courses[i+1] ,sizeof(c.cname) - 1);
                 }
              
              }
              
              
              if(flag==1)
              {
                strncpy( s2.courses[s2.ccount],"" ,sizeof(c.cname) - 1);
                s2.ccount--;
                fseek(file4, pos, SEEK_SET);
                fwrite(&s2, sizeof(Student), 1, file4);
                
                flcc.l_type = F_UNLCK;
                if (fcntl(fileno(file4), F_SETLK, &flcc) == -1) 
                {
                   perror("File unlock error");
                   fclose(file4);
                   return;
                }

            
                break;
              }
              else{
                 send(fd,"You are not enrolled in this course.\ntype continue for menu\n", strlen("You are not enrolled in this course.\ntype continue for menu\n"), 0);
                 memset(buffer,0,sizeof(buffer));
                  recv(fd,buffer,sizeof(buffer),0);//again go to menu
                  fclose(file4);
                  return;
              
              }
        }
        pos = ftell(file4); //curr position after a cycle
        
    }
 fclose(file4); 
 
 //update in course.txt: count of enrolled students "c.count"
FILE *file3=fopen("course.txt","r+");
if (file3 == NULL) {
        perror("File3 opening failed");
        return;
}
Course c1;

 found=0;
 pos = 0;
while (fread(&c1, sizeof(Course), 1, file3) == 1) {

        if(strcmp(c1.cname,c.cname)==0)
        {   
            
              struct flock flckk;
              memset(&flckk, 0, sizeof(flckk));
              flckk.l_type = F_WRLCK;  // Write lock
              flckk.l_whence = SEEK_SET;
              flckk.l_start = pos;     // Start position of the record
              flckk.l_len = sizeof(Course);  // Length of the course record
              flckk.l_pid = getpid();
          if(c1.count!=0)
          {
            if (fcntl(fileno(file3), F_SETLKW, &flckk) == -1) {
                perror("File lock error");
                send(fd,"error occured in updation try again!\ntype continue for menu\n", strlen( "error occured in updation try again!\ntype continue for menu\n"), 0);
                memset(buffer,0,sizeof(buffer));
                recv(fd,buffer,sizeof(buffer),0);//again go to menu
                fclose(file3);
                return;
            }
            
          
            (c1.count)--;
            found=1;
        
            fseek(file3, pos, SEEK_SET);
      
            fwrite(&c1, sizeof(Course), 1, file3);
            
            flckk.l_type = F_UNLCK;
            if (fcntl(fileno(file3), F_SETLK, &flckk) == -1) 
            {
                 perror("File unlock error");
                 fclose(file3);
                 return;
             }

            break;
        }
        else
        {
            send(fd,"You are not enrolled in this course.\ntype continue for menu\n", strlen("You are not enrolled in this course.\ntype continue for menu\n"), 0);
            memset(buffer,0,sizeof(buffer));
            recv(fd,buffer,sizeof(buffer),0);//again go to menu
            fclose(file3);
            return;
        }
        
       } 
        pos = ftell(file3); //curr position after a cycle
    }

fclose(file3); 

if(found==0){
 send(fd,"you Entered  wrong course name \ntype continue for menu\n", strlen("you Entered wrong course name \ntype continue for menu\n"), 0);
 memset(buffer,0,sizeof(buffer));
 recv(fd,buffer,sizeof(buffer),0);//again go to menu
 return;
}
 
 
             
 if(flag==1){
            
    send(fd,"Successfully Dropped .\nYou wannt to continue or exit type", strlen("Successfully Dropped .\nYou wannt to continue or exit type"), 0);
    memset(buffer,0,sizeof(buffer));
    recv(fd,buffer,sizeof(buffer),0);//for again go to menu
 } 
 else{
    send(fd,"Unknown error .", strlen("Unknown error ."), 0);
 }
           
      
   
return;
}
        


void viewCourses(int fd,Student *s1){
     
      char coursesBuffer[1024] = ""; // Buffer to store all the courses
      
       for (int i = 0; i < s1->ccount; i++) 
       {
             strcat(coursesBuffer, s1->courses[i]);
             if (i < s1->ccount - 1)
              {
                    strcat(coursesBuffer, ", "); // Add a comma and space between courses
              }
        }
        
        send(fd,coursesBuffer,sizeof(coursesBuffer),0);
        memset(coursesBuffer,0,sizeof(coursesBuffer));
        recv(fd,coursesBuffer,sizeof(coursesBuffer),0);//for again go to menu
        
        return;


}


void changePassword(int fd){

    Student s2;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    send(fd, "Enter your userName again:", strlen("Enter your userName again:"), 0);
    recv(fd, buffer, sizeof(buffer), 0);
    strncpy(s2.sname, buffer, sizeof(s2.sname) - 1);

    FILE *file1 = fopen("student.txt", "r+");
    if (file1 == NULL) {
        perror("File opening failed");
        return;
    }

    Student s3;
    int found = 0;
    long pos = 0;
    while (fread(&s3, sizeof(Student), 1, file1) == 1) {
       
        if (strcmp(s3.sname, s2.sname) == 0) {
            found = 1;
            char currPassword[10];
            memset(buffer, 0, sizeof(buffer));
            send(fd, "Enter Current Password", strlen("Enter Current Password"), 0);
            recv(fd, buffer, sizeof(buffer), 0);
            strncpy(currPassword, buffer, sizeof(currPassword) - 1);
     
            if (strcmp(s3.spassword, currPassword) == 0) 
            {
                  memset(buffer, 0, sizeof(buffer));
                  send(fd, "Enter new Password(not more than 8 characters):", strlen("Enter new Password(not more than 8 characters):"), 0);
                  recv(fd, buffer, sizeof(buffer), 0);
                  strncpy(s3.spassword, buffer, sizeof(s3.spassword) - 1);
                  found=2;
                  
             }
             else{ 
                  
                  send(fd, "you type wrong current password !!", strlen("you type wrong current password !!"), 0);
                  fclose(file1);
                  return;
            }
         }   

            
          if(found==2){
            fseek(file1, pos, SEEK_SET);
            fwrite(&s3, sizeof(Faculty), 1, file1);
            send(fd, "Password changed successfully.\nYou wannt to continue or exit type", strlen("Password changed successfully.\nyou wannt to continue or exit type"), 0);
            memset(buffer,0,sizeof(buffer));
            recv(fd,buffer,sizeof(buffer),0);//for again go to menu
            break;
          } 
        
        pos = ftell(file1); //curr position after a cycle
    }

    

    fclose(file1);
    return; 

}
           


void student(int fd){

Student s;

char buffer[1024];

char type;
send(fd,"you want to register or login(enter r for register and l for login):", strlen("you want to register or login(enter r for register and l for login):"), 0);
recv(fd,buffer,sizeof(buffer),0);
type=buffer[0];


memset(buffer, 0, sizeof(buffer));
send(fd,"Enter User Name:", strlen("Enter User Name:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(s.sname, buffer, sizeof(s.sname) - 1);



FILE *file = fopen("student.txt", "a+");
    
if (file == NULL) {
    perror("File opening failed");
    return;
}

 if(type=='r'){
    int r =noStudents();
    strncpy(s.spassword,"12345",sizeof(s.spassword) - 1);
    sprintf(s.rollno, "MT2023%d", r);
    sprintf(s.email, "%s@iiitb.ac.in",s.sname);
    s.ccount=0;
    
    
fwrite(&s, sizeof(Student),1,file);

 send(fd,"Successfully registered.with default password 12345\n", strlen("Successfully registered.with default password 12345\n"), 0);   
}
else if(type=='l'){

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter Password:", strlen("Enter Password:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(s.spassword, buffer, sizeof(s.spassword) - 1);

Student s1;
int flag=0;

while (fread(&s1, sizeof(Student), 1, file) == 1) {

        if(strcmp(s1.sname,s.sname)==0)
        {   
           flag=1;
           if (strcmp(s1.spassword,s.spassword)==0) 
           {
             while(1)
             {
             
             
             send(fd,"You are logged in..\n......Welcome to Student Menu......\n1.View All offered Courses \n2.Enroll in New Course\n3.Drop Course\n4.View Enrolled Courses\n5.Change Password\n6.Logout", strlen("Successfully logged in..\n......Welcome to Student Menu......\n1.View All offered Courses \n2.Enroll in New Course\n3.Drop Course\n4.View Enrolled Courses\n5.Change Password\n6.Logout"), 0); 
             memset(buffer, 0, sizeof(buffer));
             recv(fd,buffer,sizeof(buffer),0);
             type=buffer[0];
             
             switch(type){
                 case '1':
                     viewAllCourses(fd);
                     break;
                 case '2':
                     enrollnewCourse(fd,&s1);
                     break;
                 case '3':
                     dropCourse(fd,&s1);
                     break;
                 case '4':
                     viewCourses(fd,&s1);
                     break;
                 case '5':
                     changePassword(fd);
                     break;    
                 case '6': 
                     fclose(file);
                     return;
                     
                default:
                     send(fd,"Invalid Choice!!Sorry\nexited..", strlen("Invalid Choice!!Sorry\nexited.."), 0);
                     fclose(file);
                     return;
               
               
               }
               
            }
            break; 
        }
        else{
        send(fd,"wrong password!!\nexitng..", strlen("wrong password!!\nexitng.."), 0);
        fclose(file);
        return;
        }
   }
  }
  
  if(flag==0){
    send(fd,"wrong credentials !! Sorry.\nexited", strlen("wrong credentials !! Sorry.\nexited"), 0);
    
  }
  
}
else{
   send(fd,"you type wrong char !! Sorry.\nexited", strlen("you type wrong char !! Sorry.\nexited"), 0);
  
}

    
    fclose(file);
    return;
}



#endif

