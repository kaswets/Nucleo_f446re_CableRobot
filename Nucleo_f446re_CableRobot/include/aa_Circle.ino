void circle(double mx, double my, double angle, double speed)
{
int dir=1;
if(angle<0){
  dir=-1;
  angle=-angle;
  }
double DX=blockx-mx;
double DY=blocky-my;
double R=sqrt(DX*DX+DY*DY);
double theta0;
if(DX>=0.0)
  theta0=asin(DY/R);
else
  theta0=phi-asin(DY/R);
double stepangle = steplength/R;;
double steptime = steplength/speed;
int nsteps=(int)(angle/stepangle);
double rest = angle-nsteps*stepangle;
double theta = theta0;
stepangle=dir*stepangle;

//for(int s=0;s<nsteps;s++){
 // theta+=stepangle;
 // blockx=mx+R*cos(theta);
 // blocky=my+R*sin(theta);
  //grid->drawDot(blockx,blocky,5,clBlue); // UPDATE HIER DE 8 KABELLENGTES
//  Sleep((int)(steptime*1000.0));
//  }
//if(rest>0.0){
 // theta=theta0 + dir*angle;
 // blockx=mx+R*cos(theta);
 // blocky=my+R*sin(theta);
  //grid->drawDot(blockx,blocky,5,clBlue); // UPDATE HIER DE 8 KABELLENGTES
 // Sleep((int)(steptime*1000.0));
 // }
}
