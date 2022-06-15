// /*
//  * IM_param_tune2.c
//  *
//  * Created: 07/10/2018 11:33:28 ص
//  *  Author: ahmed
//  */ 
// 
// #include <IM_param_tune2.h>
// #include <motor_data.h>
// #include <vfd.h>
// #include <arm_math.h>
// #include <DACC_interface.h>
// #include <axistransform.h>
// #include <Inverter.h>
// 
// //filter definisions
// #define h0 (40 * 1)
// #define h1 (90 * 1)
// 
// #define Pgain (40)
// 
// 
// 
// 
// 
// float cc0, cc1, cc3, cc4, cc5;
// 
// #define c0 cc0
// #define c1 cc1
// #define c2 c1
// #define c3 cc3
// #define c4 cc5
// #define c5 c4
// #define c10 cc3
// #define c11 cc4
// #define c12 cc5
// 
// float est_out;
// 
// 
// float time_lapse = 0;
// float va, prev_v, prev_i;
// 
// float param_a[2], param_b[2]; //estimated parameter vecotrs
// float pa[2][2], pb[2][2]; //covariance matricies
// float phi_a[2], phi_b[2]; //measured data vectors
// float g_a[2], g_b[2]; //gains vecotrs
// 
// float param[4]; //estimated parameter vecotrs
// float p[4][4]; //covariance matricies
// float phi[4]; //measured data vectors
// float g[4]; //gains vecotrs
// 
// void update_measured_data_vectors(float v, float i)
// {
// 	
// 	phi_a[0] = (c0 * phi_a[0]) + (c1 * v) + (c2 * prev_v);
// 	phi_a[1] = (c3 * phi_a[1]) + (c4 * v) + (c5 * prev_v);
// 	
// 	phi_b[0] = (c0 * phi_b[0]) + (c1 * i) + (c2 * prev_i);
// 	phi_b[1] = (c3 * phi_b[1]) + (c4 * i) + (c5 * prev_i);
// 
// 
// // 	phi_a[0] = (c0 * phi_a[0]) + (c1 * v) + (c2 * prev_v);
// // 	phi_a[1] = (c0 * phi_a[1]) + (c1 * i) + (c2 * prev_i);
// 	
// // 	phi_a[0] = i;
// // 	phi_a[1] = (i - prev_i) / TSAMPLE;
// 	
// // 	phi_b[0] = (i - prev_i) / TSAMPLE;
// // 	phi_b[1] = prev_v;
// 
// // 	phi[0] = i;
// // 	phi[1] = (i - prev_i) / TSAMPLE;
// // 	
// // 	phi[2] = (i - prev_i) / TSAMPLE;
// // 	phi[3] = prev_v;
// 	
// 	prev_i = i;
// 	prev_v = v;
// }
// 
// void update_gains_vectors(void)
// {
// 	float tmp[2], tmp1;
// 	
// 	//update gain1
// 	tmp[0] = (phi_a[0] * pa[0][0]) + (phi_a[1] * pa[0][1]);
// 	tmp[1] = (phi_a[0] * pa[1][0]) + (phi_a[1] * pa[1][1]);
// 	
// 	tmp1 = ((phi_a[0] * tmp[0]) + (phi_a[1] * tmp[1]));
// 	tmp1 += (1.0f);
// 	
// 	g_a[0] = tmp[0] / tmp1;
// 	g_a[1] = tmp[1] / tmp1;
// 	
// 	//update gain2
// 	tmp[0] = (phi_b[0] * pb[0][0]) + (phi_b[1] * pb[0][1]);
// 	tmp[1] = (phi_b[0] * pb[1][0]) + (phi_b[1] * pb[1][1]);
// 	
// 	tmp1 = ((phi_b[0] * tmp[0]) + (phi_b[1] * tmp[1]));
// 	tmp1 += (1.0f);
// 
// 	g_b[0] = tmp[0] / tmp1;
// 	g_b[1] = tmp[1] / tmp1;
// 
// // 	float tmp[4], tmp1;
// // 	
// // 	//update gain
// // 	tmp[0] = phi[0] * p[0][0] + phi[1] * p[0][1] + phi[2] * p[0][2] + phi[3] * p[0][3];
// // 	tmp[1] = phi[0] * p[1][0] + phi[1] * p[1][1] + phi[2] * p[1][2] + phi[3] * p[1][3];
// // 	tmp[2] = phi[0] * p[2][0] + phi[1] * p[2][1] + phi[2] * p[2][2] + phi[3] * p[2][3];
// // 	tmp[3] = phi[0] * p[3][0] + phi[1] * p[3][1] + phi[2] * p[3][2] + phi[3] * p[3][3];
// // 	
// // 	tmp1 = ((phi[0] * tmp[0]) + (phi[1] * tmp[1]) + (phi[2] * tmp[2]) + (phi[3] * tmp[3]));
// // 	tmp1 += (1.0f);
// // 	
// // 	g[0] = tmp[0] / tmp1;
// // 	g[1] = tmp[1] / tmp1;
// // 	g[2] = tmp[2] / tmp1;
// // 	g[3] = tmp[3] / tmp1;
// }
// 
// void update_covariance_matricies(void)
// {
// 	float tmp[2];
// 	
// 	//update cov1
// 	tmp[0] = (phi_a[0] * pa[0][0]) + (phi_a[1] * pa[1][0]);
// 	tmp[1] = (phi_a[0] * pa[0][1]) + (phi_a[1] * pa[1][1]);
// 	
// 	pa[0][0] -= tmp[0] * g_a[0];
// 	pa[0][1] -= tmp[1] * g_a[0];
// 	pa[1][0] -= tmp[0] * g_a[1];
// 	pa[1][1] -= tmp[1] * g_a[1];
// 	
// 	//update cov2
// 	tmp[0] = (phi_b[0] * pb[0][0]) + (phi_b[1] * pb[1][0]);
// 	tmp[1] = (phi_b[0] * pb[0][1]) + (phi_b[1] * pb[1][1]);
// 	
// 	pb[0][0] -= tmp[0] * g_b[0];
// 	pb[0][1] -= tmp[1] * g_b[0];
// 	pb[1][0] -= tmp[0] * g_b[1];
// 	pb[1][1] -= tmp[1] * g_b[1];
// 
// // 	float tmp[4];
// // 	
// // 	//update cov
// // 	tmp[0] = (phi[0] * p[0][0]) + (phi[1] * p[1][0]) + (phi[2] * p[2][0]) + (phi[3] * p[3][0]);
// // 	tmp[1] = (phi[0] * p[0][1]) + (phi[1] * p[1][1]) + (phi[2] * p[2][1]) + (phi[3] * p[3][1]);
// // 	tmp[2] = (phi[0] * p[0][2]) + (phi[1] * p[1][2]) + (phi[2] * p[2][2]) + (phi[3] * p[3][2]);
// // 	tmp[3] = (phi[0] * p[0][3]) + (phi[1] * p[1][3]) + (phi[2] * p[2][3]) + (phi[3] * p[3][3]);
// // 	
// // 	p[0][0] -= (tmp[0] * g[0]);
// // 	p[0][1] -= (tmp[1] * g[0]);
// // 	p[0][2] -= (tmp[2] * g[0]);
// // 	p[0][3] -= (tmp[3] * g[0]);
// // 	
// // 	p[1][0] -= (tmp[0] * g[1]);
// // 	p[1][1] -= (tmp[1] * g[1]);
// // 	p[1][2] -= (tmp[2] * g[1]);
// // 	p[1][3] -= (tmp[3] * g[1]);
// // 	
// // 	p[2][0] -= (tmp[0] * g[2]);
// // 	p[2][1] -= (tmp[1] * g[2]);
// // 	p[2][2] -= (tmp[2] * g[2]);
// // 	p[2][3] -= (tmp[3] * g[2]);
// // 	
// // 	p[3][0] -= (tmp[0] * g[3]);
// // 	p[3][1] -= (tmp[1] * g[3]);
// // 	p[3][2] -= (tmp[2] * g[3]);
// // 	p[3][3] -= (tmp[3] * g[3]);
// }
// 
// void update_parameters_vectors(float output1, float output2)
// {
// 	float err1, err2, oerr;
// 	err1 = ((phi_a[0] * param_a[0] + phi_a[1] * param_a[1]));
// 	
// 	err2 = (/*output1 -*/ (phi_b[0] * param_b[0] + phi_b[1] * param_b[1]));
// 	
// 	oerr = err1 + err2;
// 	//est_out = oerr;
// 	//est_out = output;
// 	oerr = output1 - oerr;
// 	est_out = err1;
// 	err1 = output1 - err1;
// 
// 	param_a[0] += g_a[0] * oerr;
// 	param_a[1] += g_a[1] * oerr;
// 	param_b[0] += g_b[0] * oerr;
// 	param_b[1] += g_b[1] * oerr;
// 
// // 	float err1, err2, oerr;
// // 	oerr = phi[0] * param[0] + phi[1] * param[1] + phi[2] * param[2] + phi[3] * param[3];
// // 	
// // 	oerr = output1 + output2 - oerr;
// // 	
// // 	err1 = phi[0] * param[0] + phi[1] * param[1];
// // 	
// // 	err2 = phi[2] * param[2] + phi[3] * param[3];
// // 	
// // 	err1 = output1 - err1;
// // 	err2 = output2 - err2;
// // 	
// // 	est_out = oerr;
// // 	//est_out = output;
// // 	//oerr = output - oerr;
// // 	//est_out = err1 + err2;
// // 
// // // 	param[0] += g[0] * err1;
// // // 	param[1] += g[1] * err1;
// // // 	param[2] += g[2] * err2;
// // // 	param[3] += g[3] * err2;
// // 
// // 	param[0] += g[0] * oerr;
// // 	param[1] += g[1] * oerr;
// // 	param[2] += g[2] * oerr;
// // 	param[3] += g[3] * oerr;
// }
// 
// volatile float rs_h, ls_h, rr_h, lm_h;
// void extract_est_param(void)
// {
// 	float a0, a1, b0, b1;
// 	
//  	b1 = param_a[0] + param_a[1];
// 	b0 = (h0 - h1) * param_a[0] + b1 * h1;
// 	
// 	a1 = (((h0 * h0) - (h1 * h1)) / (h0 - h1)) - param_b[0] - param_b[1];
// 	a0 = (a1 * h1 - h1 * h1) - (h0 - h1) * param_b[0];
//  	
// 	rs_h = a0 / b0;
// 	ls_h = 1 / b1;
// 	rr_h = (a1 / b1) - (a0 / b0) - (b0 / (b1 * b1));
// 	lm_h = (1 * b1) / b0;
// 	lm_h *= rr_h;
// }
// 
// void reconstruct_param_vector(float f_rs, float f_ls, float f_rr, float f_lm)
// {
// 	float a0, a1, b0, b1;
// 	
// 	#define RE_TR (f_lm/f_rr)
// 	
//  	b1 = 1/f_ls;
// 	b0 = 1/(f_ls*RE_TR);
// 	
// 	a1 = (1/RE_TR)+(f_lm/(f_ls*RE_TR))+(f_rs/f_ls);
// 	a0 = f_rs/(f_ls * RE_TR);
// 	
// 	// init parameter vectors a and b to 0
// 	param_a[0] = (b0 - b1*h1)/(h0 - h1);
// 	param_a[1] = (b1*h0 - b0)/(h0 - h1);
// 	
// 	param_b[0] = (a1*h1 - a0 - (h1*h1)) /(h0 - h1);
// 	param_b[1] = (-a1*h0 + a0 + (h0*h0)) /(h0 - h1);
// }
// 
// #define rs (3.6f)
// #define rr (2.5f)
// #define ls (0.302)
// #define lr (0.301f)
// #define lm (0.273)
// #define lr_o_lm (lr / lm)
// #define tr (lm/rr)
// 
// void save_estimated_motor_parameters(void)
// {
// 	float a0, a1, b0, b1;
// 	
// //  	b1 = param_a[0] + param_a[1];
// // 	b0 = (h0 - h1) * param_a[0] + b1 * h1;
// // 	
// // 	a1 = (((h0 * h0) - (h1 * h1)) / (h0 - h1)) - param_b[0] - param_b[1];
// // 	a0 = (a1 * h1 - h1 * h1) - (h0 - h1) * param_b[0];
// //  	
// // 	rs_h = a0 / b0;
// // 	ls_h = 1 / b1;
// // 	rr_h = (a1 / b1) - (a0 / b0) - (b0 / (b1 * b1));
// // 	lm_h = (1 * b1) / b0;
// // 	lm_h *= rr_h;
// 	//lm_h *= 1000;
// 
// 	rs_h = ( h1*h0 - h1*param_b[1] - h0*param_b[0] ) / ( h0*param_a[0] + h1*param_a[1] );
// 	rr_h = ( (h1 + h0 - param_b[0] - param_b[1]) / ( param_a[0] + param_a[1]) ) - rs_h;
// 	ls_h = ( ( param_a[0] + param_a[1] ) * rr_h ) / ( h0*param_a[0] + h1*param_a[1] );
// 	lm_h = sqrt( ( ls_h * ls_h ) - ( ls_h / ( param_a[0] + param_a[1] ) ) );
// 
// // 	rs_h = param_a[0];
// // 	ls_h = param_a[1];
// 	
// // 	rs_h = (h1 - param_a[1]) / param_a[0];
// // 	ls_h = 1 / param_a[0];
// // 		
// //  	rr_h = param_b[0]/(param_b[1]*TSAMPLE);
// //  	lm_h = (TSAMPLE * rr_h) / ((1 /param_b[1]) - 1) ;
// 	 
// // 	menue_drive_data_setup.m_motor_stator_resistance = rs_h;
// // 	menue_drive_data_setup.m_motor_leakage_inductance = ls_h * 1000;
// // 	menue_drive_data_setup.m_motor_rotor_resistance = rr_h;
// // 	menue_drive_data_setup.m_motor_magnetizing_inductance = lm_h * 1000;	
// 	ls_h *= 1;
// }
// 
// #define IM_TUNE_PERIOD (10)
// 
// #define SIG1_FREQ (10.0f * 2 * PI)
// #define SIG2_FREQ (250.0f * 2 * PI)
// #define SIG3_FREQ (25.0f * 2 * PI)
// #define SIG4_FREQ (50.0f * 2 * PI)
// 
// #define red_fac (0.0)
// 
// 
// #define a  (lm / lr)
// #define lrts (lr / TSAMPLE)
// #define lmts (lm / TSAMPLE)
// 
// #define RS (rs)
// #define RR (rr)//(a * a * rr)
// #define LS (ls)//(ls - a * lm)
// #define LM (lm)//(a * lm)
// #define TR	(LM / RR)
// #define RAT_T (TR / TSAMPLE)
// 
// float prev_ia, prev_vm, prev_im, vmint;
// void IM_param_tune2_current_handler(void)
// {
// 	volatile float com_ia, ia, im;
// 	volatile float dia, vma = 0;
// 	if (time_lapse < IM_TUNE_PERIOD)
// 	{
// 		ia = axt_get_ialpha();
// 		
// 		//ia = get_sim_sout();
// 		
// 		//com_ia = 0;//1.5;
// 		com_ia = 1.5;
// 		com_ia = com_ia + 1.5 * arm_sin_f32(SIG1_FREQ * time_lapse);
// 		//com_ia = com_ia + 1 * arm_sin_f32(SIG2_FREQ * time_lapse);
// 		com_ia = com_ia + 1 * arm_sin_f32(SIG3_FREQ * time_lapse);
// 		//com_ia = com_ia + 1 * arm_sin_f32(SIG4_FREQ * time_lapse);
// 		
// 		com_ia *= 1;
// 		ia = com_ia;
// 		//com_ia *= menue_drive_data_setup.m_motor_rated_current;
// 		//com_ia /= (3.8 * FIXEDPT_ONE);
// 		
// 		dia = (ia - prev_ia) / TSAMPLE;
// 		
// 		va = rs * ia + ls * dia;
// 		
// 		vma = (lr * dia) + (ia*rr) - (vmint / tr);
// 		vma /= (1 + lr_o_lm );
// 		//vma = 0;
// 		va += vma;
// 		vmint += vma*TSAMPLE;
// 
// // 		va = rs * ia + ls * dia;
// // 		
// // 		im = ( ia*(rr + lrts) + prev_im*(lmts - rr + lrts) - prev_ia*lrts ) / ( lmts + rr + lrts );
// // 		vma = lmts * ( im - prev_im );
// // 		
// // 		va += vma;
// 		//ia -= (25.0f/48.0f);
// 		update_measured_data_vectors(va, ia);
// 		update_gains_vectors();
// 		update_covariance_matricies();
// 		update_parameters_vectors(ia, vma);
// 		
// 		//extract_est_param();
// 		//reconstruct_param_vector(rs_h, ls_h, rr_h, LM);
// 		va = (com_ia - ia) * Pgain;
// 		//va = ((com_ia) * 300) + 25.0;
// 		
// 		//dacc_interface_writeData(0, ia/*est_out*/, 20, 1);
// 		
// 		inverter_out_rect((va * FIXEDPT_ONE), 0);
// 		prev_ia = ia;
// 		prev_vm = vma;
// 		prev_im = im;
// 	} 
// 	else
// 	{
// 		vfd_estop();
// 		save_estimated_motor_parameters();
// 		return;
// 	}
// 	time_lapse += TSAMPLE;
// }
// #define seg_sqr (9 * 1000000)
// 
// void IM_param_tune2_init(void)
// {
// 	time_lapse = 0;
// 	prev_v = 0;
// 	prev_i = 0;
// 	prev_ia = 1.5;
// 	prev_vm = 0;
// 	va = 0;
// 	
// 	float a0, a1, b0, b1;
// 	
//  	b1 = 1/LS;
// 	b0 = 1/(LS*TR);
// 	
// 	a1 = (1/TR)+(LM/(LS*TR))+(RS/LS);
// 	a0 = RS/(LS * TR);
// 	
// 	//init constants
// 	cc0 = ((2 - h1*TSAMPLE) / (2 + h1*TSAMPLE));
// 	cc1 = ((TSAMPLE) / (2 + h1*TSAMPLE));
// 	cc3 = ((2 - h0*TSAMPLE) / (2 + h0*TSAMPLE));
// 	cc4 = ((2) / (2 + h0*TSAMPLE));
// 	cc5 = ((TSAMPLE) / (2 + h0*TSAMPLE));
// 	
// 	// init parameter vectors a and b to 0
// 	param_a[0] = (b0 - b1*h1)/(h0 - h1);
// 	param_a[1] = (b1*h0 - b0)/(h0 - h1);
// 	
// 	param_b[0] = (a1*h1 - a0 - (h1*h1)) /(h0 - h1);
// 	param_b[1] = (-a1*h0 + a0 + (h0*h0)) /(h0 - h1);
// 	
// 	// init covariance matrix a, and b to seg2 * I
// 	pa[0][0] = seg_sqr;
// 	pa[0][1] = 0;
// 	pa[1][0] = 0;
// 	pa[1][1] = seg_sqr;
// 	
// 	pb[0][0] = seg_sqr;
// 	pb[0][1] = 0;
// 	pb[1][0] = 0;
// 	pb[1][1] = seg_sqr;
// 	
// 	phi_a[0] = 0;
// 	phi_a[1] = 0;
// 	phi_b[0] = 0;
// 	phi_b[1] = 0;
// 
// // 	// init parameter vectors a and b to 0
// // 	param[0] = 0;//RS;
// // 	param[1] = LS * red_fac;
// // 	param[2] = (LM / (1+(TR/TSAMPLE))) *red_fac;
// // 	param[3] = 0;
// // 	
// // 	// init covariance matrix a, and b to seg2 * I
// // 	p[0][0] = seg_sqr;
// // 	p[0][1] = 0;
// // 	p[0][2] = 0;
// // 	p[0][3] = 0;
// // 	
// // 	p[1][0] = 0;
// // 	p[1][1] = seg_sqr;
// // 	p[1][2] = 0;
// // 	p[1][3] = 0;
// // 	
// // 	p[2][0] = 0;
// // 	p[2][1] = 0;
// // 	p[2][2] = seg_sqr;
// // 	p[2][3] = 0;
// // 	
// // 	p[3][0] = 0;
// // 	p[3][1] = 0;
// // 	p[3][2] = 0;
// // 	p[3][3] = seg_sqr;
// // 	
// // 	phi[0] = 0;
// // 	phi[1] = 0;
// // 	phi[2] = 0;
// // 	phi[3] = 0;
// }
// 
// float IM_param_tune2_getMdata(void)
// {
// 	return est_out;
// }