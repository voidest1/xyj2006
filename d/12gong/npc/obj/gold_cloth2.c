// by happ@YSZZ

#include <ansi.h>
#include <armor.h>

inherit CLOTH;

void create()
{
    set_name(HIY"黄金圣衣"NOR, ({ "huangjin shengyi","shengyi","armor" }) );
   set_weight(10000);
   if( clonep() )
     set_default_object(__FILE__);
   else {
     set("unit", "件");
     set("no_give",1);
     set("value",2000000);
     set("material", "steel");
     set("armor_prop/armor",150);
  }
   setup();
}
int query_autoload() { return 1; }
