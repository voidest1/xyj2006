//Cracked by Roath
//【蜀山剑派】dhxy-evil 2000.7.5
#include <ansi.h>
inherit ROOM;                                                        
                                                                     
void create ()                                                       
{                                                                    
  set ("short", HIM"镇塔柱"NOR);                                   
  set ("long", @LONG                                                 
一根巨柱矗立在你的面前，柱子旁边一条巨龙。                                           
LONG);                                                          
                                                                     
    set("exits", ([ /* sizeof() == 1 */                              
        "south" : __DIR__"hole3",                                      
        "northwest" : __DIR__"hole1",
]));                                                                                                                          
    set("objects",([
        __DIR__"npc/dragon2" : 1,
    ]));    
  setup();                                                           
}

int valid_leave(object me, string dir)
{

   if (dir == "south" ) {
        if (objectp(present("dragon", environment(me))))
   return notify_fail("还是把龙先杀掉把！”\n");
        }   
        return ::valid_leave(me, dir);
}
