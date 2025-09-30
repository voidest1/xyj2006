//【龙形搏击】dragonfight.c
// cglaem...12/19/96.
//meipai skill(only dragon related NPCs can use it)
// tomcat 修改
inherit SKILL;
#include <ansi.h>

string* skill_head =({
        "$N纵身跃起",
        "$N似退反进",
        "$$N一旋身",
        "$N忽发龙吟，使出一招",
        "$N一个筋斗翻在半空",
        "$N身形一长",
});

string* skill_tail =({
        "，双掌以排山倒海之势击向$n。",
        "，对准$n一把腰间抓了过去",
        "，双臂如铁铸一般扫向$n",
        "，双臂一阵横扫，直逼$n得连连倒退",
        "，直逼的$n节节后退。",
        "，全身劲气疾卷$n",
});

mapping *action = ({
	([	"action":
"$N纵身跃起，一招「飞龙在天」，双掌以排山倒海之势击向$n的$l",
		"dodge":		-10,
		"parry":		-10,
		"force":		150,
		"damage_type":		"瘀伤",
		"weapon" :		"双掌",
		"parry_msg":	"只听见「锵」一声，被$N劈手格开",
	]),
	([	"action":
"$N似退反进，突然欺身向前，五指如钩，对准$n的$l一把抓了过去",
		"dodge":		10,
		"parry":		-20,
		"force":		100,
		"damage_type":		"抓伤",
		"weapon":		"右手五指",
                "parry_msg":    "只听见「锵」一声，被$N劈手格开",
	]),
	([	"action":
"只见$N一旋身，双腿如铁棍一般扫向$n。这一招「神龙摆尾」当真是防不胜防",
		"dodge":		-10,
		"parry":		-20,
		"force":		120,
		"damage_type":		"砸伤",
		"weapon":		"双腿",
                "parry_msg":    "只听见「锵」一声，被$N劈手格开",
	]),
	([	"action":
"$N忽发龙吟，使出一招「龙战于野」，啸声中和身而上，向$n直撞而来",
		"dodge":		10,
		"parry":		-10,
		"force":		100,
		"damage_type":		"撞伤",
		"weapon":		"全身",
                "parry_msg":    "只听见「锵」一声，被$N劈手格开",
	]),
            ([	"action":
"只见$N一个筋斗翻在半空，一招「龙腾四海」将发未发，但是全身劲气疾卷$n",
		"dodge":		0,
		"parry":		-20,
		"force":		150,
		"damage_type":		"内伤",
		"weapon":		"劲气",
                "parry_msg":    "只听见「锵」一声，被$N劈手格开",
	]),
            ([	"action":
"$N身形一长，朝$n劈面就是一拳。这招「苍龙出海」势若奔雷，锐利无比",
		"dodge":		10,
		"parry":		-30,
		"force":		180,
		"damage_type":		"瘀伤",
		"weapon":		"拳头",
                "parry_msg":    "只听见「锵」一声，被$N劈手格开",
	]),
});

int valid_learn(object me)
{
	if( (int)me->query_skill("dragonforce",1) < (int)me->query_skill("dragonfight",1)/2)
		return notify_fail("你的龙神心法还不够精湛，无法领会更深层的龙形搏击！\n");


	if( me->query_temp("weapon") || me->query_temp("secondary_weapon") )
		return notify_fail("练龙形博击必须空手。\n");
	return 1;
}

int valid_enable(string usage)
{
	return usage=="unarmed";
}


int practice_skill(object me)
{
        if( (int)me->query_skill("dragonforce",1) < (int)me->query_skill("dragonfight",1)/2)
                return notify_fail("你的龙神心法还不够精湛，无法领会更深层的龙形搏击！\n");

	if( (int)me->query("sen") < 50)
		return notify_fail("你的精神无法集中了，休息一下再练吧。\n");

	if( (int)me->query("kee") < 50 )
		return notify_fail("你现在手足酸软，休息一下再练吧。\n");

	if( (int)me->query("force") < 20 )
		return notify_fail("你的内力不够了。\n");

	me->receive_damage("kee", 30);
	me->add("force", -20);

	return 1;
}

mapping query_action(object me, object weapon)
{
int i;
mapping* m_actions;
m_actions=me->query("skill_dragonfight");
if( !me->query_temp("HTH_perform")) {
if(me->query("skill_dragonfight")==0)
return action[random(6)];
if(random(2)) return action[random(6)];
return m_actions[random(sizeof(m_actions))];
}else{ return action[6];}
  return action[random(sizeof(action))];
}


string perform_action_file(string func)
{
	return CLASS_D("dragon") + "/dragonfight/" + func;
}

void skill_improved(object me)
{
   int m_skill=me->query_skill("dragonfight",1);

   if(m_skill>200&&m_skill%10==0){
     tell_object(me,HIW"你对龙形搏击的领悟加深了，你的浑天锤进入了一个新的境界！\n"NOR);
     if(random(me->query("kar"))<20){
        tell_object(me,HIW"然而你的心中居然产生了一种失落感！\n"NOR);
        return;
     }
     if(random(me->query("int"))<20){
       tell_object(me,HIW"然而你的心中升起一种惆怅的感觉，仿佛有一丝重要的东西没有抓住。\n"NOR);
       return;
     }
     tell_object(me,HIW"你突然福至心灵，对龙形搏击领悟出了一招新的用法！\n"NOR);
     tell_object(me,"请为这招取一个名字：");
     input_to( (: call_other, __FILE__, "name_skill", me:));
   }
   return;
}

void name_skill(object me, string arg)
{
   mapping* m_actions;
   mapping m_act=([]);
   int content;
   string msg;

   m_actions=me->query("skill_dragonfight");
   if(!pointerp(m_actions))m_actions=({});

   content=me->query("str")+me->query_skill("unarmed",1)/5;
   m_act+=(["damage":content]);
   content=-me->query("spi");
   m_act+=(["dodge" :content]);
   content=-me->query("cps");
   m_act+=(["parry" :content]);
   content=me->query("con")+me->query_skill("force",1)/10;
   m_act+=(["force" :content]);
   m_act+=(["damage_type":"瘀伤"]);

   if(!arg||arg==" ")arg="龙形搏击绝技"+chinese_number(sizeof(m_actions)+1);
   m_act+=(["name":arg]);
   msg= skill_head[random(sizeof(skill_head))]+"「"+arg+"」"+skill_tail[random(sizeof(skill_tail))];
   m_act+=(["action":msg]);
   m_actions+=({m_act});
   me->set("skill_dragonfight",m_actions);
}
