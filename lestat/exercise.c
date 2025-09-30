// exercise.c

#include <skill.h>

inherit F_CLEAN_UP;
int finishb(object me);

int main(object me, string arg)
{
	int kee_cost, busy_time;
	object where = environment(me);
	
	seteuid(getuid());
	
	if(environment(me)->query("no_fight")  ||
	   environment(me)->query("no_magic") )
	      return notify_fail("安全区内禁止练功。\n");

        if( !arg
        ||      !sscanf(arg, "%d", kee_cost) )
                return notify_fail("你要花多少气练功？\n");

        if( kee_cost <= 0 && me->query_temp("pending/exercising") ){
//                remove_call_out("exec_int");
                call_out("finish", 1, me);
                return 1;
        }
	      
        if (me->is_busy() || me->query_temp("pending/exercising"))
                return notify_fail("你现在正忙着呢。\n");

	if( me->is_fighting() )
		return notify_fail("战斗中不能练内功，会走火入魔。\n");

	if( !stringp(me->query_skill_mapped("force")) )
		return notify_fail("你必须先用 enable 选择你要用的内功心法。\n");

	if( kee_cost < 20 ) return notify_fail("你最少要花 20 点「气」才能练功。\n");

	if( (int)me->query("kee") < kee_cost )
		return notify_fail("你现在的气太少了，无法产生内息运行全身经脉。\n");

	write("你坐下来运气用功，一股内息开始在体内流动。\n");

	busy_time=kee_cost/20;
	me->start_busy(busy_time*2 + 1);
	me->set_temp("pending/exercising", 1);
	call_out("exec_int", 1, me, where, busy_time);	
	return 1;
}
int exec_int(object me, object where, int busy_time)
{
	object wherenow;
	int force_gain;
	wherenow = environment(me);

	if(!me) return 1;
	
	if( wherenow!=where ){
//		remove_call_out("exec_int");
		finishb(me);
		return 1;
	}

	if(!me->query_temp("pending/exercising")) return 1;
	
	if(!living(me)) return finishb(me);

	force_gain = (int)me->query_skill("force",1)/10
		+(int)me->query_con()/3 + random(3);

	if (force_gain < 5) force_gain = 5;
	if (force_gain > 40) force_gain = 40;

	force_gain*=2;

	me->receive_damage("kee", 20);
	me->add("force", force_gain);

        if( (int)me->query("force") > (int)me->query("max_force") * 2) {
                if( (int)me->query("max_force") >=
              (int)me->query_max_force() ) {
                        tell_object(me,"当你的内息遍布全身经脉时却没有功力提升的迹象，似乎内力修为已经遇到了瓶颈。\n");
		}else{
                        tell_object(me, "你的内力增强了！\n");
                        me->add("max_force", 1);

if ( (int)me->query("max_force") > (int)me->query("maximum_force"))
me->set("maximum_force", (int)me->query("max_force"));

		}
                me->set("force", me->query("max_force"));
//                remove_call_out("exec_int");
                call_out("finish", 1, me);
                return 1;
	}
	if (busy_time>1){
		busy_time--;
		call_out("exec_int", 1, me, where, busy_time);
	} else {
		call_out("finish", 1, me);
	}
	return 1;
}
int finishb(object me)
{
	me->delete_temp("pending/exercising");
	me->start_busy(1);
	return 1;
}
int finish(object me)
{
	me->delete_temp("pending/exercising");
	me->start_busy(1);
	tell_object(me, "你行功完毕，吸一口气，缓缓站了起来。\n");
	return 1;
}

int help(object me)
{
        write(@HELP
指令格式 : exercise|dazuo [<耗费「气」的量>]
	   exercise|dazuo 0 ：停止打坐

运气练功，控制体内的气在各经脉间流动，藉以训练人体肌肉骨骼的耐
力、爆发力，并且用内力的形式将能量储备下来。

请参考 help stats
HELP
        );
        return 1;
}

