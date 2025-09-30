#include <ansi.h>

inherit SSERVER;

int perform(object me, object target)
{
        object weapon;
	string msg,str;         
	if( !target ) target = offensive_target(me);

        if( !target
        ||      !target->is_character()
        ||      target->is_corpse()
        ||      target==me)
                return notify_fail("你要对谁施展这一招掌心雷？\n");

        if(!me->is_fighting())
                return notify_fail("掌心雷只能在战斗中使用！\n");
                
	if(me->query("family/family_name")!="火云洞")
           return notify_fail("“掌心雷”只有火云洞门人才可以用！\n");

       if( time()-(int)me->query_temp("xiao_end") < 5 )
          return notify_fail("绝招用多就不灵了！\n"); 
          
        if((int)me->query("force") < 500 )
                return notify_fail("你的内力不够！\n");

        if((int)me->query("kee") < 300 )
                return notify_fail("你的气血不足，没法子施用外功！\n");

        if((int)me->query_skill("moyun-shou", 1) < 70)
                return notify_fail("你的摩云手级别还不够，使用这一招会有困难！\n");

	me->delete("env/brief_message");

	message_vision("\n$N大喝一声“掌心雷”！\n", me);


	me->set_temp("moyunshou_per", 6);
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"));

	me->delete_temp("moyunshou_per");

	me->receive_damage("kee", 50);
	me->add("force", -100);
    if (target->query("eff_kee")<0 || (!living(target) && target->query("kee")<0))  
                       {str=target->name()+HIM"被"+me->name()+"用摩云洞绝学"NOR+HIC"「掌心雷」"+HIM"炸死了！";
	                 message("channel:rumor",HIM"【空穴来风】某人："+str+"\n"NOR,users());
	               }
    me->set_temp("xiao_end",time());
	return 1;
}

