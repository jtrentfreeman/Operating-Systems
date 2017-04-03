#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER 1024            // store bytes written to it up to a constant buffer size (at least 1KB)
static char sBuffer[BUFFER];   // stores the string

int main()
{
   int ret, thisFile;
   char sendString[BUFFER];
   printf("Starting device test code example...\n");
   thisFile = open("/dev/ebbchar", O_RDWR);             
   if (thisFile < 0)
   {
      perror("Failed to open the device...");
      return errno;
   }

   printf("Type in a short string to send to the kernel module:\n");
   scanf("%[^\n]%*c", sendString);               
   
   char newString[BUFFER];
   if(strlen(sendString) > BUFFER)
   {
      strncpy(newString, sendString, 1024);
      ret = write(thisFile, newString, strlen(newString));
      printf("Writing message to the device [%s].\n", newString);
   }
   else
   {
      printf("Writing message to the device [%s].\n", sendString);
      ret = write(thisFile, sendString, strlen(sendString)); 
   }
   if (ret < 0)
   {
      perror("Failed to write the message to the device.");
      return errno;
   }

   printf("Press ENTER to read back from the device...\n");
   getchar();

   printf("Reading from the device...\n");
   ret = read(thisFile, sBuffer, BUFFER);        
   if (ret < 0)
   {
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("The received message is: [%s]\n", sBuffer);
   printf("End of the program\n");
   return 0;
}
