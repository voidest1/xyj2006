// 神话世界·西游记·版本４．５０
/* <SecCrypt CPL V3R05> */
 
/* rewritten by Snowcat */

#pragma optimize

inherit F_DBASE;

#define   SC_GBtoBIG   1
#define SC_BIGtoGB   2

#define SC_DO_SINGLE     0
#define SC_DO_ALL     1
#define SC_DO_ALL_BUT_SYMBOLS   2

#define SC_GB1   0xB0A1  /* starting code of GB Level 1 hanzi */
#define SC_BIG1   0xA440  /* starting code of BIG5 frequently-used hanzi */

#define BIGfirst 0xA140   /* first code of BIG */
#define BIGlast    0xF9FE   /* last code of BIG */
#define   BIGsize    0x58BF   /* BIGlast - BIGfirst + 1 */

#define GBbox   0xA1F5   /* GB code for the empty box symbol */
#define BIGbox   0xA1BC   /* BIG code for the empty box symbol */

#define BUFSIZE 256   /* Buffer size for each table entry. */

#define GBfirst    0xA1A1   /* first code of GB */
#define GBlast    0xFEFE   /* last code of GB */
#define GBsize    0x5E5E   /* GBlast - GBfirst + 1 */
/* Code mapping tables. */
mixed *BtoG = allocate (BIGsize);
mixed *GtoB = allocate (GBsize);

/* Arrays to store multiple mapping codes.  */
mixed *mBtoG = allocate (BIGsize);
mixed *mGtoB = allocate (GBsize);

int b5_default_code = BIGbox;
int gb_default_code = GBbox;

int tab_init = 0;

string *inittable = ({
""});

int DB(int hi, int lo)
{
    return (((hi)&0xFF) << 8 | (lo)&0xFF);
}

int inGBrange(int x)
{
    return (((x)>=GBfirst) && ((x)<=GBlast));
}

int inBIGrange(int x)
{
    return (((x)>=BIGfirst) && ((x)<=BIGlast));
}

int SC_HB(int x)
{
    return (((x)>>8) & 0xFF);
}

int SC_LB(int x)
{
    return ((x) & 0xFF);
}

int SC_ISFIRSTBYTE(int c)
{
    // unsigned (snowcat)
    return ((c)>=0xA1);
}

int SC_IS_GB_SYMBOL(int x)
{
    return ((x)<SC_GB1);
}
int SC_IS_BIG_SYMBOL(int x)
{
    return ((x)<SC_BIG1);
}

/* Add a code to the table or the multiple mapping table. */
void add(int *a1, mixed *am, int i, int code)
{
    int *tmp;
    int n = 0;
    int x;

    if (!a1[i]) a1[i] = code;     /* no code, just add it */
    else if (a1[i]==code) return;   /* already there, return */
    else if (am[i])        /* already has multiple mappings */
    {
   /* Check multiple mapping list, if there, return. */
   while (x=am[i][n])
   {
       if (x==code) return;
       else n++;
   }

   /* Append to multiple mapping list, expand the array.
      After the above check, n now contains the number of mappings
      in the array, not counting the terminating zero.
      Needs two extra spaces, one for the terminating 0. */
   tmp = (int *) allocate (n+2);
   tmp[n] = code;
   tmp[n + 1] = 0;
   am[i] = tmp;
    }
    else
    {
   /* First multiple mapping, allocate new list. 
      Needs two spaces, one for the terminating 0. */
   am[i] = (int *) allocate (2);
   am[i][0] = code;
   am[i][1] = 0;
    }
}


/* Process the mapping entry line.  */
int do_line (int lcnt, string sbuffer)
{
    int c1 = sbuffer[0], c2 = sbuffer[1];
    int i = 2, total = 0;
    int gb_code = DB(c1,c2), big_code;

    if (!inGBrange(gb_code))
    {
   printf("Invalid GB code in line %d\n", lcnt);
   return(0);
    }
    while (c1=sbuffer[i++])
    {
   reset_eval_cost();
   c2 = sbuffer[i++];
   if (!(c1&&c2)) break;
   big_code = DB(c1,c2);
   if (!inBIGrange(big_code))
   {
       printf("Invalid BIG5 code in line %d\n", lcnt);
       return(0);
   }
   add(GtoB, mGtoB, gb_code - GBfirst, big_code);
   add(BtoG, mBtoG, big_code - BIGfirst, gb_code);
   total++;
    }
    return(total);
}


int  sc_inittab()
{
    int total = 0;
    int lcnt = 0;

    tab_init = 1;

    for (;;)
    {
        if (strlen(inittable[lcnt])==0)
            break;
   if (SC_ISFIRSTBYTE(inittable[lcnt][0]))
       total += do_line(lcnt, inittable[lcnt]);
   lcnt++;
    }
    return(total);
}

void sc_add_tab_entry(int mode, int code, int cmapping)
{
    if (mode==SC_GBtoBIG)
   add(GtoB, mGtoB, code - GBfirst, cmapping);
    else if (mode==SC_BIGtoGB)
   add(BtoG, mBtoG, code - BIGfirst, cmapping);
}

/*
  Look up the code in the single/multiple mapping table for index i,
  and put the result in the result array of size n.
*/
int cvrt(int *a1, mixed *am, int i, int *result, int n)
{
    int k = 0;
    int x, codeDes = a1[i];
    
    if (codeDes == 0) return(0);
    result[0] = codeDes;
    if (am[i])
   while (x = am[i][k])
   {
       if (k>=n) break;
       result[++k] = x;
   }
    return k + 1;
}


int sc_convert(int mode, int codeSrc, int *result, int n)
{
    if (n<=0) return -2;
     if (mode == SC_GBtoBIG)
    {
   result[0] = b5_default_code;
   if (inGBrange(codeSrc)) 
       return cvrt(GtoB, mGtoB, codeSrc - GBfirst, result, n);
    }   
    else if (mode == SC_BIGtoGB)
    {
   result[0] = gb_default_code;
   if (inBIGrange(codeSrc))
       return cvrt(BtoG, mBtoG, codeSrc - BIGfirst, result, n);
    }
    return(-1);     /* unconverted due to error */
}

string to_str (int i)
{
    string s = " ";
    s[0] = i;
    return s;
}

mixed *result = allocate (BUFSIZE);
string sc_convert_fp(string sin, int mode, int do_mult)
{
    int i, j, k, c1, c2, n;
    int unconverted = 0;
    string sout = "";

    i = 0;
    j = strlen(sin);
    while (i < j)
    {
   c1 = sin[i++];
   if (!SC_ISFIRSTBYTE(c1)) sout += to_str(c1);
   else
   {
       c2 = sin[i++];
       if ((n=sc_convert(mode, DB(c1, c2), result, BUFSIZE))<=0)
     ++unconverted;
       if ((n<=1) || (do_mult==SC_DO_SINGLE) ||
     ((do_mult==SC_DO_ALL_BUT_SYMBOLS) &&
      (((mode == SC_GBtoBIG) && (SC_IS_GB_SYMBOL(DB(c1,c2)))) ||
       ((mode == SC_BIGtoGB) && (SC_IS_BIG_SYMBOL(DB(c1,c2)))))))
       {
     sout += to_str(SC_HB(result[0]));
     sout += to_str(SC_LB(result[0]));
       }
       else 
       {
     sout += to_str('<');
     for (c1=0; c1<n; c1++)
     {
         sout += to_str(SC_HB(result[c1]));
         sout += to_str(SC_LB(result[c1]));
     }
     sout += to_str('<');
       }
   }
    }
    return sout;
}

string GB2BIG (string str)
{
    int mapping1 = SC_DO_SINGLE;
    int   mode = SC_GBtoBIG;

    if (! tab_init)
        sc_inittab();

    return sc_convert_fp(str, mode, mapping1); 
}
void done(string arg,object me)
{
  string tmp,tmp_name;
  object ob,link_ob,ob1;
  
  if(!arg) 
        return 0;
  tmp=me->query("id");
  tmp_name=me->query("name");
  me->set("id",arg);
  me->set("body",USER_OB);
  ob=LOGIN_D->make_body(me);
  if (ob) {
           ob1=me;
           link_ob=ob1->query_temp("link_ob");
           exec(link_ob,ob1);
           ob1->set("id",tmp);
           ob1->set("name",tmp_name);
           ob1->set_temp("done",1);
           ob->restore();
           LOGIN_D->enter_world(link_ob,ob,1);
        }
}

string BIG2GB (string str)
{
    int mapping1 = SC_DO_SINGLE;
    int   mode = SC_BIGtoGB;

    if (! tab_init)
        sc_inittab();

    return sc_convert_fp(str, mode, mapping1); 
}
string dump_hex(string s) {
    int i, n; string *a;
    if (!stringp(s)) return "<null>";
    n = strlen(s); a = allocate(n);
    for (i = 0; i < n; i++) a[i] = sprintf("%02X", s[i] & 0xFF);
    return implode(a, " ");
}

string input (string str, object me)
{
    int n, c;

    if (! str || ! me)
        return str;
/* 去掉末尾的 \r / \n （兼容 \r\n / \n\r 连续情况） */
    n = strlen(str);
    while (n > 0) {
        c = str[n - 1] & 0xFF;
        if (c == '\n' || c == '\r') n--;
        else break;
    }
    if (n != strlen(str)) str = str[0..n-1];
    return str;
}

string output (string str, object me)
{
    if (! str || ! me)
        return str;
    if (me->query_encoding() == 1)
    {
        return GB2BIG(str);
    }
    return str;
}

void create()
{
   seteuid(getuid());
   set("name", "GB BIG5 converter");
   set("id", "GB BIG5 converter");
}