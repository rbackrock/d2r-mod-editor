# 常规

在计算 x,y 坐标时，似乎是最大值和最小值是 3840*2160 为基准（不确定）

# 处理文本描述

local/lng/strings 和 local/lng/strings-legacy 区别为 strings-legacy 是怀旧版的字符串文件夹

字符串中可以使用 `\n` 换行，注意：用 `\n` 换行之后，游戏里显示得字符串顺序为设置字符串得从右往左，例如
设置得字符串为 `"钥匙\n哈1\n哈2"` 游戏里显示的顺序为：哈2，哈1，钥匙

字符串中的文本颜色查资料是因为历史原因使用特殊字符串 `ÿc` 开头，大概十六位，游戏中顺序尝试，颜色编码为：
`ÿc0` 白色
`ÿc1` 红色
`ÿc2` 绿色
`ÿc3` 蓝色
`ÿc4` 暗金
`ÿc5` 浅灰
`ÿc6` 字都没有了
`ÿc7` 浅暗金
`ÿc8` 橘黄
`ÿc9` 金黄
`ÿcA` 暗绿色
`ÿcB` 蓝色
`ÿcC` 绿色
`ÿcD` 暗金
`ÿcE` 白色
`ÿcF` 白色

## item-runes.json

符文相关描述在 item-runes.json 文件，符文描述有两种

* 在背包、箱子，地上的描述，key 值规律是：[r符文编号]
* 已经做成符文之语装备上的描述，key 值得规律是：[r符文编号L]

## skills.json

* 技能名称 key 值规律：[skillname编号]，但是`火焰专精`特殊，为[skillsname编号]，搜索只有它一个

## item-modifiers.json

* 装备上词缀 key 值规律为：[ModStr编码]

## item-names.json

* 可以自己添加原本数据中没有的字符串，比如铠甲里显示装备是什么底材，qlvl 是多少，有形防御力和无形防御力区间是多少
新添加的字符串 key 属性很关键，用于关联 data\global\excel\armor.txt 文件，例如：
新添加的字符串为
```json
{
    "id": 252071,
    "Key": "base_info_uar",
    "enUS": "Sacred Armor",
    "zhTW": "ÿc5[qlvl:85,防御:487-600/e730-e900]\nSacred Armorÿc0",
    "deDE": "[fs]Heilige Rüstung",
    "esES": "[fs]Armadura sacra",
    "frFR": "[fs]Armure sacrée",
    "itIT": "[fs]Corazza Sacra",
    "koKR": "세이크리드 아머",
    "plPL": "[ms]Święty Pancerz",
    "esMX": "[fs]Armadura sagrada",
    "jaJP": "セイクレッド・アーマー",
    "ptBR": "[fs]Armadura Sagrada",
    "ruRU": "[ms]священный доспех",
    "zhCN": "神圣铠甲"
  },
```

那么在 armor.txt 文件里关联的地方就应该填
```tsv
Sacred Armor	100	0	4	1	487	600	5	232	0	0	60	0	85	0	66	373558	4872212	uar	uar			aar	aar	xar	uar	1	2	3	1	4	1	flpaar	invaar	invaaru	invaaru	1	2	2	2	2	0	0					0	xxx			tors		item_platearmor	12	item_platearmor	0	0	5	0	0	0			0	2	0	0	0			0	3					255					255					255					255					255					255					255					255					255					255					255					255					255					255					255					255					255	8	8	0	xxx	xxx	1	0		1	base_info_uar	
```

## item-nameaffixes.json

* "id": 1727 为特等

# 数据

## uniqueitems.txt

暗金装备详细数据

