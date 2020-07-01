

#include "./include/securec.h"
#include <stdio.h>
#include <wchar.h>
#include <assert.h>
#include "sample.h"

#if !defined (COUNTOF)
#define COUNTOF(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define LEN   ( 128 )

void scanfSample();
void sscanfSample( void );
void fscanfSample( void );

void swscanfSample( void );
void vsscanfSample( void );


void memcpySample(void);
void memmoveSample(void);
void memsetSample(void);

void strcpySample(void);
void wcscpySample(void);
void strncpySample(void);

void strcatSample(void);
void strncatSample(void);

void wcscatSample(void);
void wcsncatSample(void);


void strtokSample(void);

void sprintfSample(void);
void swprintfSample(void);
void vsprintfSample(void);

void snprintfSample(void);
void vsnprintfSample(void);


void getsSample(void);

void RunfSamples(void)
{
    scanfSample();
    sscanfSample( );
    fscanfSample(  );
    
    swscanfSample(  );
    vsscanfSample(  );
    
    
    memcpySample();
    memmoveSample();
    memsetSample();
    
    strcpySample();
    wcscpySample();
    strncpySample();
    
    strcatSample();
    strncatSample();
    
    wcscatSample();
    wcsncatSample();
    
    
    strtokSample();
    
    sprintfSample();
    swprintfSample();
    vsprintfSample();
    
    snprintfSample();
    vsnprintfSample();
    
    
    getsSample();
    
}

int mainSample(int argc, char* argv[])
{
    char dest[234];
    wchar_t wc = 'a', w2 = 'b';
    
    /*unsigned int k= -1 / 2 -1;*/

    (void)sprintf_s(dest, 234, "%lc  %lc", wc, w2);
    printf("%s\n\n", dest);
    
    (void)sprintf_s(dest, 234, "%lc ", 123 );
    printf("%s\n", dest);

    vsnprintfSample();
    RunfSamples();

    return 0;
}

void scanfSample()
{
    int      i,     result;
    float    fp;
    char     c,
        s[80];
    wchar_t  wc,
        ws[80];
    
    
    /* scanf_s sample */
    printf("input a integer:\n");
    result =  scanf_s( "%d", &i);
    printf("%d item converted\n", result);


    printf("%s", "input format: %d %f %c %C %s %S\n");
    result = scanf_s( "%d %f %c %C %s %S", &i, &fp, &c, 1,
        &wc, 1, s, COUNTOF(s), ws, COUNTOF(ws) );
    printf( "The number of fields input is %d\n", result );
    printf( "The contents are: %d %f %c %C %s %S\n", i, fp, c,
        wc, s, ws);
    
    /* wscanf_s sample */
    printf("%s", "input format: %d %f %hc %lc %S %ls\n");
    result = wscanf_s( L"%d %f %hc %lc %S %ls", &i, &fp, &c, 2,
        &wc, 1, s, COUNTOF(s), ws, COUNTOF(ws) );
    (void)wprintf( L"The number of fields input is %d\n", result );
    (void)wprintf( L"The contents are: %d %f %C %c %hs %s\n", i, fp,
        c, wc, s, ws);
    
    /* the following is input sample
        71 98.6 h z Byte characters
        36 92.3 y n Wide characters
        
    */
}

void sscanfSample( void )
{
    char  tokenstring[] = "15 12 14...";
    char  s[81];
    char  c;
    int   i;
    float fp;
    
    /* Input various data from tokenstring:*/
    /* max 80 character string:*/
    (void)sscanf_s( tokenstring, "%80s", s, 81 ); 
    (void)sscanf_s( tokenstring, "%c", &c, 1 );  
    (void)sscanf_s( tokenstring, "%d", &i );  
    (void)sscanf_s( tokenstring, "%f", &fp ); 
    
    /* Output the data read*/
    printf( "String    = %s\n", s );
    printf( "Character = %c\n", c );
    printf( "Integer:  = %d\n", i );
    printf( "Real:     = %f\n", fp );
}

void swscanfSample( void )
{
        wchar_t tokenstring[] = L"15 12 14 13...";
        wchar_t s[81];
        wchar_t c;
        int i;
        float fp;
        int ret = 0, totalConvertedItems = 0;
        char ch;
        char str[81]={0};

        /* Input various data from tokenstring: */
        /* max 80 character string: */
        ret = swscanf_s(tokenstring, L"%80s", s, sizeof(s)/sizeof(wchar_t)); 
        totalConvertedItems += ret;

        ret = swscanf_s( tokenstring, L"%lc", &c, sizeof(c) / sizeof(wchar_t) );  
        totalConvertedItems += ret;

        ret = swscanf_s(tokenstring, L"%d", &i); 
        totalConvertedItems += ret;

        ret = swscanf_s(tokenstring, L"%f", &fp); 
        totalConvertedItems += ret;

        printf("total %d items converted\n", totalConvertedItems);

        /* Output the data read */
        (void)wprintf(L"String = %s\n", s);
        (void)wprintf(L"Character = %c\n", c);
        printf("Integer: = %d\n", i);
        printf("Real: = %f\n", fp);

        ret = swscanf_s(tokenstring, L"%80S", str, 80); 
        wprintf(L"%d,%S\n", ret, str);

        ret = swscanf_s(tokenstring, L"%C", &ch, 1);
        wprintf(L"%d,%C\n", ret, ch);

        return 0;
}

void timerSscan(char* format, ...)
{
    va_list arglist;
    int ret;
    char userInput[250] = "2014-01-28_12:15:23,record_temperature, 7 ";
    
    va_start( arglist, format );
    ret = vsscanf_s(userInput, format, arglist ) ;
    printf("convert %d items\n", ret  );
    va_end(arglist);
    
}

void vsscanfSample( void )
{
    char date[20];
    char oper[20];
    int temperature = 0;
    
#ifdef _DEBUG
#define TIMER_SCAN timerSscan
#else
#define TIMER_SCAN (void)
#endif
    
    timerSscan("%19s,%18s,%d", date, 20, oper, 20, &temperature);
}

void fscanfSample( void )
{
    FILE *stream;
    long l;
    float fp;
    char s[81];
    char c;
    
    if( (stream = fopen( "../sampleFiles/fscanf.out", "w+" )) == NULL )
    {
        printf( "The file fscanf.out was not opened\n" );
    }
    else
    {
        fprintf( stream, "%s %ld %f%c", "a-string", 
            65000, 3.14159, 'x' );
        /* Set pointer to beginning of file: */
        if ( fseek( stream, 0L, SEEK_SET ) )
        {
            printf( "fseek failed\n" );
        }
        
        /* Read data back from file: */
        (void)fscanf_s( stream, "%s", s, 81 );
        (void)fscanf_s( stream, "%ld", &l );
        
        (void)fscanf_s( stream, "%f", &fp );
        (void)fscanf_s( stream, "%c", &c, 1 );
        
        /* Output data read: */
        printf( "%s\n", s );
        printf( "%ld\n", l );
        printf( "%f\n", fp );
        printf( "%c\n", c );
        
        fclose( stream );
        
    }
}

void strcpySample(void)
{
    char   str1[LEN];
    char   str2[LEN];
    
    (void)strcpy_s(str1, LEN, "qqweqeqeqeq");
    (void)strcpy_s(str2, LEN, "keep it simple");
    
    (void)strcpy_s(str1, LEN, str2);
}

void strncpySample(void)
{
    errno_t rc;
    char   str1[LEN];
    char   str2[LEN] = "security design department";
    
    rc = strncpy_s(str1, LEN, str2, LEN);
    if (rc != EOK)
    {
        printf("strncpy_s failed\n");
    }
}

/************************************************************************/
/*  typical difference with system function                            */
/************************************************************************/
/*            1 end with ¡¯\0¡¯
    result:
    sysRet = ab34, buffer = ab34.
    iRet = 0, buffer = ab.
*/
void strncpy_comp_Sample(void)
{
    char *p = NULL;
    errno_t rc;
    char   str1[LEN]="1234";
    char   str2[LEN] = "ab";

    p = strncpy(str1, str2, 2);
    printf("sysRet = %s, buffer = %s.\n", p, str1);

    rc = strncpy_s(str1, LEN, str2, 2);
    printf("iRet = %d, buffer = %s.\n", rc, str1);

    if (rc != EOK)
    {
        printf("strncpy_s failed\n");
    }
}
/* 2 check memory overlap*/
void strncpy_comp_Sample2(void)
{
    char *p = NULL;
    errno_t rc;
    char   str1[LEN]="1234";
    char   str2[LEN] = "ab";

    p = strncpy(str1, &str1[2], 2);
    printf("sysRet = %s, buffer = %s.\n", p, str1);

    /* memory overlap*/
    rc = strncpy_s(str1, LEN, &str1[2], 2);
    printf("iRet = %d, buffer = %s.\n", rc, str1);

    if (rc != EOK)
    {
        printf("strncpy_s failed\n");
    }
}

void wcscpySample(void)
{
    errno_t rc;
    wchar_t   str1[LEN] = L"Foo___xxxx";
    wchar_t   str2[LEN] = L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    
    
    rc = wcscpy_s(str1, LEN, str2);    
    if (rc != EOK)
    {
        printf("wcscpy_s failed\n");
    }
}

void strcatSample(void)
{
    char   str1[LEN] = "2012 Labs";
    char   str2[LEN] = "Security Design department";
    errno_t rc;
    
    rc = strcat_s(str1, LEN, str2);
    if (rc != EOK)
    {
        printf("strcat_s failed\n");
    }
}

void strncatSample(void)
{
    char   str1[LEN];
    char   str2[LEN];
    errno_t rc;
    
    (void)strcpy_s(str1, LEN, "aaaaaaaaaa");
    (void)strcpy_s(str2, LEN, "keep it simple");
    
    rc = strncat_s(str1, LEN, str2, 8);
    if (rc != EOK)
    {
        printf("strncat_s failed\n");
    }
}
/*edge*/
void strncat_sample_edge()
{
    char source[4]= "123";
    char buffer[4]={0};
    char buffer2[4]= "a";

    int ret = -1;

    ret = strncat_s(buffer, 4, source, 3);

    //printf("ret=%d,buffer=%c,%c,%c,%c\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    printf("ret=%d,buffer=%s\n",ret, buffer);

    printf("-------------------\n");
    memset(buffer,0,4);
    ret = strncat_s(buffer2, 4, source, 3);
    //printf("ret=%d,buffer=%c,%c,%c,%c\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    printf("ret=%d,buffer=%s\n",ret, buffer2);
    return ;

}

void strtokSample(void)
{
    
    char string1[] ="A string\tof ,,tokens\nand some  more tokens";
    char string2[] = "Another string\n\tparsed at the same time.";
    char seps[]   = " ,\t\n";
    
    char *token1,
        *token2,
        *next_token1,
        *next_token2;
    
    token1 = strtok_s( string1, seps, &next_token1);
    token2 = strtok_s ( string2, seps, &next_token2);
    
    /* While there are tokens in "string1" or "string2" */
    while ((token1 != NULL) || (token2 != NULL))
    {
        /* Get next token:*/
        if (token1 != NULL)
        {
            printf( " %s\n", token1 );
            token1 = strtok_s( NULL, seps, &next_token1);
        }
        if (token2 != NULL)
        {
            printf("        %s\n", token2 );
            token2 = strtok_s (NULL, seps, &next_token2); 
        }
    }
    
}

void sprintfSample(void)
{
    char  buffer[200], c = 'l';
    int   i = 35, j;
    float fp = 1.7320534f;
    
    /* Format and print various data:  */
    j  = sprintf_s( buffer, 200,     "   String:    %s\n", "computer" );
    assert( j > 0);
    j += sprintf_s( buffer + j, 200 - j, "   Character: %c\n", c );
    j += sprintf_s( buffer + j, 200 - j, "   Integer:   %d\n", i );
    j += sprintf_s( buffer + j, 200 - j, "   Real:      %f\n", fp );
    
    printf( "Output:\n%s\ncharacter count = %d\n", buffer, j );
}


void swprintfSample(void)
{
    wchar_t buf[100];
    int len = swprintf_s( buf, 100, L"%d %s", 2012, L"Hello world" );
    printf( "wrote %d characters\n", len );
    
    /* swprintf_s fails because string contains WEOF (\xffff)*/
    /*   len = swprintf_s( buf, 100, L"%s", L"Hello\xffff world" );*/
    /*  printf( "wrote %d characters\n", len );*/
    
}

void dbgPrintf(char* format, ...)
{
    va_list arglist;
    int len;
    char dest[250];
    
    va_start( arglist, format );
    len = vsprintf_s(dest, 250, format, arglist ) ;
    printf("%s   Len: %d\n", dest, len );
    va_end(arglist);

    
}

void vsprintfSample(void)
{
#ifdef _DEBUG
    #define DBG_PRINTF dbgPrintf
#else
    #define DBG_PRINTF (void)
#endif
    
    DBG_PRINTF( "%d %.30s", -23, "file not found!"); /*lint !e505*/
}

void dbgNPrintf(int count, char* format, ...)
{
    va_list arglist;
    int len;
    char dest[250];
    
    va_start( arglist, format );
    len = vsnprintf_s(dest, 250, count, format, arglist ) ;

    printf("%s    Len: %d\n", dest, len );
    va_end(arglist);
}

void vsnprintfSample(void)
{
#ifdef _DEBUG
    #define DBG_NPRINTF dbgNPrintf
#else
    #define DBG_NPRINTF (void)
#endif
    
    DBG_NPRINTF(10, "%d %.30s", -23, "file not found!"); /*lint !e505*/
    
}

void snprintfSample(void)
{
    char dest[10];
    
    int ret = snprintf_s( dest, sizeof(dest), 8, "<<<%d>>>", 121 );
    printf( "    new contents of dest: '%s'   len: %d\n", dest, ret);
    
    ret = snprintf_s( dest, sizeof(dest), 9, "<<<%d>>>", 121 );
    printf( "    new contents of dest: '%s'     len: %d\n", dest, ret );
    
}


void memcpySample(void)
{
    int a1[10], a2[100], i;
    errno_t err;
    wchar_t dest[] = L"Huawei Tech";
    /* Populate a2 with squares of integers*/
    for (i = 0; i < 100; i++)
    {
        a2[i] = i*i;
    }
    
    /* Tell memcpy_s to copy 10 ints (40 bytes), giving */
    /* the size of the a1 array (also 40 bytes).*/
    err = memcpy_s(a1, sizeof(a1), a2, 10 * sizeof (int) );    
    if (err)
    {
        printf("Error executing memcpy_s.\n");
    }
    
    (void)wmemcpy_s(dest, sizeof(dest) / sizeof (wchar_t), L"2012Labs", 8 );
    
}

void memmoveSample(void)
{
    char str[] = "0123456789";
    wchar_t wStr[] = L"0123456789";
    printf("Before: %s\n", str);
    
    /* Move six bytes from the start of the string */
    /* to a new position shifted by one byte. To protect against */
    /* buffer overrun, the secure version of memmove requires the */
    /* the length of the destination string to be specified. */
    
    (void)memmove_s((str + 1), 9, str, 6); 
    
    printf(" After: %s\n", str);
    
    (void)wmemmove_s( (wStr + 1), 9, wStr, 6);
    
}

/*edge*/
void memmove_sample_edge()
{
    char source[10]= "123456";
    char buffer[4]={0};
    char buffer2[4]= "a";

    int ret = -1;

    ret = memmove_s(buffer, 4, source, 3);

    printf("ret=%d,buffer=%c,%c,%c,%c\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    /*printf("ret=%d,buffer=%s\n",ret, buffer);*/

    printf("-------------------\n");
    memset(buffer,0,4);
    ret = memmove_s(buffer, 4, source, 4);
    printf("ret=%d,buffer=%c,%c,%c,%c\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    /*printf("ret=%d,buffer=%s\n",ret, buffer2);*/
    return ;
}

void memsetSample(void)
{
    char buffer[] = "This is a test of the memset function";
    
    printf( "Before: %s\n", buffer );
    (void)memset_s( buffer, sizeof(buffer), '*', 4 );
    printf( "After:  %s\n", buffer );
    
}
/*edge*/
void memset_sample_edge()
{
    char source[10]= "123456";
    char buffer[4]={0};
    char buffer2[4]= "a";

    int ret = -1;

    ret = memset_s(buffer, 4, 1, 3);

    printf("ret=%d,buffer=%d,%d,%d,%d\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    /*printf("ret=%d,buffer=%s\n",ret, buffer);*/

    printf("-------------------\n");
    memset(buffer,0,4);
    ret = memset_s(buffer, 4, 1, 4);
    printf("ret=%d,buffer=%d,%d,%d,%d\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    /*printf("ret=%d,buffer=%s\n",ret, buffer2);*/
    return ;
}

void getsSample(void)
{
    char line[21]; /* room for 20 chars + '\0' */
    (void)gets_s( line, 20 );
    printf( "The line entered was: %s\n", line );

}

void wcscatSample(void)
{
    wchar_t   str1[LEN] = L"2012 Labs";
    wchar_t   str2[] = L"Security Design Department";
    errno_t rc1 = 0, rc2 = 0;
    
    rc1 = wcscat_s(str1, LEN, L" ");
    rc2 = wcscat_s(str1, LEN, str2);
    if (rc1 != EOK || rc2 != EOK)
    {
        printf("wcscat_s failed\n");
    }
}

void wcsncatSample(void)
{
    wchar_t   str1[LEN] = L"2012 Labs";
    wchar_t   str2[] = L"Security Design Department";
    errno_t rc1 = 0, rc2 = 0;
    
    rc1 = wcsncat_s(str1, LEN, L" ", 1);
    rc2 = wcsncat_s(str1, LEN, str2, 15);
    if (rc1 != EOK || rc2 != EOK)
    {
        printf("wcsncat_s failed\n");
    }
}

/************************************************************************/
/*       swprintf api code                                      */
/************************************************************************/
void swprintf_sample()
{
    wchar_t wDest[200] = {0};
    char    *string = "computer";
    wchar_t *wstring = L"Unicode";
    int iRet = -1;

    #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
        /* format strings. */
        /* windows: %S match a single-byte-character string. */
        iRet = swprintf_s( wDest, 200, L"%S,%ls", string, wstring);
        wprintf(L"windows: iRet = %d, wDest = %s.\n", iRet, wDest);

        /* test edge.*/
        wprintf(L"------------------\n");
        iRet = swprintf_s( wDest, 3, L"%S", "123");
        wprintf(L"windows: iRet = %d, wDest = %s.\n",iRet, wDest);
    #else
        iRet = swprintf_s( wDest, 200, L"%s,%ls", string, wstring);
        wprintf(L"linux: iRet = %d, wDest = %ls.\n", iRet, wDest);

        /* test edge.*/
        wprintf(L"------------------\n");
        iRet = swprintf_s( wDest, 3, L"%s", "123");
        wprintf(L"linux: iRet = %d, wDest = %ls.\n", iRet, wDest);
    #endif

    return;
}

/************************************************************************/
/*     vswprintf api code                                           */
/************************************************************************/
void vswprintf_base(wchar_t* format, ... )
{
    va_list args;
    wchar_t wDest[100] = {0};
    int iRet = -1;

    va_start( args, format );

    iRet = vswprintf_s( wDest, 100, format, args );

    #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
        wprintf(L"windows: iRet = %d, wDest = %s.",iRet, wDest);
    #else
        wprintf(L"linux: iRet=%d,wDest=%ls",iRet, wDest);
    #endif

    return;
}
void vswprintf_sample(void)
{
    char   *string = "computer";
    wchar_t    *wstring = L"Unicode";

    #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
        vswprintf_base(L"%S %ls.\n", string, wstring);
    #else
        vswprintf_base(L"%s %ls.\n", string, wstring);
    #endif
}
/*edge*/
void vswprintf_sample_edge(wchar_t* format, ... )
{
    va_list args;
    wchar_t wDest[4] = {0};
    wchar_t wDest2[4] = {0};
    int iRet = -1;

    va_start( args, format );

    #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
        iRet = vswprintf_s( wDest, 3, format, args );
        wprintf(L"iRet = %d, wDest = %s.\n",iRet, wDest);

        wprintf(L"-------------------\n");
        iRet = vswprintf_s( wDest2, 4, format, args );
        wprintf(L"iRet = %d, wDest = %s.\n",iRet, wDest2);
    #endif

    return;
}
int test_vswprintf_sample_edge()
{
    vswprintf_sample_edge(L"%S", "123");
    return 0;
}


/************************************************************************/
/*         sprintf                                                      */
/************************************************************************/
void sprintf_sample()
{
    char buffer[200]={0};
    char    *string = "computer";
    wchar_t *wstring = L"Unicode";
    int iRet = -1;

    /* format strings. */
    iRet = sprintf_s( buffer, 200, "%s,%ls", string, wstring);
    printf("iRet = %d, buffer = %s.\n",iRet, buffer);

    /* test edge.*/
    printf("------------------\n");
    iRet = sprintf_s( buffer, 3, "%s", "123");
    printf("iRet = %d, buffer = %s.\n", iRet, buffer);

    return;
}

/************************************************************************/
/* snprintf                                                             */
/************************************************************************/
void snprintf_sample()
{
    char buffer[200]={0};
    char    *string = "computer";
    wchar_t *wstring = L"Unicode";

    /* format strings. */
    int iRet = snprintf_s( buffer, 200, 20, "%s,%ls", string, wstring);
    printf("iRet = %d, buffer = %s.\n", iRet, buffer);

    /* test edge.*/
    printf("------------------\n");
    iRet = snprintf_s( buffer, 3, 3, "%s", "123");
    printf("iRet = %d, buffer = %s.\n", iRet, buffer);

    return;
}

/************************************************************************/
/* vsnprintf                                                           */
/************************************************************************/
void vsnprintf_base(char * format, ...)
{
    va_list args;
    char buffer[100];
    int iRet = -1;

    va_start(args, format);
    iRet = vsnprintf_s(buffer, 100, 20, format, args);
    printf("iRet = %d, buffer = %s.\n", iRet, buffer);

    return;
}
void vsnprintf_sample(void)
{
    char    *string = "computer";
    wchar_t *wstring = L"Unicode";

    /* format strings. */
    vsnprintf_base("%s,%ls", string, wstring);

    /* test edge.*/
    printf("------------------\n");
    vsnprintf_base("%s", "12345678901234567890a");

    return;
}

/*edge test*/
void vsnprintf_edge_sample(char * format, ...)
{
    char buffer[4]={0};
    va_list args;
    int iRet = -1;

    va_start(args, format);
    iRet = vsnprintf_s(buffer, 3, 3, format, args);

    printf("iRet=%d,buffer=%c,%c,%c,%c\n",iRet, buffer[0],buffer[1],buffer[2],buffer[3]);
    printf("iRet=%d,buffer=%s\n",iRet, buffer);

    printf("-------------------\n");
    memset(buffer,0,4);
    iRet = vsnprintf_s(buffer, 4, 3, format, args);
    printf("iRet=%d,buffer=%c,%c,%c,%c\n",iRet, buffer[0],buffer[1],buffer[2],buffer[3]);
    printf("iRet=%d,buffer=%s\n",iRet, buffer);
    return ;

}

/************************************************************************/
/*         vsprintf                                                     */
/************************************************************************/
void vsprintf_base(char * format, ...)
{
    va_list args;
    char buffer[100];
    int iRet = -1;

    va_start(args, format);

    iRet = vsprintf_s(buffer, 20, format, args);
    printf("iRet = %d, buffer = %s.\n",iRet, buffer);
    
    return;
}
int vsprintf_sample()
{
    char   *string = "computer";
    wchar_t    *wstring = L"Unicode";

    /* format strings. */
    vsprintf_base("%s,%ls", string, wstring);

    /* test edge.*/
    printf("------------------\n");
    vsprintf_base("%s", "12345678901234567890a");

    return 0;
}

/*edge*/
void vsprintf_sample_edge(char * format, ...)
{
    char buffer[4]={0};
    va_list args;
    int ret = -1;

    va_start(args, format);
    ret = vsprintf_s(buffer, 3, format, args);

    printf("ret=%d,buffer=%c,%c,%c,%c\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    printf("ret=%d,buffer=%s\n",ret, buffer);

    printf("-------------------\n");
    memset(buffer,0,4);
    ret = vsprintf_s(buffer, 4, format, args);
    printf("ret=%d,buffer=%c,%c,%c,%c\n",ret, buffer[0],buffer[1],buffer[2],buffer[3]);
    printf("ret=%d,buffer=%s\n",ret, buffer);
    return ;

}
int test_vsprintf_sample_edge()
{
        vsprintf_sample_edge("%s", "123");
        return 0;
}


void outputSample(void)
{
    #define DEST_BUFFER_SIZE  300

    char buffer[DEST_BUFFER_SIZE]={0};

    char *string = "computer";
    wchar_t *wstring = L"Unicode";

    // Display strings
    printf(" printf:%s,%hs,%S,%ls.\n", string, string, wstring, wstring);

    #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
    wprintf(L"win wprintf:%S,%hs,%s,%ls.\n", string, string, wstring, wstring);
    #else
    wprintf(L"wprintf:%s,%hs,%S,%ls.\n", string, string, wstring, wstring);
    #endif
    
    return;
}

void inputSample( void )
{
    int   i, result;
    float fp;
    char  c, s[81];
    wchar_t wc, ws[81];

    printf( "Enter an int, a float, two chars and two strings\n");

    result = scanf( "%d %f %c %lc %s %ls", &i, &fp, &c, &wc, s, ws );
    printf( "The number of fields input is %d\n", result );
    printf( "The contents are: %d %f %c %lc %s %ls\n", i, fp, c, wc, s, ws);

    wprintf( L"Enter an int, a float, two chars and two strings\n");
    #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
        result = wscanf( L"%d %f %C %lc %S %ls", &i, &fp, &c, &wc, s, ws );
        wprintf( L"result = %d\n", result );
        wprintf( L"The contents are: %d %f %C %lc %S %ls\n", i, fp, c, wc, s, ws);
    #else
        result = wscanf( L"%d %f %c %lc %s %ls", &i, &fp, &c, &wc, s, ws );
        wprintf( L"\nresult = %d\n", result );
        wprintf( L"The contents are: %d %f %c %lc %s %ls\n", i, fp, c, wc, s, ws);
    #endif

    return;
}

/************************************************************************/
/*             fwscanf                                                 */
/************************************************************************/
int fwscanf_sample()
{
    FILE *stream;
    long l;
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    if( (stream = fopen("./fscanf.out", "w+" )) == NULL)
    {
        wprintf(L"The file fscanf.out was not opened\n");
    }
    else
    {
        l = fwrite(L"12 3.14159aString bHello", 80 * 2, 1, stream);
        /* Set pointer to beginning of file: */
        if (fseek( stream, 0L, SEEK_SET))
        {
            wprintf(L"fseek failed\n");
        }

        /* Read data : */
        iRet =fwscanf_s(stream, L"%d", &i);
        wprintf(L"iRet = %d, %ld\n", iRet, i);

        iRet =fwscanf_s(stream, L"%f", &fp);
        wprintf(L"iRet = %d, %f\n", iRet, fp);


        #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
            iRet =fwscanf_s(stream, L"%C", &c, 1);
            wprintf(L"iRet = %d, %C\n", iRet, c); 

            iRet =fwscanf_s(stream, L"%S", string, sizeof(string));
            wprintf(L"iRet = %d, %S\n", iRet, string);
        #else
            iRet =fwscanf_s(stream, L"%c", &c, 1);
            wprintf(L"iRet = %d, %c\n", iRet, c); 

            iRet =fwscanf_s(stream, L"%s", string, sizeof(string));
            wprintf(L"iRet = %d, %s\n", iRet, string);
        #endif

        iRet =fwscanf_s(stream, L" %lc", &wc, 1);
        wprintf(L"iRet = %d, %lc\n", iRet, wc);

        iRet =fwscanf_s(stream, L"%ls", wString, sizeof(wString)/sizeof(wchar_t));
        wprintf(L"iRet = %d, %ls\n", iRet, wString);

        fclose(stream);
    }
    return 0;
}

/************************************************************************/
/*         swscanf                                                      */
/************************************************************************/
int swscanf_sample()
{
    FILE *stream;
    wchar_t tokenstring[] = L"12 14 15...";
    long l;
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;
   
    iRet =swscanf_s(tokenstring, L"%d", &i);
    wprintf(L"iRet = %d, %ld\n", iRet, i);

    iRet =swscanf_s(tokenstring, L"%f", &fp);
    wprintf(L"iRet = %d, %f\n", iRet, fp);

    #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
        iRet =swscanf_s(tokenstring, L"%C", &c, 1);
        wprintf(L"iRet = %d, %C\n", iRet, c); 

        iRet =swscanf_s(tokenstring, L"%S", string, sizeof(string));
        wprintf(L"iRet = %d, %S\n", iRet, string);
    #else
        iRet =swscanf_s(tokenstring, L"%c", &c, 1);
        wprintf(L"iRet = %d, %c\n", iRet, c); 

        iRet =swscanf_s(tokenstring, L"%s", string, sizeof(string));
        wprintf(L"iRet = %d, %s\n", iRet, string);
    #endif

    iRet =swscanf_s(tokenstring, L" %lc", &wc, 1);
    wprintf(L"iRet = %d, %lc\n", iRet, wc);

    iRet =swscanf_s(tokenstring, L"%ls", wString, sizeof(wString)/sizeof(wchar_t));
    wprintf(L"iRet = %d, %ls\n", iRet, wString);

    return 0;
}

/************************************************************************/
/* vfwscanf                                                            */
/************************************************************************/
int vfwscanf_base(FILE* f, const wchar_t* format, ...)
{
    va_list args;
    int ret = 0;

    va_start( args, format );
    ret = vfwscanf_s(f, format, args ) ;
    va_end(args);
    return ret;

}
void vfwscanf_sample(void)
{
    FILE *stream;
    long l;
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    if((stream = fopen("./fscanf.out", "wb+")) == NULL)
    {
        wprintf("The file fscanf.out was not opened\n");
    }
    else
    {

        l = fwrite(L"12 3.14159aString bHello", 80 * 2, 1, stream);
        /* Set pointer to beginning of file: */
        if (fseek(stream, 0L, SEEK_SET ))
        {
            wprintf(L"fseek failed\n");
        }

        /* Read data : */
        iRet =vfwscanf_base(stream, L"%d", &i);
        wprintf(L"iRet = %d, %ld\n", iRet, i);

        iRet =vfwscanf_base(stream, L"%f", &fp);
        wprintf(L"iRet = %d, %f\n", iRet, fp);

        #if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
            iRet =vfwscanf_base(stream, L"%C", &c, 1);
            wprintf(L"iRet = %d, %C\n", iRet, c); 

            iRet =vfwscanf_base(stream, L"%S", string, sizeof(string));
            wprintf(L"iRet = %d, %S\n", iRet, string);
        #else
            iRet =vfwscanf_base(stream, L"%c", &c, 1);
            wprintf(L"iRet = %d, %c\n", iRet, c); 

            iRet =vfwscanf_base(stream, L"%s", string, sizeof(string));
            wprintf(L"iRet = %d, %s\n", iRet, string);
        #endif

        iRet =vfwscanf_base(stream, L" %lc", &wc, 1);
        wprintf(L"iRet = %d, %lc\n", iRet, wc);

        iRet =vfwscanf_base(stream, L"%ls", wString, sizeof(wString)/sizeof(wchar_t));
        wprintf(L"iRet = %d, %ls\n", iRet, wString);

        fclose(stream);
    }
    return;
}

/************************************************************************/
/*        vswscanf                                                  */
/************************************************************************/
int vswscanf_base(const wchar_t* str, const wchar_t* format, ...)
{
    va_list args;
    int ret;

    va_start( args, format );
    ret = vswscanf_s(str, format, args) ;
    return ret;
}
int vswscanf_sample()
{
    FILE *stream;
    long l;
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    /* Read data : */
    iRet =vswscanf_base(L"12", L"%d", &i);
    wprintf(L"iRet = %d, %ld\n", iRet, i);

    iRet =vswscanf_base(L"3.14159", L"%f", &fp);
    wprintf(L"iRet = %d, %f\n", iRet, fp);


#if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
    iRet =vswscanf_base(L"a", L"%C", &c, 1);
    wprintf(L"iRet = %d, %C\n", iRet, c); 

    iRet =vswscanf_base(L"String", L"%S", string, sizeof(string));
    wprintf(L"iRet = %d, %S\n", iRet, string);
#else
    iRet =vswscanf_base(L"a", L"%c", &c, 1);
    wprintf(L"iRet = %d, %c\n", iRet, c); 

    iRet =vswscanf_base(L"String", L"%s", string, sizeof(string));
    wprintf(L"iRet = %d, %s\n", iRet, string);
#endif

    iRet =vswscanf_base(L"b", L" %lc", &wc, 1);
    wprintf(L"iRet = %d, %lc\n", iRet, wc);

    iRet =vswscanf_base(L"Hello", L"%ls", wString, sizeof(wString)/sizeof(wchar_t));
    wprintf(L"iRet = %d, %ls\n", iRet, wString);

    return 0;
}

int wscanf_edge_sample()
{
    int result;
    char buffer[4]={0};

    result = wscanf_s(L"%S",buffer,4);
    printf("result=%d buffer=%s.\n", result,buffer);
    return 0;
}

/************************************************************************/
/*        vwscanf                                                  */
/************************************************************************/
int vwscanf_base(const wchar_t* format, ...)
{
    va_list args;
    int ret;

    va_start( args, format );
    ret = vwscanf_s(format, args) ;
    return ret;
}
int vwscanf_sample()
{
    FILE *stream;
    long l;
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    /* Read data : */
    wprintf(L"Please input a integer:");
    iRet =vwscanf_base(L"%d", &i);
    wprintf(L"iRet = %d, %ld\n", iRet, i);

    wprintf(L"Please input a float:");
    iRet =vwscanf_base(L"%f", &fp);
    wprintf(L"iRet = %d, %f\n", iRet, fp);


#if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
    wprintf(L"Please input a char:");
    iRet =vwscanf_base(L"%C", &c, 1);
    wprintf(L"iRet = %d, %C\n", iRet, c); 

    wprintf(L"Please input a string:");
    iRet =vwscanf_base(L"%S", string, sizeof(string));
    wprintf(L"iRet = %d, %S\n", iRet, string);
#else
    wprintf(L"Please input a char:");
    iRet =vwscanf_base(L"%c", &c, 1);
    wprintf(L"iRet = %d, %c\n", iRet, c); 

    wprintf(L"Please input a string:");
    iRet =vwscanf_base(L"%s", string, sizeof(string));
    wprintf(L"iRet = %d, %s\n", iRet, string);
#endif

    wprintf(L"Please input a char:");
    iRet =vwscanf_base(L" %lc", &wc, 1);
    wprintf(L"iRet = %d, %lc\n", iRet, wc);

    wprintf(L"Please input a string:");
    iRet =vwscanf_base(L"%ls", wString, sizeof(wString)/sizeof(wchar_t));
    wprintf(L"iRet = %d, %ls\n", iRet, wString);

    return 0;
}


/************************************************************************/
/*        wscanf                                                  */
/************************************************************************/
int wscanf_sample()
{
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    /* Read data : */
    wprintf(L"Please input a integer:");
    iRet =wscanf_s(L"%d", &i);
    wprintf(L"iRet = %d, %ld\n", iRet, i);

    wprintf(L"Please input a float:");
    iRet =wscanf_s(L"%f", &fp);
    wprintf(L"iRet = %d, %f\n", iRet, fp);


#if(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
    wprintf(L"Please input a char:");
    iRet =wscanf_s(L"%C", &c, 1);
    wprintf(L"iRet = %d, %C\n", iRet, c); 

    wprintf(L"Please input a string:");
    iRet =wscanf_s(L"%S", string, sizeof(string));
    wprintf(L"iRet = %d, %S\n", iRet, string);
#else
    wprintf(L"Please input a char:");
    iRet =wscanf_s(L"%c", &c, 1);
    wprintf(L"iRet = %d, %c\n", iRet, c); 

    wprintf(L"Please input a string:");
    iRet =wscanf_s(L"%s", string, sizeof(string));
    wprintf(L"iRet = %d, %s\n", iRet, string);
#endif

    wprintf(L"Please input a char:");
    iRet =wscanf_s(L" %lc", &wc, 1);
    wprintf(L"iRet = %d, %lc\n", iRet, wc);

    wprintf(L"Please input a string:");
    iRet =wscanf_s(L"%ls", wString, sizeof(wString)/sizeof(wchar_t));
    wprintf(L"iRet = %d, %ls\n", iRet, wString);

    return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int fscanf_sample()
{
    FILE *stream;
    long l;
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    if( (stream = fopen("./fscanf.out", "w+" )) == NULL)
    {
        printf("The file fscanf.out was not opened\n");
    }
    else
    {
        fprintf(stream, "%d %f%c %s %c %s", 12, 3.14159, 'a', "String", 'b', "Hello");
        /* Set pointer to beginning of file: */
        if (fseek( stream, 0L, SEEK_SET ))
        {
            fclose(stream);
            printf("fseek failed\n");
            return 0;
        }

        /* Read data : */
        iRet =fscanf_s(stream, "%d", &i);
        printf("iRet = %d, %ld\n", iRet, i);

        iRet =fscanf_s(stream, "%f", &fp);
        printf("iRet = %d, %f\n", iRet, fp);

        iRet =fscanf_s(stream, "%c", &c, 1);
        printf("iRet = %d, %c\n", iRet, c); 

        iRet =fscanf_s(stream, "%s", string, sizeof(string));
        printf("iRet = %d, %s\n", iRet, string);

        iRet =fscanf_s(stream, " %lc", &wc, 1);
        printf("iRet = %d, %lc\n", iRet, wc);

        iRet =fscanf_s(stream, "%ls", wString, sizeof(wString)/sizeof(wchar_t));
        printf("iRet = %d, %ls\n", iRet, wString);

        fclose(stream);
    }
    return 0;
}

int scanf_sample()
{
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    /* Read data : */
    printf("Please input a integer:");
    iRet =scanf_s("%d", &i);
    printf("iRet = %d, %ld\n", iRet, i);

    printf("Please input a float:");
    iRet =scanf_s("%f", &fp);
    printf("iRet = %d, %f\n", iRet, fp);

    printf("Please input a char:");
    iRet =scanf_s("%c", &c, 1);
    printf("iRet = %d, %c\n", iRet, c); 

    printf("Please input a string:");
    iRet =scanf_s("%s", string, sizeof(string));
    printf("iRet = %d, %s\n", iRet, string);

    printf("Please input a char:");
    iRet =scanf_s(" %lc", &wc, 1);
    printf("iRet = %d, %lc\n", iRet, wc);

    printf("Please input a string:");
    iRet =scanf_s("%ls", wString, sizeof(wString)/sizeof(wchar_t));
    printf("iRet = %d, %ls\n", iRet, wString);

    return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int vfscanf_base(const FILE* f, const char* format, ...)
{
    va_list args;
    int ret;

    va_start(args, format);
    ret = vfscanf_s(f, format, args) ;
    return ret;
}
int vfscanf_sample()
{
    FILE *stream;
    long l;
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    if( (stream = fopen("./fscanf.out", "w+" )) == NULL)
    {
        printf("The file fscanf.out was not opened\n");
    }
    else
    {
        fprintf(stream, "%d %f%c %s %c %s", 12, 3.14159, 'a', "String", 'b', "Hello");
        /* Set pointer to beginning of file: */
        if (fseek( stream, 0L, SEEK_SET ))
        {
            fclose(stream);
            printf("fseek failed\n");
            return 0;
        }

        /* Read data : */
        iRet =vfscanf_base(stream, "%d", &i);
        printf("iRet = %d, %ld\n", iRet, i);

        iRet =vfscanf_base(stream, "%f", &fp);
        printf("iRet = %d, %f\n", iRet, fp);

        iRet =vfscanf_base(stream, "%c", &c, 1);
        printf("iRet = %d, %c\n", iRet, c); 

        iRet =vfscanf_base(stream, "%s", string, sizeof(string));
        printf("iRet = %d, %s\n", iRet, string);

        iRet =vfscanf_base(stream, " %lc", &wc, 1);
        printf("iRet = %d, %lc\n", iRet, wc);

        iRet =vfscanf_base(stream, "%ls", wString, sizeof(wString)/sizeof(wchar_t));
        printf("iRet = %d, %ls\n", iRet, wString);

        fclose(stream);
    }
    return 0;
}

/************************************************************************/
/*     vscanf                                                          */
/************************************************************************/
int vscanf_base(const char* format, ...)
{
    va_list args;
    int ret;
    char strtmp[100] = {0};

    va_start(args, format);
    ret = vscanf_s(format, args) ;
    return ret;
}
int vscanf_sample()
{
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    /* Read data : */
    printf("Please input a integer:");
    iRet =vscanf_base("%d", &i);
    printf("iRet = %d, %ld\n", iRet, i);

    printf("Please input a float:");
    iRet =vscanf_base("%f", &fp);
    printf("iRet = %d, %f\n", iRet, fp);

    printf("Please input a char:");
    iRet =vscanf_base("%c", &c, 1);
    printf("iRet = %d, %c\n", iRet, c); 

    printf("Please input a string:");
    iRet =vscanf_base("%s", string, sizeof(string));
    printf("iRet = %d, %s\n", iRet, string);

    printf("Please input a char:");
    iRet =vscanf_base(" %lc", &wc, 1);
    printf("iRet = %d, %lc\n", iRet, wc);

    printf("Please input a string:");
    iRet =vscanf_base("%ls", wString, sizeof(wString)/sizeof(wchar_t));
    printf("iRet = %d, %ls\n", iRet, wString);

    return 0;
}

/************************************************************************/
/*         vsscanf                                                     */
/************************************************************************/
int vsscanf_base(const char* str, const char* format, ...)
{
    va_list args;
    int ret;

    va_start( args, format );
    ret = vsscanf_s(str, format, args ) ;
    return ret;
}
int vsscanf_sample()
{
    int i;
    float fp;
    char string[81];
    wchar_t wString[81];
    char c;
    wchar_t wc;
    int iRet = -1;

    /* Read data : */
    iRet =vsscanf_base("12", "%d", &i);
    printf("iRet = %d, %ld\n", iRet, i);

    iRet =vsscanf_base("3.14159", "%f", &fp);
    printf("iRet = %d, %f\n", iRet, fp);

    iRet =vsscanf_base("a", "%c", &c, 1);
    printf("iRet = %d, %c\n", iRet, c); 

    iRet =vsscanf_base("String", "%s", string, sizeof(string));
    printf("iRet = %d, %s\n", iRet, string);

    iRet =vsscanf_base("b", " %lc", &wc, 1);
    printf("iRet = %d, %lc\n", iRet, wc);

    iRet =vsscanf_base("Hello", "%ls", wString, sizeof(wString)/sizeof(wchar_t));
    printf("iRet = %d, %ls\n", iRet, wString);

    return 0;
}
