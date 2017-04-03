#include <linux/init.h>           
#include <linux/module.h>        
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>             
#include <asm/uaccess.h>          
#define  DEVICE_NAME "ebbchar"    
#define  CLASS_NAME  "ebb"        

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Freeman, Havekost, Pantino");    
MODULE_DESCRIPTION("COP4600 Programming Assignment 2");  
MODULE_VERSION("1.0");         

static int    mdNumber;                  
static char   message[1024] = {0};           
static short  messageLength;              
static int    nOpens = 0;              
static struct class*  charClass  = NULL; 
static struct device* charDev = NULL; 

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init ebbchar_init(void)
{
   printk(KERN_INFO "EBBChar: Initializing the EBBChar LKM\n");

   mdNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (mdNumber<0)
   {
      printk(KERN_ALERT "EBBChar failed to register a major number\n");
      return mdNumber;
   }
   printk(KERN_INFO "EBBChar: registered correctly with major number %d\n", mdNumber);

   charClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(charClass))
   {                
      unregister_chrdev(mdNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(charClass);          
   }
   printk(KERN_INFO "EBBChar: device class registered correctly\n");

   charDev = device_create(charClass, NULL, MKDEV(mdNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(charDev))
   {               
      class_destroy(charClass);           
      unregister_chrdev(mdNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(charDev);
   }
   printk(KERN_INFO "EBBChar: device class created correctly\n"); 
   return 0;
}


static void __exit ebbchar_exit(void)
{
   device_destroy(charClass, MKDEV(mdNumber, 0));     
   class_unregister(charClass);                          
   class_destroy(charClass);                             
   unregister_chrdev(mdNumber, DEVICE_NAME);             
   printk(KERN_INFO "EBBChar: Goodbye from the LKM!\n");
}


static int dev_open(struct inode *inodep, struct file *filep)
{
   nOpens++;
   printk(KERN_INFO "EBBChar: Device has been opened %d time(s)\n", nOpens);
   return 0;
}


static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int error_count = 0;
   error_count = copy_to_user(buffer, message, messageLength);

   if (error_count==0)
   {            
      printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", messageLength);
      return (messageLength=0);  
   }
   else 
   {
      printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;             
   }
}


static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   sprintf(message, "%s(%zu letters)", buffer, len);   
   messageLength = strlen(message);                 
   printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);
   return len;
}


static int dev_release(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO "EBBChar: Device successfully closed\n");
   return 0;
}

module_init(ebbchar_init);
module_exit(ebbchar_exit);
