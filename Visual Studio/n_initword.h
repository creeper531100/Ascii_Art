﻿#pragma once
#include "pch.h"

inline std::map<std::string, wchar_t> init_words() {
    std::map<std::string, wchar_t> map_pairs;
	map_pairs["kkkk"] = L' ';
	map_pairs["ykkk"] = L'⠁';
	map_pairs["kykk"] = L'⠂';
	map_pairs["yykk"] = L'⠃';
	map_pairs["ykyk"] = L'⠅';
	map_pairs["kkyk"] = L'⠄';
	map_pairs["yyyk"] = L'⠇';
	map_pairs["yyyy"] = L'⡇';
	map_pairs["zkkk"] = L'⠈';
	map_pairs["mkkk"] = L'⠉';
	map_pairs["zykk"] = L'⠊';
	map_pairs["mykk"] = L'⠋';
	map_pairs["zkyk"] = L'⠌';
	map_pairs["mkyk"] = L'⠍';
	map_pairs["zyyk"] = L'⠎';
	map_pairs["myyk"] = L'⠏';
	map_pairs["kzkk"] = L'⠐';
	map_pairs["yzkk"] = L'⠑';
	map_pairs["kmkk"] = L'⠒';
	map_pairs["ymkk"] = L'⠓';
	map_pairs["kzyk"] = L'⠔';
	map_pairs["yzyk"] = L'⠕';
	map_pairs["kmyk"] = L'⠖';
	map_pairs["ymyk"] = L'⠗';
	map_pairs["zzkk"] = L'⠘';
	map_pairs["mzkk"] = L'⠙';
	map_pairs["zmkk"] = L'⠚';
	map_pairs["mmkk"] = L'⠛';
	map_pairs["zzyk"] = L'⠜';
	map_pairs["mzyk"] = L'⠝';
	map_pairs["zmyk"] = L'⠞';
	map_pairs["mmyk"] = L'⠟';
	map_pairs["kyyk"] = L'⠆';

	map_pairs["kkzk"] = L'⠠';
	map_pairs["ykzk"] = L'⠡';
	map_pairs["kyzk"] = L'⠢';
	map_pairs["yyzk"] = L'⠣';
	map_pairs["kkmk"] = L'⠤';
	map_pairs["ykmk"] = L'⠥';
	map_pairs["kymk"] = L'⠦';
	map_pairs["yymk"] = L'⠧';
	map_pairs["zkzk"] = L'⠨';
	map_pairs["mkzk"] = L'⠩';
	map_pairs["zyzk"] = L'⠪';
	map_pairs["myzk"] = L'⠫';
	map_pairs["zkmk"] = L'⠬';
	map_pairs["mkmk"] = L'⠭';
	map_pairs["zymk"] = L'⠮';
	map_pairs["mymk"] = L'⠯';
	map_pairs["kzzk"] = L'⠰';
	map_pairs["yzzk"] = L'⠱';
	map_pairs["kmzk"] = L'⠲';
	map_pairs["ymzk"] = L'⠳';
	map_pairs["kzmk"] = L'⠴';
	map_pairs["yzmk"] = L'⠵';
	map_pairs["kmmk"] = L'⠶';
	map_pairs["ymmk"] = L'⠷';
	map_pairs["zzzk"] = L'⠸';
	map_pairs["mzzk"] = L'⠹';
	map_pairs["zmzk"] = L'⠺';
	map_pairs["mmzk"] = L'⠻';
	map_pairs["zzmk"] = L'⠼';
	map_pairs["mzmk"] = L'⠽';
	map_pairs["zmmk"] = L'⠾';
	map_pairs["mmmk"] = L'⠿';

	map_pairs["kkky"] = L'⡀';
	map_pairs["ykky"] = L'⡁';
	map_pairs["kyky"] = L'⡂';
	map_pairs["yyky"] = L'⡃';
	map_pairs["kkyy"] = L'⡄';
	map_pairs["ykyy"] = L'⡅';
	map_pairs["kyyy"] = L'⡆';
	map_pairs["zkky"] = L'⡈';
	map_pairs["mkky"] = L'⡉';
	map_pairs["zyky"] = L'⡊';
	map_pairs["myky"] = L'⡋';
	map_pairs["zkyy"] = L'⡌';
	map_pairs["mkyy"] = L'⡍';
	map_pairs["zyyy"] = L'⡎';
	map_pairs["myyy"] = L'⡏';
	map_pairs["kzky"] = L'⡐';
	map_pairs["yzky"] = L'⡑';
	map_pairs["kmky"] = L'⡒';
	map_pairs["ymky"] = L'⡓';
	map_pairs["kzyy"] = L'⡔';
	map_pairs["yzyy"] = L'⡕';
	map_pairs["kmyy"] = L'⡖';
	map_pairs["ymyy"] = L'⡗';
	map_pairs["zzky"] = L'⡘';
	map_pairs["mzky"] = L'⡙';
	map_pairs["zmky"] = L'⡚';
	map_pairs["mmky"] = L'⡛';
	map_pairs["zzyy"] = L'⡜';
	map_pairs["mzyy"] = L'⡝';
	map_pairs["zmyy"] = L'⡞';
	map_pairs["mmyy"] = L'⡟';

	map_pairs["kkzy"] = L'⡠';
	map_pairs["ykzy"] = L'⡡';
	map_pairs["kyzy"] = L'⡢';
	map_pairs["yyzy"] = L'⡣';
	map_pairs["kkmy"] = L'⡤';
	map_pairs["ykmy"] = L'⡥';
	map_pairs["kymy"] = L'⡦';
	map_pairs["yymy"] = L'⡧';
	map_pairs["zkzy"] = L'⡨';
	map_pairs["mkzy"] = L'⡩';
	map_pairs["zyzy"] = L'⡪';
	map_pairs["myzy"] = L'⡫';
	map_pairs["zkmy"] = L'⡬';
	map_pairs["mkmy"] = L'⡭';
	map_pairs["zymy"] = L'⡮';
	map_pairs["mymy"] = L'⡯';
	map_pairs["kzzy"] = L'⡰';
	map_pairs["yzzy"] = L'⡱';
	map_pairs["kmzy"] = L'⡲';
	map_pairs["ymzy"] = L'⡳';
	map_pairs["kzmy"] = L'⡴';
	map_pairs["yzmy"] = L'⡵';
	map_pairs["kmmy"] = L'⡶';
	map_pairs["ymmy"] = L'⡷';
	map_pairs["zzzy"] = L'⡸';
	map_pairs["mzzy"] = L'⡹';
	map_pairs["zmzy"] = L'⡺';
	map_pairs["mmzy"] = L'⡻';
	map_pairs["zzmy"] = L'⡼';
	map_pairs["mzmy"] = L'⡽';
	map_pairs["zmmy"] = L'⡾';
	map_pairs["mmmy"] = L'⡿';

	map_pairs["kkkz"] = L'⢀';
	map_pairs["ykkz"] = L'⢁';
	map_pairs["kykz"] = L'⢂';
	map_pairs["yykz"] = L'⢃';
	map_pairs["kkyz"] = L'⢄';
	map_pairs["ykyz"] = L'⢅';
	map_pairs["kyyz"] = L'⢆';
	map_pairs["yyyz"] = L'⢇';
	map_pairs["zkkz"] = L'⢈';
	map_pairs["mkkz"] = L'⢉';
	map_pairs["zykz"] = L'⢊';
	map_pairs["mykz"] = L'⢋';
	map_pairs["zkyz"] = L'⢌';
	map_pairs["mkyz"] = L'⢍';
	map_pairs["zyyz"] = L'⢎';
	map_pairs["myyz"] = L'⢏';
	map_pairs["kzkz"] = L'⢐';
	map_pairs["yzkz"] = L'⢑';
	map_pairs["kmkz"] = L'⢒';
	map_pairs["ymkz"] = L'⢓';
	map_pairs["kzyz"] = L'⢔';
	map_pairs["yzyz"] = L'⢕';
	map_pairs["kmyz"] = L'⢖';
	map_pairs["ymyz"] = L'⢗';
	map_pairs["zzkz"] = L'⢘';
	map_pairs["mzkz"] = L'⢙';
	map_pairs["zmkz"] = L'⢚';
	map_pairs["mmkz"] = L'⢛';
	map_pairs["zzyz"] = L'⢜';
	map_pairs["mzyz"] = L'⢝';
	map_pairs["zmyz"] = L'⢞';
	map_pairs["mmyz"] = L'⢟';

	map_pairs["kkzz"] = L'⢠';
	map_pairs["ykzz"] = L'⢡';
	map_pairs["kyzz"] = L'⢢';
	map_pairs["yyzz"] = L'⢣';
	map_pairs["kkmz"] = L'⢤';
	map_pairs["ykmz"] = L'⢥';
	map_pairs["kymz"] = L'⢦';
	map_pairs["yymz"] = L'⢧';
	map_pairs["zkzz"] = L'⢨';
	map_pairs["mkzz"] = L'⢩';
	map_pairs["zyzz"] = L'⢪';
	map_pairs["myzz"] = L'⢫';
	map_pairs["zkmz"] = L'⢬';
	map_pairs["mkmz"] = L'⢭';
	map_pairs["zymz"] = L'⢮';
	map_pairs["mymz"] = L'⢯';
	map_pairs["kzzz"] = L'⢰';
	map_pairs["yzzz"] = L'⢱';
	map_pairs["kmzz"] = L'⢲';
	map_pairs["ymzz"] = L'⢳';
	map_pairs["kzmz"] = L'⢴';
	map_pairs["yzmz"] = L'⢵';
	map_pairs["kmmz"] = L'⢶';
	map_pairs["ymmz"] = L'⢷';
	map_pairs["zzzz"] = L'⢸';
	map_pairs["mzzz"] = L'⢹';
	map_pairs["zmzz"] = L'⢺';
	map_pairs["mmzz"] = L'⢻';
	map_pairs["zzmz"] = L'⢼';
	map_pairs["mzmz"] = L'⢽';
	map_pairs["zmmz"] = L'⢾';
	map_pairs["mmmz"] = L'⢿';

	map_pairs["kkkm"] = L'⣀';
	map_pairs["ykkm"] = L'⣁';
	map_pairs["kykm"] = L'⣂';
	map_pairs["yykm"] = L'⣃';
	map_pairs["kkym"] = L'⣄';
	map_pairs["ykym"] = L'⣅';
	map_pairs["kyym"] = L'⣆';
	map_pairs["yyym"] = L'⣇';
	map_pairs["zkkm"] = L'⣈';
	map_pairs["mkkm"] = L'⣉';
	map_pairs["zykm"] = L'⣊';
	map_pairs["mykm"] = L'⣋';
	map_pairs["zkym"] = L'⣌';
	map_pairs["mkym"] = L'⣍';
	map_pairs["zyym"] = L'⣎';
	map_pairs["myym"] = L'⣏';
	map_pairs["kzkm"] = L'⣐';
	map_pairs["yzkm"] = L'⣑';
	map_pairs["kmkm"] = L'⣒';
	map_pairs["ymkm"] = L'⣓';
	map_pairs["kzym"] = L'⣔';
	map_pairs["yzym"] = L'⣕';
	map_pairs["kmym"] = L'⣖';
	map_pairs["ymym"] = L'⣗';
	map_pairs["zzkm"] = L'⣘';
	map_pairs["mzkm"] = L'⣙';
	map_pairs["zmkm"] = L'⣚';
	map_pairs["mmkm"] = L'⣛';
	map_pairs["zzym"] = L'⣜';
	map_pairs["mzym"] = L'⣝';
	map_pairs["zmym"] = L'⣞';
	map_pairs["mmym"] = L'⣟';

	map_pairs["kkzm"] = L'⣠';
	map_pairs["ykzm"] = L'⣡';
	map_pairs["kyzm"] = L'⣢';
	map_pairs["yyzm"] = L'⣣';
	map_pairs["kkmm"] = L'⣤';
	map_pairs["ykmm"] = L'⣥';
	map_pairs["kymm"] = L'⣦';
	map_pairs["yymm"] = L'⣧';
	map_pairs["zkzm"] = L'⣨';
	map_pairs["mkzm"] = L'⣩';
	map_pairs["zyzm"] = L'⣪';
	map_pairs["myzm"] = L'⣫';
	map_pairs["zkmm"] = L'⣬';
	map_pairs["mkmm"] = L'⣭';
	map_pairs["zymm"] = L'⣮';
	map_pairs["mymm"] = L'⣯';
	map_pairs["kzzm"] = L'⣰';
	map_pairs["yzzm"] = L'⣱';
	map_pairs["kmzm"] = L'⣲';
	map_pairs["ymzm"] = L'⣳';
	map_pairs["kzmm"] = L'⣴';
	map_pairs["yzmm"] = L'⣵';
	map_pairs["kmmm"] = L'⣶';
	map_pairs["ymmm"] = L'⣷';
	map_pairs["zzzm"] = L'⣸';
	map_pairs["mzzm"] = L'⣹';
	map_pairs["zmzm"] = L'⣺';
	map_pairs["mmzm"] = L'⣻';
	map_pairs["zzmm"] = L'⣼';
	map_pairs["mzmm"] = L'⣽';
	map_pairs["zmmm"] = L'⣾';
	map_pairs["mmmm"] = L'⣿';
	map_pairs["kymz"] = L'⢦';
	return map_pairs;
}