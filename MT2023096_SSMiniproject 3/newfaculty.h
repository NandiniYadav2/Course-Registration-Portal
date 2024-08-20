#ifndef FACULTY_H
#define FACULTY_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<errno.h>
#include<stdbool.h>
#include"newstructure.h"

void viewAllCoursesf(int fd){
 
  FILE *file2 = fopen("course.txt","r+");
  char result[300];
  int n=1;
  Course c;
    memset(result, 0, sizeof(result));
    while(fread(&c, sizeof(Course), 1, file2) == 1){
     
     sprintf(result + strlen(result), "%d.%s, ", n, c.cname);
     n++;
    }
    
    fclose(file2);
    send(fd,result, sizeof(result), 0);
    memset(result,0,sizeof(result));
    recv(fd,result,sizeof(result),0);//again menu
  
    
    return;

}
void addnewCourse(int fd){
 Course c;
char buffer[1024];

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter details of New Course\nName:", strlen("Enter details of New Course\nName:"), 0); 
recv(fd,buffer,sizeof(buffer),0);
strncpy(c.cname, buffer, sizeof(c.cname) - 1);

c.count=0;
c.maxseat=50;


FILE *file1 = fopen("course.txt", "a+");
if (file1 == NULL) {
   perror("File1 opening failed");
    return;
}

fwrite(&c, sizeof(Course), 1, file1);

send(fd,"Course Added Sucessfully.\nType Continue for Menu\n", strlen("Course Added Sucessfully.\nType Continue for Menu\n"), 0);
memset(buffer,0,sizeof(buffer));
recv(fd,buffer,sizeof(buffer),0);//for again go to menu


fclose(file1);
return;

}
void viewStudentf(int fd){
Student s;
char buffer[1024];

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter Student Name:", strlen("Enter Student Name:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(s.sname, buffer, sizeof(s.sname) - 1);

FILE *file3 = fopen("student.txt", "r");
    
if (file3 == NULL) {
    perror("File opening failed");
    return;
}

Student s1;
int flag =0;
while(fread(&s1, sizeof(Student), 1, file3) == 1) {

        if(strcmp(s1.sname,s.sname)==0)
        {   
        
           char coursesBuffer[300] = ""; // Buffer to store all the courses
            for (int i = 0; i < s1.ccount; i++) {
                strcat(coursesBuffer, s1.courses[i]);
                if (i < s1.ccount - 1) {
                    strcat(coursesBuffer, ", "); // Add a comma and space between courses
                }
            }
        
        
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "Name: %s\nRollno: %s\nEmail: %s\nCourses: %s\nno of Enrolled Courses: %d\n", s1.sname,s1.rollno,s1.email,coursesBuffer,s1.ccount);
             send(fd,buffer,sizeof(buffer),0);
             
             memset(buffer,0,sizeof(buffer));
             recv(fd,buffer,sizeof(buffer),0);//for again go to menu
             flag=1;
       }
       if(flag==1)
         break;
       

   }
   
   if(flag!=1){
      send(fd,"Student not found!!Sorry.\nType Continue for Menu\n",sizeof("Student not found!!Sorry.\nType Continue for Menu\n"),0);
      memset(buffer,0,sizeof(buffer));
      recv(fd,buffer,sizeof(buffer),0);//for again go to menu
      
   }
 fclose(file3);  
 return;

  


}
void viewCourseDetail(int fd){

Course c;
char buffer[1024];

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter Course Name:", strlen("Enter Course Name:"), 0); 
recv(fd,buffer,sizeof(buffer),0);
strncpy(c.cname, buffer, sizeof(c.cname) - 1);

FILE *file4 = fopen("course.txt", "r");
    
if (file4 == NULL) {
    perror("File opening failed");
    return;
}

Course c1;
int flag=0;
while(fread(&c1, sizeof(Course), 1, file4) == 1) {

        if(strcmp(c1.cname,c.cname)==0)
        {   
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer,"Course Name: %s\nStudents Enrolled: %d\nMax Seats : %d\n",c1.cname,c1.count,c1.maxseat);
            send(fd,buffer,sizeof(buffer),0);
            memset(buffer,0,sizeof(buffer));
            recv(fd,buffer,sizeof(buffer),0);//for again go to menu
             flag=1;
       }
       if(flag==1)
         break;
       

   }
   
   if(flag!=1){
      send(fd,"Course not found!!Sorry.\nType Continue for Menu\n",sizeof("Course not found!!Sorry.\n Type Continue for Menu\n"),0);
      memset(buffer,0,sizeof(buffer));
      recv(fd,buffer,sizeof(buffer),0);//for again go to menu
      
   }
 fclose(file4);  
 return;

}

void removeCourse(int fd){

    Course c;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    send(fd, "Enter Course Name to Remove:", strlen("Enter Course Name to Remove:"), 0);
    recv(fd, buffer, sizeof(buffer), 0);
    strncpy(c.cname, buffer, sizeof(c.cname) - 1);

    FILE *file1 = fopen("course.txt", "r+");
    if (file1 == NULL) {
        perror("File opening failed");
        return;
    }

    Course c1;
    int found = 0;
    int flag=0 ;
    long pos = 0;
    long writepos=0;

    while (fread(&c1, sizeof(Course), 1, file1) == 1) {
        if (strcmp(c1.cname, c.cname) == 0) {
            // Course found and removed, skip writing this course back
            flag = 1;
            found=2;
            writepos=pos;
        } 
        
        pos = ftell(file1);
        if(flag==1 && found>2) 
        {
            // Write other courses back to the file 
            fseek(file1,writepos, SEEK_SET);
            fwrite(&c1, sizeof(Course), 1, file1);
            // Record the position where the course was written
            writepos = ftell(file1);
            fseek(file1,pos, SEEK_SET);  //set curser one size ahead for reading
            
        }
        if(flag==1){
          found++;
        }
        
       
        
       
    }
   
    // Truncate the file at the new write position to remove empty space
    if (flag==1) {
        ftruncate(fileno(file1), writepos); /*ftruncate only changes the file size, and it doesn't affect the ability to append new data to the end of the file*/
          
         fclose(file1);

        send(fd, "Course removed successfully.\nYou wannt to continue or exit type:", strlen("Course removed successfully.\nYou wannt to continue or exit type:"), 0);
        memset(buffer,0,sizeof(buffer));
        recv(fd,buffer,sizeof(buffer),0);//for again go to menu
    } 
    else
     {
         
         fclose(file1);

        send(fd, "Course not found!!\nType Continue for Menu\n", strlen("Course not found!!\nType Continue for Menu\n"), 0);
        memset(buffer,0,sizeof(buffer));
        recv(fd,buffer,sizeof(buffer),0);//for again go to menu
    }

    
    return;
}


void changePasswordf(int fd){

    Faculty f;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    send(fd, "Enter your userName again:", strlen("Enter your userName again:"), 0);
    recv(fd, buffer, sizeof(buffer), 0);
    strncpy(f.fname, buffer, sizeof(f.fname) - 1);

    FILE *file1 = fopen("faculty.txt", "r+");
    if (file1 == NULL) {
        perror("File opening failed");
        return;
    }

    Faculty f1;
    int found = 0;
    long pos = 0;
    while (fread(&f1, sizeof(Faculty), 1, file1) == 1) {
       
        if (strcmp(f1.fname, f.fname) == 0) {
            found = 1;
            char currPassword[10];
            memset(buffer, 0, sizeof(buffer));
            send(fd, "Enter Current Password", strlen("Enter Current Password"), 0);
            
            recv(fd, buffer, sizeof(buffer), 0);
            strncpy(currPassword, buffer, sizeof(currPassword) - 1);
     
            if (strcmp(f1.fpassword, currPassword) == 0) 
            {
           
                  memset(buffer, 0, sizeof(buffer));
                  send(fd, "Enter new Password(not more than 8 characters):", strlen("Enter new Password(not more than 8 characters):"), 0);
                  recv(fd, buffer, sizeof(buffer), 0);
                  strncpy(f1.fpassword, buffer, sizeof(f1.fpassword) - 1);
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
            fwrite(&f1, sizeof(Faculty), 1, file1);
            send(fd, "Password changed successfully.\nType Continue for Menu\n", strlen("Password changed successfully.\nType Continue for Menu\n"), 0);
            memset(buffer,0,sizeof(buffer));
            recv(fd,buffer,sizeof(buffer),0);//for again go to menu
            break;
          } 
        
        pos = ftell(file1); //curr position after a cycle
    }

    

    fclose(file1);
    return; 

}


void faculty(int fd){

Faculty f;

char buffer[1024];

char type;
send(fd,"you want to register or login(enter r for register and l for login):", strlen("you want to register or login(enter r for register and l for login):"), 0);
recv(fd,buffer,sizeof(buffer),0);
type=buffer[0];


memset(buffer, 0, sizeof(buffer));
send(fd,"Enter User Name:", strlen("Enter User Name:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(f.fname, buffer, sizeof(f.fname) - 1);



FILE *file = fopen("faculty.txt", "a+");
    
if (file == NULL) {
    perror("File opening failed");
    send(fd, "Failed to open the faculty file.", strlen("Failed to open the faculty file."), 0);   
    return;
}

 if(type=='r'){
 
   memset(buffer, 0, sizeof(buffer));
   send(fd,"Enter Subject:", strlen("Enter Subject:"), 0);
   recv(fd,buffer,sizeof(buffer),0);
   strncpy(f.subject, buffer, sizeof(f.subject) - 1);
 
    strncpy(f.fpassword,"12345",sizeof(f.fpassword) - 1);
    
    sprintf(f.femail, "%s@iiitb.ac.in",f.fname);
    
     fwrite(&f, sizeof(Faculty), 1, file);
    
    send(fd,"Successfully registered.with default password 12345\n", strlen("Successfully registered.with default password 12345\n"), 0);   
}
else if(type=='l'){

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter Password:", strlen("Enter Password:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(f.fpassword, buffer, sizeof(f.fpassword) - 1);

Faculty f1;

int flag=0;

while (fread(&f1, sizeof(Faculty), 1, file) == 1) {

        if(strcmp(f1.fname,f.fname)==0)
        {   flag=1;
           if (strcmp(f1.fpassword,f.fpassword)==0) 
           {
           
             while(1){
             
             send(fd,"You are logged in..\n......Welcome to Faculty Menu......\n1.View All offered Courses \n2.Add New Course\n3.View Student Details\n4.View Course Detail\n5.Remove Course from catalog\n6.Change Password\n7.Logout", strlen("Successfully logged in..\n......Welcome to Faculty Menu......\n1.View All offered Courses \n2.Add New Course\n3.View Student Details\n4.View Course Detail\n5.Remove Course from catalog\n6.Change Password\n7.Logout"), 0);
             memset(buffer, 0, sizeof(buffer));
              recv(fd,buffer,sizeof(buffer),0);
             type=buffer[0];
             switch(type){
                 case '1':
                     viewAllCoursesf(fd);
                     break;
                 case '2':
                     addnewCourse(fd);
                     break;
                 case '3':
                     viewStudentf(fd);
                     break;
                 case '4':
                     viewCourseDetail(fd);
                     break;
                 case '5':
                     removeCourse(fd);
                     break;  
                 case '6':
                     changePasswordf(fd);
                     break;    
                 case '7':
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





