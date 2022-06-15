/*
 * atan.c
 *
 * Created: 24/06/2018 03:51:38 م
 *  Author: ahmed
 */ 
#include <atan.h>
#include <trig.h>

fixedpt atan_lut[] =
{
	fixedpt_rconst(0), fixedpt_rconst(0.286476510277075), fixedpt_rconst(0.572938697683486), fixedpt_rconst(0.859372243644682)
	, fixedpt_rconst(1.1457628381751), fixedpt_rconst(1.43209618416465), fixedpt_rconst(1.71835800165546), fixedpt_rconst(2.00453403210591)
	, fixedpt_rconst(2.29061004263853), fixedpt_rconst(2.57657183026883), fixedpt_rconst(2.86240522611175), fixedpt_rconst(3.14809609956276)
	, fixedpt_rconst(3.43363036245053), fixedpt_rconst(3.71899397315805), fixedpt_rconst(4.00417294070939), fixedpt_rconst(4.28915332881902)
	, fixedpt_rconst(4.57392125990087), fixedpt_rconst(4.85846291903429), fixedpt_rconst(5.14276455788425), fixedpt_rconst(5.42681249857283)
	, fixedpt_rconst(5.71059313749965), fixedpt_rconst(5.99409294910848), fixedpt_rconst(6.27729848959756), fixedpt_rconst(6.56019640057132)
	, fixedpt_rconst(6.84277341263095), fixedpt_rconst(7.1250163489018), fixedpt_rconst(7.40691212849524), fixedpt_rconst(7.68844776990288)
	, fixedpt_rconst(7.96961039432137), fixedpt_rconst(8.25038722890551), fixedpt_rconst(8.53076560994814), fixedpt_rconst(8.81073298598492)
	, fixedpt_rconst(9.09027692082233), fixedpt_rconst(9.36938509648749), fixedpt_rconst(9.64804531609817), fixedpt_rconst(9.92624550665171)
	, fixedpt_rconst(10.2039737217317), fixedpt_rconst(10.4812181441309), fixedpt_rconst(10.75796708839), fixedpt_rconst(11.0342090032503)
	, fixedpt_rconst(11.3099324740202), fixedpt_rconst(11.5851262248551), fixedpt_rconst(11.859779120948), fixedpt_rconst(12.1338801706334)
	, fixedpt_rconst(12.4074185274008), fixedpt_rconst(12.6803834918198), fixedpt_rconst(12.9527645133755), fixedpt_rconst(13.2245511922135)
	, fixedpt_rconst(13.4957332807958), fixedpt_rconst(13.7663006854663), fixedpt_rconst(14.0362434679265), fixedpt_rconst(14.305551846622)
	, fixedpt_rconst(14.5742161980388), fixedpt_rconst(14.8422270579111), fixedpt_rconst(15.1095751223405), fixedpt_rconst(15.3762512488262)
	, fixedpt_rconst(15.6422464572087), fixedpt_rconst(15.9075519305259), fixedpt_rconst(16.1721590157826), fixedpt_rconst(16.4360592246357)
	, fixedpt_rconst(16.6992442339936), fixedpt_rconst(16.9617058865329), fixedpt_rconst(17.2234361911315), fixedpt_rconst(17.4844273232207)
	, fixedpt_rconst(17.7446716250569), fixedpt_rconst(18.0041616059134), fixedpt_rconst(18.2628899421942), fixedpt_rconst(18.5208494774711)
	, fixedpt_rconst(18.7780332224456), fixedpt_rconst(19.0344343548352), fixedpt_rconst(19.2900462191888), fixedpt_rconst(19.5448623266288)
	, fixedpt_rconst(19.798876354525), fixedpt_rconst(20.0520821460986), fixedpt_rconst(20.3044737099605), fixedpt_rconst(20.5560452195835)
	, fixedpt_rconst(20.8067910127113), fixedpt_rconst(21.0567055907048), fixedpt_rconst(21.3057836178288), fixedpt_rconst(21.5540199204789)
	, fixedpt_rconst(21.8014094863518), fixedpt_rconst(22.0479474635604), fixedpt_rconst(22.2936291596941), fixedpt_rconst(22.5384500408281)
	, fixedpt_rconst(22.7824057304817), fixedpt_rconst(23.0254920085281), fixedpt_rconst(23.267704810057), fixedpt_rconst(23.5090402241925)
	, fixedpt_rconst(23.7494944928668), fixedpt_rconst(23.9890640095524), fixedpt_rconst(24.2277453179542), fixedpt_rconst(24.4655351106628)
	, fixedpt_rconst(24.7024302277713), fixedpt_rconst(24.9384276554573), fixedpt_rconst(25.1735245245302), fixedpt_rconst(25.4077181089485)
	, fixedpt_rconst(25.6410058243053), fixedpt_rconst(25.8733852262861), fixedpt_rconst(26.1048540090993), fixedpt_rconst(26.3354100038814)
	, fixedpt_rconst(26.565051177078), fixedpt_rconst(26.7937756288032), fixedpt_rconst(27.0215815911771), fixedpt_rconst(27.2484674266448)
	, fixedpt_rconst(27.4744316262772), fixedpt_rconst(27.699472808055), fixedpt_rconst(27.9235897151384), fixedpt_rconst(28.1467812141221)
	, fixedpt_rconst(28.3690462932786), fixedpt_rconst(28.5903840607906), fixedpt_rconst(28.8107937429731), fixedpt_rconst(29.0302746824872)
	, fixedpt_rconst(29.248826336547), fixedpt_rconst(29.4664482751201), fixedpt_rconst(29.6831401791233), fixedpt_rconst(29.8989018386146)
	, fixedpt_rconst(30.1137331509825), fixedpt_rconst(30.3276341191339), fixedpt_rconst(30.5406048496815), fixedpt_rconst(30.7526455511317)
	, fixedpt_rconst(30.9637565320736), fixedpt_rconst(31.1739381993713), fixedpt_rconst(31.3831910563591), fixedpt_rconst(31.5915157010407)
	, fixedpt_rconst(31.7989128242945), fixedpt_rconst(32.0053832080835), fixedpt_rconst(32.2109277236738), fixedpt_rconst(32.4155473298587)
	, fixedpt_rconst(32.6192430711929), fixedpt_rconst(32.8220160762343), fixedpt_rconst(33.0238675557967), fixedpt_rconst(33.224798801212)
	, fixedpt_rconst(33.4248111826038), fixedpt_rconst(33.6239061471729), fixedpt_rconst(33.822085217494), fixedpt_rconst(34.0193499898265)
	, fixedpt_rconst(34.2157021324374), fixedpt_rconst(34.4111433839388), fixedpt_rconst(34.6056755516386), fixedpt_rconst(34.7993005099073)
	, fixedpt_rconst(34.9920201985587), fixedpt_rconst(35.1838366212463), fixedpt_rconst(35.3747518438761), fixedpt_rconst(35.5647679930352)
	, fixedpt_rconst(35.7538872544368), fixedpt_rconst(35.9421118713824), fixedpt_rconst(36.1294441432409), fixedpt_rconst(36.3158864239452)
	, fixedpt_rconst(36.5014411205064), fixedpt_rconst(36.6861106915455), fixedpt_rconst(36.8698976458441), fixedpt_rconst(37.052804540912)
	, fixedpt_rconst(37.2348339815747), fixedpt_rconst(37.4159886185779), fixedpt_rconst(37.596271147212), fixedpt_rconst(37.7756843059547)
	, fixedpt_rconst(37.9542308751326), fixedpt_rconst(38.1319136756018), fixedpt_rconst(38.3087355674478), fixedpt_rconst(38.4846994487041)
	, fixedpt_rconst(38.6598082540901), fixedpt_rconst(38.8340649537685), fixedpt_rconst(39.0074725521211), fixedpt_rconst(39.1800340865445)
	, fixedpt_rconst(39.3517526262648), fixedpt_rconst(39.5226312711712), fixedpt_rconst(39.6926731506689), fixedpt_rconst(39.861881422551)
	, fixedpt_rconst(40.0302592718897), fixedpt_rconst(40.1978099099458), fixedpt_rconst(40.3645365730974), fixedpt_rconst(40.530442521788)
	, fixedpt_rconst(40.6955310394921), fixedpt_rconst(40.8598054317002), fixedpt_rconst(41.0232690249225), fixedpt_rconst(41.1859251657097)
	, fixedpt_rconst(41.3477772196937), fixedpt_rconst(41.5088285706453), fixedpt_rconst(41.66908261955), fixedpt_rconst(41.8285427837023)
	, fixedpt_rconst(41.9872124958167), fixedpt_rconst(42.1450952031573), fixedpt_rconst(42.3021943666837), fixedpt_rconst(42.4585134602153)
	, fixedpt_rconst(42.6140559696112), fixedpt_rconst(42.7688253919688), fixedpt_rconst(42.9228252348372), fixedpt_rconst(43.0760590154488)
	, fixedpt_rconst(43.228530259966), fixedpt_rconst(43.3802425027448), fixedpt_rconst(43.5311992856142), fixedpt_rconst(43.6814041571715)
	, fixedpt_rconst(43.8308606720926), fixedpt_rconst(43.9795723904589), fixedpt_rconst(44.127542877098), fixedpt_rconst(44.2747757009408)
	, fixedpt_rconst(44.4212744343923), fixedpt_rconst(44.5670426527177), fixedpt_rconst(44.712083933443), fixedpt_rconst(44.8564018557688)
	, fixedpt_rconst(45)
};

fixedpt atan45(fixedpt x)
{
	x *= 200;
	if(x > fixedpt_rconst(200)) x = fixedpt_rconst(200);
	
	fixedpt ang, angp1;
	uint32_t xreal = fixedpt_toint(x);
	x = fixedpt_fracpart(x);
	
	ang	= atan_lut[xreal];
	if(xreal < 200) angp1 = atan_lut[xreal + 1];
	else 
	{
		angp1 = atan_lut[200];
		x = 0;
	}
	ang += fixedpt_mul((angp1 - ang), x);
	
	return ang;
}

fixedpt acotan45(fixedpt x)
{
	return fixedpt_rconst(90) - atan45(x);
}

fixedpt arctan2(fixedpt x, fixedpt y)
{
	fixedpt ang, ang_offset;
	
	if((x == 0) && (y == 0)) return 0; //invalid arguments
	
	if (fixedpt_abs(x) >= fixedpt_abs(y))
	{
		ang = atan45 (fixedpt_div(fixedpt_abs(y), fixedpt_abs(x)));	
	}
	else
	{
		ang = acotan45(fixedpt_div(fixedpt_abs(x), fixedpt_abs(y)));
	}
	
	if((y >= 0) && (x < 0))
	{
		//1st quad
		ang = fixedpt_rconst(180) - ang;
	}
	else if((y < 0) && (x <= 0))
	{
		//3nd quad
		ang  += fixedpt_rconst(180);
	}
	else if((y < 0) && (x > 0))
	{
		//4rd quad
		ang = fixedpt_rconst(360) - ang;
	}
	
	return ang;
}