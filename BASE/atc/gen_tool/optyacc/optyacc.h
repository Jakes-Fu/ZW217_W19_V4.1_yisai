#ifndef OPT_YACC_H
#define OPT_YACC_H

#define KEY_PREPROCESSOR_LINE   1  /* Delete 
                                      #line ..
                                      */

#define KEY_YYDEBUG             2  /* Delete 
                                      #ifdef YYDEBUG
                                      ....
                                      #endif
                                      */

#define KEY_MICRO_BEGIN         3  /* #ifndef or #ifdef or #if */

#define KEY_ENDIF               4  /* #endif */

#define KEY_DEF_YYPURE          5  /* Delete 
                                      #ifdef YYPURE
                                      ....
                                      #endif
                                      */

#define KEY_YYLSP_NEEDED        6  /* Delete
                                      #ifdef YYLSP_NEEDED
                                      ....
                                      #endif
                                      */

#define KEY_YYSSP_YYSS          7  /* Delete
                                      if (yyssp >= yyss + yystacksize - 1)
                                      {
                                      .....
                                      }
                                      */

#define KEY_YYERRSTATUS         8  /* Delete
                                      if (! yyerrstatus)
                                      {
                                      ....
                                      }
                                      yyerrlab1:
                                      ....
                                      yyerrdefault:
                                      ....
                                      yyerrpop:
                                      ...
                                      yyerrhandle:
                                      .....
                                      */

#define KEY_DEFINE_YYACCEPT     9  /* Delete MICRO
                                      YYACCEPT
                                      YYABORT
                                      YYERROR
                                      YYFAIL
                                      YYRECOVERING
                                      YYBACKUP
                                      YYTERROR
                                      YYERRCODE
                                      */

#define KEY_BISON               10 /* Delete the definition of MICRO
                                      YYSTACK_ALLOC
                                      */

#define KEY_YYMAXDEPTH          11 /* Delete 
                                      MICRO YYMAXDEPTH
                                      __yy_memcpy
                                      YYPARSE_PARAM
                                      Change yyparse(YYPARASE_PARAM_ARG) 
                                      with int yyparse(void)
                                      */

#define KEY_YYACCEPTLAB         12 /* Delete
                                      yyacceptlab:
                                      ....
                                      yyabortlab:
                                      ....
                                      */

#define KEY_YYNERRS             13 /* Delete use of variable yynerrs */

#define KEY_YYERRSTATUS1        14 /* Delete use of variable yyerrstatus */

#define KEY_INT_YYLEN           15 /* Add 
                                      yy_c_buf_p = yybuf;
                                      */

#define MAX_R_NUM               6

#define R_YYACCEPT              1  /* Replace YYACCEPT with return 0 */
#define R_YY_LOWCASE            2  /* Replace yy with atc */
#define R_YY_UPCASE             3  /* Replace YY with ATC */
#define R_TAB                   4  /* Replace tab with 4 spaces */
#define R_CR                    5  
#define R_OTHER_NOT_CHANGE      6  /* Other char not changed */

#endif
