// by canoe 2001-03-27
#include <ansi.h>
inherit F_CLEAN_UP;
int is_friend(object me, string id);
int main(object me,string arg)
{
        object *ob,*target;
        int i;
        string ob_id,outarg;
        mapping friends;

        if(me->query("chblk_on"))
                return notify_fail("你的频道被关了！\n");
        friends = me->query("qq");
        if(!friends
        || !mapp(friends)
        || (sizeof(friends) == 0))
                return notify_fail("你没有任何的朋友！\n");
        ob=users();
        i = sizeof(ob);
        while (i--) {
                ob_id = (string)ob[i]->query("id");
                if (is_friend(me,ob_id)
                && !ob[i]->query("env/qq_talk"))
                {
                        if (sizeof(target)==0)
                                target=({ob[i]});
                         else 
                                target+=({ob[i]});
                }
        }
        if(!target) return notify_fail("你的好友都没有收听QQ频道。\n");
        if(!stringp(outarg = arg))
                outarg = "．．．．．";  
        message("vision", HBMAG "【泥潭QQ】" + 
        me->query("name") + "(" + capitalize(me->query("id")) + ")" + 
        "：" + outarg + "\n" NOR, target);      
       tell_object(me,HBMAG "【泥潭QQ】" +me->query("name") + "(" + capitalize(me->query("id")) + ")" +
"：" + outarg + "\n" NOR);
        return 1;
}

int is_friend(object me, string id)
{
        string * ids;
        mapping friends;
        int i;

        friends = me->query("qq");
        ids = keys(friends);
        for(i=0;i<sizeof(ids);i++)
        {
                if(id == ids[i])
                return 1;
        }
        return 0;
}

int help(object me)
{
write(@HELP
指令格式 : qq 内容
与自己的朋友通过【泥潭QQ】讲话。

HELP
    );
    return 1;
}

