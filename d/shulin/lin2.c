// Room: /shulin/lin2.c
inherit ROOM;
void create()
{
        set("short", "树林");
        set("long", @LONG
这是一片茂密的树林，好象被人设计了一种阵，一走进来，你仿佛迷失了方向。
LONG );
        set("exits", ([
                "east" : __DIR__"lin3",
                "west" : __DIR__"lin1",
                "south" : __FILE__,
                "north" : __FILE__,
                "southeast" : __FILE__,
                "northeast" : __FILE__,
        ]));
        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}
