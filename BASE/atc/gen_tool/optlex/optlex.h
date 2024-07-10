#ifndef OPTLEX_H
#define OPTLEX_H

#define KEY_SPECIAL            1  /*   /* Special action meaning "start processing a new file". */                                 
                                   /*  #define YY_NEW_FILE yyrestart( yyin )                      
                                       #define YY_BUF_SIZE 16384                        
                                                                                        
                                       typedef struct yy_buffer_state *YY_BUFFER_STATE; 
                                       extern int yyleng;                               
                                       extern FILE *yyin, *yyout;                       
                                                                                        
                                       #define EOB_ACT_CONTINUE_SCAN 0                  
                                       #define EOB_ACT_END_OF_FILE 1                    
                                       #define EOB_ACT_LAST_MATCH 2                     
                                                                                        
                                       yyless(n)                                        
                                        ....                                            
                                       #define unput(c) yyunput( c, yytext_ptr )        
                                       typedef unsigned int yy_size_t;                  
                                       struct yy_buffer_state                           
                                       {                                                
                                       	....                                         
                                       }                                                
                                       static YY_BUFFER_STATE yy_current_buffer = 0;    
                                       #define YY_CURRENT_BUFFER yy_current_buffer 
                                   */
                                                                                                   
#define KEY_MACROS             2  /*  /* Macros after this point can all be overridden by user definitions in                      
                                        * section 1.                                                          
                                        */                                                                    
                                                                                             
                                   /*  delete                                                            
                                       extern int yywrap YY_PROTO(( void ));                 
                                       static void yyunput YY_PROTO(( int c, char *buf_ptr ));
                                       static void yy_flex_strncpy YY_PROTO(( char *, yyconst char *, int ));
                                       static int yy_flex_strlen YY_PROTO(( yyconst char * ));
                                       static int yyinput YY_PROTO(( void ));
                                       #if YY_STACK_USED
                                           ....
                                       #endif
                                       #ifdef YY_MALLOC_DECL
                                       	.....
                                       #endif
                                       MICRO ECHO and YY_INPUT
                                       #define yyterminate() return YY_NULL
                                       delete MICRO              
                                              YY_START_STACK_INCR
                                              YY_FATAL_ERROR     
                                              YY_DECL            
                                              YY_USER_ACTION     
                                              YY_BREAK           
                                              YY_RULE_SETUP      
                                       change YY_DECL            
                                              {                  
                                       to     int yylex(void)    
                                              {          
                                     */                             

#define KEY_YY_GET_NEXT   3     /* delete routine 
                                    yy_get_next_buffer() 
                                    yy_get_previous_state()
                                    yy_try_NUL_trans()
                                    yyunput()
                                    yyinput()
                                    yyrestart()
                                    yy_switch_to_buffer()
                                    yy_load_buffer_state()
                                    yy_create_buffer()
                                    yy_delete_buffer()
                                    yy_init_buffer()
                                    yy_flush_buffer()
                                    yy_scan_buffer()
                                    yy_scan_string()
                                    yy_scan_bytes()
                                    yy_push_state()
                                    yy_pop_state()
                                    yy_top_state()
                                    yy_fatal_error()
                                    yyless()
                                    yy_flex_strncpy()
                                    yy_flex_strlen()
                                    yy_flex_alloc()
                                    yy_flex_realloc()
                                    yy_flex_free()
                                    YY_MAIN
                                  */

#define KEY_YY_CP        4      /* *yy_cp = '\0'; \  */

#define KEY_YY_RULE_SETUP  5    /*  delete MICRO YY_RULE_SETUP          */

#define KEY_YY_BREAK     6      /*  Replace YY_BREAK with break         */

#define KEY_YY_END_OF_BUFFER  7   /* delete 
                                      case YY_END_OF_BUFFER:
                                               .....
                                               .....
                                               .....
                                               break;
                                      add 
                                           return 0;
                                           break;
                                    */

#define KEY_YY_FATAL_ERROR  8    /* change
		                                    YY_FATAL_ERROR(
			                                    "fatal flex scanner internal error--no action found" );
			                         to
			                                printf("yylex internal error");
			                       */

#define KEY_FLAG              9   /* delete
                                      /* Flag which is used to allow yywrap()'s to do buffer switches          
                                       * instead of setting up a fresh yyin.  A bit of a hack ...              
                                       */                                                                      
                                   /* static int yy_did_buffer_switch_on_eof;                                  
                                                                         
                                      void yyrestart YY_PROTO(( FILE *input_file ));                           
                                                                         
                                      void yy_switch_to_buffer YY_PROTO(( YY_BUFFER_STATE new_buffer ));       
                                      void yy_load_buffer_state YY_PROTO(( void ));                            
                                      YY_BUFFER_STATE yy_create_buffer YY_PROTO(( FILE *file, int size ));     
                                      void yy_delete_buffer YY_PROTO(( YY_BUFFER_STATE b ));                   
                                      void yy_init_buffer YY_PROTO(( YY_BUFFER_STATE b, FILE *file ));         
                                      void yy_flush_buffer YY_PROTO(( YY_BUFFER_STATE b ));                    
                                      #define YY_FLUSH_BUFFER yy_flush_buffer( yy_current_buffer )             
                                                                         
                                      YY_BUFFER_STATE yy_scan_buffer YY_PROTO(( char *base, yy_size_t size )); 
                                      YY_BUFFER_STATE yy_scan_string YY_PROTO(( yyconst char *yy_str ));       
                                      YY_BUFFER_STATE yy_scan_bytes YY_PROTO(( yyconst char *bytes, int len ));
                                                                         
                                      static void *yy_flex_alloc YY_PROTO(( yy_size_t ));                      
                                      static void *yy_flex_realloc YY_PROTO(( void *, yy_size_t ));            
                                      static void yy_flex_free YY_PROTO(( void * ));                           
                                                                         
                                      #define yy_new_buffer yy_create_buffer                                   
                                                                         
                                      #define yy_set_interactive(is_interactive)
                                      #define yy_set_bol(at_bol)                                           
                                                                         
                                      #define YY_AT_BOL() (yy_current_buffer->yy_at_bol)                       
                                                                         
                                                                         
                                      #define yywrap() 1                                                       
                                      #define YY_SKIP_YYWRAP
                                      typedef unsigned char YY_CHAR;                                                  
                                      FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;                                   
                                      typedef int yy_state_type;                                                      
                                      extern char *yytext;                                                            
                                      #define yytext_ptr yytext                                                       
                                                                                                                      
                                      static yy_state_type yy_get_previous_state YY_PROTO(( void ));                  
                                      static yy_state_type yy_try_NUL_trans YY_PROTO(( yy_state_type current_state ));
                                      static int yy_get_next_buffer YY_PROTO(( void ));                               
                                      static void yy_fatal_error YY_PROTO(( yyconst char msg[] ));           
                                    */

#define KEY_YY_INIT            10   /*  delete
                                        if ( yy_init )
                                        {
                                        	....
                                        }
                                     */

#define KEY_ECHO               11   /* delete 
                                       ECHO;
                                     */

#define KEY_YYTERMINATE        12   /* replace 
                                       yyterminate()
                                       with 
                                       return 0;
                                     */

#define KEY_YY_HOLD_CHAR       13   /* delete yy_hold_char */

#define KEY_YY_C_BUF_P         14   /* replace
                                       static char *yy_c_buf_p = (char *) 0;
                                       with
                                       char *yy_c_buf_p = NULL;
                                     */

#define KEY_YY_START           15   /* replace
                                       static int yy_start = 0;
                                       with
                                       static int yy_start = 1;
                                     */

#define KEY_YY_MATCH           16   /* delete 
                                       yy_match:
                                     */

#define KEY_DO_ACTION          17   /* delete
                                       do_action:
                                     */

#define KEY_LEXICAL            18   /* delete
                                       /* A lexical scanner generated by flex */
                                    /* ......
                                       ......
                                       #define YY_NULL 0
                                     */

#define KEY_TRANSLATE          19   /* delete
                                       #define YY_START ((yy_start - 1) / 2)
                                       #define YYSTATE YY_START
                                     */

#define KEY_SUPPORT_OF         20   /* delete 
                                       *yy_cp = yy_hold_char;
                                     */

#define KEY_INT_YY_INIT        21   /* delete
                                       static int yy_init = 1;
                                     */

#define KEY_YYTEXT_PTR         22   /* replace
                                       yytext_ptr = yy_bp; \
                                       with
                                       yytext = yy_bp; \
                                     */

#define KEY_INTENT             23   /* delete
                                       #define REJECT reject_used_but_not_detected  
                                       #define yymore() yymore_used_but_not_detected
                                       #define YY_MORE_ADJ 0                        
                                       #define YY_RESTORE_YY_MORE_OFFSET            
                                     */

#define PATTERN_NUM     40                                                   



#define R_YYCONST           1  /* replace yyconst with const */
#define R_YY_LOWCASE        2  /* replace yy with atc */
#define R_YY_UPCASE         3  /* replace YY with ATC */
#define R_YY_CHAR           4  /* replace YY_CHAR with uint8 */
#define R_YY_STATE_TYPE     5  /* replace yy_state_type with uint16 */
#define R_SHORT_INT         6  /* replace short int with uint16 */
#define R_UNSIGNED_INT      7  /* replace unsigned int with uint */
#define R_TAB               8  /* replace tab char with 4 spaces */
#define R_CR                9  /* replace cr with cr */
#define R_OTHER_NOT_CHANGE  10 /* other char not changed */

#define MAX_R_NUM           10 
                          
#endif                    