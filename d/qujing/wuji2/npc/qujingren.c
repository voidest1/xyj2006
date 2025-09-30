// 取经人试玩版0.0001

#include <ansi.h>
#include <command.h>
inherit NPC;


int to_sleep();			//睡觉
int to_walk();			//起程,即跟随玩家
int to_rest();			//休息,即停止跟随玩家
void me_init();			//有各个部分自己的初始化,这样如果对于某关有
				//特殊的要求就可以在me_init中定义
int accept_object1(object who, object ob);//可重新定义的accept_obj

void create()
{
  set_name("取经人",({"qujing ren", "qujingren", "shengseng"}));
  
  set("long", "大唐东土前往西天取经的圣僧,法像庄严\n");
  set("title", "和尚");
  set("attitude", "friendly");
  set("shen_type", 1);  
  set("gender", "男性");
  set("age", 23);
  set("per", 30);
  set("int", 30);
  set("combat_exp", 1000);

  set("max_gin",800);
  set("max_kee",800);
  set("max_sen",800);
  set("can_sleep",1);		//应禁止用瞌睡虫在取经人身上
 
  set("obstacle/qujing","ren"); //取经人的特殊标志 
  set("no_kill",1);             //不能杀需要对kill等指令进行禁止
  set("no_magic",1);		//不能用法术,需要对cast等指令做限制

  set("inquiry", ([		//一般通过ask qujingren about xx来做要求
    "sleep" : (: to_sleep :),
    "walk"  : (: to_walk :),
    "rest"  : (: to_rest :),
  ]));

  me_init();			//各个关可以自己定义的初始部分
				//如果本文件单独update,要取消这句
  setup();
}

int to_sleep()
  {
  object me=this_object()			;
  object where= environment(me)		 	;
  object who=this_player()			;

  if (who->query("obstacle/qujing")!="ren")return 0;

  if( !where->query("sleep_room"))
    {
    message_vision("$N说道:这里恐怕不能睡觉吧。\n",me,who);
    return 1;
    }
  if (me->is_busy())
    {
    message_vision("$N叹道:我很忙耶！\n",me);
    return 1;
    }
  if ((time()-me->query("last_sleep"))<90)
    {
    message_vision("$N摇了摇头说道:我现在还不累！\n",me);
    return 1;
    }

  message_vision("$N点了点头说道:是休息的时候了,我且小歇片刻！\n",me);
  if (where->query("if_bed"))
    {
    message_vision(HIY "$N往被中一钻，不一会便鼾声大作，进入了梦乡。\n" NOR,me);
     }
  else 
    { 
    message_vision(HIY "$N就地一躺，不一会便鼾声大作，进入了梦乡。\n" NOR,me);
    }

    me->set("last_sleep",time());
    me->set_temp("block_msg/all",1);
    if (me->query_temp("ridee"))
	me->query_temp("ridee")->set_temp("rider",0);
    me->set_temp("ridee",0);
    me->add_temp("apply/dodge",-me->query_temp("ride/dodge"));
    me->set_temp("ride/dodge",0);
    me->disable_player("<睡梦中>");
    if (me->query("redef_wake"))   //在乌鸡国中me_init定义了redef_wake
      { 			   //这样在取经人睡觉后就有变化.	
      call_out("wakeup1",random(2)+10, me, where);
      }
    else
      {
      call_out("wakeup",random(2)+10, me, where);
      }
    return 1;   
  }

void wakeup(object me,object where)
  {
  me->enable_player();
  while( environment(me)->is_character() )
	me->move(environment(environment(me)));
  message_vision(HIY "$N一觉醒来，精力充沛地活动了一下筋骨。\n" NOR,me);
  me->set_temp("block_msg/all", 0);
}


int to_walk()
  {
  object me=this_object();
  object who=this_player();

  if (who->query("obstacle/qujing")!="ren")return 0;
  if (me->query("has_followed"))
    return 1;
  message_vision("$N对取经人说道:师傅该起程了。\n",who);
  me->set("has_followed",1);
  me->command_function("follow "+who->query("id"));
  me->set_leader(who);
  return 1; 
  }

int to_rest()
  {
  object me=this_object();
  object who=this_player();

  if (who->query("obstacle/qujing")!="ren")return 0;
  if (!(me->query("has_followed")))
    return 1;
  message_vision("$N对取经人说道:师傅歇息一会吧了。\n",who);
  me->set("has_followed",0);
  me->set_leader(0);
  return 1; 

  }


int accept_object (object who, object ob)
{
  object me = this_object();

  if (me->query("redef_accept_obj")!=1) return 0;
   
  return accept_object1(who,ob);
 
}


