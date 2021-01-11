#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
 
int main(int argc, char **argv)
{
   int clientfd;
   char *num1, *num2;
   char *host, *port;
   char buffer[MAXLINE];
   char stat1[MAXLINE];
   char stat2[MAXLINE];
   char value1[MAXLINE], value2[MAXLINE];
   size_t size = MAXLINE;
 
   if (argc != 3) {
       fprintf(stderr, "usage: %s <num1> <num2>\n", argv[0]);
       exit(0);
   }
 
   num1 = argv[1];
   num2 = argv[2];
 
   host = "localhost";
   port = "8080";
 
   clientfd = Open_clientfd(host, port);
   rio_t robustIO;
   sprintf(stat2,  "<?xml version=\"1.0\" encoding=\"UTF-8\"?><methodCall><methodName>%s</methodName><params><param><value><double>%s</double></value></param><param><value><double>%s</double></value></param></params></methodCall>", "sample.addmultiply",num1,num2); // packing the message
   int len2 = strlen(stat2); // getting length of the message
   sprintf(stat1, "POST /RPC2 HTTP/1.1\r\nHost: %s\r\nContent-Type: text/xml\nContent-Length: %d\r\n\r\n", "localhost:8080", len2); // xml state
   int len1 = strlen(stat1);  // getting length of the xml state
 
   Rio_writen(clientfd, stat1, len1); // sending the xml state
   Rio_readinitb(&robustIO, clientfd);
   Rio_writen(clientfd, stat2, len2); // sending the message
   Rio_readn(clientfd, buffer, size); // reading the message into buffer
   int count1 = 0; // count variables to keep track of whether it's the first value or the second value
   int count2 = 0;
   int i =0;
   /* parsing through the buffer */
   while (i < sizeof(buffer))
   {
       if (buffer[i] == '>') // check if it the closing parameter
       {
           i = i+1;
           if (count1 == 0) // if its the first result
           {
             while (buffer[i] != '<' && buffer[i] != '\n') // the addition result appears when < and \n doesnt appear after >
                 {  value1[count1] = buffer[i]; // storing the addition result
                     i = i+1;
                    count1 = count1 + 1; }
           }
           else
               {
                while (buffer[i] != '<' && buffer[i] != '\n') // when its the multiplication result
                  {  value2[count2] = buffer[i]; // storing the multiplication result
                     i = i+1;
                    count2 = count2 + 1; }
               }
              
 
           }
           i = i + 1;
 
       }
  
   printf("%s  %s\n", value1, value2);
   Close(clientfd); 
   exit(0); 
}

