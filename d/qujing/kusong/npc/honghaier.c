//Cracked by Roath
inherit NPC;
inherit F_MASTER;
#include <ansi.h>
int ask_niang(object me);
int do_yes();

int rank_me();

void create()
{
       set_name("红孩儿", ({"honghai er","er"}));
        set("title", "圣婴大王");
        set("long", "牛魔王与罗刹女的儿子，于火焰山中修炼了一身好本领。\n");
       set("gender", "男性");
       set("age", 16);
        set("per", 25);
        set("str", 25);
        set("int", 25);
        set("con", 25);
        set("spi", 25);

        set("attitude", "heroism");

        set("combat_exp", 1200000);
        set("daoxing", 1500000);

        set("eff_dx", -500000);
        set("nkgain", 500);

        set("class","yaomo");
        set("max_kee", 1600);
        set("max_sen", 1600);
        set("force", 2500);
        set("max_force", 1600);
        set("force_factor", 100);
        set("max_mana", 1600);
        set("mana", 2500);
        set("mana_factor", 80);
       
        set_skill("unarmed", 150);
        set_skill("dodge", 200);
        set_skill("force", 150);
        set_skill("parry", 150);
        set_skill("spear", 180);
        set_skill("spells", 150);
        set_skill("huoyun-qiang", 180);
        set_skill("moshenbu", 200);
        set_skill("moyun-shou", 150);
        set_skill("huomoforce", 180);
        set_skill("pingtian-dafa", 150);

        map_skill("force", "huomoforce");
        map_skill("spells", "pingtian-dafa");
        map_skill("unarmed", "moyun-shou");
        map_skill("dodge", "moshenbu");
        map_skill("parry", "huoyun-qiang");
        map_skill("spear", "huoyun-qiang");

        set("kusong/Teach_Hong", 1);
        set("chat_chance_combat", 30);
        set("chat_msg_combat", ({
                (: cast_spell, "sanmei" :),
        }));


        create_family("火云洞", 2, "蓝");

        set("inquiry", ([
                "职位": (: rank_me :),
                "铁扇公主": (: ask_niang :),
]) );


        setup();
        carry_object("/d/nanhai/obj/huojianqiang")->wield();
        carry_object("/d/qujing/bibotan/obj/zhanpao")->wear();
}

void attempt_apprentice(object ob)
{       ob=this_player();

        if( (string)ob->query("family/family_name") == "火云洞") {

                if( (int)ob->query("max_mana") > 200 
                && (int)ob->query("max_force") > 200
                && !(int)ob->query("kusong") ) {
        
                command("nod");
                command("recruit " + ob->query("id"));
                return ;
                }
        }
}

int recruit_apprentice(object ob)
{
        if( ::recruit_apprentice(ob) ) {
                ob->set("class", "yaomo");
                ob->set("title", "枯松涧火云洞小妖");
                ob->set("kusong/Teach_Hong", 1);
        }
}


int rank_me()
{
        object me=this_player();
        int exp;
        exp=(int)me->query("combat_exp");
        if( me->query("title") == "枯松涧积雷山碧波潭三山总洞主")
                return notify_fail("你已经是三山总洞主了，还来要什么职位．\n");
        if( (string)me->query("family/family_name") == "火云洞"
        && (string)me->query("family/master_id") == "honghai er"){

                
                if( exp <= 10000 ){
                        me->set("title", "枯松涧火云洞巡山小妖");
                } else if(exp <= 50000 ){
                        me->set("title", "枯松涧火云洞先锋");
                } else if(exp < 100000 ){
                        me->set("title", "枯松涧火云洞健将");
                } else if(exp <= 250000 ){
                        me->set("title", "枯松涧火云洞巡山大将");
                } else if(exp <= 500000 ){
                        me->set("title", "枯松涧火云洞妖王");
                } else if(exp <= 1000000 ){
                        me->set("title", "枯松涧火云洞魔王");
                } else if(exp <= 1500000 ){
                        me->set("title", "枯松涧火云洞长老");
                } else {
                        me->set("title", "枯松涧火云洞洞主");
                }

                tell_object(me, "红孩儿对你说：你可以做"+me->query("title")+"。\n");
                tell_room(environment(me),
"红孩儿对"+me->query("name")+"说：你可以做"+me->query("title")+"。\n", ({me, me}));
        }
        return 1;
}
void init()
{
        object ob = this_player();
        remove_call_out("check");
        call_out("check",1,ob);
        add_action("do_yes","ok");
}

void check(object ob)
{
        object where=environment();
        if(ob->query("family/family_name") == "火云洞")
           return;
        if(ob->query("title") == "普通百姓")
           return;
        if(!userp(ob))
           return;   
        if(ob->query_temp("kusong_killed") == 1)
           return;
        if ( ob->query_temp("apply/id") 
        && member_array("niumo",ob->query_temp("apply/id") ) )
           {
        command("say 父王怎有机会来看望孩儿。");
        command("say 孩儿正好抓到一个和尚，吃了他，可以长生不老啊。");
        command("xixi");
        return;
            }

        if( present("jin che",where) || present("mu che",where)
        ||  present("shui che",where) || present("huo che",where) ||
            present("tu che",where) )
          {
        kill_ob(ob);
        message_vision(HIR"只见红孩儿手架五行车，手中火尖枪对$N一指。\n"NOR,ob);
        message_vision(HIR"一股三昧真火带着浓烟扑向$N，$N抽身便走。\n"NOR,ob);
        ob->move("/d/qujing/kusong/guaishiya");
        ob->unconcious();
        }

}
int ask_niang(object me)
{
       me=this_player();
       command("jump");
       me->set_temp("ok",1);
       command("say 我去请我娘一起吃肉好吗（ok）？\n");
       return 1;
}

int do_yes()
{
        object ob = this_player();
        if( ! ob->query_temp("ok") ) return 0;
        command("say 父王请在家等候，孩儿快去快回。");
        command("bye");
        destruct(this_object());
        return 1;
}

int accept_fight (object ob)
{
  ob->apply_condition ("killer",100);
  kill_ob (ob);
  return 1;
}

void kill_ob (object ob)
{
  set_temp("no_return",1);
  set_temp("my_killer",ob);
  ::kill_ob(ob);
}
void announce_success (object who)
{
  int i;
  object honghaier= this_object();

  if (! who->query_temp("che1_broken"))
   {
    destruct (honghaier);
    return;
   } 
  if (! who->query_temp("che2_broken"))
   {
    destruct (honghaier);
    return;
   } 
  if (! who->query_temp("che3_broken"))
   {
    destruct (honghaier);
    return;
   } 
  if (! who->query_temp("che4_broken"))
   {
    destruct (honghaier);
    return;  
   } 
  if (! who->query_temp("che5_broken"))
   {
    destruct (honghaier);
    return;  
   } 
  honghaier->move(environment(who));
  who->set_temp("kusong_killed",1);
  message_vision (HIR"一团红色的烟雾涌了过来,红孩儿突然从里面蹦了出来.\n"NOR,who);
  message_vision (HIG"$N手中"HIR+"火尖枪"HIG+"朝$n一指:大胆臭贼,敢破坏我的五行车.\n"NOR,honghaier,who);
  message_vision (HIG"只听一声$N怒喝:我就说父王怎么会来看我.臭贼拿命来.\n"NOR,honghaier,who);
  who->command("bian");
  kill_ob(who);
}  
void die()
{
  object ob = query_temp("my_killer");
  object me = this_object();
  if (ob->query_temp("obstacle/hong_killed")) return ;
  if (ob)
  {
    ob->set_temp("obstacle/hong_killed",1);
    call_out ("pusa_appearing",1,ob);
  }
  message_vision(HIY"天上传来观音菩萨的声音：红孩儿，还不随我回南海去。\n"NOR,ob);
  load_object("/obj/empty");
  me->move("/obj/empty");
  call_out ("destruct_me",3,me);
}
void destruct_me (object me)
{
  destruct (me);
}

void pusa_appearing (object who)
{
  object pusa = new ("/d/qujing/kusong/npc/pusa");

  pusa->announce_success (who);
  destruct (pusa); 
}


void unconcious ()
{
  die();
}
