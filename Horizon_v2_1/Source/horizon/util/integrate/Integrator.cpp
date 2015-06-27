#include "Integrator.h"

namespace horizon {
	namespace util {
		namespace integrate {

Integrator::Integrator(void) {}

integratorResult Integrator::rk4(EOMS* inFunction, Matrix tSpan, Matrix y0)
{
	double h = args.getParam("h");

	int N = int((tSpan.getValue(1,2)-tSpan.getValue(1,1))/h);

	double t = tSpan.getValue(1,1);
	Matrix y = y0;

	Matrix k1;
	Matrix k2;
	Matrix k3;
	Matrix k4;
	integratorResult newMap;
	
	newMap.ODE_RESULT[t] = y;

	for (int i = 1; i <= N; ++i)
	{
		k1 = h*inFunction->operator()(t, y);
		k2 = h*inFunction->operator()(t + 0.5*h, y + 0.5*k1);
		k3 = h*inFunction->operator()(t + 0.5*h, y + 0.5*k2);
		k4 = h*inFunction->operator()(t + h, y + k3);

		t = t + h;
		y = y + 1.0/6.0 * (k1 + 2.0*k2 + 2.0*k3 + k4);

		newMap.ODE_RESULT[t] = y;
	}

	return newMap;
}

integratorResult Integrator::rk45(EOMS* inFunction, Matrix tSpanLimits, Matrix y0)
{
	integratorResult newMap;
	//printf("\r");

	double expon = 1.0/5.0;
	double temph = 0.;
	double AData[] = {1.0/5.0, 3.0/10.0, 4.0/5.0, 8.0/9.0, 1.0, 1.0}; // 1x6
	double BData[] = {1.0/5.0, 3.0/40.0,  44.0/45.0,  19372.0/6561.0,  9017.0/3168.0,   35.0/384.0, //7x6
					  0.0,     9.0/40.0, -56.0/15.0, -25360.0/2187.0, -355.0/33.0,      0.0,
					  0.0,     0.0,       32.0/9.0,   64448.0/6561.0,  46732.0/5247.0,  500.0/1113.0,
					  0.0,     0.0,       0.0,       -212.0/729.0,     49.0/176.0,      125.0/192.0,
					  0.0,     0.0,       0.0,        0.0,            -5103.0/18656.0, -2187.0/6784.0,
					  0.0,     0.0,       0.0,        0.0,             0.0,             11.0/84.0,
					  0.0,     0.0,       0.0,        0.0,             0.0,             0.0};
	double EData[] = {71.0/57600.0, 0.0, -71.0/16695.0, 71.0/1920.0, -17253.0/339200.0, 22.0/525.0, -1.0/40.0}; // 7x1
	
	Matrix A(1, 6, AData);
	Matrix hA;
	Matrix B(7, 6, BData);
	Matrix hB;
	Matrix E(7, 1, EData);
	
	unsigned nRows;
	unsigned nCols;
	y0.getSize(nRows, nCols);
	unsigned neq = max(nRows, nCols);
	Matrix f(neq,7,0.0);

	double t0 = tSpanLimits.getValue(1,1);
	double tFinal = tSpanLimits.getValue(1,2);

	int ntstep = ceil((tFinal - t0)/30.0);
	Matrix tSpan(1, ntstep, 0.0);
// TODO:  is there a faster way to allocate this memory?
	for (int i = 1; i <= ntstep; i++)
		tSpan.setValue(1, i, 30.0*(i-1)+t0);

	double hmin = args.getParam("eps"); // Small enough
	double hmax = 0.1*(tFinal - t0);
	double rtol = args.getParam("rtol");
	double atol = args.getParam("atol");
	int tdir = 1;
	unsigned nfailed = 0;
	unsigned nfevals = 0;
	double threshold = atol/rtol;
	int next = 2;
	
	if ((tFinal - t0) > 0.0)
		tdir = 1;
	else
		tdir = -1;

// Compute an initial step size h using y'(t).

	Matrix f0 = inFunction->operator()(t0, y0);

	double absh = fabs(hmax);
	Matrix tempSingle = mmax(mabs(f0/mmax(mabs(y0), threshold)));
	double rh = tempSingle.getValue(1,1) / (0.8 * pow(rtol,expon));
  
	if (absh * rh > 1)
	  absh = 1 / rh;
	
	absh = max(absh, hmin);

	f.setColumn(1,f0);
	
// THE MAIN RK45 LOOP

	Matrix y = y0;
	Matrix ynew = y;
	double t = t0;
	double tnew = t;
	bool done = false;
	bool nofailed = true;
	double err = 0.0;
	double h;
	
	newMap.ODE_RESULT[t] = y;

	while (!done){
  
// By default, hmin is a small number such that t+hmin is only slightly
// different than t.  It might be 0 if t is 0.
		hmin = args.getParam("h");
		absh = min(hmax, max(hmin, absh));
		h = tdir * absh;
  
// Stretch the step if within 10% of tfinal-t.
		if (1.1*absh >= fabs(tFinal - t)){
			h = tFinal - t;
			absh = fabs(h);
			done = true;
		}
		
		// LOOP FOR ADVANCING ONE STEP.
		nofailed = true;    // no failed attempts
		while (true){
			hA = h * A;
			hB = h * B;

			f.setColumn(2, inFunction->operator()(t+hA(1,1),y+f*hB(colon(1,7),1)));
			f.setColumn(3, inFunction->operator()(t+hA(1,2),y+f*hB(colon(1,7),2)));
			f.setColumn(4, inFunction->operator()(t+hA(1,3),y+f*hB(colon(1,7),3)));
			f.setColumn(5, inFunction->operator()(t+hA(1,4),y+f*hB(colon(1,7),4)));
			f.setColumn(6, inFunction->operator()(t+hA(1,5),y+f*hB(colon(1,7),5)));
			
			tnew = t + hA.getValue(1,6);
			if (done)
				tnew = tFinal;   // Hit end point exactly.

			h = tnew - t;      // Purify h
    
			ynew = y + f*hB(colon(1,7),6);
			
			f.setColumn(7, inFunction->operator()(tnew, ynew));
			
			nfevals = nfevals + 6;              
    
//			err = absh * norm(      (f * E) ./ max (max ( abs(y), abs(ynew)), threshold),inf);
			err = (absh * mmax(mabs( (f * E)  / mmax(mmax( mabs(y),mabs(ynew) ), threshold) )))(1);

/*
    % Accept the solution only if the weighted error is no more than the
    % tolerance rtol.  Estimate an h that will yield an error of rtol on
    % the next step or the next try at taking this step, as the case may be,
    % and use 0.8 of this value to avoid failures.
*/
			if (err > rtol){		// Failed Step
				nfailed++;            
/* 
			if (absh <= hmin)
				 return;
			  end
*/
				if (nofailed){
					nofailed = false;
					absh = max(hmin, absh * max(0.1, 0.8*pow(rtol/err, expon)));
				}
				else
					absh = max(hmin, 0.5 * absh);
				
				h = tdir * absh;
				done = false;
			}
			else  //  Successful step
				break;
		}

		int nout_new =  0;
		Matrix tout_new(1,1,0.0);
		Matrix yout_new(neq,1,0.0);
		bool tout_cat = false;
		bool yout_cat = false;
		while (next <= ntstep  ){
			double cTime = tSpan.getValue(1, next);
			Matrix mcTime(1,1,cTime);
			if (tdir * (tnew - cTime) < 0)
				break;
			nout_new++;
			if (!tout_cat){
				tout_new = mcTime;
				tout_cat = true;
			}
			else
				horzcat2(tout_new, mcTime); // inplace horizontal concatination
			
			if (cTime == tnew){
				if (!yout_cat){
					yout_new = ynew;
					yout_cat = true;
				}
				else
					horzcat2(yout_new, ynew);
			}
			else
				if (!yout_cat){
					yout_new = ntrp45(cTime,t,y,h,f);
					yout_cat = true;
				}
				else
					horzcat2(yout_new, ntrp45(cTime,t,y,h,f));
		  
			next++;
		}
		
		for (int i = 1; i <= nout_new; i++)
			newMap.ODE_RESULT[tout_new(i)] = yout_new(colon(1,neq), i);

		if (done)
			newMap.ODE_RESULT[tnew] = ynew;

		if (nofailed){
			// Note that absh may shrink by 0.8, and that err may be 0.
			temph = 1.25*pow(err/rtol, expon);
			if (temph > 0.2)
				absh = absh / temph;
			else
				absh = 5.0*absh;
		}
		
		t = tnew;
		y = ynew;


		
		printf("Integrator State:  %4.2f%% done\r", 100*t/(tFinal-t0));

		f.setColumn(1,f(colon(1,neq),7));       // Already evaluated feval(odeFcn,tnew,ynew,integratorArgs)
	}

	printf("\n");
	return newMap;
}

Matrix Integrator::trapz(vector<Matrix> mVec, double dt)
{
	Matrix result(mVec.front().getNumRows(), mVec.front().getNumCols(), 0.0);
	vector<Matrix>::iterator iter;

	iter = mVec.begin();
	result = result + (*iter)*(dt/2);
	iter++;
	result = result + mVec.back()*(dt/2);
	mVec.pop_back();
	for(; iter != mVec.end(); ++iter)
		result = result + (*iter)*(dt);

	return result;
}

double Integrator::trapz(vector<double> mVec, double dt)
{
	double result = 0;
	vector<double>::iterator iter;

	iter = mVec.begin();
	result = result + (*iter)*(dt/2);
	iter++;
	result = result + mVec.back()*(dt/2);
	mVec.pop_back();
	for(; iter != mVec.end(); ++iter)
		result = result + (*iter)*(dt);

	return result;
}

double Integrator::getParam(const string param) const
{
	return args.getParam(param);
}

void Integrator::setParam(const string param, const double paramValue)
{
	args.setParam(param, paramValue);
}

Matrix Integrator::ntrp45(double tinterp, double t, Matrix y, double h, Matrix f)
{
	double BIData[] = {1.0,	-183.0/64.0,     37.0/12.0,    -145.0/128.0,
					   0.0,  0.0,            0.0,           0.0,
					   0.0,  1500.0/371.0,  -1000.0/159.0,  1000.0/371.0,
					   0.0, -125.0/32.0,     125.0/12.0,   -375.0/64.0, 
					   0.0,  9477.0/3392.0, -729.0/106.0,   25515.0/6784.0,
					   0.0, -11.0/7.0,       11.0/3.0,     -55.0/28.0,
					   0.0,  3.0/2.0,       -4.0,           5.0/2.0};

	Matrix BI(7, 4, BIData);

	double s = (tinterp - t)/h;  
	Matrix S(4,1,s);

	return y + f*(h*BI)*cumprod(S);
}
}}} // end namespace horizon::util::integrate
