/* Generated by CIL v. 1.7.3 */
/* print_CIL_Input is true */

typedef unsigned long size_t;
typedef long __off_t;
typedef long __off64_t;
struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef void _IO_lock_t;
struct _IO_marker {
   struct _IO_marker *_next ;
   struct _IO_FILE *_sbuf ;
   int _pos ;
};
struct _IO_FILE {
   int _flags ;
   char *_IO_read_ptr ;
   char *_IO_read_end ;
   char *_IO_read_base ;
   char *_IO_write_base ;
   char *_IO_write_ptr ;
   char *_IO_write_end ;
   char *_IO_buf_base ;
   char *_IO_buf_end ;
   char *_IO_save_base ;
   char *_IO_backup_base ;
   char *_IO_save_end ;
   struct _IO_marker *_markers ;
   struct _IO_FILE *_chain ;
   int _fileno ;
   int _flags2 ;
   __off_t _old_offset ;
   unsigned short _cur_column ;
   signed char _vtable_offset ;
   char _shortbuf[1] ;
   _IO_lock_t *_lock ;
   __off64_t _offset ;
   void *__pad1 ;
   void *__pad2 ;
   void *__pad3 ;
   void *__pad4 ;
   size_t __pad5 ;
   int _mode ;
   char _unused2[(15UL * sizeof(int ) - 4UL * sizeof(void *)) - sizeof(size_t )] ;
};
typedef char bool;
typedef char character;
typedef char string[100];
#pragma merger("0","./empty.i","-w")
#pragma merger("0","./replace.i","-w")
extern void exit() ;
extern struct _IO_FILE *stdin ;
extern struct _IO_FILE *stdout ;
extern int fprintf(FILE * __restrict  __stream , char const   * __restrict  __format 
                   , ...) ;
extern int fputc(int __c , FILE *__stream ) ;
extern char *fgets(char * __restrict  __s , int __n , FILE * __restrict  __stream ) ;
void Caseerror(int n ) ;
bool getline_in_replace(char *s , int maxsize ) 
{ 
                

  {
  {
                                                                              
  }
                                                                          
}
}
int addstr(char c , char *outset , int *j , int maxset ) 
{ 
               

  {
                     
                      
          
                       
            
                      
   
                        
}
}
char esc(char *s , int *i ) 
{ 
  char result ;

  {
  if ((int )*(s + *i) != 64) {
    result = *(s + *i);
  } else { //Added block
                                   
                        
          
            
                                 
                        
                          
                                 
                       
            
                         
     
     
   
  }
                  
}
}
void change(char *pat , char *sub ) ;
extern int ( /* missing proto */  isalnum)() ;
void dodash(char delim , char *src , int *i , char *dest , int *j , int maxset ) 
{ 
  int k ;
             
                
           
               
               
               
               
  int tmp___4 ;
  int tmp___5 ;

  {
  {
  while (1) {
                     ;/* CIL Label */ ;
                                           
                                       
                         
       
            
                       
     
    if ((int )*(src + (*i - 1)) == 64) {
      {
                            
                                             
                        
      }
    } else { //Added block
    if ((int )*(src + *i) != 45) {
      {
                                                     
                            
      }
    } else { //Added block
    if (*j <= 1) {
      {


      }
    } else { //Added block
    if ((int )*(src + (*i + 1)) == 0) {
      {
                                                    
                            
      }
    } else {
      {
                                                 
                                                 
      }
      if (tmp___4 & tmp___5) {
        if ((int )*(src + (*i - 1)) <= (int )*(src + (*i + 1))) {
                                          
          {
          while (1) {
                                 ;/* CIL Label */ ;
                                                   
                                   
             
            {
                                                        
                                  
            k ++;
            }
          }
                            ;/* CIL Label */ ;
          }
                  
        } else {
          {
                                                        
                                
          }
        }
      } else {
        {
                                                      
                              
        }
      }
    }
    }
    }
    }
            
  }
                ;/* CIL Label */ ;
  }
         
}
}
bool getccl(char *arg , int *i , char *pat , int *j ) 
{ 
              
             
           
               
               

  {
          
                                
     
                                          
                      
            
     
          
     
                                              
                          
     
   
  {
              
                                         
                        
                                          
                                               
  }
                                            
}
}
void stclose(char *pat , int *j , int lastj ) 
{ 
          
          
             
           

  {
              
  {
             
                                       
                          
                       
     
     
                
                                              
                      
          
     
   
                ;/* CIL Label */ ;
  }
          
                              
         
}
}
bool in_set_2(char c ) 
{ 
           

  {
                      
            
                        
                      

                        
                      
            
          
            
   
   
   
                      
}
}
bool in_pat_set(char c ) 
{ 
           

  {
                      
            
                        
                      
            
                        
                      
            
                        
                      
            
                        
                      
            
                        
                      
            
                        
                      
            
          

   
   
   
   
   
   
   
                      
}
}
int makepat(char *arg , int start , char delim , char *pat ) 
{ 
              
  int i ;
         
             
          
             
             
               
  char escjunk ;
           
               
               
                
               
               
               

  {
        
            
            
                  
  {
  while (1) {
                     ;/* CIL Label */ ;
                 
                                            
                                        

         
              
                         
       
            
                       
     
           
    if ((int )*(arg + i) == 63) {
      {
                                              
                        
      }
    } else { //Added block
    if ((int )*(arg + i) == 37) {
                       
         
                                                    
                              
         
              
                    
       
    } else { //Added block
             ;/* CIL Label */ 
    if ((int )*(arg + i) == 36) {
                                                  
         
                                                    
                              
         
              
                
       
    } else { //Added block
         ;/* CIL Label */ 
    if ((int )*(arg + i) == 91) {
      {
                                          
                                        
      }
    } else { //Added block
    if ((int )*(arg + i) == 42) {
                      
         
                   
                                        
         
                      
                          
                
           
                                   
           
         
              
         
                                                    
                              
                                
                                                 
                              
         
       
    } else {
      {
                                                  
                            
      escjunk = esc(arg, & i);
                                               
                            
      }
    }
    }
    }
    }
    }
               
                 
           
     
  }
                ;/* CIL Label */ ;
  }
  {
                                                 
                        
  }
             
               
                        
                                        

                        
               
               
          
               
   
   
   
                  
}
}
int getpat(char *arg , char *pat ) 
{ 
  int makeres ;

  {
  {
  makeres = makepat(arg, 0, (char )'\000', pat);
  }
                       
}
}
int makesub(char *arg , int from , character delim , char *sub ) 
{ 
              
         
         
             
                     
           
               
               

  {
        
           
  {
             
                                       
                                          
                                      

       
            
                       
     
                                           
       
                                            
                        
       
            
       
                              
                                               
                            
       
     
         
   
                ;/* CIL Label */ ;
  }
                                        

          
     
                                                   
                          
     
                 
                 
            
                 
     
   
                  
}
}
bool getsub(char *arg , char *sub ) 
{ 
  int makeres ;

  {
  {
  makeres = makesub(arg, 0, (character )'\000', sub);
  }
                                
}
}
void subline(char *lin , char *pat , char *sub ) ;
bool locate(character c , char *pat , int offset ) 
{ 
         
             

  {
                  
                                     
  {
             
                                       
                         
                       
     
                                      
                      
                 
            
           
     
   
                ;/* CIL Label */ ;
  }
                
}
}
extern int ( /* missing proto */  abort)() ;
bool omatch(char *lin , int *i , char *pat , int j ) 
{ 
                
               
            
                
                

  {
                     
                               
                      
          
     
                                     
     
                  
       
                                   
                     
       
                                   
                     
       
                                   
                     
       
                                   
                     
       
                                   
                     
       
                                   
                     
       

                                 
                                                        
                          
       
                        
                                 
                    
                          
       
                        
                                 
                                    
                          
       
                        
                                 
                                    
                          
       
                        
                                 
       
                                            
       
                
                          
       
                        
                                 
                                    
         
                                                  
         
                        
                            
         
       
                        
                                        
       

       
                                       
       
            
       


       
     
   
                           
                        
                      
          
                      
   
                  
}
}
int patsize(char *pat , int n ) 
{ 
  int size ;
            

  {
  {
                               
  }
            
     
                                 
                   
     
                                 
                   
     
                                 
                   
     
                                 
                   
     
                                 
                   
     
                                 
                   
     
                                 
                   
     

                               
             
                      
                               
                               
                               
             
                      
                               
                               
                                      
                      
                               
             
                      
                                      
     

     
                                     
     
          
     


     
   
  return (size);
}
}
int amatch(char *lin , int offset , char *pat , int j ) 
{ 
         
         
               
             
           
               
               

  {
                  
  {
             
                                       
                 
                                      
                         
       
            
                       
     
                                 
       
                            
               
                 
       
       
                 
                                               
                     
                                          
                                 
           
                
                               
         
         
                                          
         
                       
                          
         
       
                                          
       
                      
       
                 
                                               
                     
                                
                                 
           
                
                               
         
         
                                  
                                             
         
                     
                          
                
               
         
       
                                          
       
                 
                      
            
       
                                             
       
                     
                    
                        
              
         
                                  
                     
         
       
     
   
                ;/* CIL Label */ ;
  }
                  
}
}
void putsub(char *lin , int s1 , int s2 , char *sub ) 
{ 
         
         

  {
        
  {
             
                                       
                                    
                       
     
                                 
             
       
                 
                                               
                         
                               
         
         
                                        
             
         
       
                                          
       
            
       
                                      
       
     
         
   
                ;/* CIL Label */ ;
  }
         
}
}
void subline(char *lin , char *pat , char *sub ) 
{ 
  int i ;
             
         

  {
             
        
  {
  while (1) {
                     ;/* CIL Label */ ;
    if (! ((int )*(lin + i) != 0)) {
      goto while_break;
    }
    {
                               
    }
                 
                       
         
                               
                  
         
       
     
                  
       
                                      
           
       
                          
                 
       
                                      
           
       
            
            
     
     
  }
  while_break: ;/* CIL Label */ ;
  }
         
}
}
void change(char *pat , char *sub ) 
{ 
               
               

  {
  {
                                         
  }
  {
             
                                       
                   
                       
     
     
                            
                                           
     
   
                ;/* CIL Label */ ;
  }
         
}
}
int main(int argc , char **argv ) 
{ 
              
              
               
           

  {
                 
     
                                                                                                   
            
     
   
  {
                                 
                      
  }
                 
     
                                                                                                            
            
     
   
                  
     
                                      
     
                   
       
                                                                                                           
              
       
     
          
                           
   
  {
                   
  }
             
}
}
void Caseerror(int n ) 
{









}
