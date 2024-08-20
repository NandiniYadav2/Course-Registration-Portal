
#include <stdio.h>
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

int noStudent(){
  FILE *fi=fopen("student.txt","r");
  int count=0;
  Student s2;
  while(fread(&s2, sizeof(Student), 1, fi) == 1){
    count++;
  }
 fclose(fi);
 return count;
}

void addStudent(int fd){


Student s;
char buffer[1024];

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter details of Student\nName:", strlen("Enter details of Student\nName:"), 0); 
recv(fd,buffer,sizeof(buffer),0);
strncpy(s.sname, buffer, sizeof(s.sname) - 1);
strncpy(s.spassword,"12345",sizeof(s.spassword) - 1);
int scount=noStudent();
sprintf(s.rollno, "MT2023%d",scount );
sprintf(s.email, "%s@iiitb.ac.in",s.sname);
s.ccount=0;




FILE *file1 = fopen("student.txt", "a+");
if (file1 == NULL) {
   perror("File1 opening failed");
    return;
}

fwrite(&s, sizeof(Student),1,file1);

send(fd,"Student Added Sucessfully with default password 12345.\nYou wannt to continue or exit type:", strlen("Student Added Sucessfully with default password 12345.\nYou wannt to continue or exit type:"), 0);
memset(buffer,0,sizeof(buffer));
recv(fd,buffer,sizeof(buffer),0);//for again go to menu


fclose(file1);
return;
}


void addFaculty(int fd){
Faculty f;
char buffer[1024];


memset(buffer, 0, sizeof(buffer));
send(fd,"Enter details of Faculty\nName:", strlen("Enter details of Faculty\nName:"), 0); 
recv(fd,buffer,sizeof(buffer),0);
strncpy(f.fname,buffer,sizeof(f.fname) - 1);




memset(buffer, 0, sizeof(buffer));
send(fd,"Subject:", strlen("Subject:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(f.subject,buffer,sizeof(f.subject) - 1);

strncpy(f.fpassword,"12345",sizeof(f.fpassword) - 1);
sprintf(f.femail, "%s@iiitb.ac.in",f.fname);

FILE *file2 = fopen("faculty.txt", "a+");
if (file2 == NULL) {
   perror("File2 opening failed");
    return;
}

fwrite(&f, sizeof(Faculty),1,file2);
 
  
send(fd,"Faculty Added Sucessfully with default password :12345.\nYou wannt to continue or exit type:", strlen("Faculty Added Sucessfully with default password :12345.\nYou wannt to continue or exit type:"), 0);
memset(buffer,0,sizeof(buffer));
recv(fd,buffer,sizeof(buffer),0);//for again go to menu


fclose(file2);
return;

}

void viewStudent(int fd){

Student s;
char buffer[1024];

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter Student Name:", strlen("Enter Student Name:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(s.sname, buffer, sizeof(s.sname) - 1);

FILE *file3 = fopen("student.txt", "a+");
    
if (file3 == NULL) {
    perror("File opening failed");
    return;
}

Student s1;
int flag =0;
while(fread(&s1, sizeof(Student), 1, file3) == 1) {

        if(strcmp(s1.sname,s.sname)==0)
        {   
             
            memset(buffer, 0, sizeof(buffer));
            
            char buf[300];
            if(s1.ccount!=0){
            
            strcpy(buf,s1.courses[0]);
            for(int i=0;i<s1.ccount;i++){
            strcat(buf, ",");
            strcat(buf, s1.courses[i]);
            
            }
           }
           else{
            strcpy(buf,"");
           }
            sprintf(buffer, "Name: %s\nEmail: %s\nRoll No: %s\nEnrolled Courses: %s\n", s1.sname,s1.email,s1.rollno,buf);
            send(fd,buffer,sizeof(buffer),0);
             flag=1;
       }
       if(flag==1){
         memset(buffer,0,sizeof(buffer));
         recv(fd,buffer,sizeof(buffer),0);//for again go to menu
         break;
         }
       

   }
   
   if(flag!=1){
      send(fd,"Student not found!!Sorry.\nYou wannt to continue or exit type:",sizeof("Student not found!!Sorry.\nYou wannt to continue or exit type:"),0);
      memset(buffer,0,sizeof(buffer));
      recv(fd,buffer,sizeof(buffer),0);//for again go to menu
      
   }
 fclose(file3); 
 return;

  
}

void viewFaculty(int fd){

Faculty f;
char buffer[1024];

memset(buffer, 0, sizeof(buffer));
send(fd,"Enter Faculty Name:", strlen("Enter Faculty Name:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(f.fname, buffer, sizeof(f.fname) - 1);

FILE *file4 = fopen("faculty.txt", "a+");
    
if (file4 == NULL) {
    perror("File opening failed");
    return;
}

Faculty f1;
int flag =0;

while (fread(&f1, sizeof(Faculty), 1, file4)==1) {

        if(strcmp(f1.fname,f.fname)==0)
        {   
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "Name: %s\nEmail: %s\nSubject: %s\n", f1.fname,f1.femail,f1.subject);
             send(fd,buffer,sizeof(buffer),0);
             memset(buffer,0,sizeof(buffer));
             recv(fd,buffer,sizeof(buffer),0);//for again go to menu
             flag=1;
       }
       if(flag==1)
         break;
       

   }
   
   if(flag!=1){
      send(fd,"Faculty not found!!Sorry.\nYou wannt to continue or exit type:",sizeof("Faculty not found!!Sorry.\nYou wannt to continue or exit type:"),0);
      memset(buffer,0,sizeof(buffer));
      recv(fd,buffer,sizeof(buffer),0);//for again go to menu
      
   }
   
 return;

  
}


void modifyFacultyDetails(int fd) {
    Faculty f;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    send(fd, "Enter Faculty Name:", strlen("Enter Faculty Name:"), 0);
    recv(fd, buffer, sizeof(buffer), 0);
    strncpy(f.fname, buffer, sizeof(f.fname) - 1);

    FILE *file = fopen("faculty.txt", "r+");
    if (file == NULL) {
        perror("File opening failed");
        return;
    }

    Faculty f1;
    int found = 0;
    long pos = 0;

    while (fread(&f1, sizeof(Faculty), 1, file) == 1) {
       
        if (strcmp(f1.fname, f.fname) == 0) {
            found = 1;
            memset(buffer, 0, sizeof(buffer));
            send(fd, "What you want to update? (1.Email , 2.Subject , 3.both):", strlen("What you want to update? (1.Email , 2.Subject , 3.both):"), 0);
            char type;
             recv(fd,buffer,sizeof(buffer),0);
             type=buffer[0];
     
            
            switch(type){
               case  '1':
                  memset(buffer, 0, sizeof(buffer));
                  send(fd, "Enter new email:", strlen("Enter new email:"), 0);
                  recv(fd, buffer, sizeof(buffer), 0);
                  strncpy(f1.femail, buffer, sizeof(f1.femail) - 1);
                  break;
              case  '2':
                  memset(buffer, 0, sizeof(buffer));
                  send(fd, "Enter New Subject:", strlen("Enter New Subject:"), 0);
                  recv(fd, buffer, sizeof(buffer), 0);
                  strncpy(f1.subject, buffer, sizeof(f1.subject) - 1);
                  break;
              case '3':
                  memset(buffer, 0, sizeof(buffer));
                  send(fd, "Enter new email:", strlen("Enter new email:"), 0);
                  recv(fd, buffer, sizeof(buffer), 0);
                  strncpy(f1.femail, buffer, sizeof(f1.femail) - 1);
                  
                  
                  memset(buffer, 0, sizeof(buffer));
                  send(fd, "Enter New Subject:", strlen("Enter New Subject:"), 0);
                  recv(fd, buffer, sizeof(buffer), 0);
                  strncpy(f1.subject, buffer, sizeof(f1.subject) - 1);
                  break;
              default:
                  send(fd, "invalid choice", strlen("invalid choice"), 0);
                  return;
            }
            

            // Set the file pointer to the beginning of the record.
            fseek(file, pos, SEEK_SET);

            
            fwrite(&f1, sizeof(Faculty), 1, file);
            break; 
        }
        pos = ftell(file); //curr position after a cycle
    }

    if (found) {
        send(fd, "Faculty details modified successfully.\nYou wannt to continue or exit type:", strlen("Faculty details modified successfully.\nYou wannt to continue or exit type:"), 0);
        memset(buffer,0,sizeof(buffer));
        recv(fd,buffer,sizeof(buffer),0);//for again go to menu
    } else {
        send(fd, "Faculty not found!! Sorry.\nYou wannt to continue or exit type:", strlen("Faculty not found!! Sorry.\nYou wannt to continue or exit type:"), 0);
        memset(buffer,0,sizeof(buffer));
        recv(fd,buffer,sizeof(buffer),0);//for again go to menu
    }

    fclose(file);
    return;
}

void modifyStudentDetails(int fd) {
    Student s;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    send(fd, "Enter Student Name:", strlen("Enter Student Name:"), 0);
    recv(fd, buffer, sizeof(buffer), 0);
    strncpy(s.sname, buffer, sizeof(s.sname) - 1);

    FILE *file = fopen("student.txt", "r+");
    if (file == NULL) {
        perror("File opening failed");
        return;
    }

    Student s1;
    int found = 0;
    long pos = 0;

    while (fread(&s1, sizeof(Student), 1, file) == 1) {
       
        if (strcmp(s1.sname, s.sname) == 0) {
            found = 1;
            memset(buffer, 0, sizeof(buffer));
            send(fd, "What you want to update? (1.Email):", strlen("What you want to update? (1.Email):"), 0);
            char type ;
             recv(fd,buffer,sizeof(buffer),0);
              type=buffer[0];
     
            
            switch(type){
               case  '1':
                  memset(buffer, 0, sizeof(buffer));
                  send(fd, "Enter New Email:", strlen("Enter New Email:"), 0);
                  recv(fd, buffer, sizeof(buffer), 0);
                  strncpy(s1.email, buffer, sizeof(s1.email) - 1);
                  break;
             
              default:
                  send(fd, "invalid choice", strlen("invalid choice"), 0);
                  return;
                   
            }
            

            // Set the file pointer to the beginning of the record.
            fseek(file, pos, SEEK_SET);

            
            fwrite(&s1, sizeof(Student), 1, file);
            break; 
        }
        pos = ftell(file); //curr position after a cycle
    }

    if (found) {
        send(fd, "Student details modified successfully.\nYou wannt to continue or exit type:", strlen("Student details modified successfully.\nYou wannt to continue or exit type:"), 0);
        memset(buffer,0,sizeof(buffer));
        recv(fd,buffer,sizeof(buffer),0);//for again go to menu
    } else {
        send(fd, "Student not found!! Sorry.\nYou wannt to continue or exit type:", strlen("Student not found!! Sorry.\nYou wannt to continue or exit type:"), 0);
        memset(buffer,0,sizeof(buffer));
        recv(fd,buffer,sizeof(buffer),0);//for again go to menu
    }

    fclose(file);
    return;
}





void admin(int fd){

Admin a ;
char buffer[1024];

char type;
send(fd,"you want to register or login(enter r for register and l for login):", strlen("you want to register or login(enter r for register and l for login):"), 0);
recv(fd,buffer,sizeof(buffer),0);
type=buffer[0];


memset(buffer, 0, sizeof(buffer));
send(fd,"Enter User Name:", strlen("Enter User Name:"), 0);
recv(fd,buffer,sizeof(buffer),0);
strncpy(a.aname, buffer, sizeof(a.aname) - 1);


memset(buffer, 0, sizeof(buffer));
send(fd,"Enter Password:", strlen("Enter Password:"), 0);
recv(fd,buffer,sizeof(buffer),0);

strncpy(a.apassword, buffer, sizeof(a.apassword) - 1);

FILE *file = fopen("admin.txt", "a+");
    
if (file == NULL) {
    perror("File opening failed");
    return;
}

 if(type=='r'){
    fwrite(&a, sizeof(Admin),1,file);
    
 send(fd,"Successfully registered.\n", strlen("Successfully registered.\n"), 0);   
}
else if(type=='l'){
Admin a1;
int flag=0;
while (fread(&a1, sizeof(Admin), 1, file) == 1) {

        if(strcmp(a1.aname,a.aname)==0)
        {  
           flag=1; 
           if (strcmp(a1.apassword,a.apassword)==0) 
           {
             while(1){ 
             
             send(fd,"You are logged in..\n......Welcome to Admin Menu......\n1.Add Student\n2.Add Faculty\n3.View Student Details\n4.View Faculty Details\n5.Modify Student Details\n6.Modify Faculty Details\n7.Exit", strlen("Successfully logged in..\n......Welcome to Admin Menu......\n1.Add Student\n2.Add Faculty\n3.View Student Details\n4.View Faculty Details\n5.Modify Student Details\n6.Modify Faculty Details\n7.Exit"), 0); 
             memset(buffer, 0, sizeof(buffer));
             recv(fd,buffer,sizeof(buffer),0);
             type=buffer[0];
             switch(type){
                case '1':
                     addStudent(fd);
                     break;
                case '2':
                     addFaculty(fd);
                     break;
                case '3':
                     viewStudent(fd);
                     break;
                case '4':
                     viewFaculty(fd);
                     break;
                case '5':
                     modifyStudentDetails(fd);
                     break;
                case '6':
                     modifyFacultyDetails(fd);
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


