#include <armor.h>

inherit HEAD;

void create()
{
        set_name("毡帽", ({ "hat" }) );
        set_weight(800);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "顶");
                set("material", "cloth");
		set("value", 100);
                set("armor_prop/armor", 2);
        }
        setup();
}

