// hongniang.c

inherit NPC;
#include <ansi.h>
#include <quest.h>
void create()
{
        set_name("红娘", ({ "hong niang","hong" }) );
        set("title", "红娘庄");
        set("gender", "女性" );
        set("age", 16);
        
	set("quest_type", "dancer");
        set("inquiry", ([
		"yaoqin": (: give_quest:),
		"瑶琴": (: give_quest:),
        ]));

        setup();
        add_money("gold", 10);
//        carry_object("/clone/misc/cloth")->wear();
}
