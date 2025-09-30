#include <ansi.h>
inherit SSERVER;

int perform(object me, object target)
{
        object weapon;
        int damage, sword;
        string msg;

if(me->query("family/family_name")!="蜀山派")
                return notify_fail("「剑气」乃蜀山派不传之密也！\n");


        if( !target && me->is_fighting() ) target = offensive_target(me);

        if( !target
        ||  !target->is_character()
        ||  !me->is_fighting(target) )
                return notify_fail("「剑气」只能对战斗中的对手使用。\n");

        if( !objectp(weapon = me->query_temp("weapon")) 
           || weapon->query("skill_type") != "sword" )
                return notify_fail("你手中无剑，怎能运用「剑气」？！\n");

//        if (weapon->query("unique"))
//                return notify_fail("你手中之剑已是锋利之极，使用「剑气」没有什么作用！\n");

        if (uptime() - me->query_temp("last_pf_time") < 5 ) 
                return notify_fail("你刚使完「剑气」，目前气血翻涌，无法再次运用！\n");
        
 if( (int)me->query_skill("mindsword", 1) < 100 ||
//    if( (int)me->query_skill("mindsword", 1) < 900 ||

           (int)me->query_skill("sword", 1) < 100 ||
            (int)me->query_str() <=25 )
  return notify_fail("你修为还不够，还未能领悟「剑气」！\n");
// return notify_fail("剑气关闭,等待调整！\n");

        if( me->query_skill("zixia-shengong", 1) < 100 )
                return notify_fail("你的内功修为火候未到，施展只会伤及自身！\n");

        if( me->query_skill_mapped("force") != "zixia-shengong")
                return notify_fail("你所用的内功与「剑气」心法相悖！\n");
        if (me->query_skill_mapped("sword") != "mindsword" ||
            me->query_skill_mapped("parry") != "mindsword")
                return notify_fail("「剑气」乃是桃花剑法中的绝招。\n");                                                                                 
      
        if( me->query("max_force") <= 1000 )
                return notify_fail("你的内力修为不足，劲力不足以施展「剑气」！\n");

        if( me->query("force") <= 300 )
                return notify_fail("你的内力不够，劲力不足以施展「剑气」！\n");

        
        message_vision(HIM"\n突然$N脸上紫气隐现，潜运内力将"+weapon->name()+HIM"一抖。这一下使得剑气纵横，竟然声震山谷。不明其理之人，无不骇异！\n\n" NOR, me);
        
        if (random(me->query("combat_exp")) > target->query("combat_exp")/3 )
        {
                me->start_busy(1);

                damage = (int)me->query_skill("mindsword", 1) + (int)me->query_skill("force",1);
                damage = damage/2 + random(damage/2);
                target->receive_damage("kee", damage);
                target->start_busy(2+random(3));
                me->add("force", -100);
                msg = HIR"$n看到$N这气拔千钧的剑气，竟不知如何招架！\n"NOR;
        }
        else
        {
                me->start_busy(random(3));
                msg = HIR"$n不以为动狂笑道:米粒之光,也敢与日月争辉！\n"NOR;
        }
        message_vision(msg, me, target);
        me->set_temp("last_pf_time",uptime());
        return 1;
}


