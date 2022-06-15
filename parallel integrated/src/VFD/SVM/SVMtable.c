﻿/*
 * SVMtable.c
 *
 * Created: 29/01/2017 11:38:37 ص
 *  Author: ahmed
 */ 
#include "fixedptc.h"

const fixedpt SVM_SIN_tab[] = {
		fixedpt_rconst(0), fixedpt_rconst(14.2942175453412), fixedpt_rconst(28.5882609201526), fixedpt_rconst(42.8819559560265)
		, fixedpt_rconst(57.1751284887999), fixedpt_rconst(71.467604360676), fixedpt_rconst(85.7592094223467)
		, fixedpt_rconst(100.049769535115), fixedpt_rconst(114.339110573015), fixedpt_rconst(128.627058424935)
		, fixedpt_rconst(142.913438996741), fixedpt_rconst(157.198078213394), fixedpt_rconst(171.480802021072)
		, fixedpt_rconst(185.761436389294), fixedpt_rconst(200.039807313036), fixedpt_rconst(214.315740814855)
		, fixedpt_rconst(228.589062947006), fixedpt_rconst(242.859599793563), fixedpt_rconst(257.127177472538)
		, fixedpt_rconst(271.391622138001), fixedpt_rconst(285.652759982193), fixedpt_rconst(299.910417237652)
		, fixedpt_rconst(314.164420179322), fixedpt_rconst(328.414595126677), fixedpt_rconst(342.660768445832)
		, fixedpt_rconst(356.90276655166), fixedpt_rconst(371.14041590991), fixedpt_rconst(385.373543039316)
		, fixedpt_rconst(399.601974513717), fixedpt_rconst(413.825536964165), fixedpt_rconst(428.044057081041)
		, fixedpt_rconst(442.257361616164), fixedpt_rconst(456.465277384903), fixedpt_rconst(470.667631268289)
		, fixedpt_rconst(484.864250215122), fixedpt_rconst(499.054961244079), fixedpt_rconst(513.239591445826)
		, fixedpt_rconst(527.41796798512), fixedpt_rconst(541.589918102917), fixedpt_rconst(555.755269118477)
		, fixedpt_rconst(569.913848431468), fixedpt_rconst(584.06548352407), fixedpt_rconst(598.210001963076)
		, fixedpt_rconst(612.347231401991), fixedpt_rconst(626.476999583136), fixedpt_rconst(640.599134339745)
		, fixedpt_rconst(654.713463598064), fixedpt_rconst(668.819815379443), fixedpt_rconst(682.918017802439)
		, fixedpt_rconst(697.007899084903), fixedpt_rconst(711.08928754608), fixedpt_rconst(725.162011608693)
		, fixedpt_rconst(739.225899801043), fixedpt_rconst(753.280780759089), fixedpt_rconst(767.326483228542)
		, fixedpt_rconst(781.36283606695), fixedpt_rconst(795.389668245785), fixedpt_rconst(809.406808852521)
		, fixedpt_rconst(823.414087092725), fixedpt_rconst(837.411332292131), fixedpt_rconst(851.398373898724)
		, fixedpt_rconst(865.375041484816), fixedpt_rconst(879.341164749123), fixedpt_rconst(893.296573518841)
		, fixedpt_rconst(907.241097751718), fixedpt_rconst(921.174567538126), fixedpt_rconst(935.096813103134)
		, fixedpt_rconst(949.007664808573), fixedpt_rconst(962.906953155105), fixedpt_rconst(976.794508784287)
		, fixedpt_rconst(990.670162480638), fixedpt_rconst(1004.5337451737), fixedpt_rconst(1018.38508794008)
		, fixedpt_rconst(1032.22402200555), fixedpt_rconst(1046.05037874706), fixedpt_rconst(1059.86398969482)
		, fixedpt_rconst(1073.66468653434), fixedpt_rconst(1087.45230110848), fixedpt_rconst(1101.22666541951)
		, fixedpt_rconst(1114.98761163115), fixedpt_rconst(1128.73497207061), fixedpt_rconst(1142.46857923064)
		, fixedpt_rconst(1156.18826577158), fixedpt_rconst(1169.89386452336), fixedpt_rconst(1183.58520848761)
		, fixedpt_rconst(1197.26213083961), fixedpt_rconst(1210.92446493037), fixedpt_rconst(1224.57204428867)
		, fixedpt_rconst(1238.20470262307), fixedpt_rconst(1251.82227382392), fixedpt_rconst(1265.42459196541)
		, fixedpt_rconst(1279.01149130759), fixedpt_rconst(1292.58280629838), fixedpt_rconst(1306.13837157559)
		, fixedpt_rconst(1319.67802196894), fixedpt_rconst(1333.20159250205), fixedpt_rconst(1346.7089183945)
		, fixedpt_rconst(1360.19983506376), fixedpt_rconst(1373.67417812729), fixedpt_rconst(1387.13178340447)
		, fixedpt_rconst(1400.57248691861), fixedpt_rconst(1413.996124899), fixedpt_rconst(1427.40253378283)
		, fixedpt_rconst(1440.79155021725), fixedpt_rconst(1454.16301106132), fixedpt_rconst(1467.516753388)
		, fixedpt_rconst(1480.85261448617), fixedpt_rconst(1494.17043186255), fixedpt_rconst(1507.47004324375)
		, fixedpt_rconst(1520.75128657821), fixedpt_rconst(1534.01400003816), fixedpt_rconst(1547.25802202162)
		, fixedpt_rconst(1560.48319115437), fixedpt_rconst(1573.68934629189), fixedpt_rconst(1586.87632652136)
		, fixedpt_rconst(1600.04397116357), fixedpt_rconst(1613.19211977494), fixedpt_rconst(1626.32061214942)
		, fixedpt_rconst(1639.42928832048), fixedpt_rconst(1652.51798856303), fixedpt_rconst(1665.5865533954)
		, fixedpt_rconst(1678.63482358124), fixedpt_rconst(1691.66264013149), fixedpt_rconst(1704.66984430634)
		, fixedpt_rconst(1717.65627761709), fixedpt_rconst(1730.62178182816), fixedpt_rconst(1743.56619895897)
		, fixedpt_rconst(1756.48937128588), fixedpt_rconst(1769.39114134411), fixedpt_rconst(1782.27135192968)
		, fixedpt_rconst(1795.12984610127), fixedpt_rconst(1807.96646718219), fixedpt_rconst(1820.78105876227)
		, fixedpt_rconst(1833.57346469976), fixedpt_rconst(1846.34352912322), fixedpt_rconst(1859.09109643344)
		, fixedpt_rconst(1871.81601130535), fixedpt_rconst(1884.51811868987), fixedpt_rconst(1897.19726381583)
		, fixedpt_rconst(1909.85329219185), fixedpt_rconst(1922.48604960822), fixedpt_rconst(1935.09538213877)
		, fixedpt_rconst(1947.68113614277), fixedpt_rconst(1960.24315826678), fixedpt_rconst(1972.78129544652)
		, fixedpt_rconst(1985.29539490875), fixedpt_rconst(1997.78530417311), fixedpt_rconst(2010.250871054)
		, fixedpt_rconst(2022.69194366242), fixedpt_rconst(2035.10837040783), fixedpt_rconst(2047.5)
		, fixedpt_rconst(2059.86668145081), fixedpt_rconst(2072.20826407616), fixedpt_rconst(2084.52459749777)
		, fixedpt_rconst(2096.81553164499), fixedpt_rconst(2109.08091675667), fixedpt_rconst(2121.32060338296)
		, fixedpt_rconst(2133.53444238716), fixedpt_rconst(2145.72228494747), fixedpt_rconst(2157.88398255889)
		, fixedpt_rconst(2170.01938703497), fixedpt_rconst(2182.12835050963), fixedpt_rconst(2194.21072543899)
		, fixedpt_rconst(2206.2663646031), fixedpt_rconst(2218.29512110782), fixedpt_rconst(2230.29684838653)
		, fixedpt_rconst(2242.27140020199), fixedpt_rconst(2254.21863064805), fixedpt_rconst(2266.13839415149)
		, fixedpt_rconst(2278.03054547374), fixedpt_rconst(2289.8949397127), fixedpt_rconst(2301.73143230447)
		, fixedpt_rconst(2313.53987902512), fixedpt_rconst(2325.32013599245), fixedpt_rconst(2337.07205966775)
		, fixedpt_rconst(2348.79550685753), fixedpt_rconst(2360.4903347153), fixedpt_rconst(2372.15640074327)
		, fixedpt_rconst(2383.7935627941), fixedpt_rconst(2395.40167907265), fixedpt_rconst(2406.98060813768)
		, fixedpt_rconst(2418.53020890359), fixedpt_rconst(2430.05034064216), fixedpt_rconst(2441.5408629842)
		, fixedpt_rconst(2453.00163592132), fixedpt_rconst(2464.43251980764), fixedpt_rconst(2475.83337536142)
		, fixedpt_rconst(2487.20406366686), fixedpt_rconst(2498.54444617569), fixedpt_rconst(2509.85438470894)
		, fixedpt_rconst(2521.13374145857), fixedpt_rconst(2532.38237898918), fixedpt_rconst(2543.60016023968)
		, fixedpt_rconst(2554.78694852494), fixedpt_rconst(2565.94260753747), fixedpt_rconst(2577.06700134908)
		, fixedpt_rconst(2588.15999441257), fixedpt_rconst(2599.22145156329), fixedpt_rconst(2610.25123802089)
		, fixedpt_rconst(2621.24921939089), fixedpt_rconst(2632.21526166638), fixedpt_rconst(2643.14923122958)
		, fixedpt_rconst(2654.05099485352), fixedpt_rconst(2664.92041970366), fixedpt_rconst(2675.75737333948)
		, fixedpt_rconst(2686.56172371613), fixedpt_rconst(2697.333339186), fixedpt_rconst(2708.07208850036)
		, fixedpt_rconst(2718.77784081093), fixedpt_rconst(2729.45046567151), fixedpt_rconst(2740.08983303953)
		, fixedpt_rconst(2750.69581327765), fixedpt_rconst(2761.26827715535), fixedpt_rconst(2771.8070958505)
		, fixedpt_rconst(2782.31214095091), fixedpt_rconst(2792.78328445593), fixedpt_rconst(2803.22039877798)
		, fixedpt_rconst(2813.62335674412), fixedpt_rconst(2823.99203159757), fixedpt_rconst(2834.32629699932)
		, fixedpt_rconst(2844.6260270296), fixedpt_rconst(2854.89109618945), fixedpt_rconst(2865.12137940224)
		, fixedpt_rconst(2875.31675201519), fixedpt_rconst(2885.47708980093), fixedpt_rconst(2895.60226895892)
		, fixedpt_rconst(2905.69216611706), fixedpt_rconst(2915.74665833314), fixedpt_rconst(2925.76562309633)
		, fixedpt_rconst(2935.74893832873), fixedpt_rconst(2945.69648238678), fixedpt_rconst(2955.6081340628)
		, fixedpt_rconst(2965.48377258644), fixedpt_rconst(2975.32327762615), fixedpt_rconst(2985.12652929069)
		, fixedpt_rconst(2994.89340813051), fixedpt_rconst(3004.62379513929), fixedpt_rconst(3014.31757175532)
		, fixedpt_rconst(3023.974619863), fixedpt_rconst(3033.59482179425), fixedpt_rconst(3043.17806032994)
		, fixedpt_rconst(3052.72421870133), fixedpt_rconst(3062.23318059149), fixedpt_rconst(3071.70483013674)
		, fixedpt_rconst(3081.139051928), fixedpt_rconst(3090.53573101226), fixedpt_rconst(3099.89475289395)
		, fixedpt_rconst(3109.21600353634), fixedpt_rconst(3118.49936936294), fixedpt_rconst(3127.74473725887)
		, fixedpt_rconst(3136.95199457222), fixedpt_rconst(3146.12102911549), fixedpt_rconst(3155.25172916687)
		, fixedpt_rconst(3164.34398347166), fixedpt_rconst(3173.39768124362), fixedpt_rconst(3182.4127121663)
		, fixedpt_rconst(3191.38896639439), fixedpt_rconst(3200.32633455507), fixedpt_rconst(3209.22470774932)
		, fixedpt_rconst(3218.08397755328), fixedpt_rconst(3226.90403601953), fixedpt_rconst(3235.68477567846)
		, fixedpt_rconst(3244.42608953952), fixedpt_rconst(3253.12787109255), fixedpt_rconst(3261.79001430909)
		, fixedpt_rconst(3270.41241364367), fixedpt_rconst(3278.99496403507), fixedpt_rconst(3287.53756090762)
		, fixedpt_rconst(3296.04010017248), fixedpt_rconst(3304.50247822888), fixedpt_rconst(3312.92459196542)
		, fixedpt_rconst(3321.3063387613), fixedpt_rconst(3329.64761648759), fixedpt_rconst(3337.94832350844)
		, fixedpt_rconst(3346.20835868236), fixedpt_rconst(3354.42762136343), fixedpt_rconst(3362.60601140253)
		, fixedpt_rconst(3370.74342914855), fixedpt_rconst(3378.83977544963), fixedpt_rconst(3386.89495165434)
		, fixedpt_rconst(3394.90885961291), fixedpt_rconst(3402.88140167838), fixedpt_rconst(3410.81248070787)
		, fixedpt_rconst(3418.70200006366), fixedpt_rconst(3426.54986361446), fixedpt_rconst(3434.35597573652)
		, fixedpt_rconst(3442.12024131484), fixedpt_rconst(3449.84256574428), fixedpt_rconst(3457.52285493076)
		, fixedpt_rconst(3465.16101529239), fixedpt_rconst(3472.75695376058), fixedpt_rconst(3480.31057778122)
		, fixedpt_rconst(3487.82179531579), fixedpt_rconst(3495.29051484248), fixedpt_rconst(3502.71664535729)
		, fixedpt_rconst(3510.10009637516), fixedpt_rconst(3517.44077793108), fixedpt_rconst(3524.73860058116)
		, fixedpt_rconst(3531.99347540373), fixedpt_rconst(3539.20531400043), fixedpt_rconst(3546.37402849729)
};