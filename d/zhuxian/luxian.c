inherit ROOM;                                                        

void create ()
{
  set ("short", "戮仙门" );   

    set("exits", ([ 
               "northeast" : __DIR__"tianjue",
                  ]));
    set("objects", ([ 
               "/d/zhuxian/npc/huoshen" : 1,
                  ]));
    setup();
}
