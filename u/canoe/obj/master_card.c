#include <ansi.h>
inherit ITEM;
string money_str(int amount);
int pay_player(object who, int amount);
int help(object me);
void create()
{
        set_name(HIY"大唐金龙卡"NOR, ({"master card","card"}) );
        set_weight(10);
        set("no_get",1);
        set("no_drop","这么贵重的东西怎么能丢掉呢。\n");
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "张");
                set("material", "steal");
                set("long", "
            [1;33m    大唐金龙卡(Master card)[m   
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※               [0;1;37;40m  您可以使用如下命令：[m                    ※
※     取钱：    命令格式：withdraw <数量> <货币单位>   ※
※     取潜能：  命令格式：withdraw <数量> 潜能         ※
※     存钱：    命令格式：deposit  <数量> <货币单位>   ※
※     存潜能：  命令格式：deposit  <数量> 潜能。       ※
※     查询余额：命令格式：bank                         ※
※     [0;1;37;34m西游记2006的通用信用卡，代表着你在本站的信誉。 [m    ※
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
");         
   }
}

void init()
{
        object me = this_object();
        object who = this_player();
        object where = environment();
        if (! me->query("my_owner") &&
             interactive (who))
        me->set("my_owner",who->query("id"));
       if (userp(where))
        {
         if (me->query("owned"))
           {
            if (me->query("owned")!=where->query("id"))
                  call_out("destruct_me",1,where,me);
           }
         else me->set("owned",where->query("id"));
       }
        add_action("do_deposit","deposit");
        add_action("do_withdraw","withdraw");
        add_action("do_check", "bank");
}
int do_deposit(string arg)
{
        string what;
        int amount;
        object what_ob, me;

        me = this_player();
        if( me->is_busy()||me->is_fight())
            return notify_fail("你现在正忙着呢...。\n");
        
        if(arg && sscanf(arg, "%d %s", amount, what)==2)
        {
        if (what=="潜能")
        {
        if (amount < 1||me->query("potential")-me->query("learned_points") < 1)
                  return notify_fail("你想存多少潜能？\n");
        if (amount>me->query("potential")-me->query("learned_points"))
                  return notify_fail("你的潜能不够存。\n");
        me->start_busy(1);
        message_vision(sprintf("$N拿出%s点潜能，存进了"HIY"大唐金龙卡"NOR"。\n",
                       chinese_number(amount)), me);
        me->add("c_potential", amount*9/10);
        me->add("potential",-amount);
        me->save();
        return 1;
        }
        else what_ob = present(what + "_money", me);
        if (!what_ob)  
             return notify_fail("你身上没有带这种钱。\n");
        if (amount < 1)
             return notify_fail("你想存多少" + what_ob->query("name") + "？\n");
        if ((int)what_ob->query_amount() < amount)
             return notify_fail("你带的" + what_ob->query("name") + "不够。\n");
        me->start_busy(1);
        message_vision(sprintf("$N拿出%s%s%s，存进了"HIY"大唐金龙卡"NOR"。\n",
                chinese_number(amount), what_ob->query("base_unit"),
                what_ob->query("name")), me);
        me->add("c_balance", what_ob->query("base_value") * amount);
        if ((int)what_ob->query_amount() == amount)
            destruct(what_ob);
        else
            what_ob->add_amount(-amount);
        me->save();
        return 1;
        }
        else return help(me);
}
 
int do_check()
{
   int total = (int)this_player()->query("c_balance");

   if(!total || total < 0)  {
        this_player()->set("c_balance", 0);
        write("您这张卡上已经没钱了。\n");
   }
   else write("您卡中目前还有" + money_str(total) + "余额。\n");
   total = (int)this_player()->query("c_potential");
   if(!total || total < 0)  {
        this_player()->set("c_potential", 0);
        return notify_fail("您这张卡上没有任何潜能余额。\n");
   }
   write("您卡中目前还有" + chinese_number(total)+ "点潜能余额。\n");
   return 1;
}

int do_withdraw(string arg)
{
        int amount, v, rv;
        string what;
        object me;

        me = this_player();
        if( me->is_busy()||me->is_fight())
            return notify_fail("你现在正忙着呢...。\n");

        if (arg && sscanf(arg, "%d %s", amount, what) == 2)
        {
        if (amount < 1)
                   return notify_fail("你想取多少？\n");
        if(what=="潜能")
        {
        if (amount > me->query("c_potential"))
                     return notify_fail("你卡里的潜能不够取。\n");
        me->start_busy(1);
        me->add("c_potential",-amount);
        me->add("potential",amount);
        me->save();
        message_vision(sprintf("$N从"HIY"大唐金龙卡"NOR"里取出%s点潜能。\n", 
                      chinese_number(amount)),me);
        return 1;
        }
        else if (file_size("/obj/money/" + what + ".c") < 0)
                   return notify_fail("你想取出什么钱？\n");
        what = "/obj/money/" + what;
        if ((v = amount * what->query("base_value")) > me->query("c_balance"))
                   return notify_fail("你卡里的钱不够取。\n");
        if( v < what->query("base_value") ) 
                   return notify_fail("不要这样做，不然。。。！\n");
        me->start_busy(1);
        rv = v;
        v=v-pay_player(me, rv);
        me->add("c_balance",  -v);
        me->save();
        message_vision(sprintf("$N从"HIY"大唐金龙卡"NOR"里取出%s。\n", money_str(v)),
                me);
        return 1;
        }
        else return help(me);
}

string money_str(int amount)
{        
       string output;
        if (amount / 10000) {
                output = chinese_number(amount / 10000) + "两黄金";
                amount %= 10000;
        }
        else
                output = "";
        if (amount / 100) {
                output = output + chinese_number(amount / 100) + "两白银";
                amount %= 100;
        }
        if (amount || sizeof(output)<2)
                return output + chinese_number(amount) + "文铜板";
        return output;
}
int pay_player(object who, int amount)
{
        int v;
        object ob;

        seteuid(getuid());
        if (amount < 1)
                amount = 0;
        if (v = amount / 10000) {
                ob = new("/obj/money/gold");
                ob->set_amount(amount / 10000);
                if(!(ob->move(who)))
                    return amount;
                amount %= 10000;
        }
        if (amount / 100) {
                ob = new("/obj/money/silver");
                ob->set_amount(amount / 100);
                if(!(ob->move(who)))
                    return amount;
                amount %= 100;
        }
        if (amount) {
                ob = new("/obj/money/coin");
                ob->set_amount(amount);
                if(!(ob->move(who)))
                    return amount;
        }
        return 0;
}
int help(object me)
{
write(@HELP
        
取钱：  命令格式：withdraw <数量> <货币单位>
取潜能：命令格式：withdraw <数量> 潜能
存钱：  命令格式：deposit  <数量> <货币单位>
存潜能：命令格式：deposit  <数量> 潜能
查询余额：命令格式：bank 
HELP
    );
    return 1;
}

void destruct_me(object where, object me)
{
  message_vision("千面怪鬼鬼祟祟地拿起$n撒脚就跑，快追！\n",where,me);
  destruct (me);
}
int query_autoload() { return 1; }      
